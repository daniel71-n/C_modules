#include <stdio.h>
#include <string.h>
#include "vect.h"
#include <stdlib.h>
#include <assert.h>


void vectInit(Vect *vector_to_initialize, VectType inner_array_type, size_t initial_size){
    /* Initialize the inner array for an already declared Vect object
       to the length specified by the initial_size argument, so that the said Vect object can start being used.

       The inner array can be either an int or a char array. This is to be specified with the inner_array_type
       argument, which is an VectType enum, which possible values of I_ARRAY and C_ARRAY. 
       In other words, those are the two values that can be passed as the inner_array_type argument. 

       Depending on the value of inner_array_type, memory is allocated to one of the two members (int *i, char *c)
       of the dynarray union nested in the Vect struct.

       Depending on how you initialized the Vect you called VectInit on (i.e. as a C_ARRAY or an I_ARRAY),
       a different set of functions need to be called to manage it. The functions are essentially the same, 
       but structs whose inner array type is C_ARRAY must call the functions prefixed with 'c_', e.g.
       c_vectAppend, c_VectRem, c_VectPop etc., while structs whose inner array has been initialized
       as type I_ARRAY must call the functions prefixed with 'i_' instead, e.g. i_vectAppend,
       i_vectRem, i_VectPop, etc.
    */
    printf("initial size arg is %zu, and the vecttype arg is %i\n", initial_size, inner_array_type);

    vector_to_initialize->type = inner_array_type;  // set the type of the array based on the VectType enum passed as parameter;
    switch (vector_to_initialize->type) {
        case C_ARRAY:   // if the argument specifies a char array
            {   // can't declare variables if not wrapped in curly braces 
                char *check_not_null;
                if (!(check_not_null = calloc(initial_size, sizeof(char)))){     // if the returned value is NULL
                     // printf("calloc failed to allocate memory to char array\n");
                    exit(EXIT_FAILURE);
                }
                vector_to_initialize->dynarray.c = check_not_null;
                vector_to_initialize->total_array_length=initial_size;
                vector_to_initialize->last_index = -1;
                break;
            }

            case I_ARRAY:   // if the argument specifies an int array
            {
                int *check_not_null; 
                if (!(check_not_null = calloc(initial_size, sizeof(int)))){     // if the returned value is NULL
                    printf("calloc failed to allocate memory to int array\n");
                    exit(EXIT_FAILURE);
                }
                vector_to_initialize->dynarray.i= check_not_null; 
                vector_to_initialize->total_array_length=initial_size;
                vector_to_initialize->last_index = -1;
                break;
            }
       // printf("initialized a vector with size %zu\n", initial_size);
    }
}



void c_vectAppend(Vect *target_vector, char val){
    /* Append a value (specified by the val argument) to the managed array of
       the Vect object specified in the target_vector argument.

      The index the val argument is assigned to is the last_index member in the Vect struct, + 1.  
    */
    assert(target_vector->type==C_ARRAY && "is C_ARRAY");

    target_vector->dynarray.c[target_vector->last_index+1]=val;       // last_index's initial value is set to -1, so the first value will be appended at index -1+1 => 0.
    target_vector->last_index++;                                    // increment last_index, and consequently the position of the next append operation. 
    target_vector->dynarray.c[target_vector->last_index+2]= '\0';     // set the value of the index immediately following last_index to NUL.
     //the above isn't needed because calloc initializes the array to all 0s
     // printf("appending to position %u\n", target_vector->last_index);

    /*
    printf("total array length, %zu\n", target_vector->total_array_length);
    for (unsigned int ind = 0; ind < target_vector->total_array_length; ind++) {
       printf("current index %u, value %c\n", ind, target_vector->dynarray.c[ind]);
    }
    */
    if (target_vector->last_index+2 == target_vector->total_array_length){
       char *check_not_null;
       if (!(check_not_null = realloc(target_vector->dynarray.c, target_vector->total_array_length*2))){     // if the returned value is NULL
             // printf("realloc failed to allocate memory to char array\n");
            exit(EXIT_FAILURE);
       }  
       target_vector->dynarray.c = check_not_null;     // double the innerarray of the vector struct in size.
       target_vector->total_array_length = target_vector->total_array_length*2;
        // printf("the array has been doubled in size.\n The new size is %zu\n", target_vector->total_array_length);    
    }
}
     


