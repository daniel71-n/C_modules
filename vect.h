#include <stdlib.h>

typedef struct vect Vect;  

void vectRem(Vect *target_vector, unsigned int index);
char vectPop(Vect *target_vector);
void vectAdd(Vect *target_vector, char *string_to_append);
int vectContains(Vect *target_vector, char val);
void vectInit(Vect *vector_to_initialize, size_t initial_size);
void vectAppend(Vect *target_vector, char val);
void vectSet(Vect *target_vector, char val, unsigned int index);    

struct vect{
    char *dynArray;             // an inner array to be resized dynamically
    size_t total_array_length;  // reference variable holding the size of the array
    size_t space_available;     // how many array slots are unused; if less than half of them are, shrink dynArray to half the size
    int last_index;             // if last_index == total_array_length-2, double the size of dynArray
};


