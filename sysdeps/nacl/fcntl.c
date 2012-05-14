/* Copyright (C) 1991, 1995, 1996, 1997, 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>

#include "lind_syscalls.h"
#include "nacl_util.h"

/* Perform file control operations on FD.
 right now we have mapped the async and the sync version of this
function to the same thing.  when w*/
int
__fcntl (int fd, int cmd, ...) {
  if (fd < 0) {
      __set_errno (EBADF);
      return -1;
  }

  int result = -1; 
  if (is_system_handle(fd)) {
    /* Nacl did not support this, so we dont either */
    result = -ENOSYS;
  } else {
    /* for getops, send as they are, for set, grab the extra long argument */
    if (cmd == F_GETFD || cmd == F_GETFL || cmd == F_GETOWN ) {
      /* these commands don't have an arg */
      result = lind_fcntl_get_rpc(fd, cmd);
    } else if (cmd == F_SETFD || cmd == F_SETFL || cmd == F_SETOWN ) {
      /* These commands have a single long arg */
      va_list argp;
      va_start(argp, cmd);
      
      long set_op = va_arg(argp, long);
      result = lind_fcntl_set_rpc(fd, cmd, set_op);

    } else {
      /*  right now repy does not support any other commands */
      result = -ENOSYS;
    }
  }

  /*fixup return code and setup errno if needed*/
  if (result < 0) {
    __set_errno ( -result);
    return -1;
  }
  return result;


}
libc_hidden_def (__fcntl)
weak_alias (__fcntl, fcntl)
strong_alias (__fcntl, __libc_fcntl)
strong_alias (__fcntl, __fcntl_nocancel)
