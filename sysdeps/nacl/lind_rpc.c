/* Chris Matthews, 2011 */
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include <nacl_rpc.h>
#include <nacl_syscalls.h>
#include <stdarg.h>
#include "strace.h"
#include "nacl_util.h"
#include "lind_rpc.h"
/* if we start logging to early, we crash the system.
 So wait until the first file is opened, we are safe
by then.  */

static lind_rpc_status unsafe_nacl_rpc_syscall(lind_request * request, lind_reply * reply, int nargs, va_list ertra_args);



static int _lind_ready_to_log = 0;

static char * lind_rpc_status_messages[] = {"RPC OK", 
					    "RPC Write Error", 
					    "RPC Read Error", 
					    "RPC Argument Error",
					    "RPC Protocol Error",
					    NULL};


int get_logging_status(void) {
  return _lind_ready_to_log;
}

void set_ready_to_log(void) {
  _lind_ready_to_log = 1;
}

void set_no_logging(void) {
  _lind_ready_to_log = 0;  
}

void nacl_strace(const char* message, ...) {
  /* Var args dont work */
  if (get_logging_status()) { 
    va_list args;
    va_start(args, message);
    lind_request request;
    static lind_reply reply;
    memset(&request, 0, sizeof(lind_request));
    
    request.message.body = strdup(message);
    request.message.len = strlen(message) + 1;
    request.format = nacl_itoa(strlen(message));
    request.format = concat(request.format, "s");
    request.call_number = NACL_STRACE_SYSCALL;
      
    unsafe_nacl_rpc_syscall(&request, &reply, 0, args);
    va_end(args);
    
  }    
}


struct nacl_rpc_syscall {
  /* Used to check on the python side we are getting the struct correctly */
  int magic_number;
  /* The system call number  */
  unsigned int syscall_number;

  unsigned int format_string_len;

  /* another number to make sure we get everything we expect. */
  int validation_number;
};


static void nacl_rpc_setup_header(struct nacl_rpc_syscall * current_call, lind_request * request) {
  
  /* Something signed that we can verify we are getting the correct stuff on the other side */
  const int magic = -2;

  int format_num_chars = strlen(request->format);

  memset( (void*) current_call, 0, sizeof(struct nacl_rpc_syscall));

  current_call->magic_number = magic;
  current_call->syscall_number = request->call_number;
  current_call->format_string_len = format_num_chars;
  current_call->validation_number = magic + request->call_number + format_num_chars;

}


lind_rpc_status depricated_nacl_rpc_syscall(unsigned int call_number, const char* format, unsigned int len, void* body, int * retval, int nargs, ...) {

  lind_rpc_status rc;
  va_list args;
  lind_request request;
  lind_reply reply;
  va_start(args, nargs);
  memset(&request, 0, sizeof(request));
  memset(&reply, 0, sizeof(reply));

  request.call_number = call_number;
  request.format = (char*)format;
  request.message.len = len;
  request.message.body = body;


  rc = unsafe_nacl_rpc_syscall(&request, &reply, nargs, args);
  
  if ( rc == RPC_OK ) {
    if (reply.is_error) {
      reply.return_code *= -1;
    }
    va_end(args);
    *retval = reply.return_code;
    return rc;
  } else if (rc > RPC_OK && rc <= RPC_ARGS_ERROR) {
    nacl_strace(concat("RPC Failed with: ", lind_rpc_status_messages[rc]));
  } else {
    nacl_strace("Invalid RPC return state. This should never happen!");
  }
  va_end(args);
  return rc;

}


lind_rpc_status nacl_rpc_syscall_proxy(lind_request * request, lind_reply * reply, int nargs, ...) {

  lind_rpc_status rc;
  va_list argp;
  va_start(argp, nargs);
  rc = unsafe_nacl_rpc_syscall(request, reply, nargs, argp);
  
  if ( rc == RPC_OK ) {
    va_end(argp);
    return rc;
  } else if (rc > RPC_OK && rc <= RPC_ARGS_ERROR) {
    nacl_strace(concat("RPC Failed with: ", lind_rpc_status_messages[rc]));
  } else {
    nacl_strace("Invalid RPC return state. This should never happen!");
  }
  va_end(argp);
  return rc;

}

