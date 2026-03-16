#include <stdio.h>
#include <stdlib.h>

struct VectorString {
  char *raw_data;
  unsigned int used_len;
  unsigned int total_cap;
};

void init_vector(struct VectorString *vs) {
  vs->total_cap = 4;
  vs->used_len = 0;
  vs->raw_data = malloc(4);
}

void destroy_vector(struct VectorString *vs) { 
  free(vs->raw_data); 
}

void push_char(struct VectorString *vs, char val) {
  if (vs->used_len == vs->total_cap) {
    vs->total_cap *= 2;
    char *new_buffer = malloc(vs->total_cap);

    for (unsigned int m = 0; m < vs->used_len; m++) {
      new_buffer[m] = vs->raw_data[m];
    }

    free(vs->raw_data);
    vs->raw_data = new_buffer;
  }

  vs->raw_data[vs->used_len] = val;
  vs->used_len++;
}

void clear_vector(struct VectorString *vs) { 
  vs->used_len = 0; 
}

/* Updated to print just the raw string content */
void print_raw(struct VectorString *vs) {
  for (unsigned int m = 0; m < vs->used_len; m++) {
    printf("%c", vs->raw_data[m]);
  }
  printf("\n");
}

/* Updated to match your specific multi-line requirement */
void debug_dump(struct VectorString *vs) {
  printf("String: \"");
  for (unsigned int m = 0; m < vs->used_len; m++) {
    printf("%c", vs->raw_data[m]);
  }
  printf("\"\n");
  printf("Length: %u\n", vs->used_len);
  printf("Capacity: %u\n", vs->total_cap);
}

void copy_vector(struct VectorString *dest, struct VectorString *src) {
  clear_vector(dest);
  for (unsigned int m = 0; m < src->used_len; m++) {
    push_char(dest, src->raw_data[m]);
  }
}

void concat_vectors(struct VectorString *result, struct VectorString *left, struct VectorString *right) {
  struct VectorString temp_storage;
  init_vector(&temp_storage);

  for (unsigned int m = 0; m < left->used_len; m++) {
    push_char(&temp_storage, left->raw_data[m]);
  }

  for (unsigned int m = 0; m < right->used_len; m++) {
    push_char(&temp_storage, right->raw_data[m]);
  }

  free(result->raw_data);
  result->raw_data = temp_storage.raw_data;
  result->used_len = temp_storage.used_len;
  result->total_cap = temp_storage.total_cap;
}

int main() {
  struct VectorString vec_1, vec_2, vec_3, vec_4;

  init_vector(&vec_1);
  init_vector(&vec_2);
  init_vector(&vec_3);
  init_vector(&vec_4);

  char command;

  while (1) {
    if (scanf(" %c", &command) != 1) {
      break;
    }

    if (command == 'q') {
      break;
    }

    if (command == 'r' || command == 'a') {
      char selector;
      scanf(" %c", &selector);

      struct VectorString *current_vec;

      if (selector == 'a') current_vec = &vec_1;
      else if (selector == 'b') current_vec = &vec_2;
      else if (selector == 'c') current_vec = &vec_3;
      else current_vec = &vec_4;

      int next_char;
      while ((next_char = getchar()) != EOF) {
        if (next_char != ' ' && next_char != '\t' && next_char != '\n' && next_char != '\r') {
          break;
        }
      }

      if (next_char == '"') {
        if (command == 'r') clear_vector(current_vec);
        int input_byte;
        while ((input_byte = getchar()) != EOF) {
          if (input_byte == '"') break;
          push_char(current_vec, input_byte);
        }
      } else if (next_char != EOF) {
        if (command == 'r') clear_vector(current_vec);
        push_char(current_vec, next_char);
        int input_byte;
        while ((input_byte = getchar()) != EOF) {
          if (input_byte == ' ' || input_byte == '\t' || input_byte == '\n' || input_byte == '\r')
            break;
          push_char(current_vec, input_byte);
        }
      }
    }

    else if (command == 'p') {
      char selector;
      scanf(" %c", &selector);

      if (selector == 'a') print_raw(&vec_1);
      else if (selector == 'b') print_raw(&vec_2);
      else if (selector == 'c') print_raw(&vec_3);
      else print_raw(&vec_4);
    }

    else if (command == 'd') {
      char selector;
      scanf(" %c", &selector);

      if (selector == 'a') debug_dump(&vec_1);
      else if (selector == 'b') debug_dump(&vec_2);
      else if (selector == 'c') debug_dump(&vec_3);
      else debug_dump(&vec_4);
    }

    else if (command == 'c') {
      char key1, key2, key3;
      scanf(" %c %c %c", &key1, &key2, &key3);

      struct VectorString *target, *source_a, *source_b;

      if (key1 == 'a') target = &vec_1;
      else if (key1 == 'b') target = &vec_2;
      else if (key1 == 'c') target = &vec_3;
      else target = &vec_4;

      if (key2 == 'a') source_a = &vec_1;
      else if (key2 == 'b') source_a = &vec_2;
      else if (key2 == 'c') source_a = &vec_3;
      else source_a = &vec_4;

      if (key3 == 'a') source_b = &vec_1;
      else if (key3 == 'b') source_b = &vec_2;
      else if (key3 == 'c') source_b = &vec_3;
      else source_b = &vec_4;

      concat_vectors(target, source_a, source_b);
    }
  }

  destroy_vector(&vec_1);
  destroy_vector(&vec_2);
  destroy_vector(&vec_3);
  destroy_vector(&vec_4);

  return 0;
}