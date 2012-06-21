/* Copyright (C) 1999,2002,2003,2004,2005,2006 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Andreas Jaeger <aj@suse.de>.

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

#include "ifreq.h"
#include <kernel-features.h>
#include "strace.h"
#include "nacl_util.h"


void *memset(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);

struct repy_ifreq {
    int flags;
    int flags2;
    int addr; // - this is an IPv4 address in network order byte binary format
    int addr2;
    int netmask; // - in network byte order binary format
    int netmask2;
    int broadcast;
    int broadcast2;
    char name[4];
    char name2[3];
};

/* Variable to signal whether SIOCGIFCONF is not available.  */
#if __ASSUME_SIOCGIFNAME == 0 || 1
static int old_siocgifconf;
#else
# define old_siocgifconf 0
#endif

#define PREFIX "sysdeps/linux/ifreq: "

void
__ifreq (struct ifreq **ifreqs, int *num_ifs, int sockfd)
{

  int fd = sockfd;
  struct ifconf ifc;
  int rq_len;
  int nifs;
# define RQ_IFS	4

  if (fd < 0)
    fd = __opensock ();
  if (fd < 0)
      {
      *num_ifs = 0;
      *ifreqs = NULL;
      return;
    }

  ifc.ifc_buf = NULL;

  /* We may be able to get the needed buffer size directly, rather than
     guessing.  */
  ifc.ifc_buf = NULL;
  ifc.ifc_len = 39;
  rq_len = ifc.ifc_len;

  /* Read all the interfaces out of the kernel.  */
  void * newp =  realloc (ifc.ifc_buf, sizeof(struct repy_ifreq) * 2 );

  if (newp == NULL || (ifc.ifc_buf = newp, __ioctl (fd, SIOCGIFCONF, &ifc)) < 0)
      {
          free (ifc.ifc_buf);
          
          if (fd != sockfd)
              __close (fd);
          
          *num_ifs = 0;
          *ifreqs = NULL;
          return;
      }

  struct ifreq * if1 = malloc(sizeof(struct ifreq) * 2);
  memset((void*)if1, 0, sizeof(struct ifreq) * 2);

  *ifreqs = if1;
  memcpy( if1->ifr_name, "eth0", strlen("eth0") + 1);
  *num_ifs = 1;

  // Now put it all back together again.

}
