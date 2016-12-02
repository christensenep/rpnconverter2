#include <string.h>
#include <stdlib.h>

#include "rpn.h"
#include "dynamic_string.h"

char* rpn_infix_to_postfix(const char* infixString) {
  rpn_DynamicString* operatorDynString = rpn_DynamicString_create();
  rpn_DynamicString* postfixDynString = rpn_DynamicString_create();

  const char* currentInfixStringPos = infixString;;
  
  while (*currentInfixStringPos != '\0') {
    if (*currentInfixStringPos == '+') {
      if (operatorDynString->currentLength > 0) {
        rpn_DynamicString_addChar(postfixDynString, rpn_DynamicString_popChar(operatorDynString));
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
  return postfixString;
}
