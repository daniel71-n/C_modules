#include <stdint.h>
#include <stdbool.h>

void str_rev_ip(char string_arg[]);   // reverse string_arg in place
void str_rev(char string_to_reverse[], char string_reversed[]);  // reverse string_arg and store the result in string_reversed

unsigned int str_len(char string_arg[]);  // return the length of string_arg, not counting the terminating Nul character

// convert a string to an int, provided the string doesn't contain prohibited (non-numeric) characters
long str_to_int(char string_arg[]);  

// get a char array offering the ASCII representation of the digits in num
char *str_from_int(long num);

// count digits in num
unsigned short str_count_digits(long num);

// split string_arg into tokens and return a pointer to the next token on each call
char *str_tokenize(char string_arg[], char delimiter);


/* Copy the contents of str2 into str1 until NULL is encountered.

   Return -1 if any of the strings are NULL (and thus invalid input),
   or else the value of the index when the copying is done, i.e.
   where in str1 the process finished.

   This might be useful for knowing exactly where to terminate str1
   or/and knowing where to continue copying from another string.

   For example, if str1 is 25 chars long and str2 is 10 chars long,
   str_copy will copy everything in str2 into str1 (10 chars),
   and then it will stop, because it ran into NUL, the string 
   terminatior for str2.
   Str_copy does not terminate str1. Instead, it will return 
   the value 11, in this case. 
   The caller can then either insert NULL here and terminate str1
   ( str1[the_return_value_of_str_copy] = '\0'), or call
   str_copy with str1 again, and another string.
   Str_copy will then start copying from str2 and inserting
   the read items into str1 starting at str1[11].
*/
long str_copy(char str1[], char str2[]);


/* Compare str1 to str2. Return 0 if str > str2,
 * 1 if str1 == str2, and 2 if str1 < str2.
 * The comparison is based on alphabetical comparisons. 
 * That is, each char in string1 is compared with the char
 * in str2 at that specific position. If the 2 chars are 
 * equal, the comparison moves on to the next char in the strings.
 * Otherwise, return 0 if the char in str1 is smaller than the char
 * in str2, or 2 if it's the other way around.
 * 1 is returned if the end of both strings has been reached and 
 * the two strings are equal in length. 
 * Otherwise, is str1 is shorter in length than str2, with them
 * being otherwise equal, 0 is returned.
 *
 * ----- NOTES -----
 *  Uppercase characters are considered to be larger than their 
 *  lower case counterpart. The comparison is based on the 
 *  position of the character in the ASCII set
 *
 * In line with the position of the characters in the ASCII character
 * set, uppercase is considered to be smaller than lowercase. 
 * i.e. 'A' < 'a'.
 */
unsigned short str_compare(char str1[], char str2[]);

bool str_is_same(char str1[], char str2[]);




