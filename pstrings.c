#include <stdlib.h>
#include "pstrings.h"


unsigned int str_len(char string_arg[]){
/* Find and return the length of string_arg. 

   A string ends when Nul is encountered, i.e.
   all strings are Nul-terminated. If a char array
   is not Nul terminated, then it's not a string,
   and the length can't be determined correctly.
*/
    unsigned int ind = 0, count = 0;

    while(string_arg[ind] != '\0'){
        ind++;
        count++;
    }
    return count;
};


void str_rev(char string_to_reverse[], char string_reversed[]){
/* Reverse string_to_reverse, writing the result to the string_reversed char array.*/

    int orig_length = str_find_length(string_to_reverse);     //  get the length of 'orig'; this doesn't count the terminating NULL character
    string_reversed[orig_length] = '\0';  // all strings need to be NULL-terminated

    unsigned int ind = 0;
    for(unsigned i = 0, k = orig_length-1; i < orig_length; i++, k--){
        string_reversed[k] = string_to_reverse[i];
    }
}



void str_rev_ip(char string_arg[]){
    /* Reverse the string argument (an array of chars) IN PLACE */
    
    unsigned int str_length = str_find_length(string_arg);  
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




