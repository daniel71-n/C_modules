#include <string.h>
#include <stdlib.h>
#include "pstrings.h"

void str_rev(char string_to_reverse[], char string_reversed[]){
    /* Reverse string_to_reverse, writing the result to the string_reversed char array.
       string_reversed is uninitialized to a specific length, in the global scope, when this function is called.

       This function calloc-ates memory to string_reversed based on what's determined to be sufficient,
       modifies this newly-created array by assigning values to its indexes, then returns a pointer to the
       first item in the array. The result is that the program calling this function doesn't have to initialize
       an array to a specific length before passing it as an argument for the string_reversed parameter, 
       but can instead just do << char someArr = str_rev(array_to_reverse, someArr); >>, and the function
       will take care of initializing the array, as explained above, to the correct length and populated
       by the chars in string_to_reverse in reversed order.    
       
       By the time the function returns a value, string_reversed has already been modified. That's already the 
       end product. So technically, the function returning a value isn't even needed, since string_reversed
       already, at this point, contains exactly the value that the function is returning. All the work has been done. 
       However, string_reversed is being initialized in global scope by being assigned to the return value of this function.
       Even though the value isn't needed, it would be a syntax error not to return a value since you can't assign nothing 
       to something. You can assign it to null, of course, but null isn't void (null, obviously, isn't a viable return value
       in the first place sicne assigning string_reversed to null would render this function useless).
       So that's why a value is being returned - for syntax reasons only - otherwise the assignment operation
       --i.e. e.g. char* someArr = str_rev(string_to_reverse, someArr) -- would fail.      

       The caller will have to call free() manually to deallocate the memory when string_reversed 
       is no longer needed and can be destroyed.
    */

    int orig_length = strlen(string_to_reverse);     //  get the length of 'orig'; this doesn't count the terminating NULL character
    string_reversed[orig_length] = '\0';  // all strings need to be NULL-terminated

    unsigned int ind = 0;
    for(unsigned i = 0, k = orig_length-1; i < orig_length; i++, k--){
        string_reversed[k] = string_to_reverse[i];
    }
}



void str_rev_ip(char string_arg[]){
    /* Reverse the string argument (an array of chars) IN PLACE */
    
    unsigned int str_length = strlen(string_arg);  
    unsigned int largest_index = str_length-1;  // the length is -1 since counting starts at 0. 
    char temp; 

    if (str_length % 2 == 0){
        // if the length is even
        // printf("length is even, %i\n", str_length)
        for (unsigned int start_index=0, end_index=largest_index; start_index<end_index; start_index++, end_index--){
            // stop the iteration as soon as start_index < end_index becomes start_index > end_index
            // which will happen when they both move on from the two median elements
            temp = string_arg[start_index];
            string_arg[start_index] = string_arg[end_index];
            string_arg[end_index] = temp;
        }
    }
    else if (str_length % 2 == 1){
        // if the length is odd
        // printf("length is odd: %i\n", str_length);
        for (unsigned int start_index=0, end_index=largest_index; start_index != end_index; start_index++, end_index--){
            // stop the iteration when start_index and end_index become equal - which will
            // happen when both indexes reach the median element 
            temp=string_arg[start_index];
            string_arg[start_index] = string_arg[end_index];
            string_arg[end_index] = temp;
        }
    }
}




