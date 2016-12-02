#include <string.h>
#include <stdlib.h>

#include "rpn.h"

char* rpn_infix_to_postfix(const char* infixString) {
  char operators[5];
  char* postfixString = (char*) calloc(6, sizeof(char));
  int charactersWritten = 0;
  int operatorsStored = 0;

  while (*infixString != '\0') {
    if (*infixString == '+') {
      if (operatorsStored > 0) {
        operatorsStored--;
        postfixString[charactersWritten] = operators[operatorsStored];
        charactersWritten++;
      }

      operators[operatorsStored] = *infixString;
      operatorsStored++;
    }
    else {
      postfixString[charactersWritten] = *infixString;
      charactersWritten++;
    }

    infixString++;
  }

  while (operatorsStored > 0) {
    postfixString[charactersWritten] = operators[operatorsStored - 1];
    charactersWritten++;
    operatorsStored--;
  }

  return postfixString;
}
