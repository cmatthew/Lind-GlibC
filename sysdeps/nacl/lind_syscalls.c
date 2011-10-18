#include <sys/statfs.h>
#include <stddef.h>
#include <string.h>

#include "lind_rpc.h"
#include "lind_syscalls.h"
#include "strace.h"
#include "nacl_util.h"
#include "nacl_syscalls.h"

#define MAX_FILENAME_LENGTH 512

struct lind_open_rpc_s {
  int flags;
  int mode;
  int filename_len;
  char filename[MAX_FILENAME_LENGTH];
};

struct lind_read_rpc_s {
  int handle;
  int size;
};


int lind_open_rpc (const char * filename, int flags, int mode) {
  lind_request req;
  memset(&req, 0, sizeof(req));
  lind_reply rep;
  memset(&rep, 0, sizeof(req));
  struct lind_open_rpc_s args;
  memset(&args, 0, sizeof(struct lind_open_rpc_s));


  int fd = -1;
  args.flags = flags;
  args.mode = mode;
  args.filename_len = strlen(filename);
  strncpy(&(args.filename[0]), filename, MAX_FILENAME_LENGTH - 1);

  req.call_number = NACL_sys_open;
  req.format = "<i<i<i511s";
  
  req.message.len = sizeof(struct lind_open_rpc_s);
  nacl_strace( concat("MessageLen: ", nacl_itoa(req.message.len) ) );
  req.message.body = &args;
  
  nacl_rpc_syscall_proxy(&req, &rep);
  // make the error code negative.
  fd = rep.return_code * ((rep.is_error)?-1:1);

  return fd;

}


int lind_read_rpc(int handle, int size, void * where_to) {

  lind_request req;
  memset(&req, 0, sizeof(req));
  lind_reply rep;
  memset(&rep, 0, sizeof(req));
  struct lind_read_rpc_s args;
  memset(&args, 0, sizeof(struct lind_read_rpc_s));

  int return_code = -1;
  args.handle = handle;
  args.size = size;

  req.call_number = NACL_sys_read;
  req.format = "<i<i";

  req.message.len = sizeof(struct lind_read_rpc_s);
  req.message.body = &args;
 

  nacl_rpc_syscall_proxy(&req, &rep);
  // make the error code negative.
  return_code = rep.return_code * ((rep.is_error)?-1:1);
  if (return_code > 0) {
    memcpy( where_to, rep.contents, return_code);
  }
  
  return return_code;

}


struct lind_fstat_rpc_s {
  int fd;
};



int lind_fstat_rpc(int fd, struct statfs *buf) {

  lind_request req;
  memset(&req, 0, sizeof(req));
  lind_reply rep;
  memset(&rep, 0, sizeof(req));
  struct lind_fstat_rpc_s args;
  memset(&args, 0, sizeof(struct lind_fstat_rpc_s));

  int return_code = -1;
  args.fd = fd;

  req.call_number = NACL_sys_fstat;
  req.format = "<i";

  req.message.len = sizeof(struct lind_fstat_rpc_s);
  req.message.body = &args;
 
  nacl_rpc_syscall_proxy(&req, &rep);
  // make the error code negative.
  return_code = rep.return_code * ((rep.is_error)?-1:1);
  if (return_code > 0) {
    memcpy(buf, rep.contents, sizeof(struct statfs));
  }
  
  return return_code;

}