void i_vectAppend(Vect *target_vector, int val){
    /* Append a value (specified by the val argument) to the managed array of 
       the Vect object specified in the target_vector argument.

      The index the val argument is assigned to is the last_index member in the Vect struct+1.  
    */
    assert(target_vector->type==I_ARRAY && "is I_ARRAY");
    
    target_vector->dynarray.i[target_vector->last_index+1]=val;       // last_index's initial value is set to -1, so the first value will be appended at index -1+1 => 0.
    target_vector->last_index++;                                    // increment last_index, and consequently the position of the next append operation. 
     // printf("appending to position %u\n", target_vector->last_index);

    /*
    printf("total array length, %zu\n", target_vector->total_array_length);
    for (unsigned int ind = 0; ind <= target_vector->last_index; ind++) {
        printf("current index %u, value %i\n", ind, target_vector->dynarray.i[ind]);
    }
    */
    if (target_vector->last_index+1 == target_vector->total_array_length){  // it's only last_index+1 here because int arrays don't need to be NUL terminated
                                                                            // so in a 4-item array, for example, you can assign even to the last index, i.e.
                                                                            // [3], before resizing the array, whereas char arrays would resize after [2] is assigned to.

       int *check_not_null;
       if (!(check_not_null = realloc(target_vector->dynarray.i, sizeof(int) * target_vector->total_array_length*2))){     // if the returned value is NULL
         // printf("realloc failed to allocate memory to char array\n");
        exit(EXIT_FAILURE);
        }  
       target_vector->dynarray.i = check_not_null;     // double the innerarray of the vector struct in size.
       target_vector->total_array_length = target_vector->total_array_length*2;
        // printf("the array has been doubled in size.\n The new size is %zu\n", target_vector->total_array_length);
    }
}




void c_vectAdd(Vect *target_vector, char *string_to_append){
    /* Append each char in string_to_append to target_vector's managed array  */ 
    assert(target_vector->type==C_ARRAY && "is C_ARRAY");   // abort if .type !=  C_ARRAY

    for (unsigned int ind = 0; string_to_append[ind] != '\0'; ind++) {
         // printf("appending %c\n", string_to_append[ind]);
         // printf("string now %s\n", target_vector->dynarray.c);
        c_vectAppend(target_vector, string_to_append[ind]);
    }
}



void i_vectAdd(Vect *target_vector, int *int_array_to_append, unsigned int how_many){
    /* Append how_many items from int_array_to_append to target_vector's managed array.

       Since '\0' (NUL) can't be used to mark and find the end of the array, 
       i_vectAdd - unlike c_vectAdd - has a third parameter (how_many),
       specifying the number of items to be copied from int_array_to_append.

       An alternative with be using another sentinel value e.g. -1, but that's 
       counting on the int_array_to_append not forgetting to include one,
       which is more precarious than simply passing an argument.
    */ 
    assert(target_vector->type==I_ARRAY && "is I_ARRAY");   // abort if .type !=  C_ARRAY

    for (unsigned int ind = 0; ind < how_many; ind++) {     // '\0' can't be used to find the end of an array, so -1 is being used as the sentinel instead 
         // printf("appending %i\n", int_array_to_append[ind]);
         // printf("last index is now %i\n", target_vector->dynarray.i[target_vector->last_index]);
        i_vectAppend(target_vector, int_array_to_append[ind]);
    }
}



char c_vectPop(Vect *target_vector){
    /* Get the last char in the managed array of the Vect struct and return it. 
       The char will be REMOVED from the array by setting that index position to NUL,
       and decrementing last_index, shrinking the length of the string by 1.
    */ 
    assert(target_vector->type==I_ARRAY && "is C_ARRAY");   // abort if .type !=  C_ARRAY

    char popped = target_vector->dynarray.c[target_vector->last_index];
    target_vector->dynarray.c[target_vector->last_index] = '\0';
    target_vector->last_index --;
    return popped;
}



int i_vectPop(Vect *target_vector){
    /* Get the last int in the managed array of the Vect struct and return it. 
       The int will be REMOVED from the array, and last_index will be decremented, 
       shrinking the length of the array (i.e. the value of total_array_length) by 1.
    */ 
    assert(target_vector->type==I_ARRAY && "is I_ARRAY");   // abort if .type !=  C_ARRAY
   
    int popped = target_vector->dynarray.i[target_vector->last_index];
    target_vector->dynarray.i[target_vector->last_index] = 0;
    target_vector->last_index --;
    return popped;
}



