#include <string.h>
#include <stdlib.h>

#include "rpn.h"
#include "dynamic_string.h"

char* rpn_infix_to_postfix(const char* infixString) {
  char operators[50];
  int charactersWritten = 0;
  int operatorsStored = 0;
  rpn_DynamicString* postfixDynString = rpn_DynamicString_create();

  while (*infixString != '\0') {
    if (*infixString == '+') {
      if (operatorsStored > 0) {
        operatorsStored--;
        rpn_DynamicString_addChar(postfixDynString, operators[operatorsStored]);
        charactersWritten++;
      }

      operators[operatorsStored] = *infixString;
      operatorsStored++;
    }
    else {
      rpn_DynamicString_addChar(postfixDynString, *infixString);
      charactersWritten++;
    }

    infixString++;
  }

  while (operatorsStored > 0) {
    rpn_DynamicString_addChar(postfixDynString, operators[operatorsStored - 1]);
    charactersWritten++;
    operatorsStored--;
  }

  char* postfixString = rpn_DynamicString_toString(postfixDynString);
  rpn_DynamicString_delete(postfixDynString);
  return postfixString;
}
