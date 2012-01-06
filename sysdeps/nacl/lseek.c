#include <errno.h>
#include <unistd.h>

#include <nacl_syscalls.h>
#include "lind_syscalls.h"
#include "strace.h"
#include "nacl_util.h"

off_t __lseek (int fd, off_t offset, int whence)
{
  nacl_strace(combine(6, "lseek on ", nacl_itoa(fd), " with offset ", nacl_itoa(offset), " with whence ", nacl_itoa(whence)));
  nacl_abi_off_t nacl_offset = offset;
  int result;
  if (is_system_handle(fd)) {
    result = NACL_SYSCALL (lseek) (fd, &nacl_offset, whence);
      if (result < 0) {
    errno = -result;
    return -1;
  }
  return nacl_offset;


  }
  else{
    off_t returned_offset = 0;
    result = lind_lseek_rpc(fd, offset, whence, &returned_offset);
    if (result < 0) {
      errno = -result;
      return -1;
    }
    return returned_offset;


  }
    
}
libc_hidden_def (__lseek)
weak_alias (__lseek, lseek)
