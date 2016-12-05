#ifndef RPN_STRING_STACK
#define RPN_STRING_STACK

#include <stdbool.h>

typedef struct _rpn_StringStack rpn_StringStack;

rpn_StringStack* rpn_StringStack_create(void);
void rpn_StringStack_delete(rpn_StringStack* stringStack);
void rpn_StringStack_pushString(rpn_StringStack* stringStack, const char* string);
char* rpn_StringStack_popString(rpn_StringStack* stringStack);
bool rpn_StringStack_isEmpty(rpn_StringStack* stringStack);

#endif /* RPN_STRING_STACK */
