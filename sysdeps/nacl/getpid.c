#include <unistd.h>
#include <errno.h>
#include <nacl_syscalls.h>
#include "nacl_util.h"
#include "strace.h"
#include "lind_rpc.h"

pid_t __getpid ()
{
  pid_t lind_pid = -1;
  nacl_strace("getpid");

  const lind_rpc_status rc = depricated_nacl_rpc_syscall(NACL_sys_getpid, "", 0, NULL, &lind_pid, 0);
  if (rc != RPC_OK) {
    nacl_strace("getpid - failed!");
    return -1;
  }
  return lind_pid;
}
libc_hidden_def (__getpid)
weak_alias (__getpid, getpid)
libc_hidden_def (getpid)
