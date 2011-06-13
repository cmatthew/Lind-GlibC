
#include <errno.h>
#include <unistd.h>
#include <sysdep.h>

#include <nacl_syscalls.h>
#include "strace.h"

void *__curbrk = 0;
weak_alias (__curbrk, ___brk_addr)

int __brk (void *addr)
{
  /* nacl_strace("brk"); */	  

  void *result = NACL_SYSCALL (sysbrk) (addr);
  if ((unsigned int) result > 0xfffff000u) {
    errno = -(int) result;
    return -1;
  }
  __curbrk = result;
  if (result < addr) {
    errno = ENOMEM;
    return -1;
  }
  return 0;
}
weak_alias (__brk, brk)
