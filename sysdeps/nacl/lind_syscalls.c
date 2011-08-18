#include "lind_rpc.h"
#include "lind_syscalls.h"
#include <string.h>
#include "strace.h"
#include "nacl_util.h"
#include "nacl_syscalls.h"

#define MAX_FILENAME_LENGTH 512

struct lind_open_rpc_s {
  int flags;
  int mode;
  int filename_len;
  char filename[MAX_FILENAME_LENGTH];
};

int lind_open_rpc (const char * filename, int flags, int mode) {
  int fd = -1;
  struct lind_open_rpc_s args;
  memset(&args, 0, sizeof(struct lind_open_rpc_s));
  args.flags = flags;
  args.mode = mode;
  args.filename_len = strlen(filename);
  strncpy((char*)&(args.filename), filename, MAX_FILENAME_LENGTH - 1);

  lind_rpc_status rc = nacl_rpc_syscall( NACL_sys_open, "<i<i<i511s",
					 sizeof(struct lind_open_rpc_s), &args,
					 &fd);

  nacl_strace(concat("lind rpc got: ", nacl_itoa(fd)));
  return fd;
}
