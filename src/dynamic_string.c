#include <stdlib.h>
#include <string.h>

#include "dynamic_string.h"

#define RPN_INITIAL_STRING_LENGTH 8

rpn_DynamicString* rpn_DynamicString_create() {
  rpn_DynamicString* dynamicString = (rpn_DynamicString*) malloc(sizeof(rpn_DynamicString));
  dynamicString->maxLength = RPN_INITIAL_STRING_LENGTH;
  dynamicString->currentLength = 0;
  dynamicString->string = (char*) calloc((size_t) RPN_INITIAL_STRING_LENGTH, sizeof(char));
  return dynamicString;
}

void rpn_DynamicString_delete(rpn_DynamicString* dynamicString) {
  free(dynamicString->string);
  free(dynamicString);
}

void rpn_DynamicString_addChar(rpn_DynamicString* dynamicString, char newChar) {
  if (dynamicString->currentLength == dynamicString->maxLength) {
    dynamicString->maxLength *= 2;
    dynamicString->string = (char*) realloc(dynamicString->string, sizeof(char) * dynamicString->maxLength);
  }

  dynamicString->string[dynamicString->currentLength] = newChar;
  dynamicString->currentLength++;
}

char rpn_DynamicString_popChar(rpn_DynamicString* dynamicString) {
  if (dynamicString->currentLength == 0) {
    return '\0';
  }
  
  dynamicString->currentLength--;
  return dynamicString->string[dynamicString->currentLength];
}

char* rpn_DynamicString_toString(const rpn_DynamicString* dynamicString) {
  char* returnString;

  returnString = (char*) malloc(sizeof(char) * (dynamicString->currentLength + 1));
  memcpy(returnString, dynamicString->string, dynamicString->currentLength);
  returnString[dynamicString->currentLength] = '\0';

  return returnString;
}
