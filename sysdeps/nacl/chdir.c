#include <errno.h>
#include <stddef.h>
#include <unistd.h>
#include "lind_syscalls.h"
#include "strace.h"

/* Change the current directory to PATH.  */
int
__chdir (path)
     const char *path;
{
  if (path == NULL)
    {
      __set_errno (EINVAL);
      return -1;
    }

  /* since everything is okay, forward to lind server. */
  int return_code = lind_chdir_rpc(path);

  if (return_code < 0) {
    __set_errno ( -1 * return_code);
    return -1;
  } else {
    return return_code;
  }
  
}

weak_alias (__chdir, chdir)
