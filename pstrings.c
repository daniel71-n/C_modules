#include <string.h>
#include <stdlib.h>
#include "pstrings.h"

char *str_rev(char *string_to_reverse, char *string_reversed){
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

    char *string_arg = string_to_reverse;
    int orig_length = strlen(string_arg);     //  get the length of 'orig'; this doesn't count the ending null character
    string_reversed = calloc(orig_length+1, sizeof(char));             // an array equal to the length of 'orig' is needed, +1 to hold the ending null character  

    for (int i=orig_length-1, x=0; i>=0; i--, x++) {
    // i tracks the index in the original array; x, in the reversed array
    string_reversed[x]=string_arg[i];    // since they have the same length, when the original array is traversed, the 'reversed' array also is
    }
    return string_reversed;
}



void str_rev_ip(char *string_arg){
    /* reverse the string argument (an array of chars) IN PLACE */
    
    unsigned int str_length = strlen(string_arg);  
    unsigned int largest_index = str_length-1;  // the length is -1 since counting starts at 0. 
    char temp; 

    if (str_length % 2 == 0){
        // if the length is even
        // printf("length is even, %i\n", str_length);
        for (unsigned int start_index=0, end_index=largest_index; start_index<end_index; start_index++, end_index--){
            // stop the iteration as soon as start_index == end_index, which will happen when they both reach the middle character
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
            // happen in the middle of the string, after the innermost characters get swapped
            temp=string_arg[start_index];
            string_arg[start_index] = string_arg[end_index];
            string_arg[end_index] = temp;
        }
    }
}



void get_binary_string(char *holding_string, void *number_to_convert){
/* get the binary representation of number_to_convert, and store it in 
   the holding_string char array.

   The number_to_convert parameter is a void pointer so that that argument passed can
   be either a  short, an int, a long, a long long etc.
   Whatever the case, the pointer will be cast to an unsigned long long pointer, which will then be dereferenced, 
   and its value will be cast to an unsigned long long - the largest integer type. 
   The value of the variable pointed to by the argument won't be changed, 
   but copied into the local scope of the function and only modified here.

   The holding_string argument is a character array that needs to be large
   enough to hold all the characters necesarry for the binary representation
   of number_to_convert, as well as a terminating NUL. A char array instead of an integer array 
   is used for space efficiency. Since each item will only be either 1 or 0, and since 
   a char is guaranteed to only be 1 byte whereas an int is normally larger, 
   it makes more sense to use an array storing '1's and '0's rather than 1s and 0s, 
   as the latter would likely be at least twice as large as the former.
*/

unsigned long long *num_pointer = (unsigned long long *)number_to_convert;
// casting the number pointer argument to an unsigned long long; 
// the pointer could well point to something smaller - a signed int, for example - 
// but promoting the value it points to (below) to an unsigned long long would cause no loss. 

unsigned long long num = *num_pointer;
// since the value gets cast to a U long long, it means a pointer to anything <= to that can
// be passed as an argument, in line with however large the number that's wanted to be converted is.

unsigned int compare = 1; // the value that each bit in number_to_convert is AND-ed with 
unsigned int ind = 0 ;    // indexes the 'holding_string' array
unsigned int tmp = 0;     //the result (to be converted to a char) of the AND bitwise operation mentioned above
char to_add; // tmp converted to a char

do {
	tmp = num & compare;  
	to_add = tmp + '0';  
	holding_string[ind]= to_add;
	ind++;
	num = num>>1;
} while(num != 0);
// ++ Reverse the string, since at this point the 'bits' are stored backward, from right to left
/*
   While number_to_convert isn't 0: 
    - AND it with the 'compare' variable, and assign the result to 'tmp'. 'Tmp' will be 1 if the rightmost
      bit in number_to_compare is 1, and 0 if the rightmost bit in number_to_compare is 0. 
      The rightmost bit in number_to_compare is the one bit being ANDed with 'compare', since compare
      only has one '1' bit (its value is simply 1). 

    - Convert tmp to a char, i.e. from 1 to '1' or 0 to '0'. This is done by adding tmp with '0'.
	This takes advantage of the fact that ASCII's character set has consecutive 
	number values for representing the '0', '1','2','3'..'9' characters. This means that '5''s value 
	is 5 places from '0's value, i.e. '0' + 5. 
    '0' + tmp (which is 1 or 0) will get the ASCII value for '1' or '0'.

    - Shift number_to_convert to the right by one bit and assign this back to number_to_convert. 
      This discards the rightmost bit, so the subsequent iteration of the loop will have the next bit to the left
      ANDed with 'compare' until number_to_compare runs out of bits and becomes 0. 
*/
}



