#ifndef NUTIL_H
#define NUTIL_H
/** 
Chris Matthews 2011
cmatthew@cs.uvic.ca
*/

/* Note comment in C fie. Put the ascii representation of int into char* with max digitis */
char * nacl_itoa(int);


/* Combine "op" and "arg" into a new larger string "oparg"
 op and arg are left untouched. Oparg is allocated by a 
 calloc call.  */
char * concat(const char* op, const char * arg);
  

#endif // NUTIL_H
