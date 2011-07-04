/* Chris Matthews, 2011 */
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <nacl_rpc.h>
#include <nacl_syscalls.h>

#include "strace.h"
#include "nacl_util.h"


/* if we start logging to early, we crash the system.
 So wait until the first file is opened, we are safe
by then.  */
static int _lind_ready_to_log = 0;

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

      nacl_rpc_syscall(NACL_STRACE_SYSCALL, format, strlen(message) + 1, message );
      free(message); 
  }    
}


struct nacl_rpc_syscall {
  /* Used to check on the python side we are getting the struct correctly */
  int magic_number;
  /* The system call number  */
  unsigned int syscall_number;
  unsigned int format_string_len;
  /* const char syscall format_string[6]; */
  /* The system call's arguments */
  /* void * payload; */
  /* another number to make sure we get everything we expect. */
  int validation_number;
};

void nacl_rpc_syscall(unsigned int call_number, const char* format, unsigned int len, void* body ) {

  /* Construct a message such that first part is a message header,
     then the syscall specific data.  */ 
  const int iov_len = 3;
  struct NaClImcMsgIoVec iov[iov_len]; 

  struct nacl_rpc_syscall current_call;
  /* Something signed that we can verify we are getting the correct stuff on the other side */
  const int magic = -2;
  int format_len = strlen(format);

  current_call.magic_number = magic;
  current_call.syscall_number = call_number;
  current_call.format_string_len = format_len; 
  current_call.validation_number = magic + call_number + format_len;
 
  /* current_call.payload = NULL; */
  
  iov[0].base =  &current_call;
  iov[0].length = sizeof(struct nacl_rpc_syscall);
  
  iov[1].base = format;
  iov[1].length = format_len;
 
  iov[2].base = body;
  iov[2].length = len;
 
 
  struct NaClImcMsgHdr msg;
  msg.iov = iov;
  msg.iov_length = iov_len;
  msg.descv = NULL;
  msg.desc_length = 0;

  imc_sendmsg (NACL_PLUGIN_ASYNC_FROM_CHILD_FD, &msg, 0);

}

