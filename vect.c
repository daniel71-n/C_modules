#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "vect.h"


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


void private_vectShrink(Vect *target_vector, void *managed_array, size_t size_of_item){
    /*  Halve the amount of memory allocated to the managed array of the Vect object  */
    void *temp = realloc(managed_array, target_vector->total_array_length/2 * size_of_item);
    if (!temp){
        printf("shrinking failed\n");
        exit(EXIT_FAILURE);
    }
    else{
        if (target_vector->type == C_ARRAY) {target_vector->dynarray.c=temp;}
        else if (target_vector->type == I_ARRAY) {target_vector->dynarray.i=temp;}
        target_vector->total_array_length/=2;
    }
}



void private_vectCheckSize_shrink(Vect *target_vector, VectType inner_array_type){
    /*  Check if the last_index value is smaller that half of total_array_length,
        i.e. if the managed array is using less than half of the memory allocated, 
        and call private_vectShrink if that's found to be the case.
       
    */
    switch(inner_array_type){
        case C_ARRAY:
            if (target_vector->last_index < target_vector->total_array_length/2 - 2){   // if only less than half the array capacity is being used 
                private_vectShrink(target_vector, target_vector->dynarray.c, sizeof(char));
            }
            break;
        case I_ARRAY:
            if (target_vector->last_index < target_vector->total_array_length/2 - 1){  
                private_vectShrink(target_vector, target_vector->dynarray.i, sizeof(int));
            }
            break;
    }
}



void private_vectGrow(Vect *target_vector, void *managed_array, size_t size_of_item){
    /*  Double the amount of memory allocated to the managed array of the Vect object  */
    void *temp = realloc(managed_array, target_vector->total_array_length * 2 * size_of_item);
    if (!temp){
        printf("allocation failed\n");
        exit(EXIT_FAILURE);
    }
    else{
        if (target_vector->type == C_ARRAY) {target_vector->dynarray.c=temp;}
        else if (target_vector->type == I_ARRAY) {target_vector->dynarray.i=temp;}
        target_vector->total_array_length*=2;
    }
}



void private_vectCheckSize_grow(Vect *target_vector, VectType inner_array_type){
    /* Check whether the array needs to be doubled in size.

       Specifically, check to see if there's only one position not occupied in the array, at 
       last_index+1. This is to be reserved for the NUL sentinel in char arrays and will otherwise
       be empty in int arrays and such. 

       The way the function determines this is by evaluating if last_index+3 > total_array_length. 
       In more detail: given 5-item array, total_array_length will be 5. But the last possible index,
       since counting starts at 0, is 4. This is either empty or filled by a NUL. So the last actual
       index filled with a value is 3, which is the fourth item in the array.
       Therefore when a value is assigned to index 3 (the last item bar NUL), the array should be 
       doubled in size. The smallest value that would satisfy the condition given above, upon evaluation,
       and trigger the calling of private_vectGrow is 3. If the value were 2 instead, last_index
       would be incremented by one again up to 4 (continuing with the 5-item array example above),
       which would make it the last allowable index inside the array bounds, and NUL
       would be overwritten. So private_vectGrow needs to be called BEFORE this point,
       when index 3, rather than 4, is assigned a value.
    */
    if (target_vector->last_index+3 > target_vector->total_array_length){
        switch(inner_array_type){
            case C_ARRAY:
                private_vectGrow(target_vector, target_vector->dynarray.c, sizeof(char)); 
                break;
            case I_ARRAY:
                private_vectGrow(target_vector, target_vector->dynarray.i, sizeof(int));
                break;
            }
    }
}






