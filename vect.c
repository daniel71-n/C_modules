#include <stdio.h>
#include <string.h>
#include "vect.h"
#include <stdlib.h>

void vectInit(Vect *vector_to_initialize, size_t initial_size){
    /* Initialize the .dynaArray inner array member of an already declared Vect object
       to the length set by VECT_SIZE, so that the said Vect object can start being used.
    */
    vector_to_initialize->dynArray = calloc(initial_size, sizeof(char));
    vector_to_initialize->total_array_length=initial_size;
    vector_to_initialize->last_index = -1;
    printf("initialized a vector with size %zu\n", initial_size);
}


void vectAppend(Vect *target_vector, char val){
    /* Append a value (specified by the val argument) to the dynArray inner array member of 
       the Vect object specified in the target_vector argument.

       A search is first carried out for the occurence of null. 
    */
    target_vector->dynArray[target_vector->last_index+1]=val;       // last_index's initial value is set to -1, so the first value will be appended at index -1+1 => 0.
    target_vector->last_index++;                                    // increment last_index, and consequently the position of the next append operation. 
     // target_vector->dynArray[target_vector->last_index+2]= '\0';     // set the value of the index immediately following last_index to NULL.
    printf("appending to position %u\n", target_vector->last_index);

    printf("total array length, %zu\n", target_vector->total_array_length);
    for (unsigned int ind = 0; ind < target_vector->total_array_length; ind++) {
        printf("current index %u, value %c\n", ind, target_vector->dynArray[ind]);
    }

    if (target_vector->last_index+2 == target_vector->total_array_length){
       target_vector->dynArray = realloc(target_vector->dynArray, target_vector->total_array_length*2);     // double the innerarray of the vector struct in size.
       target_vector->total_array_length = target_vector->total_array_length*2;
       printf("the array has been doubled in size.\n The new size is %zu\n", target_vector->total_array_length);
    }
}


int vectContains(Vect *target_vector, char val){
    /* Return an integer representing the index of the first occurence of val, if found, 
       else -1. 
    */
    printf("val to search for is %c\n", val);
    int res;
    for (unsigned int ind=0; ind < target_vector->total_array_length-1; ind++){
        if (target_vector->dynArray[ind] == val){
            res=ind;
            break;
        }
        else if (target_vector->dynArray[ind] == '\0'){
            res=-1;
            printf("bumped into null at index %i\n", ind);
            break;
        }
    }
    return res;
}



void vectSet(Vect *target_vector, char val, unsigned int index){
    if (index <= target_vector->last_index){
        target_vector->dynArray[index] = val;
    }
    else{
        printf("attempted to change an empty slot. Appending instead...\n");
        vectAppend(target_vector, val);
    }
}










