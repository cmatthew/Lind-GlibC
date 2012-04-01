#include <errno.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#include <nacl_rpc.h>
#include <nacl_syscalls.h>

#include "component.h"
#include "strace.h"
#include "nacl_util.h"
#include "lind_syscalls.h"



/* Perform the I/O control operation specified by REQUEST on FD.
   The actual type and use of ARG and the return value depend on REQUEST.  */
int
__ioctl (int fd, unsigned long int request, ...)
{
  nacl_strace(combine(4 , "ioctl fd=", nacl_itoa(fd), " req=", nacl_itoa((int)request) ));

  int result = -1;

  if (is_system_handle(fd)) {
    /* I think system handles can't have ioctls because they are not implemented
     * in NaCl, so we should return ENOSYS on error no and fail for this handle  */
    /* with one exception, out component system FD, which is always set to 9 */
    if (fd == COMP_FIXED_FD && request >= LIND_LOW_IOCTL && request <= LIND_HIGH_IOCTL) {
      /* Do a lind component call */
      va_list argp;
      va_start(argp, request);
  
      int size =  va_arg(argp,int);
      void* buf = va_arg(argp, void*);
     
      result = lind_comp_rpc(request, size, buf);  
     
      va_end(argp);
    } else {
      /* if we are the system handles return ENOSYS */
       result = -ENOSYS;
    }
  } else {
    /* send to lind server. For now, don't send optional args  */
      result = -ENOSYS;//lind_ioctl_rpc(fd, request);
  }
    
  /* setup errno */
  if (result < 0) {
    __set_errno(-result);
    return -1;
  }
  return result;

}

weak_alias (__ioctl, ioctl)
