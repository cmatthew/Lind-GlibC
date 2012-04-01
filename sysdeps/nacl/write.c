
#include <errno.h>
#include <unistd.h>
#include <sysdep.h>

#include <nacl_syscalls.h>
#include "lind_syscalls.h"
#include "strace.h"
#include "nacl_util.h"


ssize_t __write(int desc, void const *buf, size_t count)
{
  int result;
  nacl_strace(combine(3, "[glibc] calling write on ", nacl_itoa(desc), "\n"));
  if (is_system_handle(desc)) {
    result = NACL_SYSCALL (write) (desc, buf, count);
  } else {
      result = lind_write_rpc(desc, count, buf); 
  }
  if (result < 0) {
    errno = -result;
    return -1;
  }
  return result;
}
libc_hidden_def (__write)
weak_alias (__write, write)
strong_alias (__write, __libc_write)
libc_hidden_def (__libc_write)
strong_alias (__write, __write_nocancel)