void c_vectRem(Vect *target_vector, unsigned int index){
    /* REMOVE the value at index INDEX in the managed array of the Vect
       struct. This will be a comparatively costly operation, since all the values at index n > INDEX 
       have to be shifted back to n-1.
    */
    assert(target_vector->type==I_ARRAY && "is C_ARRAY");   // abort if .type !=  C_ARRAY
 
    if (index <= target_vector->last_index){
        for( unsigned int ind = index; ind<= target_vector->last_index; ind++){
            target_vector->dynarray.c[ind] = target_vector->dynarray.c[ind+1];  // shift back on position all values from start_index onward ;
        }
        target_vector->last_index--;
    }
    else{
         printf("index not part of string\n");
    }
}



void i_vectRem(Vect *target_vector, unsigned int index){
    /* REMOVE the value at index INDEX in the managed array of the Vect struct. 
       This will be a comparatively costly operation, since all the values at index n > INDEX 
       have to be shifted back to n-1.
    */ 
    assert(target_vector->type==I_ARRAY && "is I_ARRAY");   // abort if .type !=  C_ARRAY
    
    if (index <= target_vector->last_index){
        for( unsigned int ind = index; ind<= target_vector->last_index; ind++){
            target_vector->dynarray.i[ind] = target_vector->dynarray.i[ind+1];  // shift back on position all values from start_index onward ;
        }
        target_vector->last_index--;
    }
    else{
        printf("index out of bounds\n");
    }
}




int c_vectContains(Vect *target_vector, char val){
    /* Return an integer representing the index of the first occurence of val, if found, 
       else -1. 
    */
    assert(target_vector->type==I_ARRAY && "is C_ARRAY");   // abort if .type !=  C_ARRAY
     // printf("val to search for is %c\n", val);
    int res;
    for (unsigned int ind=0; ind <= target_vector->last_index; ind++){
        if (target_vector->dynarray.c[ind] == val){
            res=ind;
            break;
        }
        else if (target_vector->dynarray.c[ind] == '\0'){
            res=-1;
             // printf("bumped into NUL at index %i\n", ind);
            break;
        }
    }
    return res;
}


int i_vectContains(Vect *target_vector, int val){
    /* Return an integer representing the index of the first occurence of val, if found, 
       else -1. 
    */
    assert(target_vector->type==I_ARRAY && "is I_ARRAY");   // abort if .type !=  C_ARRAY
     // printf("val to search for is %i\n", val);
    int res;
    for (unsigned int ind=0; ind <= target_vector->last_index; ind++){
        if (target_vector->dynarray.i[ind] == val){
            res=ind;
            return res;
        }
    }
    return -1;
}



void c_vectSet(Vect *target_vector, char val, unsigned int index){
    /* assign val to the managed array of the Vect struct at index INDEX,
       if index is < last_index, else append the value instead.
    */
    assert(target_vector->type==I_ARRAY && "is C_ARRAY");   // abort if .type !=  C_ARRAY
   
    if (index <= target_vector->last_index){
        target_vector->dynarray.c[index] = val;
    }
    else{
         // printf("attempted to change an empty slot. Appending instead...\n");
        c_vectAppend(target_vector, val);
    }
}



void i_vectSet(Vect *target_vector, int val, unsigned int index){
    /* assign val to the managed array of the Vect struct at index INDEX,
       if index is < last_index, else append the value instead.
    */
    assert(target_vector->type==I_ARRAY && "is I_ARRAY");   // abort if .type !=  C_ARRAY
   
    if (index <= target_vector->last_index){
        target_vector->dynarray.i[index] = val;
    }
    else{
         // printf("attempted to change an empty slot. Appending instead...\n");
        i_vectAppend(target_vector, val);
    }
}

void c_vectFree(Vect *target_vector){
    /*  Free the memory allocated to the managed array of the Vect struct. */
    assert(target_vector->type==I_ARRAY && "is C_ARRAY");   // abort if .type !=  C_ARRAY

    free(target_vector->dynarray.c);
    target_vector->dynarray.c = NULL;
    target_vector->total_array_length=0;
    target_vector->last_index=-1;
     // printf("freed. size is now %zu", target_vector->total_array_length);

}
void i_vectFree(Vect *target_vector){
    /*  Free the memory allocated to the managed array of the Vect struct. */
    assert(target_vector->type==I_ARRAY && "is C_ARRAY");   // abort if .type !=  C_ARRAY

    free(target_vector->dynarray.i);
    target_vector->dynarray.i = NULL;
    target_vector->total_array_length=0;
    target_vector->last_index=-1;
     // printf("freed. size is now %zu", target_vector->total_array_length);

}
