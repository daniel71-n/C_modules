#include <stdlib.h>


/* ------- Structs and Enums ------- */
enum array_types{C_ARRAY, I_ARRAY};

struct vector{
    union{
    char *c;             // inner array to be resized dynamically : used if type == C_ARRAY; 
    int *i;             // memory will be allocated to this instead if type == I_ARRAY;
    }dynarray;          // managed array. To be initialized by vectInit as either an int or a char array
    enum array_types type;      // enum specifying the type of the managed array - and consequently, which inner array gets allocated memory
    size_t total_array_length;  // reference variable holding the size of the array. This is not the number of bytes, but the total number of 'positions'/indexes in the array;
    int last_index;             // stores the last non-NUL index that currently has a value assigned to it. It'll at most be total_array_length - 3 before the array is automatically grown.
};


/* ------- Typedefs ------- */
typedef struct vector Vect;
typedef enum array_types VectType;



/* ------- Function headers ------- */
void vectInit(Vect *vector_to_initialize, VectType inner_array_type, size_t initial_size);

void vectFree(Vect *target_vector);

void vectRem(Vect *target_vector, unsigned int index);
void vectRanRem(Vect *target_vector, unsigned int starting_index, unsigned int ending_index);
void vectAppend(Vect *target_vector, void *val);
int vectContains(Vect *target_vector, void *val);
void vectSet(Vect *target_vector, void *val, unsigned int index);    

char c_vectPop(Vect *target_vector);
int i_vectPop(Vect *target_vector);

void c_vectAdd(Vect *target_vector, char *string_to_append);
void i_vectAdd(Vect *target_vector, int *int_array_to_append, unsigned int how_many);


/* ------------ NOT to be called directly ------------- */
void private_vectCheckSize_shrink(Vect *target_vector, VectType inner_array_type);
void private_vectShrink(Vect *target_vector, void *managed_array, size_t size_of_item);
void private_vectCheckSize_grow(Vect *target_vector, VectType inner_array_type);
void private_vectGrow(Vect *target_vector, void *managed_array, size_t size_of_item);
/* ------------------------------ */

