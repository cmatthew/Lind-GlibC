#include <string.h>
#include <stdlib.h>

#include <nacl_rpc.h>
#include <nacl_syscalls.h>


static char * concat(char* op, char * arg) {
  int len = 0;
  len = strlen(op);
  len += strlen(arg) + 2;
  char * msg = calloc(1,len);
  strcat(msg, op);
  strcat(msg, arg);
  return msg;
}
  

static void nacl_strace(const char* syscall) {
  char * message = strdup(syscall);
  struct NaClImcMsgIoVec iov[1]; 
  iov[0].base = message;
  iov[0].length = strlen (message); 
  /* iov[1].base = (char *) filename; */
  /* iov[1].length = strlen (filename); */
  struct NaClImcMsgHdr msg;
  msg.iov = iov;
  msg.iov_length = 1;
  msg.descv = NULL;
  msg.desc_length = 0;
  /* make sure to free message */
  imc_sendmsg (NACL_PLUGIN_ASYNC_FROM_CHILD_FD, &msg, 0);
    
}

