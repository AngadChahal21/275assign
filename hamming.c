#include <stdio.h>
#include <stdlib.h>

unsigned int str_to_uint(char *s) {
  unsigned int result = 0;
  int i = 0;

  while (s[i] != '\0') {
    result = result * 10 + (s[i] - '0');
    i++;
  }

  return result;
}

int startsWithInteger(char *str) {
    int i = 0;

    // Optional sign
    if (str[i] == '+' || str[i] == '-') {
        i++;
    }

    // Must have at least one digit
    if (str[i] < '0' || str[i] > '9') {
        return 0;   // false
    }

    return 1;       // true
  }

int main(int argc, char *argv[]) {

  
  if (argc != 4) {
    fprintf(stderr, "Usage: ./hamming_sample n1 n2 base\n");
    return 1;
  }


    if (!startsWithInteger(argv[1])) {
        fprintf(stderr, "First argument must be an integer\n");
        return 1;
    }

    if (!startsWithInteger(argv[2])) {
        fprintf(stderr, "Second argument must be an integer\n");
        return 1;
    }

    if (!startsWithInteger(argv[3])) {
        fprintf(stderr, "Third argument must be an integer\n");
        return 1;
    }

  unsigned int num1 = str_to_uint(argv[1]);
  unsigned int num2 = str_to_uint(argv[2]);
  unsigned int base = str_to_uint(argv[3]);

  unsigned int distance = 0;

  /* Track whether each number still has digits remaining.
   * A missing digit (absence) always differs from any value,
   * even 0. We can't just compare remainders because
   * 0 % base == 0 which would falsely match a real 0 digit. */
  int has_digits1 = (num1 > 0);
  int has_digits2 = (num2 > 0);

  while (has_digits1 || has_digits2) {
    unsigned int digit1 = num1 % base;
    unsigned int digit2 = num2 % base;

    if (has_digits1 != has_digits2) {
      /* One number has a digit here, the other doesn't — always differs */
      distance++;
    } else if (digit1 != digit2) {
      distance++;
    }

    num1 = num1 / base;
    num2 = num2 / base;
    has_digits1 = (num1 > 0);
    has_digits2 = (num2 > 0);
  }

  printf("%u\n", distance);

  return 0;
}
