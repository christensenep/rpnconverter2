#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "rpn.h"
#include "dynamic_string.h"
#include "string_stack.h"

#define RPN_OPERATORS "+-*/^"

typedef int RPN_PARSE_ERROR;

#ifndef RPN_INVALID_PARENTHETICAL
#define RPN_INVALID_PARENTHETICAL 1
#endif

#ifndef RPN_PARSE_SUCCESSFUL
#define RPN_PARSE_SUCCESSFUL 0
#endif 

static bool isOperator(char character) {
  if (character != '\0' && strchr(RPN_OPERATORS, character) != NULL) {
    return true;
  }
  else {
    return false;
  }
}

static bool isOperand(char character) {
  return islower(character);
}

static int getPriority(char operator) {
  const char* ptr = strchr(RPN_OPERATORS, operator);
  if (ptr == NULL || operator == '\0') {
    return -1;
  }
    
  return ptr - RPN_OPERATORS;
}

static void consumeInfixOperator(rpn_DynamicString* operatorDynString, rpn_DynamicString* postfixDynString, const char* currentInfixStringPos, bool* expectingOperandOrOpenParens) {
  while (getPriority(*currentInfixStringPos) <= getPriority(rpn_DynamicString_lastChar(operatorDynString))) {
    rpn_DynamicString_addChar(postfixDynString, rpn_DynamicString_popChar(operatorDynString));
  }
  rpn_DynamicString_addChar(operatorDynString, *currentInfixStringPos);
  *expectingOperandOrOpenParens = true;
}

static void consumeInfixOperand(rpn_DynamicString* postfixDynString, const char* currentInfixStringPos, bool* expectingOperandOrOpenParens) {
  rpn_DynamicString_addChar(postfixDynString, *currentInfixStringPos);
  *expectingOperandOrOpenParens =  false;
}

static void consumeInfixOpenParenthesis(rpn_DynamicString* operatorDynString, const char* currentInfixStringPos, bool* expectingOperandOrOpenParens) {
  rpn_DynamicString_addChar(operatorDynString, *currentInfixStringPos);
  *expectingOperandOrOpenParens = true;
}

static RPN_PARSE_ERROR consumeInfixCloseParenthesis(rpn_DynamicString* operatorDynString, rpn_DynamicString* postfixDynString, bool* expectingOperandOrOpenParens) {
  char poppedOperator = rpn_DynamicString_popChar(operatorDynString);
  while (poppedOperator != '(') {
    if (poppedOperator == '\0') {
      return RPN_INVALID_PARENTHETICAL;
    }

    rpn_DynamicString_addChar(postfixDynString, poppedOperator);
    poppedOperator = rpn_DynamicString_popChar(operatorDynString);
  }
  *expectingOperandOrOpenParens = false;
  return RPN_PARSE_SUCCESSFUL;
}

static RPN_PARSE_ERROR consumeInfixRemainingOperators(rpn_DynamicString* operatorDynString, rpn_DynamicString* postfixDynString) {
  while (rpn_DynamicString_length(operatorDynString) != 0) {
    char poppedOperator = rpn_DynamicString_popChar(operatorDynString);
    if (poppedOperator == '(') {
      return RPN_INVALID_PARENTHETICAL;
    }

    rpn_DynamicString_addChar(postfixDynString, poppedOperator);
  }
  return RPN_PARSE_SUCCESSFUL;
}

static bool isCharacterPositionValid(const char* currentInfixStringPos, bool expectingOperandOrOpenParens) {
  if ((isOperand(*currentInfixStringPos) || (*currentInfixStringPos == '(')) != expectingOperandOrOpenParens) {
    return false;
  }

  return true;
}

