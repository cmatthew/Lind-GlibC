#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>

#include "lind_rpc.h"
#include "lind_syscalls.h"
#include "strace.h"
#include "nacl_util.h"
#include "nacl_syscalls.h"
#include "component.h"
/* #include <kernel_stat.h> */
#include <nacl_stat.h>
#include <sys/statfs.h>
#include <sys/stat.h>


#define MAX_FILENAME_LENGTH 512

#define FMT_INT "<i"
#define FMT_UINT "<I"
#define FMT_LONG "<q"
#define FMT_ULONG "<Q"
#define FMT_STR(len) #len "s"


/* struct lind_open_rpc_s { */
/*   int flags; */
/*   int mode; */
/*   size_t filename_len; */
/* }; */

/* struct lind_rw_rpc_s { */
/*   int handle; */
/*   int size; */
/* }; */


/* struct lind_lseek_rpc_s { */
/*   int fd; */
/*   int whence; */
/*   off_t offset; */

/* }; */

struct lind_fd_rpc_s {
    int fd;
};



/** Send an open request to the lind server.
 filename is the string path to the file to open*/
/* int lind_open_rpc (const char * filename, int flags, int mode) { */

/*   lind_request request; */
/*   memset(&request, 0, sizeof(request)); */

/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */

/*   struct lind_open_rpc_s args; */
/*   memset(&args, 0, sizeof(struct lind_open_rpc_s)); */
  
/*   /\* setup arguments *\/ */
/*   int return_code = -1; */
/*   args.flags = flags; */
/*   args.mode = mode; */
/*   size_t filename_len = strlen(filename); */
/*   args.filename_len = filename_len; */
/*   size_t filename_siz = filename_len + 1;  */
 
/*   const char * filename_len_str = nacl_itoa(args.filename_len); */
/*   /\* <i<i<i(STRLEN)s *\/ */
/*   request.format = combine(5, FMT_INT, FMT_INT, FMT_INT, filename_len_str, "s"); */
/*   request.call_number = NACL_sys_open; */

/*   request.message.len = sizeof(struct lind_open_rpc_s); */
/*   request.message.body = &args; */
  
/*   nacl_rpc_syscall_proxy(&request, &reply, 1, filename, filename_siz); */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*   } */


/*   return return_code; */

/* } */


/* int lind_read_rpc(int handle, int size, void * where_to) { */

/*   lind_request request; */
/*   memset(&request, 0, sizeof(request)); */
/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */
/*   struct lind_rw_rpc_s args; */
/*   memset(&args, 0, sizeof(struct lind_rw_rpc_s)); */

/*   int return_code = -1; */
/*   args.handle = handle; */
/*   args.size = size; */

/*   request.call_number = NACL_sys_read; */
/*   request.format = "<i<i"; */

/*   request.message.len = sizeof(struct lind_rw_rpc_s); */
/*   request.message.body = &args; */
 

/*   nacl_rpc_syscall_proxy(&request, &reply, 0); */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*     assert( CONTENTS_SIZ(reply) <= size); */
/*     memcpy( where_to, reply.contents, return_code); */
/*   } */

/*   return return_code; */

/* } */



/* struct lind_fxstat_rpc_s { */
/*   int fd; */
/*   int vers; */
/* }; */


/* int lind_fxstat_rpc(int fd, int vers, struct stat *buf) { */

/*   lind_request request; */
/*   memset(&request, 0, sizeof(request)); */
/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */
/*   struct lind_fxstat_rpc_s args; */
/*   memset(&args, 0, sizeof(struct lind_fxstat_rpc_s)); */

/*   int return_code = -1; */
/*   args.fd = fd; */
/*   args.vers = vers; */

/*   request.call_number = NACL_sys_fstat; */
/*   request.format = FMT_INT FMT_INT; */

/*   request.message.len = sizeof(struct lind_fxstat_rpc_s); */
/*   request.message.body = &args; */
 
