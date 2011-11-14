#include <sys/types.h>

int lind_open_rpc (const char * filename, int flags, int mode);
int lind_read_rpc(int handle, int size, void * where_to);
int lind_lseek_rpc(int fd, off_t offset, int whence);
int lind_fstat_rpc(int fd, struct statfs *buf);
int lind_close_rpc(int fd);
ssize_t lind_write_rpc(int desc, void const *buf, size_t count);
int lind_ioctl_rpc (int fd, unsigned long int request, ...);
int lind_access_rpc(const char * file, int type);
int lind_unlink_rpc(const char * name);
int lind_link_rpc(const char * from, const char * to);
int lind_chdir_rpc(const char * name);
int lind_mkdir_rpc (const char* path, mode_t mode);
int lind_rmdir_rpc (const char* path);


