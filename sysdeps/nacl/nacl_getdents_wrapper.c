#include "lind_syscalls.h"
#include "strace.h"


/* The purpose of this file is to be #included by generic readdir
   implementations.  */

static const int d_name_shift = offsetof (DIRENT_TYPE, d_name) -
    offsetof (struct nacl_abi_dirent, nacl_abi_d_name);

/* Calls NACL_SYSCALL(getdents) and converts resulting buffer to glibc abi.
   This wrapper is required since glibc abi for DIRENT_TYPE differs from
   struct nacl_abi_dirent. */
__ssize_t internal_function __GETDENTS (int fd, char *buf, size_t buf_size)
{
  nacl_strace("getdents");
  ssize_t result = lind_getdents_rpc(fd, buf, buf_size);
  return result;

}
