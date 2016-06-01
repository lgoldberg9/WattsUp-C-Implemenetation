#include "ArrayList.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

array_list* initialize_arr_list(int capacity) {

  array_list *arrList;

  if (capacity > 0) {
    if ((arrList = (array_list*) malloc(sizeof(array_list))) == NULL) {
      fprintf(stderr, "Failed to allocate memory for array list.\n");
      exit(EXIT_FAILURE);
    }
    arrList->array = malloc(sizeof(int) * capacity);
    arrList->size = 0;
    arrList->capacity = capacity;
  }

  return arrList;

}

void clear_arr_list(array_list *arrList) {
  free(arrList->array);
  free(arrList);
}

void ensure_capacity(array_list *arrList) {
  if (arrList->size == arrList->capacity) {
    int *oldArr = arrList->array;
    if ((arrList->array =
	 (int*) malloc(sizeof(int) * arrList->capacity * 2)) == NULL) {
      fprintf(stderr, "Failed to allocate memory for array.\n");
      exit(EXIT_FAILURE);
    }
    for (int index = 0; index < arrList->size; index++) {
      arrList->array[index] = oldArr[index];
    }
    arrList->capacity *= 2;
    free(oldArr);
  }
}

void add_value(array_list *arrList, double val) {
  ensure_capacity(arrList);
  arrList->array[arrList->size++] = val;
}

double get_value(array_list *arrList, int index) {
  return arrList->array[index];
}