/*   nacl_rpc_syscall_proxy(&request, &reply, 0); */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*     assert( CONTENTS_SIZ(reply) == sizeof(struct stat)); */
/*     memcpy(buf, reply.contents, sizeof(struct stat)); */
/*   } */

/*   return return_code; */

/* } */



/* lseek system call.   Has three arguments (int fd, off_t offset, int whence)
 * returns a off_t (__SQUAD_TYPE) which is a 64 bit signed type. */
/* int lind_lseek_rpc(int fd, off_t offset, int whence, off_t* ret_loc) { */

/*   lind_request request; */
/*   memset(&request, 0, sizeof(request)); */

/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */

/*   struct lind_lseek_rpc_s args; */
/*   memset(&args, 0, sizeof(struct lind_lseek_rpc_s)); */

/*   int return_code = -1; */
/*   args.fd = fd; */
/*   args.offset = offset; */
/*   args.whence = whence; */

/*   request.call_number = NACL_sys_lseek; */
/*   /\* int fd, int whence, long (as string) *\/ */
/*   request.format = FMT_INT FMT_INT FMT_LONG; */

/*   request.message.len = sizeof( struct lind_lseek_rpc_s ); */
/*   request.message.body = &args; */
 
/*   nacl_rpc_syscall_proxy(&request, &reply, 0); */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*     assert( CONTENTS_SIZ(reply) == sizeof(off_t)); */
/*     memcpy(ret_loc, reply.contents, CONTENTS_SIZ(reply)); */

/*   } */
  
/*   return return_code; */

/* } */


/* int lind_close_rpc(int fd) { */
/*   lind_request request; */
/*   memset(&request, 0, sizeof(request)); */
/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */
/*   struct lind_fd_rpc_s args; */
/*   memset(&args, 0, sizeof(struct lind_fd_rpc_s)); */

/*   int return_code = -1; */
/*   args.fd = fd; */

/*   request.call_number = NACL_sys_close; */
/*   request.format = "<i"; */

/*   request.message.len = sizeof(struct lind_fd_rpc_s); */
/*   request.message.body = &args; */
 
/*   nacl_rpc_syscall_proxy(&request, &reply, 0); */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*   } */
  
/*   return return_code; */

/* } */

/* ssize_t lind_write_rpc(int desc, void const *buf, size_t count) { */
/*   lind_request request; */
/*   memset(&request, 0, sizeof(request)); */
/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */
/*   struct lind_rw_rpc_s args; */
/*   memset(&args, 0, sizeof(struct lind_rw_rpc_s)); */
/*   int return_code = -1; */
/*   args.handle = desc; */
/*   args.size = count; */
/*   request.call_number = NACL_sys_write; */
/*   request.format = concat(concat("<i<I", nacl_itoa(count-1)),"s");  */
/*   request.message.len = sizeof(struct lind_rw_rpc_s); */
/*   request.message.body = &args; */
/*   nacl_rpc_syscall_proxy(&request, &reply, 1, buf, count); */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*   } */


/*   return return_code; */
/* } */


struct lind_ioctl_rpc_s {
  int fd;
  unsigned long int request;
};



int lind_ioctl_rpc (int fd, unsigned long int ioctl_request, ...) {

  lind_request request;
  memset(&request, 0, sizeof(request));
  lind_reply reply;
  memset(&reply, 0, sizeof(reply));
  struct lind_ioctl_rpc_s args;
  memset(&args, 0, sizeof(struct lind_ioctl_rpc_s));

  int return_code = -1;
  args.fd = fd;
  args.request = ioctl_request;

  request.call_number = NACL_sys_ioctl;
  request.format = FMT_INT FMT_ULONG;

  request.message.len = sizeof(struct lind_ioctl_rpc_s);
  request.message.body = &args;
 
  nacl_rpc_syscall_proxy(&request, &reply, 0);

  /* on error return negative so we can set ERRNO. */  
  if (reply.is_error) {
    return_code = reply.return_code * -1;
  } else {
    return_code = reply.return_code;
  }
  
  return return_code;

}


