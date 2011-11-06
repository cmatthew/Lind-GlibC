#include <errno.h>
#include <unistd.h>

#include <nacl_syscalls.h>
#include "lind_syscalls.h"
#include "strace.h"
#include "nacl_util.h"

off_t __lseek (int fd, off_t offset, int whence)
{
  nacl_strace("lseek");
  nacl_abi_off_t nacl_offset = offset;
  int result;
  if (is_system_handle(fd)) {
    result = NACL_SYSCALL (lseek) (fd, &nacl_offset, whence);
  }
  else{
    /* We want to pass this over to the Repy process */
    /* JAC: I'm going to pass the original offset through.   I don't think we
     * need the nacl offset on the Repy side. */
    result = lind_lseek_rpc(fd, offset, whence);
  }
    
  if (result < 0) {
    errno = -result;
    return -1;
  }
  return nacl_offset;
}
libc_hidden_def (__lseek)
weak_alias (__lseek, lseek)
