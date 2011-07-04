#ifndef STRACE_H
#define STRACE_H


int get_logging_status(void);


void set_ready_to_log(void);


void nacl_strace(const char* syscall);


#define NACL_STRACE_SYSCALL 1


#endif // STRACE_H