/* struct lind_access_rpc_s { */
/*   int type; */
/* }; */



/* /\** Send access call to RePy via lind RPC.  file is the name of the file to check */
/*  type is the access mode ( W_OK, R_OK, X_OK or an | of them). *\/ */
/* int lind_access_rpc (const char * file, int type) { */

/*   lind_request request; */
/*   memset(&request, 0, sizeof(request)); */
/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */
/*   struct lind_access_rpc_s args; */
/*   memset(&args, 0, sizeof(struct lind_access_rpc_s)); */

/*   int return_code = -1; */
/*   args.type = type; */
/*   request.call_number = NACL_sys_access; */


/*   /\* Now build the format string which is <iLENGTHs *\/ */
/*   size_t file_name_length = strlen(file); */
/*   size_t file_name_size = file_name_length + 1; /\* size in bytes *\/ */
/*   const char * str_len = nacl_itoa(file_name_length); */
/*   const char * str_len_s = concat(str_len,"s"); */
/*   request.format = concat(FMT_INT, str_len_s); */
/*   free( (void*) str_len); */
/*   free( (void*) str_len_s); */

/*   request.message.len = sizeof( struct lind_access_rpc_s ); */
/*   request.message.body = &args; */
 
/*   nacl_rpc_syscall_proxy(&request, &reply, 1, file, file_name_size); */

/*   /\* concat malloced this earlier. *\/ */
/*   free( (void*)request.format ); */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*   } */
  
/*   return return_code; */

/* } */


/** Send unlink call to RePy via lind RPC.  Name is a path */
/* int lind_unlink_rpc (const char * name) { */

/*   lind_request request; */
/*   memset(&request, 0, sizeof(request)); */
/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */
  
/*   int return_code = -1; */
/*   request.call_number = NACL_sys_unlink; */

/*   /\* Now build the format string which is LENGTHs *\/ */
/*   size_t file_name_length = strlen(name); */
/*   size_t file_name_size = file_name_length + 1; /\* size in bytes *\/ */
/*   const char * str_len = nacl_itoa(file_name_length); */
/*   const char * str_len_s = combine(2, str_len, "s");  */
/*   request.format = str_len_s; */

/*   request.message.len = 0; */
/*   request.message.body = NULL; */
 
/*   nacl_rpc_syscall_proxy(&request, &reply, 1, name, file_name_size); */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*   } */
  
/*   return return_code; */

/* } */


/* /\** Send link call to RePy via lind RPC.  old name and new name are file paths*\/ */
/* int lind_link_rpc (const char * from, const char * to) { */

/*   lind_request request; */
/*   memset(&request, 0, sizeof(request)); */
/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */
  
/*   int return_code = -1; */
  
/*   request.call_number = NACL_sys_link; */

/*   /\* Now build the format string which is LENGTHsLENGTHs *\/ */
/*   size_t from_length = strlen(from); */
/*   size_t from_size = from_length + 1; /\* size in bytes *\/ */
/*   const char * str_len = nacl_itoa(from_length); */

/*   size_t to_length = strlen(to); */
/*   size_t to_size = to_length + 1; /\* size in bytes *\/ */
/*   const char * new_str_len = nacl_itoa(to_length); */


/*   request.format = combine(4, str_len, "s", new_str_len, "s"); */
/*   /\* free( (void*) str_len); *\/ */
/*   /\* free( (void*) str_len_s); *\/ */
/*   /\* free( (void*) new_str_len_s); *\/ */
/*   /\* free( (void*) new_str_len); *\/ */

/*   request.message.len = 0; */
/*   request.message.body = NULL; */
 
/*   nacl_rpc_syscall_proxy(&request, &reply, 2, from, from_size, to, to_size); */

/*   /\* concat malloced this earlier. *\/ */
/*   /\* free( (void*)request.format ); *\/ */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*   } */
  
/*   return return_code; */

/* } */


/* /\** Send chdir call to RePy via lind RPC.  Name is a path *\/ */
/* int lind_chdir_rpc (const char * name) { */

