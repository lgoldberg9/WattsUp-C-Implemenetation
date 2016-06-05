#ifndef __ARRAYLIST_H__
#define __ARRAYLIST_H__

typedef struct ArrayList {
  double *array;
  int size;
  int capacity;
} array_list;

/* Parameters: capacity, a positive integer
   Purpose: initializes an array list. 
   Preconditions: [No additional]
   Postconditions: array_list with a positive sized array is allocated.  
   Abort program and return EXIT_FAILURE if failure to allocate space. */
array_list* initialize_arr_list(int capacity);


/* Parameters: arrList, an array list
   Purpose: frees the space occupied by arrList.
   Preconditions: arrList must be malloced.
   Postconditions: arrList is freed. */
void clear_arr_list(array_list *arrList);

/* Parameters: arrList, an array list
   Purpose: ensures that add_value has a capacity to add integers to array. 
   Preconditions: arrList must be malloced.
   Postconditions: array size is left alone if there is space. Otherwise, space
   is doubled. Abort program and return EXIT_FAILURE if failure to allocate space. */
void ensure_capacity(array_list *arrList);

/* Parameters: arrList, an array list
               val, a double to add
   Purpose: adds a double to an array list
   Preconditions: arrList must be malloced.
   Postconditions: val is added to end of array. */
void add_value(array_list *arrList, double val);

/* Parameters: arrList, an array list
               index, an index to an array
   Purpose: gets the value at a specified index.
   Preconditions: arrList must be malloced, index must be nonnegative
   Postconditions: double is retrived. */
double get_value(array_list *arrList, int index);

#endif
