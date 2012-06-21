/* Chris Matthews, 2011 */
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include <nacl_rpc.h>
#include <nacl_syscalls.h>
#include <stdarg.h>
#include "lindlock.h"
#include "strace.h"
#include "nacl_util.h"
#include "lind_rpc.h"


/* if we start logging to early, we crash the system.
 So wait until the first file is opened, we are safe
by then.  */

static struct {
  lindlock socket_lock;  /* To lock sequence number*/
  int _lind_ready_to_log; /* Is the system ready to start logging? */
  int sequence_number; /* Message sequence number */
} lind = {-1, 0, -10};


static lind_rpc_status unsafe_nacl_rpc_syscall(lind_request * request, lind_reply * reply, int nargs, va_list ertra_args);


static const char * lind_rpc_status_messages[] = {"RPC OK", 
					    "RPC Write Error", 
					    "RPC Read Error", 
					    "RPC Argument Error",
					    "RPC Protocol Error",
					    NULL};


int get_logging_status(void) {
  return lind._lind_ready_to_log;
}


void set_ready_to_log(void) {
  lind._lind_ready_to_log = 1;
}


void set_no_logging(void) {
  lind._lind_ready_to_log = 0;  
}


/** Implementation of the nacl_strace function.
 * it is redirected here by a macro.  Just prints
 * the message in python. 
 *
 *  Note: since this is used for error message printing
 * if it fails, we cannot print a error message.
 */
void _lind_strace(const char* message) {
  if (get_logging_status()) { 
    lind_request request;
    static lind_reply reply;
    memset(&request, 0, sizeof(lind_request));
    
    request.message.body = strdup(message);
    request.message.len = strlen(message);
    request.format = nacl_itoa(strlen(message));
    request.format = concat(request.format, "s");
    request.call_number = NACL_sys_trace;
      
    int rc = unsafe_nacl_rpc_syscall(&request, &reply, 0, NULL);  /* we do unsafe because we could not do much if this fails */

    if (rc != RPC_OK) {
      dbg_print("Warning: RPC strace failed");
    }
  }    
}


/** This struct is the wire format for the header sent to Python.
 *
 */
struct nacl_rpc_syscall {

  /* Used to check on the python side we are getting the struct correctly */
  int magic_number;

  /* The system call number  */
  unsigned int syscall_number;

  unsigned int format_string_len;
  
  /* size of the payload, excluding the header and format string. */
  unsigned int payload_size;

  /* message sequence number.  One bigger each time hopefully! */
  unsigned int sequence_number;

  /* another number to make sure we get everything we expect. */
  int validation_number;
};

/* check then populate header information for syscall struct. */
static void nacl_rpc_setup_header(struct nacl_rpc_syscall * current_call, lind_request * request, unsigned int size) {
  
  /* Something signed that we can verify we are getting the correct stuff on the other side */
  const int magic = -2;

  int format_num_chars = strlen(request->format);

  memset( (void*) current_call, 0, sizeof(struct nacl_rpc_syscall));

  current_call->magic_number = magic;
  current_call->payload_size = size;
  current_call->syscall_number = request->call_number;
  current_call->format_string_len = format_num_chars;
  current_call->validation_number = magic + request->call_number + format_num_chars; /* simple checksum style validation of message */

}


/**  This function used to be used for system calls. Now it is not, but trace still uses it, so we keep it here.
 *
 */
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
    dbg_print("Warning: An RPC failed.");
    print_error(concat("RPC Failed with: ", lind_rpc_status_messages[rc]));
  } else {
    print_error("Invalid RPC return state. This should never happen!");
  }
  va_end(args);
  return rc;

}


/* make a syscall, but do is safely. If there is a problem report an error message. 
 */
lind_rpc_status nacl_rpc_syscall_proxy(lind_request * request, lind_reply * reply, int nargs, ...) {

  lind_rpc_status rc;
  va_list argp;
  va_start(argp, nargs);
  rc = unsafe_nacl_rpc_syscall(request, reply, nargs, argp);
  
  if ( rc == RPC_OK ) {
    va_end(argp);
    return rc;
  } else if (rc > RPC_OK && rc <= RPC_ARGS_ERROR) {
    print_error(concat("RPC Failed with: ", lind_rpc_status_messages[rc]));
  } else {
    print_error("Invalid RPC return state. This should never happen!");
  }
  va_end(argp);
  return rc;

}

