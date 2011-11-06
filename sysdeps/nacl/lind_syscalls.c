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

struct lind_rw_rpc_s {
  int handle;
  int size;
};


struct lind_lseek_rpc_s {
  int fd;
  int whence;
  off_t offset;
};



int lind_open_rpc (const char * filename, int flags, int mode) {
  lind_request req;
  memset(&req, 0, sizeof(req));
  lind_reply rep;
  memset(&rep, 0, sizeof(rep));
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
  
  nacl_rpc_syscall_proxy(&req, &rep, 0);
  // make the error code negative.
  fd = rep.return_code * ((rep.is_error)?-1:1);

  return fd;

}


int lind_read_rpc(int handle, int size, void * where_to) {

  lind_request req;
  memset(&req, 0, sizeof(req));
  lind_reply rep;
  memset(&rep, 0, sizeof(rep));
  struct lind_rw_rpc_s args;
  memset(&args, 0, sizeof(struct lind_rw_rpc_s));

  int return_code = -1;
  args.handle = handle;
  args.size = size;

  req.call_number = NACL_sys_read;
  req.format = "<i<i";

  req.message.len = sizeof(struct lind_rw_rpc_s);
  req.message.body = &args;
 

  nacl_rpc_syscall_proxy(&req, &rep, 0);
  // make the error code negative.
  return_code = rep.return_code * ((rep.is_error)?-1:1);
  if (return_code > 0) {
    memcpy( where_to, rep.contents, return_code);
  }
  
  return return_code;

}


struct lind_fd_rpc_s {
  int fd;
};



int lind_fstat_rpc(int fd, struct statfs *buf) {

  lind_request req;
  memset(&req, 0, sizeof(req));
  lind_reply rep;
  memset(&rep, 0, sizeof(rep));
  struct lind_fd_rpc_s args;
  memset(&args, 0, sizeof(struct lind_fd_rpc_s));

  int return_code = -1;
  args.fd = fd;

  req.call_number = NACL_sys_fstat;
  req.format = "<i";

  req.message.len = sizeof(struct lind_fd_rpc_s);
  req.message.body = &args;
 
  nacl_rpc_syscall_proxy(&req, &rep, 0);
  // make the error code negative.
  return_code = rep.return_code * ((rep.is_error)?-1:1);
  if (return_code > 0) {
    memcpy(buf, rep.contents, sizeof(struct statfs));
  }
  
  return return_code;

}



/* lseek system call.   Has three arguments (int fd, off_t offset, int whence)
 * returns a off_t (__SQUAD_TYPE) which is a 64 bit signed type. */
int lind_lseek_rpc(int fd, off_t offset, int whence) {

  lind_request req;
  memset(&req, 0, sizeof(req));

  lind_reply rep;
  memset(&rep, 0, sizeof(rep));

  struct lind_lseek_rpc_s args;
  memset(&args, 0, sizeof(struct lind_lseek_rpc_s));

  int return_code = -1;
  args.fd = fd;
  args.offset = offset;
  args.whence = whence;

  req.call_number = NACL_sys_lseek;
  /* int fd, int whence, long (as string) */
  req.format = "<i<i7s";

  req.message.len = sizeof( struct lind_lseek_rpc_s );
  req.message.body = &args;
 
  nacl_rpc_syscall_proxy(&req, &rep, 0);

  /* on error return negative so we can set ERRNO. */  
  if (rep.is_error) {
    return_code = rep.return_code * -1;
  } else {
    return_code = rep.return_code;
  }
  
  return return_code;

}


int lind_close_rpc(int fd) {
  lind_request req;
  memset(&req, 0, sizeof(req));
  lind_reply rep;
  memset(&rep, 0, sizeof(rep));
  struct lind_fd_rpc_s args;
  memset(&args, 0, sizeof(struct lind_fd_rpc_s));

  int return_code = -1;
  args.fd = fd;

  req.call_number = NACL_sys_close;
  req.format = "<i";

  req.message.len = sizeof(struct lind_fd_rpc_s);
  req.message.body = &args;
 
  nacl_rpc_syscall_proxy(&req, &rep, 0);
  // make the error code negative.
  return_code = rep.return_code * ((rep.is_error)?-1:1);
  
  return return_code;

}

ssize_t lind_write_rpc(int desc, void const *buf, size_t count) {
  lind_request req;
  memset(&req, 0, sizeof(req));
  lind_reply rep;
  memset(&rep, 0, sizeof(rep));
  struct lind_rw_rpc_s args;
  memset(&args, 0, sizeof(struct lind_rw_rpc_s));
  int return_code = -1;
  args.handle = desc;
  args.size = count;
  req.call_number = NACL_sys_write;
  req.format = concat(concat("<i<I", nacl_itoa(count-1)),"s"); 
  req.message.len = sizeof(struct lind_rw_rpc_s);
  req.message.body = &args;
  nacl_rpc_syscall_proxy(&req, &rep, 1, buf, count);
  // make the error code negative.
  return_code = rep.return_code * ((rep.is_error)?-1:1);

  return return_code;
}

