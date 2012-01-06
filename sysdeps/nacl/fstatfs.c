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
  nacl_strace( combine(3, "fstatfs fd=", nacl_itoa(fd), " is disabled\n" ) );
 
  if (is_system_handle(fd)) {
      nacl_strace( "not checked becuase system handle." );

       __set_errno (ENOSYS);
       return -1;
  }
  
  int lind_rc = lind_fstatfs_rpc(fd, buf);
  
  if (lind_rc < 0) {
    __set_errno (-lind_rc);
     return -1;
  }
   __set_errno (ENOSYS);
   return 0;
}

weak_alias (__fstatfs, fstatfs)
