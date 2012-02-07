#include "lindlock.h"
#include "nacl_syscalls.h"


lindlock lindlock_init() {
  return NACL_SYSCALL (mutex_create) ();
}


void lindlock_lock(lindlock l) {
    NACL_SYSCALL (mutex_lock) (l);
}

void lindlock_unlock(lindlock l) {
	NACL_SYSCALL (mutex_unlock) (l);
}
