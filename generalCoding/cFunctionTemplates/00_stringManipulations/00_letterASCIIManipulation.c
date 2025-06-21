// Here I will write and store C functions, which manipulate string inputs.
// We will handle functions that do not return allocated memory for now. 
// Those will need to be handled with much care and memory needs to be freed by the user.
//

#ifndef LETTERASCIIMANIPULATION.C
#define LETTERASCIIMANIPULATION.C


// ========== This block will count lower-case characters in a given string ==========
#include <stdlib.h>
#include <string.h>

void countCharacters(int* countChar, char* str) {
  size_t strLength = strlen(str);
  for (size_t i = 0; i < strLength; ++i) {
    ++countChar[str[i] - 'a'];
  }
  return;
}
// ===================================================================================







#endif
