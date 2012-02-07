#ifndef _LINDLOCK_H_
#define _LINDLOCK_H_

/*  Simple typed wrapper aound the NaCl locking
system calls.  */


/* A handle to a system lock */
typedef int lindlock;

/*  Allocate a new lock. */
lindlock lindlock_init(void);

/* Lock the lock. */
void lindlock_lock(lindlock l);


/* Unlock the lock.*/
void lindlock_unlock(lindlock l);


#endif /* _LINDLOCK_H_ */

