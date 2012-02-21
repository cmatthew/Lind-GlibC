#ifndef STRACE_H
#define STRACE_H


int get_logging_status(void);

void set_ready_to_log(void);

void set_no_logging(void); 

void _lind_strace(const char* syscall);


/* #define USE_TRACE  */

#ifdef USE_TRACE
#define nacl_strace(x) _lind_strace(x)
#else
#define nacl_strace(x) 
#endif

#define print_error(x) _lind_strace(x)

#endif // STRACE_H