/** Make a rpc system call, but without printing error status after.  Use this for things like trace
    which has to work, or from places where printing an error wont work.
    See nacl_rpc_syscall_proxy for safe wrapper!

	Function builds an I/O vector with a header as the first element, then
	 the format string, then the value parameters, then one element per
	 reference parameter.  

*/
static lind_rpc_status unsafe_nacl_rpc_syscall(lind_request * request, lind_reply * reply, int nargs, va_list extra_args) {

  if ( lind.socket_lock == -1) {
	lind.socket_lock = lindlock_init();
  }
  unsigned int size = 0;
  /* Construct a message such that first part is a message header,
     then the syscall specific data.  */
  int iov_len = (request->message.body == NULL)?2:3; /* is there a payload in this message? */
  iov_len += nargs; 
  int iov_pos = 0; 		/* increment through, header, format, body1, then each nargs elements */
  struct NaClImcMsgIoVec iov[iov_len];
  memset(iov, 0, sizeof(iov));
  struct nacl_rpc_syscall current_call;
  nacl_rpc_setup_header(&current_call, request, size); 

  /* add header */
  iov[iov_pos].base =  &current_call;
  iov[iov_pos].length = sizeof( struct nacl_rpc_syscall );
  iov_pos += 1;

  /* add format string */
  iov[iov_pos].base = (void*) request->format;
  iov[iov_pos].length = strlen(request->format)+1;
  iov_pos += 1;

  /* if message has body add it */
  if (request->message.body != NULL) {
    iov[iov_pos].base = request->message.body;
    iov[iov_pos].length = request->message.len;
	size += request->message.len;
    iov_pos += 1;
  }

  /* add in each of the var args pointers */
  int i;
  for (i = 0; i < nargs; i++) {
    iov[iov_pos].base = va_arg(extra_args, void *);
    iov[iov_pos].length = va_arg(extra_args, int);
	size += iov[iov_pos].length;
    iov_pos += 1;
  }

  current_call.payload_size = size;

  struct NaClImcMsgHdr request_msg;
  memset(&request_msg, 0, sizeof(struct NaClImcMsgHdr));
  request_msg.iov = iov;
  request_msg.iov_length = iov_len; 
  request_msg.descv = NULL;
  request_msg.desc_length = 0;

  int write_rc = 0;
  lindlock_lock(lind.socket_lock);
  current_call.sequence_number = lind.sequence_number++;

  if ( (write_rc = imc_sendmsg (NACL_PLUGIN_ASYNC_FROM_CHILD_FD, &request_msg, 0)) < 1) {
    dbg_print("Warning: RPC error in imc_sendmesg.");
    return RPC_WRITE_ERROR;
  }
  
  /* now lets get the reply */
  memset(reply, 0, sizeof(lind_reply));

  int rc = -1;
  struct NaClImcMsgIoVec reply_iov;
  memset(&reply_iov, 0, sizeof(struct NaClImcMsgIoVec));
  struct NaClImcMsgHdr reply_msg;
  memset(&reply_msg, 0, sizeof(struct NaClImcMsgHdr));
  
  reply_iov.base = (void *) reply;
  reply_iov.length = sizeof(struct lind_rpc_reply);
  reply_msg.iov = &reply_iov;
  reply_msg.iov_length = 1;
  reply_msg.descv = &rc;
  reply_msg.desc_length = 1;
  reply_msg.flags = 0;

  if (imc_recvmsg (NACL_PLUGIN_ASYNC_TO_CHILD_FD, &reply_msg, 0 ) < 1) {
    lindlock_unlock(lind.socket_lock);
    dbg_print("Warning: RPC error in recvmsg.");
    return RPC_READ_ERROR;
  } else {
	lindlock_unlock(lind.socket_lock);
  }
  if (reply->magic_number != MAGIC) {
    dbg_print("Warning: RPC error in recv magic number");
    return RPC_PROTOCOL_ERROR;
  }

  return RPC_OK;
}

