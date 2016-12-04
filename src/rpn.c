#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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

int isOperand(char character) {
  return islower(character);
}

int getPriority(char operator) {
  const char* ptr = strchr(RPN_OPERATORS, operator);
  if (ptr == NULL || operator == '\0') {
    return -1;
  }
    
  return ptr - RPN_OPERATORS;
}

char* parse_infix_to_postfix(rpn_DynamicString* operatorDynString, rpn_DynamicString* postfixDynString, const char* infixString) {

  const char* currentInfixStringPos = infixString;
  
  while (*currentInfixStringPos != '\0') {
    if (isOperator(*currentInfixStringPos)) {
      char poppedOperator;
      while (getPriority(*currentInfixStringPos) <= getPriority(poppedOperator = rpn_DynamicString_popChar(operatorDynString))) {
        rpn_DynamicString_addChar(postfixDynString, poppedOperator);
      }

      if (poppedOperator != '\0') {
        rpn_DynamicString_addChar(operatorDynString, poppedOperator);
      }

      rpn_DynamicString_addChar(operatorDynString, *currentInfixStringPos);
    }
    else if (isOperand(*currentInfixStringPos)) {
      rpn_DynamicString_addChar(postfixDynString, *currentInfixStringPos);
    }
    else if (*currentInfixStringPos == '(') {
      rpn_DynamicString_addChar(operatorDynString, *currentInfixStringPos);
    }
    else if (*currentInfixStringPos == ')') {
      char poppedOperator = rpn_DynamicString_popChar(operatorDynString);
      while (poppedOperator != '(') {
        if (poppedOperator == '\0') {
          return NULL;
        }
        
        rpn_DynamicString_addChar(postfixDynString, poppedOperator);
        poppedOperator = rpn_DynamicString_popChar(operatorDynString);
      }
    }
    else {
      return NULL;
    }

    currentInfixStringPos++;
  }

  while (operatorDynString->currentLength != 0) {
    rpn_DynamicString_addChar(postfixDynString, rpn_DynamicString_popChar(operatorDynString));
  }

  return rpn_DynamicString_toString(postfixDynString);
}

char* rpn_infix_to_postfix(const char* infixString) {
  rpn_DynamicString* operatorDynString = rpn_DynamicString_create();
  rpn_DynamicString* postfixDynString = rpn_DynamicString_create();
  
  char* postfixString = parse_infix_to_postfix(operatorDynString, postfixDynString, infixString);

  rpn_DynamicString_delete(postfixDynString);
  rpn_DynamicString_delete(operatorDynString);
  return postfixString;
}