/** Make a rpc system call, but without printing error status after.
    See nacl_rpc_syscall_proxy for safe wrapper!

*/
static lind_rpc_status unsafe_nacl_rpc_syscall(lind_request * request, lind_reply * reply, int nargs, va_list extra_args) {

  /* Construct a message such that first part is a message header,
     then the syscall specific data.  */
  int iov_len = (request->message.body == NULL)?2:3;
  iov_len += nargs;
  struct NaClImcMsgIoVec iov[iov_len];
  memset(iov, 0, sizeof(iov));
  struct nacl_rpc_syscall current_call;
  nacl_rpc_setup_header(&current_call, request);
  int i;
  for (i = 0; i < nargs; i++) {
    iov[3+i].base = va_arg(extra_args, void *);
    iov[3+i].length = va_arg(extra_args, int);
  }

  iov[0].base =  &current_call;
  iov[0].length = sizeof( struct nacl_rpc_syscall );
  

  iov[1].base = (void*) request->format;
  iov[1].length = strlen(request->format)+1;
 
  if (iov_len == 3) {
    iov[2].base = request->message.body;
    iov[2].length = request->message.len;
  }
  struct NaClImcMsgHdr request_msg;
  memset(&request_msg, 0, sizeof(struct NaClImcMsgHdr));
  request_msg.iov = iov;
  request_msg.iov_length = iov_len;
  request_msg.descv = NULL;
  request_msg.desc_length = 0;

  int write_rc = 0;
  if ( (write_rc = imc_sendmsg (NACL_PLUGIN_ASYNC_FROM_CHILD_FD, &request_msg, 0)) < 1) {
    return RPC_WRITE_ERROR;
  }

  
  memset(reply, 0, sizeof(lind_reply));

  int rc = -1;
  struct NaClImcMsgIoVec reply_iov;
  memset(&reply_iov, 0, sizeof(struct NaClImcMsgIoVec));
  struct NaClImcMsgHdr reply_msg;
  memset(&reply_msg, 0, sizeof(struct NaClImcMsgHdr));
  
  reply_iov.base = (void *)reply;
  reply_iov.length = sizeof(struct lind_rpc_reply);
  reply_msg.iov = &reply_iov;
  reply_msg.iov_length = 1;
  reply_msg.descv = &rc;
  reply_msg.desc_length = 1;
  reply_msg.flags = 0;

  if (imc_recvmsg (NACL_PLUGIN_ASYNC_TO_CHILD_FD, &reply_msg, 0 ) < 1) {
    return RPC_READ_ERROR;
  }

  /*  "<i<i<i<iNs", len(self.message) + 12, 101010, 1, return_code, message
 */

  
  if (reply->magic_number != MAGIC) {
     return RPC_PROTOCOL_ERROR;
   }


  return RPC_OK;


  /* void * response_buffer = malloc(*size); */
  /* memset(response_buffer, 0, *size); */
  /* int response_rc = -1; */
  /* struct NaClImcMsgIoVec response_iov; */
  /* memset(&response_iov, 0, sizeof(struct NaClImcMsgIoVec)); */
  /* struct NaClImcMsgHdr response_msg; */
  /* memset(&response_msg, 0, sizeof(struct NaClImcMsgHdr)); */
  
  /* response_iov.base = (void *) &response_buffer; */
  /* response_iov.length = sizeof(response_buffer); */
  /* response_msg.iov = &response_iov; */
  /* response_msg.iov_length = 1; */
  /* response_msg.descv = &response_rc; */
  /* response_msg.desc_length = 1; */
  /* response_msg.flags = 0; */


  /* if (imc_recvmsg (NACL_PLUGIN_ASYNC_TO_CHILD_FD, &response_msg, *size ) < 1) { */
  /*   return RPC_READ_ERROR; */
  /* } */

  /* *retval = *((int*)response_buffer); */

  /* return RPC_OK; */

}

