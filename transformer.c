#include <stdio.h>
#include <stdlib.h>

struct Pixel {
  int r;
  int g;
  int b;
};

/* Read image from stdin */
struct Pixel *read_image(int *width, int *height) {
  char format[3];

  if (scanf("%2s", format) != 1) {
    return NULL;
  }

  if (format[0] != 'P' || format[1] != '3') {
    return NULL;
  }

  if (scanf("%d %d", width, height) != 2) {
    return NULL;
  }

  int max;
  if (scanf("%d", &max) != 1) {
    return NULL;
  }

  struct Pixel *image = malloc((*width) * (*height) * sizeof(struct Pixel));
  if (image == NULL) {
    return NULL;
  }

  for (int i = 0; i < (*width) * (*height); i++) {
    if (scanf("%d %d %d", &image[i].r, &image[i].g, &image[i].b) != 3) {
      free(image);
      return NULL;
    }
  }

  return image;
}

/* Apply sepia filter */
void apply_sepia(struct Pixel *image, int width, int height) {
  int total = width * height;

  for (int i = 0; i < total; i++) {
    int r = image[i].r;
    int g = image[i].g;
    int b = image[i].b;

    int newR = (int)(r * 0.393 + g * 0.769 + b * 0.189);
    int newG = (int)(r * 0.349 + g * 0.686 + b * 0.168);
    int newB = (int)(r * 0.272 + g * 0.534 + b * 0.131);

    if (newR > 255)
      newR = 255;
    if (newG > 255)
      newG = 255;
    if (newB > 255)
      newB = 255;

    image[i].r = newR;
    image[i].g = newG;
    image[i].b = newB;
  }
}

/* Flip image horizontally */
void flip_image(struct Pixel *image, int width, int height) {
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width / 2; col++) {

      int left_index = row * width + col;
      int right_index = row * width + (width - 1 - col);

      struct Pixel temp = image[left_index];
      image[left_index] = image[right_index];
      image[right_index] = temp;
    }
  }
}

/* Print image */
void print_image(struct Pixel *image, int width, int height) {
  printf("P3\n");
  printf("%d %d\n", width, height);
  printf("255\n");

  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      int index = row * width + col;
      if (col > 0)
        printf(" ");
      printf("%d %d %d", image[index].r, image[index].g, image[index].b);
    }
    printf(" \n");
  }
}

int main(int argc, char *argv[]) {

  int flip = 0;
  int sepia = 0;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == 'f') {
      flip = 1;
    } else if (argv[i][0] == '-' && argv[i][1] == 's') {
      sepia = 1;
    }
  }

  int width, height;
  struct Pixel *image = read_image(&width, &height);

  if (image == NULL) {
    return 1;
  }

  if (sepia) {
    apply_sepia(image, width, height);
  }

  if (flip) {
    flip_image(image, width, height);
  }

  print_image(image, width, height);

  free(image);

  return 0;
}