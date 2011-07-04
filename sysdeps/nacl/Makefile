
ifeq ($(subdir),io)
sysdep_routines += imc_sendmsg imc_recvmsg imc_accept strace nacl_util
endif

# Remove libpthread.so's version of a function that is also provided
# by libc.so.
libpthread-routines := $(filter-out pt-vfork,$(libpthread-routines))
