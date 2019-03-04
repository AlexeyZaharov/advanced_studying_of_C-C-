#include <stdio.h>
#include <stdlib.h>

int* find_max(int* num, size_t n, size_t m) {
  int* max = calloc(m, sizeof(int));

  for(size_t i = 0; i < m; ++i) {
      for(size_t j = i + 1; j < n; ++j) {
          if (num[j] > num[i]) {
              int t = num[i];
              num[i] = num[j];
              num[j] = t;
            }
        }
      max[i] = num[i];
    }

  return max;
}

int main()
{
  size_t n = 0;
  if(!scanf("%lu", &n) ) {
    printf("[error]");
    return 0;
    }

  int* numbers = calloc(n, sizeof(int));

  for (size_t i = 0; i < n; ++i) {
      if(!scanf("%d", &numbers[i])) {
        free(numbers);
        printf("[error]");
        return 0;
        }
      char c;
      scanf("%c", &c);
      if ((c == '\n' && i != n-1) || (c != '\n' && i == n-1)) {
          free(numbers);
          printf("[error]");
          return 0;
        }
      }

  size_t m = 0;
  if (!scanf("%lu", &m) || m>n) {
     free(numbers);
     printf("[error]");
     return 0;
     }

  int* max = find_max(numbers, n, m);

  for (size_t i = 0; i < m; ++i) {
      printf("%d ", max[i]);
    }

  free(max);
  free(numbers);

  return 0;
}
