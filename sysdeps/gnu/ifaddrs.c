/* getifaddrs -- get names and addresses of all network interfaces
   Copyright (C) 2002, 2003 Free Software Foundation, Inc.
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

#include <ifaddrs.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>

#include "strace.h"
#include "nacl_util.h"
#include "lind_syscalls.h"

#include "ifreq.h"


void *memset(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);

// this should match the defintion in seattlelib/net_getifaddrs.repy
struct repy_ifreq {
    int flags;
    int flags2;
    int addr; // - this is an IPv4 address in network order byte binary format
    int addr2;
    int netmask; // - in network byte order binary format
    int netmask2;
    int broadcast;
    int broadcast2;
    char name1[5];
    char name2[4];
};


/* Create a linked list of `struct ifaddrs' structures, one for each
   network interface on the host machine.  If successful, store the
   list in *IFAP and return 0.  On errors, return -1 and set `errno'.  */
int
getifaddrs (struct ifaddrs **ifap)
{
    struct repy_ifreq r;
    struct ifaddrs * storage1 = NULL;
    struct ifaddrs * storage2 = NULL;
    memset(&r, 0, sizeof(struct repy_ifreq));
    int result = 0;

    result = lind_getifaddrs_rpc(sizeof(struct repy_ifreq), &r);

    if ( result != 0 ) {
        return -1;
    }

    // make two ifaddrs structs
    storage1 = malloc(sizeof(struct ifaddrs)); 
    storage2 = malloc(sizeof(struct ifaddrs));

    if (storage2 == NULL || storage1 == NULL) {
        return -1;
    }

    memset(storage1, 0, sizeof(struct ifaddrs));
    memset(storage2, 0, sizeof(struct ifaddrs));
    storage1->ifa_next = storage2;

    // set up names
    storage1->ifa_name = strdup(r.name1);
    storage2->ifa_name = strdup(r.name2 ); 

    // add in IP addresses
    storage1->ifa_addr = malloc(sizeof(struct sockaddr));
    storage1->ifa_addr->sa_family = AF_INET;
    struct sockaddr_in * s4 = (struct sockaddr_in *)(storage1->ifa_addr);
    memcpy(&(s4->sin_addr), &(r.addr),sizeof(int));

    storage2->ifa_addr = malloc(sizeof(struct sockaddr));
    storage2->ifa_addr->sa_family = AF_INET;
    s4 = (struct sockaddr_in *)(storage2->ifa_addr);
    memcpy(&(s4->sin_addr), &(r.addr2),sizeof(int));

    // set the adapters to up
    storage1->ifa_flags = storage1->ifa_flags | IFF_UP;
    storage2->ifa_flags = storage2->ifa_flags | IFF_UP;

    // set loopback on one
    storage2->ifa_flags = storage2->ifa_flags | IFF_LOOPBACK;

    // set netmask
    storage1->ifa_netmask = malloc(sizeof(struct sockaddr));
    storage1->ifa_netmask->sa_family = AF_INET;
    s4 = (struct sockaddr_in *)(storage1->ifa_netmask);
    memcpy(&(s4->sin_addr), &(r.netmask),sizeof(int));

    storage2->ifa_netmask = malloc(sizeof(struct sockaddr));
    storage2->ifa_netmask->sa_family = AF_INET;
    s4 = (struct sockaddr_in *)(storage2->ifa_netmask);
    memcpy(&(s4->sin_addr), &(r.netmask2),sizeof(int));

    
   *ifap = storage1;
    return 0;


}
#ifndef getifaddrs
libc_hidden_def (getifaddrs)
#endif

void
freeifaddrs (struct ifaddrs *ifa)
{
    free(ifa->ifa_next->ifa_name);
    free(ifa->ifa_next->ifa_addr);
    free(ifa->ifa_next->ifa_netmask);

    free(ifa->ifa_name);
    free(ifa->ifa_addr);
    free(ifa->ifa_netmask);

    free(ifa->ifa_next);
    free (ifa);
}
libc_hidden_def (freeifaddrs)
