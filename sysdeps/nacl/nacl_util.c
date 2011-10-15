#include <string.h>
#include <stdlib.h>
#include <assert.h>

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
  


char * concat(const char* op, const char * arg) {
  int len = 0;
  len = strlen(op);
  len += strlen(arg) + 2;
  char * msg = calloc(1,len);
  strcat(msg, op);
  strcat(msg, arg);
  return msg;
}


int is_system_handle(int fd) {

  if (fd < 10) {
    return 1;
  } else {
    return 0;
  }

}
