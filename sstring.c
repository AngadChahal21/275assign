#include <stdio.h>
#include <stdlib.h>

struct DynamicString {
  char *text_ptr;
  unsigned int length;
  unsigned int max_size;
};

void setup_str(struct DynamicString *ds) {
  ds->max_size = 4;
  ds->length = 0;
  ds->text_ptr = malloc(4);
}

void release_str(struct DynamicString *ds) { 
  free(ds->text_ptr); 
}

void add_item(struct DynamicString *ds, char symbol) {
  if (ds->length == ds->max_size) {
    ds->max_size *= 2;
    char *resized_area = malloc(ds->max_size);

    for (unsigned int k = 0; k < ds->length; k++) {
      resized_area[k] = ds->text_ptr[k];
    }

    free(ds->text_ptr);
    ds->text_ptr = resized_area;
  }

  ds->text_ptr[ds->length] = symbol;
  ds->length++;
}

void wipe_str(struct DynamicString *ds) { 
  ds->length = 0; 
}

/* Updated to print just the raw string content */
void display_str(struct DynamicString *ds) {
  for (unsigned int k = 0; k < ds->length; k++) {
    printf("%c", ds->text_ptr[k]);
  }
  printf("\n");
}

/* Updated to match your specific multi-line requirement */
void show_info(struct DynamicString *ds) {
  printf("String: \"");
  for (unsigned int k = 0; k < ds->length; k++) {
    printf("%c", ds->text_ptr[k]);
  }
  printf("\"\n");
  printf("Length: %u\n", ds->length);
  printf("Capacity: %u\n", ds->max_size);
}

void replicate(struct DynamicString *target, struct DynamicString *origin) {
  wipe_str(target);
  for (unsigned int k = 0; k < origin->length; k++) {
    add_item(target, origin->text_ptr[k]);
  }
}

void join_str(struct DynamicString *out, struct DynamicString *p1, struct DynamicString *p2) {
  struct DynamicString buffer;
  setup_str(&buffer);

  for (unsigned int k = 0; k < p1->length; k++) {
    add_item(&buffer, p1->text_ptr[k]);
  }

  for (unsigned int k = 0; k < p2->length; k++) {
    add_item(&buffer, p2->text_ptr[k]);
  }

  free(out->text_ptr);
  out->text_ptr = buffer.text_ptr;
  out->length = buffer.length;
  out->max_size = buffer.max_size;
}

int main() {
  struct DynamicString str_a, str_b, str_c, str_d;

  setup_str(&str_a);
  setup_str(&str_b);
  setup_str(&str_c);
  setup_str(&str_d);

  char op_code;

  while (1) {
    if (scanf(" %c", &op_code) != 1) {
      break;
    }

    if (op_code == 'q') {
      break;
    }

    if (op_code == 'r' || op_code == 'a') {
      char target_id;
      scanf(" %c", &target_id);

      struct DynamicString *active_ptr;

      if (target_id == 'a') active_ptr = &str_a;
      else if (target_id == 'b') active_ptr = &str_b;
      else if (target_id == 'c') active_ptr = &str_c;
      else active_ptr = &str_d;

      int lookahead;
      while ((lookahead = getchar()) != EOF) {
        if (lookahead != ' ' && lookahead != '\t' && lookahead != '\n' && lookahead != '\r') {
          break;
        }
      }

      if (lookahead == '"') {
        if (op_code == 'r') wipe_str(active_ptr);
        int stream_char;
        while ((stream_char = getchar()) != EOF) {
          if (stream_char == '"') break;
          add_item(active_ptr, stream_char);
        }
      } else if (lookahead != EOF) {
        if (op_code == 'r') wipe_str(active_ptr);
        add_item(active_ptr, lookahead);
        int stream_char;
        while ((stream_char = getchar()) != EOF) {
          if (stream_char == ' ' || stream_char == '\t' || stream_char == '\n' || stream_char == '\r')
            break;
          add_item(active_ptr, stream_char);
        }
      }
    }

    else if (op_code == 'p') {
      char target_id;
      scanf(" %c", &target_id);

      if (target_id == 'a') display_str(&str_a);
      else if (target_id == 'b') display_str(&str_b);
      else if (target_id == 'c') display_str(&str_c);
      else display_str(&str_d);
    }

    else if (op_code == 'd') {
      char target_id;
      scanf(" %c", &target_id);

      if (target_id == 'a') show_info(&str_a);
      else if (target_id == 'b') show_info(&str_b);
      else if (target_id == 'c') show_info(&str_c);
      else show_info(&str_d);
    }

    else if (op_code == 'c') {
      char id1, id2, id3;
      scanf(" %c %c %c", &id1, &id2, &id3);

      struct DynamicString *out_ptr, *src1, *src2;

      if (id1 == 'a') out_ptr = &str_a;
      else if (id1 == 'b') out_ptr = &str_b;
      else if (id1 == 'c') out_ptr = &str_c;
      else out_ptr = &str_d;

      if (id2 == 'a') src1 = &str_a;
      else if (id2 == 'b') src1 = &str_b;
      else if (id2 == 'c') src1 = &str_c;
      else src1 = &str_d;

      if (id3 == 'a') src2 = &str_a;
      else if (id3 == 'b') src2 = &str_b;
      else if (id3 == 'c') src2 = &str_c;
      else src2 = &str_d;

      join_str(out_ptr, src1, src2);
    }
  }

  release_str(&str_a);
  release_str(&str_b);
  release_str(&str_c);
  release_str(&str_d);

  return 0;
}