void c_vectAppend(Vect *target_vector, char val){
    /* Append a value (specified by the val argument) to the managed array of
       the Vect object specified in the target_vector argument.

      The index the val argument is assigned to is the last_index member in the Vect struct, + 1.  
    */
    assert(target_vector->type==C_ARRAY && "is C_ARRAY");
    printf("last index initial value is %i", target_vector->last_index);
    target_vector->dynarray.c[target_vector->last_index+1]=val;       // last_index's initial value is set to -1, so the first value will be appended at index -1+1 => 0.
    target_vector->last_index = target_vector->last_index+1;                                    // increment last_index, and consequently the position of the next append operation. 
    printf("in vectappend: last index is now %i", target_vector->last_index);
    target_vector->dynarray.c[target_vector->last_index+1]= '\0';     // set the value of the index immediately following last_index to NUL.
    printf("appending to NUL to position %u\n", target_vector->last_index+1);
    private_vectCheckSize_grow(target_vector, C_ARRAY);
    /*
    printf("total array length, %zu\n", target_vector->total_array_length);
    for (unsigned int ind = 0; ind < target_vector->total_array_length; ind++) {
       printf("current index %u, value %c\n", ind, target_vector->dynarray.c[ind]);
    }
    */
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
    if (target_vector->last_index+2 > target_vector->total_array_length){  // it's only last_index+2 here because int arrays don't need to be NUL terminated
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
         printf("appending %c\n", string_to_append[ind]);
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
    assert(target_vector->type==C_ARRAY && "is C_ARRAY");   // abort if .type !=  C_ARRAY

    char popped = target_vector->dynarray.c[target_vector->last_index];
    target_vector->dynarray.c[target_vector->last_index] = '\0';
    target_vector->last_index --;
    private_vectCheckSize_shrink(target_vector, C_ARRAY);
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
    private_vectCheckSize_shrink(target_vector, I_ARRAY);
    return popped;
}




void vectRem(Vect *target_vector, unsigned int index){
    /* REMOVE the value at index INDEX in the managed array of the Vect
       struct. This will be a comparatively costly operation, since all the values at index n > INDEX 
       have to be shifted back to n-1.

       Also call private_vectCheckSize to determine whether the array needs to be shrunk.
    */

    switch(target_vector->type){
        case C_ARRAY:   // if the managed array is a char array
            if (index <= target_vector->last_index){
                for( unsigned int ind = index; ind<= target_vector->last_index; ind++){
                    target_vector->dynarray.c[ind] = target_vector->dynarray.c[ind+1];  // shift back one position all values from start_index onward ;
                }
                target_vector->last_index--; 
                target_vector->dynarray.c[target_vector->last_index+1]='\0'; 
                private_vectCheckSize_shrink(target_vector, C_ARRAY);
            }
            else{
                 printf("index not part of string\n");
            }
            break; 

        case I_ARRAY:   // else if the managed arra is an int array
            if (index <= target_vector->last_index){
                for( unsigned int ind = index; ind<= target_vector->last_index; ind++){
                    target_vector->dynarray.i[ind] = target_vector->dynarray.i[ind+1];  // shift back on position all values from start_index onward ;
                }
                target_vector->last_index--;
                private_vectCheckSize_shrink(target_vector, I_ARRAY); 
            }
            else{
                printf("index out of bounds\n");
            }
            break;
    }
}




void vectRanRem(Vect *target_vector, unsigned int starting_index, unsigned int ending_index){
    /* Range Remove : REMOVE the values in the managed array of the Vect struct that are between 
       starting_index (inclusive) and ending_index (exclusive).

       If less than half of the array capacity is used (call private_vectCheckSize to check), 
       halve the memory allocated to it (private_vectCheckSize will in turn call private_vectSrink).
    */
    assert(starting_index < ending_index);  // raise an exception is ending index <= starting_index
    
    if (! (starting_index < target_vector->last_index && ending_index <= target_vector->last_index+1)){
        printf("starting_index and/or ending_index out of bounds\n");
        exit(EXIT_FAILURE);
    }

    unsigned int last_index_before_deletion = target_vector->last_index;
    
    switch(target_vector->type){
        case C_ARRAY:
            for (unsigned int start = starting_index; start < ending_index; start++){
                target_vector->dynarray.c[start] = '\0';
                target_vector->last_index--;    // safe to be decremented since last_index_before_deletion stores the initial value from before any change
                 // printf("decrementing last_index by one\n");
            }
            // determine how many items will be removed, and then how many items will need shifting backward how many positions
            if (ending_index < last_index_before_deletion+1){    
            // if the range of deleted items is within the array, rather than up to the end of it (as in that case nothing would need to be shifted back)
                unsigned int num_of_pos_to_shift_back = ending_index-starting_index;     // the number of positions all the items at index n>=ending_index need to be shifted back;
                for (unsigned int start = ending_index; start<=last_index_before_deletion; start++){
                    target_vector->dynarray.c[start-num_of_pos_to_shift_back]=target_vector->dynarray.c[start];
                }
                for (unsigned int start = target_vector->last_index+1; start<=last_index_before_deletion; start++){
                    target_vector->dynarray.c[start] = '\0';  // set to NUL all everything after last_index up to and including last_index_after_deletion
                }
            }           
            private_vectCheckSize_shrink(target_vector, C_ARRAY);
            break;
    
        case I_ARRAY:
            for (unsigned int start = starting_index; start < ending_index; start++){
                target_vector->dynarray.i[start] = 0;
                target_vector->last_index--;    // safe to be decremented since last_index_before_deletion stores the initial value from before any change
                 // printf("decrementing last_index by one\n");
            }
            if (ending_index < last_index_before_deletion+1){    
                unsigned int num_of_pos_to_shift_back = ending_index-starting_index;     
                for (unsigned int start = ending_index; start<=last_index_before_deletion; start++){
                    target_vector->dynarray.i[start-num_of_pos_to_shift_back]=target_vector->dynarray.i[start];
                }
                for (unsigned int start = target_vector->last_index+1; start<=last_index_before_deletion; start++){
                    target_vector->dynarray.i[start] = 0;  // set to 0 all everything after last_index up to and including last_index_after_deletion
                }
            }           
            private_vectCheckSize_shrink(target_vector, I_ARRAY); 
            break;
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



void vectFree(Vect *target_vector){
    /*  Free the memory allocated to the managed array of the Vect struct. */
    switch(target_vector->type){
     
        case C_ARRAY:
            free(target_vector->dynarray.c);
            target_vector->dynarray.c = NULL;
            target_vector->total_array_length=0;
            target_vector->last_index=-1;
             // printf("freed. size is now %zu", target_vector->total_array_length);
            break;
        
        case I_ARRAY:   
            free(target_vector->dynarray.i);
            target_vector->dynarray.i = NULL;
            target_vector->total_array_length=0;
            target_vector->last_index=-1;
             // printf("freed. size is now %zu", target_vector->total_array_length);
            break;
        }
}


