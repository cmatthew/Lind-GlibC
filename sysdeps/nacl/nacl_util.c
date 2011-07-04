#include <string.h>
#include <stdlib.h>
#include <assert.h>


/* Get the string representation of integer.   */
char* nacl_itoa(int integer)
{
  if (integer <= 0) {
    return "0";
  }
  // we dont deal with negatives, but this is a common case.
  if (integer == -1) {
    return "-1";
  }
  int digits = 0;
  int tmp = integer;
  while (tmp > 10) {
    tmp = tmp/10;
    digits += 1;
  }
  //for 0
  digits += 1;
  
  static char digit_table[] = {'0','1','2','3','4','5','6','7','8','9'};

  char * result = malloc(digits + 1);
  char * curr = result + digits;
  *curr = '\0';
  curr--;

  while(integer > 0) {
    char d = digit_table[integer%10];
    *curr = d;
    curr--;
    integer = integer/10;
  }

  return result;
}  
  

static int test_itoa() {
  assert(strcmp("1", nacl_itoa(1)) == 0 );
  assert(strcmp("22", nacl_itoa(22)) == 0 );
  assert(strcmp("333", nacl_itoa(333)) == 0 );
  assert(strcmp("4444", nacl_itoa(4444)) == 0 );
  assert(strcmp("0", nacl_itoa(-1)) == 0 );
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
  
