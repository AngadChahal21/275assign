#include <stdio.h>
#include <stdlib.h>

struct RGBData {
  int red_val;
  int green_val;
  int blue_val;
};

/* Read image from stdin */
struct RGBData *import_ppm(int *w_ptr, int *h_ptr) {
  char magic_number[3];

  if (scanf("%2s", magic_number) != 1) {
    return NULL;
  }

  if (magic_number[0] != 'P' || magic_number[1] != '3') {
    return NULL;
  }

  if (scanf("%d %d", w_ptr, h_ptr) != 2) {
    return NULL;
  }

  int scale_max;
  if (scanf("%d", &scale_max) != 1) {
    return NULL;
  }

  struct RGBData *pixel_grid = malloc((*w_ptr) * (*h_ptr) * sizeof(struct RGBData));
  if (pixel_grid == NULL) {
    return NULL;
  }

  for (int idx = 0; idx < (*w_ptr) * (*h_ptr); idx++) {
    if (scanf("%d %d %d", &pixel_grid[idx].red_val, &pixel_grid[idx].green_val, &pixel_grid[idx].blue_val) != 3) {
      free(pixel_grid);
      return NULL;
    }
  }

  return pixel_grid;
}

/* Apply sepia filter */
void tint_sepia(struct RGBData *buffer, int cols, int rows) {
  int pixel_count = cols * rows;

  for (int i = 0; i < pixel_count; i++) {
    int oldR = buffer[i].red_val;
    int oldG = buffer[i].green_val;
    int oldB = buffer[i].blue_val;

    int filteredR = (int)(oldR * 0.393 + oldG * 0.769 + oldB * 0.189);
    int filteredG = (int)(oldR * 0.349 + oldG * 0.686 + oldB * 0.168);
    int filteredB = (int)(oldR * 0.272 + oldG * 0.534 + oldB * 0.131);

    if (filteredR > 255)
      filteredR = 255;
    if (filteredG > 255)
      filteredG = 255;
    if (filteredB > 255)
      filteredB = 255;

    buffer[i].red_val = filteredR;
    buffer[i].green_val = filteredG;
    buffer[i].blue_val = filteredB;
  }
}

/* Flip image horizontally */
void mirror_horizontal(struct RGBData *canvas, int width, int height) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width / 2; x++) {

      int pos_start = y * width + x;
      int pos_end = y * width + (width - 1 - x);

      struct RGBData swap_holder = canvas[pos_start];
      canvas[pos_start] = canvas[pos_end];
      canvas[pos_end] = swap_holder;
    }
  }
}

/* Print image */
void export_ppm(struct RGBData *output, int out_w, int out_h) {
  printf("P3\n");
  printf("%d %d\n", out_w, out_h);
  printf("255\n");

  for (int y = 0; y < out_h; y++) {
    for (int x = 0; x < out_w; x++) {
      int cursor = y * out_w + x;
      if (x > 0)
        printf(" ");
      printf("%d %d %d", output[cursor].red_val, output[cursor].green_val, output[cursor].blue_val);
    }
    printf(" \n");
  }
}

int main(int argument_count, char *argument_vector[]) {

  int do_mirror = 0;
  int do_sepia = 0;

  for (int k = 1; k < argument_count; k++) {
    if (argument_vector[k][0] == '-' && argument_vector[k][1] == 'f') {
      do_mirror = 1;
    } else if (argument_vector[k][0] == '-' && argument_vector[k][1] == 's') {
      do_sepia = 1;
    }
  }

  int img_w, img_h;
  struct RGBData *img_data = import_ppm(&img_w, &img_h);

  if (img_data == NULL) {
    return 1;
  }

  if (do_sepia) {
    tint_sepia(img_data, img_w, img_h);
  }

  if (do_mirror) {
    mirror_horizontal(img_data, img_w, img_h);
  }

  export_ppm(img_data, img_w, img_h);

  free(img_data);

  return 0;
}