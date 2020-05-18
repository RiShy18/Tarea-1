#include <stdio.h>
float *median(float out[], const float in[], int nx, int ny);
 
int main(void)
{
  enum {NCOL = 3, NROW = 3};
  const float in[NCOL*NROW] = {1, 1, 1, 1, 2, 1, 1, 1, 1};
  float out[NCOL*NROW];
 
  median(out, in, NCOL, NROW);
  printf("The filtered image is:\n");
  for (int y = 0; y < NROW; y++) {
    for (int x = 0; x < NCOL; x++)
      printf("%0.f ", out[y*NCOL + x]);
    printf("\n");
  }
        return 0;
}
