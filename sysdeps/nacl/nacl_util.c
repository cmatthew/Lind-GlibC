#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#ifdef TESTING
#include <stdio.h>
#endif
/* Get the string representation of integer. 
 * Caller must free memory.  */
char* nacl_itoa(int integer)
{
  int negative = 0;
  if (integer < 0) {
    negative = 1;
    integer = -1 * integer;
  }

  int digits = 1;
  int tmp = integer;
  while (tmp >= 10) {
    tmp = tmp/10;
    digits += 1;
  }
  char * result = malloc(digits + negative + 1);
  char * curr = result + digits + negative;
  *curr = '\0';
  curr--;

  do  {
    char d = '0' + integer%10;
    *curr = d;
    curr--;
    integer = integer/10;
  } while(integer > 0);

  if (negative) {
    *curr = '-';
  }
  return result;
}  
  
char * aitoa(char ** spot, int integer) {
  char * result = nacl_itoa(integer);
  *spot = result;
  return result;
}


#ifdef TESTING

static int __attribute ((unused)) test_itoa() {
  assert(strcmp("1", nacl_itoa(1)) == 0 );
  assert(strcmp("2147483647", nacl_itoa(2147483647)) == 0 );
  assert(strcmp("22", nacl_itoa(22)) == 0 );
  assert(strcmp("333", nacl_itoa(333)) == 0 );
  assert(strcmp("4444", nacl_itoa(4444)) == 0 );
  assert(strcmp("-1", nacl_itoa(-1)) == 0 );
  assert(strcmp("-10", nacl_itoa(-10)) == 0 );
  assert(strcmp("-2147483647", nacl_itoa(-2147483647)) == 0 );
  assert(strcmp("0", nacl_itoa(0)) == 0 );
 
  return 0;
}

#endif
  

char * concat(const char* op, const char * arg) {
  int len = 0;
  len = strlen(op);
  len += strlen(arg) + 1;
  char * msg = calloc(1,len);
  strcat(msg, op);
  strcat(msg, arg);
  return msg;
}

char * combine(unsigned int num_strs, ...) {
  va_list argp;
  va_start(argp, num_strs);
  int len = 0;
  int i = 0;
  for (; i<num_strs; i++) {
    char* str = va_arg(argp, char*);
    len += strlen(str);
  }
  va_end(argp);
  va_start(argp,num_strs);

  char * msg = (char*) malloc(len+1);
  memset(msg, 0, len + 1);
  for (i = 0; i < num_strs; i++) {
    strcat(msg, va_arg(argp, char*));
  }
  va_end(argp);
  return msg;
}


int is_system_handle(int fd) {

  if (fd < 10) {
    return 1;
  } else {
    return 0;
  }

}


/* int primative_sizeof(char * item) { */

/*   if (strcmp(item, "i")) { */
/*     return sizeof(int); */
/*   }  else if (strcmp(item, "I")) { */
/*       return sizeof(unsigned int); */
/*   }  else if (strcmp(item, "q")) { */
/*       return sizeof(long); */
/*   }  else if (strcmp(item, "Q")) { */
/*       return sizeof(unsigned long); */
/*   }  else if (strcmp(item, "s")) { */
/*       return sizeof(char); */
/*   }  else if (strcmp(item, "c")) { */
/*     return sizeof(char); */
/*   } else { */
/*     return 0; */

/* } */

/* ssize_t calc_size(char * format) { */

/* } */

#ifdef TESTING

static int __attribute ((unused)) test_combine() {
  assert(strcmp("ab",combine(2, "a", "b") ) == 0 );
  assert(strcmp("a",combine(1, "a") ) == 0 );
  assert(strcmp("abc",combine(3, "a", "b", "c") ) == 0 );
  assert(strcmp("assbasscassdass",combine(4, "ass", "bass", "cass","dass") ) == 0 );
  return 0;
}


int main() {
  test_combine();
  test_itoa();
}

#endif


