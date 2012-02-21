#ifndef NUTIL_H
#define NUTIL_H
/** 
Chris Matthews 2011
cmatthew@cs.uvic.ca
*/

/* Note comment in C fie. Put the ascii representation of int into char* with max digitis */
char * nacl_itoa(int);

/* call itoa, storing the result ptr in arg */
char * aitoa(char **, int);

/* Combine "op" and "arg" into a new larger string "oparg"
 op and arg are left untouched. Oparg is allocated by a 
 calloc call.  */
char * concat(const char * op, const char * arg);
char * combine(unsigned int num, ...);


/* Is this file handle a system handle, or a Lind handle? */
int is_system_handle(int fd);  
#endif // NUTIL_H
