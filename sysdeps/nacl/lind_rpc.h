

typedef enum lind_rpc_status_e {RPC_OK = 0, RPC_WRITE_ERROR, RPC_READ_ERROR, RPC_ARGS_ERROR} lind_rpc_status;


lind_rpc_status nacl_rpc_syscall(unsigned int, const char*, unsigned int, void*, int *);


/**
   Unsafe syscall is the same as regular, but error messages are not printed to
   logging (since that requires a syscall itself).  Error are printed using the
   strace logging interface.  
*/
lind_rpc_status unsafe_nacl_rpc_syscall(unsigned int, const char*, unsigned int, void*, int *);
