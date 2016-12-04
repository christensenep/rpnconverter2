#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "rpn.h"
#include "dynamic_string.h"

#define RPN_OPERATORS "+-*/^"

bool isOperator(char character) {
  if (character != '\0' && strchr(RPN_OPERATORS, character) != NULL) {
    return true;
  }
  else {
    return false;
  }
}

bool isOperand(char character) {
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
  int expectingOperandOrOpenParens = true;

  while (*currentInfixStringPos != '\0') {
    if ((isOperand(*currentInfixStringPos) || (*currentInfixStringPos == '(')) != expectingOperandOrOpenParens) {
      return NULL;
    }

    if (isOperator(*currentInfixStringPos)) {
      while (getPriority(*currentInfixStringPos) <= getPriority(rpn_DynamicString_lastChar(operatorDynString))) {
        rpn_DynamicString_addChar(postfixDynString, rpn_DynamicString_popChar(operatorDynString));
      }

      rpn_DynamicString_addChar(operatorDynString, *currentInfixStringPos);
      expectingOperandOrOpenParens = true;
    }
    else if (isOperand(*currentInfixStringPos)) {
      rpn_DynamicString_addChar(postfixDynString, *currentInfixStringPos);
      expectingOperandOrOpenParens = false;
    }
    else if (*currentInfixStringPos == '(') {
      rpn_DynamicString_addChar(operatorDynString, *currentInfixStringPos);
      expectingOperandOrOpenParens = true;
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
      expectingOperandOrOpenParens = false;
    }
    else {
      return NULL;
    }

    currentInfixStringPos++;
  }

  if (expectingOperandOrOpenParens) {
    return NULL;
  }

  while (operatorDynString->currentLength != 0) {
    char poppedOperator = rpn_DynamicString_popChar(operatorDynString);
    if (poppedOperator == '(') {
      return NULL;
    }

    rpn_DynamicString_addChar(postfixDynString, poppedOperator);
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
