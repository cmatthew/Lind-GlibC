#ifndef _LIND_SYSCALLS_H_
#define _LIND_SYSCALLS_H_




#include <sys/types.h>
/* #include <kernel_stat.h> */
#include <nacl_stat.h>
#include <sys/statfs.h>
#include <sys/stat.h>
#include <unistd.h>


struct select_results {
    struct timeval used_t;
    fd_set r;
    fd_set w;
    fd_set e;
};




/* int lind_open_rpc (const char * filename, int flags, int mode); */
/* int lind_read_rpc(int handle, int size, void * where_to); */
/* int lind_lseek_rpc(int fd, off_t offset, int whence, off_t* ret); */
/* int lind_fxstat_rpc(int fd, int vers, struct stat *buf); */
/* int lind_fstatfs_rpc (int fd, struct statfs *buf); */
/* int lind_close_rpc(int fd); */
/* ssize_t lind_write_rpc(int desc, void const *buf, size_t count); */
int lind_ioctl_rpc (int fd, unsigned long int request, ...);
/* int lind_access_rpc(const char * file, int type); */
/* int lind_unlink_rpc(const char * name); */
/* int lind_link_rpc(const char * from, const char * to); */
/* int lind_chdir_rpc(const char * name); */
/* int lind_mkdir_rpc (const char* path, mode_t mode); */
/* int lind_rmdir_rpc (const char* path); */
/* int lind_noop_rpc (void); */
/* int lind_getpid_rpc(pid_t * pid_buf ); */
/* int lind_xstat_rpc (int version, const char *path, struct stat *buf); */
/* ssize_t lind_getdents_rpc(int fd, char *buf, size_t nbytes); */
int lind_comp_rpc(int request, int nbytes, void *buf);

/* int lind_dup_rpc(int oldfd); */
/* int lind_dup2_rpc(int oldfd, int newfd); */
int lind_fcntl_rpc (int fd, int cmd, long set_op);
/* int lind_statfs_rpc (const char * path, struct statfs *buf); */


#include "lind_rpc_gen.h"

#endif /* _LIND_SYSCALLS_H_ */

