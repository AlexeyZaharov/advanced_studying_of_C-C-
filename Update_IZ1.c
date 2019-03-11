#include <stdio.h>
#include <stdlib.h>

int* find_max(int* array, const size_t size_of_array, int* max, const size_t number_of_max);
int* new_array (size_t* num);
void fill_array (const size_t size_of_array, int* array);

enum {ERROR, OK} success = OK;

int main()
{
  size_t size_of_array = 0;
  int* numbers = NULL;
  int* max = NULL;

  numbers = new_array(&size_of_array);

  if (success != ERROR) {
      fill_array(size_of_array, numbers);

      if (success != ERROR) {
          size_t size_of_max = 0;
          max = new_array(&size_of_max);

          if (size_of_max > size_of_array) {
              success = ERROR;
            }

          if (success != ERROR) {
              find_max (numbers, size_of_array, max, size_of_max);

              for (size_t i = 0; i < size_of_max; ++i) {
                  printf("%d ", max[i]);
                }
            }
        }
    }

  if (success == ERROR){
      printf("[error]");
    }

  free(numbers);
  free(max);

  return 0;
}

int* find_max(int* array, const size_t size_of_array, int* max, const size_t number_of_max) {
  for(size_t i = 0; i < number_of_max; ++i) {
      for(size_t j = i + 1; j < size_of_array; ++j) {
          if (array[j] > array[i]) {
              int t = array[i];
              array[i] = array[j];
              array[j] = t;
            }
        }
      max[i] = array[i];
    }

  return max;
}

int* new_array (size_t* size) {
  int* array = NULL;

  if(scanf("%lu", size) ) {
    array = calloc(*size, sizeof(int));
  }

  if (array == NULL) {
      success = ERROR;
    }

  return array;
}

void fill_array (const size_t size_of_array, int* array) {
  for (size_t i = 0; i < size_of_array; ++i) {
      if(!scanf("%d", &array[i])) {
        success = ERROR;
        break;
        }
      char c = '\0';
      scanf("%c", &c);
      if ((c == '\n' && i != size_of_array-1) || (c != '\n' && i == size_of_array-1)) {
          success = ERROR;
          break;
        }
      }
}
