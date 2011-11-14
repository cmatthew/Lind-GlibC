#include <errno.h>
#include <stddef.h>
#include <unistd.h>

#include "strace.h"
#include "nacl_util.h"
#include "lind_syscalls.h"


/* Remove the directory PATH.  */
int
__rmdir (path)
     const char *path;
{
  nacl_strace(concat("rmdir ",name));

  if (path == NULL)
    {
      __set_errno (EINVAL);
      return -1;
    }


  /* since everything is okay, forward to lind server. */
  int return_code = lind_rmdir_rpc(path);

  if (return_code < 0) {
    __set_errno ( -1 * return_code);
    return -1;
  } else {
    return return_code;
  }

}xxxxxxxx

weak_alias (__rmdir, rmdir)