/*   lind_request request; */
/*   memset(&request, 0, sizeof(request)); */
/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */

/*   int return_code = -1; */
/*   request.call_number = NACL_sys_chdir; */

/*   /\* Now build the format string which is LENGTHs *\/ */
/*   size_t file_name_length = strlen(name); */
/*   size_t file_name_size = file_name_length + 1; /\* size in bytes *\/ */
/*   const char * str_len = nacl_itoa(file_name_length); */
/*   const char * str_len_s = combine(2, str_len, "s");  */
/*   request.format = str_len_s; */

/*   request.message.len = 0; */
/*   request.message.body = NULL; */
 
/*   nacl_rpc_syscall_proxy(&request, &reply, 1, name, file_name_size); */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*   } */
  
/*   return return_code; */

/* } */

/* struct lind_mkdir_rpc_s { */
/*   mode_t mode; */
/* }; */


/* /\** Send mkdir call to RePy via rind RPC.  Path is a path *\/ */
/* int lind_mkdir_rpc (const char * path, mode_t mode) { */

/*   lind_request request; */
/*   memset(&request, 0, sizeof(request)); */
/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */

/*   struct lind_mkdir_rpc_s args; */
/*   memset(&args, 0, sizeof(struct lind_mkdir_rpc_s)); */


/*   int return_code = -1; */
/*   request.call_number = NACL_sys_mkdir; */
  
/*   args.mode = mode; */

/*   /\* Now build the format string which is LENGTHs *\/ */
/*   size_t file_path_length = strlen(path); */
/*   size_t file_path_size = file_path_length + 1; /\* size in bytes *\/ */
/*   const char * str_len = nacl_itoa(file_path_length); */
/*   const char * str_len_s = combine(3, FMT_INT, str_len, "s");  */
/*   request.format = str_len_s; */

/*   request.message.len = sizeof(args); */
/*   request.message.body = &args; */
 
/*   nacl_rpc_syscall_proxy(&request, &reply, 1, path, file_path_size); */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*   } */
  
/*   return return_code; */
/* } */


/* /\** Send rmdir call to RePy via Lind RPC.  Path is a path *\/ */
/* int lind_rmdir_rpc (const char * path) { */

/*   lind_request request; */
/*   memset(&request, 0, sizeof(request)); */
/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */
  
/*   int return_code = -1; */
/*   request.call_number = NACL_sys_rmdir; */

/*   /\* Now build the format string which is LENGTHs *\/ */
/*   size_t file_path_length = strlen(path); */
/*   size_t file_path_size = file_path_length + 1; /\* size in bytes *\/ */
/*   const char * str_len = nacl_itoa(file_path_length); */
/*   const char * str_len_s = combine(2, str_len, "s");  */
/*   request.format = str_len_s; */

/*   request.message.len = 0; */
/*   request.message.body = NULL; */
 
/*   nacl_rpc_syscall_proxy(&request, &reply, 1, path, file_path_size); */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*   } */
  
/*   return return_code; */

/* } */


/** Send noop call to RePy via lind RPC.  Path is a path */
/* int lind_noop_rpc (void) { */

/*   lind_request request; */
/*   memset(&request, 0, sizeof(request)); */
/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */
  
/*   int return_code = -1; */
/*   request.call_number = NACL_sys_null; */
/*   request.format = (char*) ""; */

/*   request.message.len = 0; */
/*   request.message.body = NULL; */
 
/*   nacl_rpc_syscall_proxy(&request, &reply, 0); */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*   } */
  
/*   return return_code; */

/* } */


/** Send getpid call to RePy via lind RPC.  Path is a path */
/* int lind_getpid_rpc (pid_t* buffer) { */

/*   lind_request request; */
/*   memset(&request, 0, sizeof(request)); */
/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */
  
/*   int return_code = -1; */
/*   request.call_number = NACL_sys_getpid; */
/*   request.format = (char*) ""; */

