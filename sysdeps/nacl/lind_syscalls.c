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
  lind_request request;
  memset(&request, 0, sizeof(request));
  lind_reply reply;
  memset(&reply, 0, sizeof(reply));
  struct lind_open_rpc_s args;
  memset(&args, 0, sizeof(struct lind_open_rpc_s));

  int return_code = -1;
  args.flags = flags;
  args.mode = mode;
  args.filename_len = strlen(filename);
  strncpy(&(args.filename[0]), filename, MAX_FILENAME_LENGTH - 1);

  request.call_number = NACL_sys_open;
  request.format = "<i<i<i511s";
  
  request.message.len = sizeof(struct lind_open_rpc_s);
  request.message.body = &args;
  
  nacl_rpc_syscall_proxy(&request, &reply, 0);

  /* on error return negative so we can set ERRNO. */  
  if (reply.is_error) {
    return_code = reply.return_code * -1;
  } else {
    return_code = reply.return_code;
  }


  return return_code;

}


int lind_read_rpc(int handle, int size, void * where_to) {

  lind_request request;
  memset(&request, 0, sizeof(request));
  lind_reply reply;
  memset(&reply, 0, sizeof(reply));
  struct lind_rw_rpc_s args;
  memset(&args, 0, sizeof(struct lind_rw_rpc_s));

  int return_code = -1;
  args.handle = handle;
  args.size = size;

  request.call_number = NACL_sys_read;
  request.format = "<i<i";

  request.message.len = sizeof(struct lind_rw_rpc_s);
  request.message.body = &args;
 

  nacl_rpc_syscall_proxy(&request, &reply, 0);

  /* on error return negative so we can set ERRNO. */  
  if (reply.is_error) {
    return_code = reply.return_code * -1;
  } else {
    return_code = reply.return_code;
    memcpy( where_to, reply.contents, return_code);
  }

  return return_code;

}


struct lind_fd_rpc_s {
  int fd;
};



int lind_fstat_rpc(int fd, struct statfs *buf) {

  lind_request request;
  memset(&request, 0, sizeof(request));
  lind_reply reply;
  memset(&reply, 0, sizeof(reply));
  struct lind_fd_rpc_s args;
  memset(&args, 0, sizeof(struct lind_fd_rpc_s));

  int return_code = -1;
  args.fd = fd;

  request.call_number = NACL_sys_fstat;
  request.format = "<i";

  request.message.len = sizeof(struct lind_fd_rpc_s);
  request.message.body = &args;
 
  nacl_rpc_syscall_proxy(&request, &reply, 0);

  /* on error return negative so we can set ERRNO. */  
  if (reply.is_error) {
    return_code = reply.return_code * -1;
  } else {
    return_code = reply.return_code;
    memcpy(buf, reply.contents, sizeof(struct statfs));
  }

  return return_code;

}



/* lseek system call.   Has three arguments (int fd, off_t offset, int whence)
 * returns a off_t (__SQUAD_TYPE) which is a 64 bit signed type. */
int lind_lseek_rpc(int fd, off_t offset, int whence) {

  lind_request request;
  memset(&request, 0, sizeof(request));

  lind_reply reply;
  memset(&reply, 0, sizeof(reply));

  struct lind_lseek_rpc_s args;
  memset(&args, 0, sizeof(struct lind_lseek_rpc_s));

  int return_code = -1;
  args.fd = fd;
  args.offset = offset;
  args.whence = whence;

  request.call_number = NACL_sys_lseek;
  /* int fd, int whence, long (as string) */
  request.format = "<i<i7s";

  request.message.len = sizeof( struct lind_lseek_rpc_s );
  request.message.body = &args;
 
  nacl_rpc_syscall_proxy(&request, &reply, 0);

  /* on error return negative so we can set ERRNO. */  
  if (reply.is_error) {
    return_code = reply.return_code * -1;
  } else {
    return_code = reply.return_code;
  }
  
  return return_code;

}


int lind_close_rpc(int fd) {
  lind_request request;
  memset(&request, 0, sizeof(request));
  lind_reply reply;
  memset(&reply, 0, sizeof(reply));
  struct lind_fd_rpc_s args;
  memset(&args, 0, sizeof(struct lind_fd_rpc_s));

  int return_code = -1;
  args.fd = fd;

  request.call_number = NACL_sys_close;
  request.format = "<i";

  request.message.len = sizeof(struct lind_fd_rpc_s);
  request.message.body = &args;
 
  nacl_rpc_syscall_proxy(&request, &reply, 0);

  /* on error return negative so we can set ERRNO. */  
  if (reply.is_error) {
    return_code = reply.return_code * -1;
  } else {
    return_code = reply.return_code;
  }
  
  return return_code;

}

ssize_t lind_write_rpc(int desc, void const *buf, size_t count) {
  lind_request request;
  memset(&request, 0, sizeof(request));
  lind_reply reply;
  memset(&reply, 0, sizeof(reply));
  struct lind_rw_rpc_s args;
  memset(&args, 0, sizeof(struct lind_rw_rpc_s));
  int return_code = -1;
  args.handle = desc;
  args.size = count;
  request.call_number = NACL_sys_write;
  request.format = concat(concat("<i<I", nacl_itoa(count-1)),"s"); 
  request.message.len = sizeof(struct lind_rw_rpc_s);
  request.message.body = &args;
  nacl_rpc_syscall_proxy(&request, &reply, 1, buf, count);

  /* on error return negative so we can set ERRNO. */  
  if (reply.is_error) {
    return_code = reply.return_code * -1;
  } else {
    return_code = reply.return_code;
  }


  return return_code;
}

