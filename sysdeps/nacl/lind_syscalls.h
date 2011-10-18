int lind_open_rpc (const char * filename, int flags, int mode);
int lind_read_rpc(int handle, int size, void * where_to);
int lind_fstat_rpc(int fd, struct statfs *buf);
int lind_close_rpc(int fd);
ssize_t lind_write_rpc(int desc, void const *buf, size_t count);