/*   request.message.len = 0; */
/*   request.message.body = NULL; */
 
/*   nacl_rpc_syscall_proxy(&request, &reply, 0); */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*     assert(CONTENTS_SIZ(reply) == sizeof(pid_t)); */
/*     memcpy(buffer, reply.contents, sizeof(pid_t)); */
/*   } */
  
/*   return return_code; */

/* } */

/* struct lind_xstat_rpc_s { */
/*   int version; */
/* }; */


/* int lind_xstat_rpc (int version, const char *path, struct stat *buf) { */

/*   lind_request request; */
/*   memset(&request, 0, sizeof(request)); */

/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */

/*   struct lind_xstat_rpc_s args; */
/*   memset(&args, 0, sizeof(struct lind_xstat_rpc_s)); */
  
/*   /\* setup arguments *\/ */
/*   int return_code = -1; */
/*   args.version = version; */
/*   size_t path_len = strlen(path); */
/*   size_t path_siz = path_len + 1;  */
 
/*   const char * path_len_str = nacl_itoa(path_len); */
/*   /\* <i(STRLEN)s *\/ */
/*   request.format = combine(3, FMT_INT, path_len_str, "s"); */
/*   request.call_number = NACL_sys_xstat; */

/*   request.message.len = sizeof(struct lind_xstat_rpc_s); */
/*   request.message.body = &args; */
  
/*   nacl_rpc_syscall_proxy(&request, &reply, 1, path, path_siz); */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*     assert( CONTENTS_SIZ(reply) == sizeof(struct stat)); */
/*     memcpy(buf, reply.contents, CONTENTS_SIZ(reply)); */
/*   } */

/*   return return_code; */
/* } */


/* struct lind_getdents_rpc_s { */
/*   int fd; */
/*   size_t nbytes; */
/* }; */

/* /\** For now we exclude basep. */
/*  *\/ */
/* ssize_t lind_getdents_rpc (int fd, char *buf, size_t nbytes) { */

/*  lind_request request; */
/*   memset(&request, 0, sizeof(request)); */

/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */

/*   struct lind_getdents_rpc_s args; */
/*   memset(&args, 0, sizeof(struct lind_getdents_rpc_s)); */
  
/*   /\* setup arguments *\/ */
/*   int return_code = -1; */
/*   args.fd = fd; */
/*   args.nbytes = nbytes; */
 
/*   /\* <i(STRLEN)s *\/ */
/*   request.format = FMT_INT FMT_INT; */
/*   request.call_number = NACL_sys_getdents; */

/*   request.message.len = sizeof(struct lind_getdents_rpc_s); */
/*   request.message.body = &args; */
  
/*   nacl_rpc_syscall_proxy(&request, &reply, 0); */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*     assert( CONTENTS_SIZ(reply) <= nbytes); */
/*     memcpy(buf, reply.contents, CONTENTS_SIZ(reply)); */
/*   } */

/*   return return_code; */

/* } */


#define MIN(X,Y) ( (X) < (Y) ? (X) : (Y))

