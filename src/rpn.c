#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "rpn.h"
#include "dynamic_string.h"
#include "string_stack.h"

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

char* createInfixExpression(const char* firstOperand, const char* secondOperand, char operator) {
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

char* rpn_postfix_to_infix(const char* postfixString) {
  rpn_StringStack* operandStack = rpn_StringStack_create();

  const char* currentPostfixStringPos = postfixString;

  while (*currentPostfixStringPos != '\0') {
    if (isOperand(*currentPostfixStringPos)) {
      char operandString[2] = { *currentPostfixStringPos, '\0' };
      rpn_StringStack_pushString(operandStack, operandString);
    }
    else if (isOperator(*currentPostfixStringPos)) {
      char* secondOperand = rpn_StringStack_popString(operandStack);
      char* firstOperand = rpn_StringStack_popString(operandStack);
      char* resultingInfixExpression = createInfixExpression(firstOperand, secondOperand, *currentPostfixStringPos);
      rpn_StringStack_pushString(operandStack, resultingInfixExpression);
      free(resultingInfixExpression);
      free(firstOperand);
      free(secondOperand);
    }

    currentPostfixStringPos++;
  }

  char* infixString = rpn_StringStack_popString(operandStack);
  if (infixString == NULL || !rpn_StringStack_isEmpty(operandStack)) {
    return NULL;
  }

  rpn_StringStack_delete(operandStack);
  return infixString;
}
