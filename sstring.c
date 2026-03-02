#include <stdio.h>
#include <stdlib.h>

struct SString {
  char *data;
  unsigned int size;
  unsigned int capacity;
};

void init(struct SString *s) {
  s->capacity = 4;
  s->size = 0;
  s->data = malloc(4);
}

void free_sstring(struct SString *s) { free(s->data); }

void append_char(struct SString *s, char c) {

  if (s->size == s->capacity) {

    s->capacity *= 2;
    char *new_data = malloc(s->capacity);

    for (unsigned int i = 0; i < s->size; i++) {
      new_data[i] = s->data[i];
    }

    free(s->data);
    s->data = new_data;
  }

  s->data[s->size] = c;
  s->size++;
}

void clear(struct SString *s) { s->size = 0; }

void print_sstring(struct SString *s) {
  for (unsigned int i = 0; i < s->size; i++) {
    printf("%c", s->data[i]);
  }
  printf("\n");
}

void details(struct SString *s) {

  printf("\"");

  for (unsigned int i = 0; i < s->size; i++) {
    printf("%c", s->data[i]);
  }

  printf("\" size: %u capacity: %u\n", s->size, s->capacity);
}

void copy(struct SString *dest, struct SString *src) {

  clear(dest);

  for (unsigned int i = 0; i < src->size; i++) {
    append_char(dest, src->data[i]);
  }
}

void concat(struct SString *dest, struct SString *s1, struct SString *s2) {

  /* Use a temp to handle aliasing (e.g. c a a b where dest == s1) */
  struct SString temp;
  init(&temp);

  for (unsigned int i = 0; i < s1->size; i++) {
    append_char(&temp, s1->data[i]);
  }

  for (unsigned int i = 0; i < s2->size; i++) {
    append_char(&temp, s2->data[i]);
  }

  free(dest->data);
  dest->data = temp.data;
  dest->size = temp.size;
  dest->capacity = temp.capacity;
}

void read_word(struct SString *s) {

  clear(s);

  int c;

  /* Skip leading whitespace to get to actual word */
  while ((c = getchar()) != EOF) {
    if (c != ' ' && c != '\n' && c != '\t' && c != '\r') {
      append_char(s, c);
      break;
    }
  }

  /* Read until whitespace or EOF */
  while ((c = getchar()) != EOF) {
    if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
      break;
    }
    append_char(s, c);
  }
}

void read_quoted(struct SString *s) {

  clear(s);

  int c;

  /* Skip until we find the opening quote */
  while ((c = getchar()) != EOF) {
    if (c == '"') {
      break;
    }
  }

  /* Read until closing quote or EOF */
  while ((c = getchar()) != EOF) {
    if (c == '"') {
      break;
    }
    append_char(s, c);
  }
}

int main() {

  struct SString a, b, c, d;

  init(&a);
  init(&b);
  init(&c);
  init(&d);

  char command;

  while (1) {

    if (scanf(" %c", &command) != 1) {
      break;
    }

    if (command == 'q') {
      break;
    }

    if (command == 'r' || command == 'a') {

      char target;
      scanf(" %c", &target);

      struct SString *s;

      if (target == 'a')
        s = &a;
      else if (target == 'b')
        s = &b;
      else if (target == 'c')
        s = &c;
      else
        s = &d;

      /* Skip whitespace, then check if next char is a quote */
      int next;
      while ((next = getchar()) != EOF) {
        if (next != ' ' && next != '\t' && next != '\n' && next != '\r') {
          break;
        }
      }

      if (next == '"') {
        /* Quoted string — read_quoted will read until closing " */
        if (command == 'r') {
          clear(s);
          int ch;
          while ((ch = getchar()) != EOF) {
            if (ch == '"')
              break;
            append_char(s, ch);
          }
        } else {
          int ch;
          while ((ch = getchar()) != EOF) {
            if (ch == '"')
              break;
            append_char(s, ch);
          }
        }
      } else {
        /* Unquoted word — first non-whitespace char is 'next' */
        if (next != EOF) {
          if (command == 'r') {
            clear(s);
            append_char(s, next);
            int ch;
            while ((ch = getchar()) != EOF) {
              if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
                break;
              append_char(s, ch);
            }
          } else {
            append_char(s, next);
            int ch;
            while ((ch = getchar()) != EOF) {
              if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
                break;
              append_char(s, ch);
            }
          }
        }
      }
    }

    else if (command == 'p') {

      char target;
      scanf(" %c", &target);

      if (target == 'a')
        print_sstring(&a);
      else if (target == 'b')
        print_sstring(&b);
      else if (target == 'c')
        print_sstring(&c);
      else
        print_sstring(&d);
    }

    else if (command == 'd') {

      char target;
      scanf(" %c", &target);

      if (target == 'a')
        details(&a);
      else if (target == 'b')
        details(&b);
      else if (target == 'c')
        details(&c);
      else
        details(&d);
    }

    else if (command == 'c') {

      char t1, t2, t3;
      scanf(" %c %c %c", &t1, &t2, &t3);

      struct SString *dest, *s1, *s2;

      if (t1 == 'a')
        dest = &a;
      else if (t1 == 'b')
        dest = &b;
      else if (t1 == 'c')
        dest = &c;
      else
        dest = &d;

      if (t2 == 'a')
        s1 = &a;
      else if (t2 == 'b')
        s1 = &b;
      else if (t2 == 'c')
        s1 = &c;
      else
        s1 = &d;

      if (t3 == 'a')
        s2 = &a;
      else if (t3 == 'b')
        s2 = &b;
      else if (t3 == 'c')
        s2 = &c;
      else
        s2 = &d;

      concat(dest, s1, s2);
    }
  }

  free_sstring(&a);
  free_sstring(&b);
  free_sstring(&c);
  free_sstring(&d);

  return 0;
}