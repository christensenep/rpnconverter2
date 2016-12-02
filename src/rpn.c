#include <string.h>
#include <stdlib.h>

#include "rpn.h"
#include "dynamic_string.h"

#define RPN_OPERATORS "+-*/^"

int isOperator(char character) {
  if (character != '\0' && strchr(RPN_OPERATORS, character) != NULL) {
    return 1;
  }
  else {
    return 0;
  }
}

int getPriority(char operator) {
  const char* ptr = strchr(RPN_OPERATORS, operator);
  if (ptr == NULL || operator == '\0') {
    return -1;
  }
    
  return ptr - RPN_OPERATORS;
}

char* rpn_infix_to_postfix(const char* infixString) {
  rpn_DynamicString* operatorDynString = rpn_DynamicString_create();
  rpn_DynamicString* postfixDynString = rpn_DynamicString_create();

  const char* currentInfixStringPos = infixString;;
  
  while (*currentInfixStringPos != '\0') {
    if (isOperator(*currentInfixStringPos)) {
      char lastOperator = rpn_DynamicString_popChar(operatorDynString);

      while (getPriority(lastOperator) >= getPriority(*currentInfixStringPos)) {
        rpn_DynamicString_addChar(postfixDynString, lastOperator);
        lastOperator = rpn_DynamicString_popChar(operatorDynString);
      }

      if (lastOperator != '\0') {
        rpn_DynamicString_addChar(operatorDynString, lastOperator);
      }

      rpn_DynamicString_addChar(operatorDynString, *currentInfixStringPos);
    }
    else {
      rpn_DynamicString_addChar(postfixDynString, *currentInfixStringPos);
    }

    currentInfixStringPos++;
  }

  while (operatorDynString->currentLength != 0) {
    rpn_DynamicString_addChar(postfixDynString, rpn_DynamicString_popChar(operatorDynString));
  }

  char* postfixString = rpn_DynamicString_toString(postfixDynString);
  rpn_DynamicString_delete(postfixDynString);
  rpn_DynamicString_delete(operatorDynString);
  return postfixString;
}