static char* parseInfixToPostfix(rpn_DynamicString* operatorDynString, rpn_DynamicString* postfixDynString, const char* infixString) {
  const char* currentInfixStringPos = infixString;
  bool expectingOperandOrOpenParens = true;

  while (*currentInfixStringPos != '\0') {
    if (!isCharacterPositionValid(currentInfixStringPos, expectingOperandOrOpenParens)) { return NULL; }

    if (isOperator(*currentInfixStringPos)) {
      consumeInfixOperator(operatorDynString, postfixDynString, currentInfixStringPos, &expectingOperandOrOpenParens);
    }
    else if (isOperand(*currentInfixStringPos)) {
      consumeInfixOperand(postfixDynString, currentInfixStringPos, &expectingOperandOrOpenParens);
    }
    else if (*currentInfixStringPos == '(') {
      consumeInfixOpenParenthesis(operatorDynString, currentInfixStringPos, &expectingOperandOrOpenParens);
    }
    else if (*currentInfixStringPos == ')') {
      if (consumeInfixCloseParenthesis(operatorDynString, postfixDynString, &expectingOperandOrOpenParens) == RPN_INVALID_PARENTHETICAL) { return NULL; }
    }
    else {
      return NULL;
    }

    currentInfixStringPos++;
  }

  if (expectingOperandOrOpenParens) { return NULL; }

  if (consumeInfixRemainingOperators(operatorDynString, postfixDynString) == RPN_INVALID_PARENTHETICAL) { return NULL; }

  return rpn_DynamicString_toString(postfixDynString);
}

static char* createInfixExpression(const char* firstOperand, const char* secondOperand, char operator) {
  bool leftNeedsParens = strlen(firstOperand) > 1;
  bool rightNeedsParens = strlen(secondOperand) > 1;

  int leftSize = leftNeedsParens ? strlen(firstOperand) + 2 : strlen(firstOperand);
  int rightSize = rightNeedsParens ? strlen(secondOperand) + 2 : strlen(secondOperand);

  char* newExpression = (char*) malloc(sizeof(char) * (leftSize + 1 + rightSize + 1));

  if (leftNeedsParens) {
    sprintf(newExpression, "(%s)%c", firstOperand, operator);
  }
  else {
    sprintf(newExpression, "%s%c", firstOperand, operator);
  }

  if (rightNeedsParens) {
    sprintf(newExpression+leftSize+1, "(%s)", secondOperand);
  }
  else {
    sprintf(newExpression+leftSize+1, "%s", secondOperand);
  }

  return newExpression;
}

static char* parsePostfixToInfix(rpn_StringStack* operandStack, const char* postfixString) {
  const char* currentPostfixStringPos = postfixString;

  while (*currentPostfixStringPos != '\0') {
    if (isOperand(*currentPostfixStringPos)) {
      char operandString[2] = { *currentPostfixStringPos, '\0' };
      rpn_StringStack_pushString(operandStack, operandString);
    }
    else if (isOperator(*currentPostfixStringPos)) {
      char* secondOperand = rpn_StringStack_popString(operandStack);
      char* firstOperand = rpn_StringStack_popString(operandStack);
      
      if (firstOperand == NULL) {
        free(firstOperand);
        free(secondOperand);
        return NULL;
      }

      char* resultingInfixExpression = createInfixExpression(firstOperand, secondOperand, *currentPostfixStringPos);
      rpn_StringStack_pushString(operandStack, resultingInfixExpression);
      free(resultingInfixExpression);
      free(firstOperand);
      free(secondOperand);
    }
    else {
      return NULL;
    }

    currentPostfixStringPos++;
  }

  char* infixString = rpn_StringStack_popString(operandStack);
  if (!rpn_StringStack_isEmpty(operandStack) || infixString == NULL) {
    free(infixString);
    return NULL;
  }

  return infixString;
}

char* rpn_infix_to_postfix(const char* infixString) {
  rpn_DynamicString* operatorDynString = rpn_DynamicString_create();
  rpn_DynamicString* postfixDynString = rpn_DynamicString_create();
  
  char* postfixString = parseInfixToPostfix(operatorDynString, postfixDynString, infixString);

  rpn_DynamicString_delete(postfixDynString);
  rpn_DynamicString_delete(operatorDynString);
  return postfixString;
}

char* rpn_postfix_to_infix(const char* postfixString) {
  rpn_StringStack* operandStack = rpn_StringStack_create();

  char* infixString = parsePostfixToInfix(operandStack, postfixString);

  rpn_StringStack_delete(operandStack);
  return infixString;
}
