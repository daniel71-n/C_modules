
#include "bit_utils.h"
#include <stdint.h>



uint8_t Count_bits(long long num){
/* Count and return the number of bits in num 

   num can be any integer type <= long long
*/
    uint8_t count = 0;

    while (num){
        count++;
        num >>= 1;
    }
    return count;
};



long long Reverse_bits(long long num){
/* Return the bits in num in reverse order.

   The leftmost bit is a sentinel value and should thus 
   be accounted for when retrieving the bits. 

   So if num is 1011100 in binary, the value returned
   will be |1|0011101 (in binary).
   As mentioned, the leftmost value (as indicated above
   by the vertical bars) is used as a sentinel value,
   for easy detection of where the bits in num end. 
   So it's not part of the reversed value. 

   Therefore when retrieving the bits, the caller 
   needs to disregard the leftmost bit. For example:
        long long reversed  = Reverse_bits(734);
        while (reversed > 1){ // stop at the last bit
            // do something
        }
*/
    long long reversed = 1; // the leftmost 1 serves as a Sentinel Value 
    uint8_t tmp = 0;
    
    while (num){
        tmp = num & 1;    
        reversed <<= 1;
        reversed += tmp;
        num >>= 1;
    }
    return reversed;
}




void get_binary_string(char *holding_string, long long number_to_convert){
/* Get the binary representation of number_to_convert, and store it in 
   the holding_string char array.

   number_to_convert doesn't neccessarily need to to be a long long. That is,
   the function can be called with anything <= to a long long, and the value
   will be safely promoted to that of a (signed) long long, as long as it fits
   into it.

   The holding_string argument is a character array that needs to be large
   enough to hold all the characters necesarry for the binary representation
   of number_to_convert, as well as a terminating NUL. A char array instead of an integer array 
   is used for space efficiency. Since each item will only be either 1 or 0, and since 
   a char is guaranteed to only be 1 byte whereas an int is normally larger, 
   it makes more sense to use an array storing '1's and '0's rather than 1s and 0s, 
   as the latter would likely be a few times as large as the former.

   holding_string should be (at least) of length equal to the number of bits in number_to_convert +1
   (for the terminating NULL). This value could, for example, be obtained with a call to Count_bits().

                                        * * *

    NOTES

    tmp below is converted from an integer with a value of 1 or 0 to a char, i.e. from 1 to '1' or 0 to '0'. 
    This is done by adding tmp up with '0'.
	This takes advantage of the fact that ASCII's character set has consecutive 
	number values for representing the '0', '1','2','3'..'9' characters. This means that '5''s value 
	is 5 places from '0's value, i.e. '0' + 5. 
    '0' + tmp (which is 1 or 0) will get the ASCII value for '1' or '0'.
*/
    uint8_t ind = 0 ; 
    uint8_t tmp = 0;  
    char to_add; // tmp converted to a char --> '0' or '1'
    
    number_to_convert = Reverse_bits(number_to_convert);    // reverse the bits in number_to_convert first

    do {
        tmp = number_to_convert & 1;  
	    to_add = tmp + '0';  
        holding_string[ind] = to_add;
        ind++;
        number_to_convert = number_to_convert >> 1;
    } while(number_to_convert > 1);     // see the comments under the Reverse_bits() function
    
}



