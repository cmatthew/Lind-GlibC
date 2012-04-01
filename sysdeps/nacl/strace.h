#ifndef STRACE_H
#define STRACE_H


int get_logging_status(void);

void set_ready_to_log(void);

void set_no_logging(void); 

void _lind_strace(const char* syscall);

ssize_t __write(int desc, void const *buf, size_t count);

#define dbg_print(x)   { const char * dbg_message = (x);__write(2, dbg_message, strlen(dbg_message)); }


/*   #define USE_TRACE  */

#ifdef USE_TRACE
#define nacl_strace(x) _lind_strace(x)
#else
#define nacl_strace(x) 
#endif

#define print_error(x) _lind_strace(x)

#endif // STRACE_H
