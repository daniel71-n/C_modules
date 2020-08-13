#include <stdlib.h>

#define VECT_SIZE 10  // the dynamic array should start off with a length of 10

typedef struct vect Vect;  // an alias for the the whole structure

int vectContains(Vect *target_vector, char val);
void vectInit(Vect *vector_to_initialize, size_t initial_size);
void vectAppend(Vect *target_vector, char val);
void vectSet(Vect *target_vector, char val, unsigned int index);    
struct vect{
char *dynArray;     // an inner array to be resized dynamically
size_t total_array_length;  // reference variable holding the size of the array
size_t space_available;     // how many array slots are unused; if less than half of them are, shrink dynArray to half the size
int last_index;        // if last_index == total_array_length-2, double the size of dynArray

};