int lind_comp_rpc(int request_num, int nbytes, void *buf) {
  int return_code = -1;

  /* lind_request request; */
  /* memset(&request, 0, sizeof(request)); */

  /* lind_reply reply; */
  /* memset(&reply, 0, sizeof(reply)); */

  /* switch(request_num) { */

  /*   case LIND_CIA_IOCTL: */
  /*     request.format = FMT_INT FMT_INT; */
  /*     request.call_number = NACL_sys_comp_cia; */
  /*     request.message.len = nbytes; */
  /*     request.message.body = buf; */
  /*     nacl_rpc_syscall_proxy(&request, &reply, 0); */
  /*     break; */

  /*   case LIND_CALL_IOCTL:; */
  /*     struct call_s * call_args = (struct call_s *) buf; */
  /*     /\* string part of struct is everything execpt the first 2 ints, and the null terminator. *\/ */
  /*     request.format = combine(3, FMT_INT FMT_UINT FMT_UINT, nacl_itoa((int)(call_args->siz)-1), "s"); */
  /*     request.call_number = NACL_sys_comp_call; */
  /*     request.message.len = sizeof(struct call_s); */
  /*     request.message.body = buf; */
  /*     nacl_rpc_syscall_proxy(&request, &reply, 1, call_args->message, call_args->siz); */
  /*     break; */

  /*   case LIND_ACCEPT_IOCTL: */
  /*     request.format = FMT_INT FMT_INT FMT_INT; */
  /*     request.call_number = NACL_sys_comp_accept; */
  /*     request.message.len = nbytes; */
  /*     request.message.body = buf; */
  /*     nacl_rpc_syscall_proxy(&request, &reply, 0); */
  /*     break; */
  /* } */

  /* /\* on error return negative so we can set ERRNO. *\/   */
  /* if (reply.is_error) { */
  /*   return_code = reply.return_code * -1; */
  /* } else { */
  /*   return_code = reply.return_code; */

  /*   if (request_num == LIND_ACCEPT_IOCTL) { */
      
  /*     struct comp_accept_s * accept_args = (struct comp_accept_s *) buf; */
  /*     int content_size = return_code; */
  /*     int buff_size = accept_args->max; */
  /*     int min_size = MIN(content_size, buff_size); */
  /*     assert( CONTENTS_SIZ(reply) <= min_size); */
  /*     memcpy(accept_args->buff, reply.contents, min_size); */
  /*   } */
  /* } */
  
  return return_code;
}


/* int lind_fstatfs_rpc (int fd, struct statfs *buf) { */
/*   lind_request request; */
/*   memset(&request, 0, sizeof(request)); */

/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */

/*   struct lind_fd_rpc_s args; */
/*   memset(&args, 0, sizeof(args)); */
  
/*   /\* setup arguments *\/ */
/*   int return_code = -1; */
/*   args.fd = fd; */

/*   request.format = FMT_INT; */
/*   request.call_number = NACL_sys_fstatfs; */

/*   request.message.len = sizeof(struct lind_fd_rpc_s); */
/*   request.message.body = &args; */
  
/*   nacl_rpc_syscall_proxy(&request, &reply, 0); */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*     assert( CONTENTS_SIZ(reply) == sizeof(struct statfs)); */
/*     memcpy(buf, reply.contents, CONTENTS_SIZ(reply)); */
/*   } */

/*   return return_code; */
/* } */


/* int lind_dup_rpc(int oldfd) { */
/*   lind_request request; */
/*   memset(&request, 0, sizeof(request)); */
/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */
/*   struct lind_fd_rpc_s args; */
/*   memset(&args, 0, sizeof(struct lind_fd_rpc_s)); */

/*   int return_code = -1; */
/*   args.fd = oldfd; */

/*   request.call_number = NACL_sys_dup; */
/*   request.format = "<i"; */

/*   request.message.len = sizeof(struct lind_fd_rpc_s); */
/*   request.message.body = &args; */
 
/*   nacl_rpc_syscall_proxy(&request, &reply, 0); */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*   } */
  
/*   return return_code; */

/* } */


/* struct lind_dup2_rpc_s { */
/*   int fd; */
/*   int fd2; */
/* }; */



/* int lind_dup2_rpc(int oldfd, int newfd) { */
/*   lind_request request; */
/*   memset(&request, 0, sizeof(request)); */
/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */
/*   struct lind_dup2_rpc_s args; */
/*   memset(&args, 0, sizeof(struct lind_dup2_rpc_s)); */

/*   int return_code = -1; */
/*   args.fd = oldfd; */
/*   args.fd2 = newfd; */

/*   request.call_number = NACL_sys_dup2; */
/*   request.format = FMT_INT FMT_INT; */

/*   request.message.len = sizeof(struct lind_dup2_rpc_s); */
/*   request.message.body = &args; */
 
/*   nacl_rpc_syscall_proxy(&request, &reply, 0); */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*   } */
  
/*   return return_code; */

