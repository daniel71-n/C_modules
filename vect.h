#include <stdlib.h>


/* ------- Structs and Enums ------- */
enum array_types{C_ARRAY, I_ARRAY};

struct vector{
    union{
    char *c;             // inner array to be resized dynamically : used if type == C_ARRAY; 
    int *i;             // memory will be allocated to this instead if type == I_ARRAY;
    }dynarray; 
    enum array_types type;      // enum specifying the type of the inner array - and consequently, which inner array gets allocated memory
    size_t total_array_length;  // reference variable holding the size of the array
    size_t space_available;     // how many array slots are unused; if less than half of them are, shrink dynArray to half the size
    int last_index;             // if last_index == total_array_length-2, double the size of dynArray

/* the reason last_index is checked to be equal to total_array_length - 2 is that, for example, a 15-item array
   will have its last item at index 14 - not 15, since counting starts at 0. 
   So to get the last item, you would do array[total_array_length-1].
   But in the case of strings, they need to be NUL terminated - meaning that total_array_length - 1
   will be set to NUL. So the last (non-Nul) item will then be at the index preceding that: 
   i.e. at array[total_array_length - 2].

   Non-char arrays, of course, don't need NUL termination, so last_index can be equal to total_array_length - 1 there.
*/
};


/* ------- Typedefs ------- */
typedef struct vector Vect;
typedef enum array_types VectType;



/* ------- Function headers ------- */
void vectInit(Vect *vector_to_initialize, VectType inner_array_type, size_t initial_size);

void c_vectAppend(Vect *target_vector, char val);
void i_vectAppend(Vect *target_vector, int val);

char c_vectPop(Vect *target_vector);
int i_vectPop(Vect *target_vector);

void c_vectRem(Vect *target_vector, unsigned int index);
void i_vectRem(Vect *target_vector, unsigned int index);

void c_vectAdd(Vect *target_vector, char *string_to_append);
void i_vectAdd(Vect *target_vector, int *int_array_to_append, unsigned int how_many);

int c_vectContains(Vect *target_vector, char val);
int i_vectContains(Vect *target_vector, int val);

void c_vectSet(Vect *target_vector, char val, unsigned int index);    
void i_vectSet(Vect *target_vector, int val, unsigned int index);    

void c_vectFree(Vect *target_vector);
void i_vectFree(Vect *target_vector);
