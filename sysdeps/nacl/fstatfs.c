#include <errno.h>
#include <sys/statfs.h>
#include <stddef.h>

#include "nacl_util.h"
#include "strace.h"
#include "lind_syscalls.h"

/* Return information about the filesystem on which FD resides.  */
int
__fstatfs (int fd, struct statfs *buf)
{
  nacl_strace( concat("fstatfs fd=",nacl_itoa(fd)) );
 
  if (is_system_handle(fd)) {
       __set_errno (ENOSYS);
       return -1;
  }

  int lind_rc = lind_fstat_rpc(fd, buf);

  if (lind_rc < 0) {
    __set_errno (-lind_rc);
     return -1;
  }
  
  return lind_rc;
}

weak_alias (__fstatfs, fstatfs)
