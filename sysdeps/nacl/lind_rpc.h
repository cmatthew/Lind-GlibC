#ifndef _lind_rpc_h_
#define _lind_rpc_h_

typedef enum lind_rpc_status_e {RPC_OK = 0, RPC_WRITE_ERROR, RPC_READ_ERROR, RPC_ARGS_ERROR, RPC_PROTOCOL_ERROR} lind_rpc_status;

/* This number is checked on the other side to make sure we are sending valid data. */
#define MAGIC 101010

/* The size of the send and recv buffers. As far as I can tell, this is the 
 unix domain sockets biggest. */
#define buf_siz (4096*4)

/* When we are sending small messages, no point in allocating a huge buffer. */
#define small_buf_siz 32

struct lind_message {
  unsigned int len;		/* size of the message in bytes */
  void * body;			/* location of the body of the message */
};

typedef struct lind_rpc_request {
  unsigned int call_number;
  const char * format;
  struct lind_message message;
} lind_request;

typedef struct lind_rpc_reply {
  int message_size; 		/* size of the total reply message including feilds */
  int magic_number;		/* number to make sure we are really getting a reply */
  int is_error;			/* did the call end in error or with a success? */
  int return_code;		/* if error, the errno, if success, the return value */
  char contents[buf_siz - sizeof(int) * 4]; /* data payload */
} lind_reply;

#define CONTENTS_SIZ(x) (x->message_size - sizeof(int)*4)

lind_rpc_status depricated_nacl_rpc_syscall(unsigned int call_number, 
					    const char* format, 
					    unsigned int len, 
					    void* body, 
					    int * retval, int nargs, ...);

lind_rpc_status nacl_rpc_syscall_proxy(lind_request *, lind_reply *, int nargs, ...);


/**
   Unsafe syscall is the same as regular, but error messages are not printed to
   logging (since that requires a syscall itself).  Error are printed using the
   strace logging interface.  
*/
/* lind_rpc_status unsafe_nacl_rpc_syscall(lind_request * request, lind_reply * reply); */

#endif // _lind_rpc_h_
