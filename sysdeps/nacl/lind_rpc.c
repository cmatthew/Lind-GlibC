/* Chris Matthews, 2011 */
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include <nacl_rpc.h>
#include <nacl_syscalls.h>

#include "strace.h"
#include "nacl_util.h"
#include "lind_rpc.h"
/* if we start logging to early, we crash the system.
 So wait until the first file is opened, we are safe
by then.  */

static int _lind_ready_to_log = 0;

static char ** lind_rpc_status_messages = {"RPC OK", 
					   "RPC Write Error", 
					   "RPC Read Error", 
					   "RPC Argument Error",
                                           "RPC Protocol Error" };


int get_logging_status(void) {
  return _lind_ready_to_log;
}

void set_ready_to_log(void) {
  _lind_ready_to_log = 1;
}



void nacl_strace(const char* syscall) {
  if (get_logging_status()) { 
      char * message = strdup(syscall); 
      char * format = nacl_itoa(strlen(message));
      format = concat(format, "s");
      int returncode;
      unsafe_nacl_rpc_syscall(NACL_STRACE_SYSCALL, format, strlen(message) + 1, message, &returncode );
      free(message); 
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


static void nacl_rpc_setup_header(struct nacl_rpc_syscall * current_call, unsigned int call_number, const char * format) {
  
  /* Something signed that we can verify we are getting the correct stuff on the other side */
  const int magic = -2;

  int format_num_chars = strlen(format);

  memset( (void*) current_call, 0, sizeof(struct nacl_rpc_syscall));

  current_call->magic_number = magic;
  current_call->syscall_number = call_number;
  current_call->format_string_len = format_num_chars;
  current_call->validation_number = magic + call_number + format_num_chars;

}


lind_rpc_status nacl_rpc_syscall(unsigned int call_number, const char* format, unsigned int len, void* body, int * retval) {

  lind_rpc_status rc;
  
  rc = unsafe_nacl_rpc_syscall(call_number, format, len, body, retval);
  
  if ( rc == RPC_OK ) {
    return rc;
  } else if (rc > RPC_OK && rc <= RPC_ARGS_ERROR) {
    nacl_strace(concat("RPC Failed with: ", lind_rpc_status_messages[rc]));
  } else {
    nacl_strace("Invalid RPC return state. This should never happen!");
  }
  

}

lind_rpc_status unsafe_nacl_rpc_syscall(unsigned int call_number, const char* format, unsigned int len, void* body, int * retval) {

  /* Construct a message such that first part is a message header,
     then the syscall specific data.  */
  int iov_len;
  iov_len = (body == NULL)?2:3;
  struct NaClImcMsgIoVec iov[iov_len];
  memset(iov, 0, sizeof(iov));
  struct nacl_rpc_syscall current_call;
  nacl_rpc_setup_header(&current_call, call_number, format);
  /* current_call.payload = NULL; */
  
  iov[0].base =  &current_call;
  iov[0].length = sizeof( struct nacl_rpc_syscall );
  

  iov[1].base = (void*) format;
  iov[1].length = strlen(format);
 
  if (iov_len == 3) {
    iov[2].base = body;
    iov[2].length = len;
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
  const int buf_siz = 1024;
  int buf[buf_siz];
  memset(&buf, 0, buf_siz*sizeof(int));
  int rc = -1;
  struct NaClImcMsgIoVec reply_iov;
  memset(&reply_iov, 0, sizeof(struct NaClImcMsgIoVec));
  struct NaClImcMsgHdr reply_msg;
  memset(&reply_msg, 0, sizeof(struct NaClImcMsgHdr));
  
  reply_iov.base = (void *) &buf;
  reply_iov.length = buf_siz * sizeof(int);
  reply_msg.iov = &reply_iov;
  reply_msg.iov_length = 1;
  reply_msg.descv = &rc;
  reply_msg.desc_length = 1;
  reply_msg.flags = 0;

  if (imc_recvmsg (NACL_PLUGIN_ASYNC_TO_CHILD_FD, &reply_msg, 0 ) < 1) {
    return RPC_READ_ERROR;
  }
  int * size =  &buf[0];
  int * magic =  &buf[1];
  int * is_error = &buf[2];
  int error = (-1)*(*is_error);
  *retval = error * buf[3];
  

  return RPC_OK;

  /* if (*magic != 101010) { */
  /*   return RPC_PROTOCOL_ERROR; */
  /* } */

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