/* } */


struct lind_fcntl_rpc_s {
  int fd;
  int cmd;
};



int lind_fcntl_noop_rpc (int fd, int cmd)  {

  lind_request request;
  memset(&request, 0, sizeof(request));
  lind_reply reply;
  memset(&reply, 0, sizeof(reply));
  struct lind_fcntl_rpc_s args;
  memset(&args, 0, sizeof(struct lind_fcntl_rpc_s));

  int return_code = -1;
  args.fd = fd;
  args.cmd = cmd;

  request.call_number = NACL_sys_fcntl;
  request.format = FMT_INT FMT_INT;

  request.message.len = sizeof(struct lind_fcntl_rpc_s);
  request.message.body = &args;
 
  nacl_rpc_syscall_proxy(&request, &reply, 0);

  /* on error return negative so we can set ERRNO. */  
  if (reply.is_error) {
    return_code = reply.return_code * -1;
  } else {
    return_code = reply.return_code;
  }
  
  return return_code;

}

struct lind_fcntl_op_rpc_s {
  int fd;
  int cmd;
  long set_op;
};


int lind_fcntl_op_rpc (int fd, int cmd, long set_op)  {

  lind_request request;
  memset(&request, 0, sizeof(request));
  lind_reply reply;
  memset(&reply, 0, sizeof(reply));
  struct lind_fcntl_op_rpc_s args;
  memset(&args, 0, sizeof(struct lind_fcntl_op_rpc_s));

  int return_code = -1;
  args.fd = fd;
  args.cmd = cmd;
  args.set_op = set_op;

  request.call_number = NACL_sys_fcntl;
  request.format = FMT_INT FMT_INT FMT_LONG;

  request.message.len = sizeof(struct lind_fcntl_op_rpc_s);
  request.message.body = &args;
 
  nacl_rpc_syscall_proxy(&request, &reply, 0);

  /* on error return negative so we can set ERRNO. */  
  if (reply.is_error) {
    return_code = reply.return_code * -1;
  } else {
    return_code = reply.return_code;
  }
  return return_code;
}

int lind_fcntl_rpc (int fd, int cmd, long set_op)  {
  /* We break this one up to send with and without the argument. */
  if (cmd == F_GETFD || cmd == F_GETFL || cmd == F_GETOWN ) {
    /* these commands don't have an arg */
    return lind_fcntl_noop_rpc(fd, cmd);

  } else if (cmd == F_SETFD || cmd == F_SETFL || cmd == F_SETOWN ) {
    /* These commands have a single long arg */
    return lind_fcntl_op_rpc(fd, cmd, set_op);
  } else {
    return -1;
  }

}


/* int lind_statfs_rpc (const char * path, struct statfs *buf) { */

/*   lind_request request; */
/*   memset(&request, 0, sizeof(request)); */
/*   lind_reply reply; */
/*   memset(&reply, 0, sizeof(reply)); */

/*   int return_code = -1; */
/*   request.call_number = NACL_sys_statfs; */

/*   /\* Now build the format string which is LENGTHs *\/ */
/*   size_t file_name_length = strlen(path); */
/*   size_t file_name_size = file_name_length + 1; /\* size in bytes *\/ */
/*   const char * str_len = nacl_itoa(file_name_length); */
/*   const char * str_len_s = combine(2, str_len, "s");  */
/*   request.format = str_len_s; */

/*   request.message.len = 0; */
/*   request.message.body = NULL; */
 
/*   nacl_rpc_syscall_proxy(&request, &reply, 1, path, file_name_size); */

/*   /\* on error return negative so we can set ERRNO. *\/   */
/*   if (reply.is_error) { */
/*     return_code = reply.return_code * -1; */
/*   } else { */
/*     return_code = reply.return_code; */
/*     assert( CONTENTS_SIZ(reply) == sizeof(struct statfs)); */
/*     memcpy(buf, reply.contents, CONTENTS_SIZ(reply)); */
/*   } */
  
/*   return return_code; */

/* } */



