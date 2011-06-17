/* Chris Matthews, 2011 */
#include <string.h>
#include <stdlib.h>

#include <nacl_rpc.h>
#include <nacl_syscalls.h>


/* if we start logging to early, we crash the system.
 So wait until the first file is opened, we are safe
by then.  */

static int lind_ready_to_log = 0;

int get_logging_status() {
  return lind_ready_to_log;
}

void set_ready_to_log() {
  lind_ready_to_log = 1;
}


char * concat(char* op, char * arg) {
  int len = 0;
  len = strlen(op);
  len += strlen(arg) + 2;
  char * msg = calloc(1,len);
  strcat(msg, op);
  strcat(msg, arg);
  return msg;
}
  


void nacl_strace(const char* syscall) {
  if (get_logging_status()) { 
      char * message = strdup(syscall);
      struct NaClImcMsgIoVec iov[1]; 
      iov[0].base = message;
      iov[0].length = strlen (message); 
      struct NaClImcMsgHdr msg;
      msg.iov = iov;
      msg.iov_length = 1;
      msg.descv = NULL;
      msg.desc_length = 0;
      imc_sendmsg (NACL_PLUGIN_ASYNC_FROM_CHILD_FD, &msg, 0);
    }   
}


