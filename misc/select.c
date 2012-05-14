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

#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>

#include <lind_syscalls.h>
#include <nacl_syscalls.h>
#include <strace.h>
#include <nacl_util.h>


/* Check the first NFDS descriptors each in READFDS (if not NULL) for read
   readiness, in WRITEFDS (if not NULL) for write readiness, and in EXCEPTFDS
   (if not NULL) for exceptional conditions.  If TIMEOUT is not NULL, time out
   after waiting the interval specified therein.  Returns the number of ready
   descriptors, or -1 for errors.  */
int
__select (nfds, readfds, writefds, exceptfds, timeout)
     int nfds;
     fd_set *readfds;
     fd_set *writefds;
     fd_set *exceptfds;
     struct timeval *timeout;
{
    __set_errno (ENOSYS);
    return -1;
    /* this works, but breaks wget. */
    /* struct timeval t; */

    /* struct select_results s; */

    /* memset(&s, 0, sizeof(struct select_results)); */

    /* t.tv_sec = 0; */
    /* t.tv_usec = 4000000;  // not a naturally occuring value (always between 1-1,000,000) so use it as a sentenal for not set */
    /* int rc = -1; */
    /* int i; */
    /* void * nothing = NULL; */

    /* int have_read = -1; */

    /* if (readfds==NULL) { */
    /*     have_read = 1; */
    /*     readfds = (fd_set*) &nothing; */
    /* } else { */
    /*     have_read = sizeof(fd_set); */
    /* } */


    /* int have_write = -1; */
    /* if (writefds==NULL) { */
    /*     have_write = 1; */
    /*     writefds = (fd_set *) &nothing; */
    /* } else { */
    /*     have_write = sizeof(fd_set); */
    /* } */

    /* int have_except = -1; */
    /* if (exceptfds==NULL) { */
    /*     have_except = 1; */
    /*     exceptfds=(fd_set*) &nothing; */
    /* } else { */
    /*     have_except = sizeof(fd_set); */
    /* } */

    /* if (timeout == NULL) { */
    /*     timeout = &t; */
    /* } */
    /* rc = lind_select_rpc(nfds, have_read, have_write, have_except,  */
    /*                      readfds, writefds, exceptfds, timeout, &s);  */

    /* if (readfds != 1) {  */
    /*     memcpy(&(s.r), readfds, sizeof(fd_set)); */
    /*  } */
    /* if (writefds != 1) {  */
    /*     memcpy(&(s.w), writefds, sizeof(fd_set)); */
    /*  } */
    /* if (exceptfds != 1) {  */
    /*     memcpy(&(s.e), exceptfds, sizeof(fd_set)); */
    /*  } */
    
    /* memcpy(&(s.used_t), timeout, sizeof(struct timeval)); */
    
    /* return rc; */
}

libc_hidden_def (__select)
weak_alias (__select, select)
