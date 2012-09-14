#ifndef STRACE_H
#define STRACE_H


size_t strlen(const char *s);


int get_logging_status(void);

void set_ready_to_log(void);

void set_no_logging(void); 

void _lind_strace(const char* syscall);

ssize_t __write(int desc, void const *buf, size_t count);

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

//#define USE_TRACE

#ifdef USE_TRACE
  #define nacl_strace(x) _lind_strace(x)
  #define dbp(x)   {__write(2, __FILE__ ":" TOSTRING(__LINE__) ": " x "\n", strlen( __FILE__ ":" TOSTRING(__LINE__) ": "  x "\n")); }
  #define dbg_print(x)   { const char * dbg_message = (x);__write(2, dbg_message, strlen(dbg_message)); }
#else
  #define nacl_strace(x) 
  #define dbp(x)    {;} 
  #define dbg_print(x)    {;} 
#endif

#define print_error(x) _lind_strace(x)

#endif // STRACE_H
