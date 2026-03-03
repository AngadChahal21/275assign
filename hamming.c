#include <stdio.h>
#include <stdlib.h>

unsigned int convert_to_unsigned(char *input_ptr) {
  unsigned int total_val = 0;
  int cursor = 0;

  while (input_ptr[cursor] != '\0') {
    total_val = total_val * 10 + (input_ptr[cursor] - '0');
    cursor++;
  }

  return total_val;
}

int begins_with_digit(char *buffer) {
    int idx = 0;

    // Optional sign
    if (buffer[idx] == '+' || buffer[idx] == '-') {
        idx++;
    }

    // Must have at least one digit
    if (buffer[idx] < '0' || buffer[idx] > '9') {
        return 0;   // false
    }

    return 1;       // true
  }

int main(int arg_count, char *arg_values[]) {

    int failure_flag = 0;
  
  if (arg_count != 4) {
    fprintf(stderr, "Usage: ./hamming_sample n1 n2 base\n");
    return 1;
  }


    if (!begins_with_digit(arg_values[1])) {
        fprintf(stderr, "First argument must be an integer\n");
        failure_flag = 1;
    }

    if (!begins_with_digit(arg_values[2])) {
        fprintf(stderr, "Second argument must be an integer\n");
        failure_flag = 1;
    }

    if (!begins_with_digit(arg_values[3])) {
        fprintf(stderr, "Third argument must be an integer\n");
        failure_flag = 1;
    }

    if(failure_flag){
        return 1;
    }

  unsigned int first_val = convert_to_unsigned(arg_values[1]);
  unsigned int second_val = convert_to_unsigned(arg_values[2]);
  unsigned int radix_base = convert_to_unsigned(arg_values[3]);

  unsigned int diff_count = 0;

  
  int active_1 = (first_val > 0);
  int active_2 = (second_val > 0);

  while (active_1 || active_2) {
    unsigned int remainder_1 = first_val % radix_base;
    unsigned int remainder_2 = second_val % radix_base;

    if (active_1 != active_2) {
      /* One number has a digit here, the other doesn't — always differs */
      diff_count++;
    } else if (remainder_1 != remainder_2) {
      diff_count++;
    }

    first_val = first_val / radix_base;
    second_val = second_val / radix_base;
    active_1 = (first_val > 0);
    active_2 = (second_val > 0);
  }

  printf("%u\n", diff_count);

  return 0;
}