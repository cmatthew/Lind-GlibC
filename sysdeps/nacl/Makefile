
ifeq ($(subdir),io)
sysdep_routines += imc_sendmsg imc_recvmsg imc_accept lind_rpc nacl_util lind_syscalls
endif

# Remove libpthread.so's version of a function that is also provided
# by libc.so.
libpthread-routines := $(filter-out pt-vfork,$(libpthread-routines))
