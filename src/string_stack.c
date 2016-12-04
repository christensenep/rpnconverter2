#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "string_stack.h"

typedef struct _Node {
  char* string;
  struct _Node* next;
} Node;

struct _rpn_StringStack {
  Node* first;
};

rpn_StringStack* rpn_StringStack_create() {
  rpn_StringStack* stringStack = (rpn_StringStack*) malloc(sizeof(rpn_StringStack));
  stringStack->first = NULL;
  return stringStack;
}

void deleteNode(Node* node) {
  if (node == NULL) {
    return;
  }

  deleteNode(node->next);
  free(node->string);
  free(node);
}

void rpn_StringStack_delete(rpn_StringStack* stringStack) {
  deleteNode(stringStack->first);
}

void rpn_StringStack_pushString(rpn_StringStack* stringStack, const char* string) {
  Node* newNode = (Node*) malloc(sizeof(Node));
  newNode->next = stringStack->first;
  newNode->string = (char*) malloc(sizeof(char) * strlen(string));
  strcpy(newNode->string, string);
  stringStack->first = newNode;
}

char* rpn_StringStack_popString(rpn_StringStack* stringStack) {
  Node* poppedNode = stringStack->first;
  if (poppedNode == NULL) {
    return NULL;
  }

  char* poppedString = (char*) malloc(sizeof(char) * strlen(poppedNode->string));
  strcpy(poppedString, poppedNode->string);

  Node* newFirstNode = poppedNode->next;
  free(poppedNode->string);
  free(poppedNode);
  stringStack->first = newFirstNode;
  return poppedString;
}

bool rpn_StringStack_isEmpty(rpn_StringStack* stringStack) {
  return stringStack->first == NULL;
}
