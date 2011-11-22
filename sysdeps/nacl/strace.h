#ifndef STRACE_H
#define STRACE_H


int get_logging_status(void);


void set_ready_to_log(void);

// #define USE_TRACE
void _nacl_strace(const char* syscall);

#ifdef USE_TRACE
#define nacl_strace(x) _nacl_strace(x)
#else
#define nacl_strace(x) 
#endif

void set_no_logging(void); 


#endif // STRACE_H
