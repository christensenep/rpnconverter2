#ifndef RPN_DYNAMIC_STRING
#define RPN_DYNAMIC_STRING

typedef struct _rpn_DynamicString rpn_DynamicString;

rpn_DynamicString* rpn_DynamicString_create(void);
void rpn_DynamicString_delete(rpn_DynamicString* dynamicString);
void rpn_DynamicString_addChar(rpn_DynamicString* dynamicString, char newChar);
char rpn_DynamicString_popChar(rpn_DynamicString* dynamicString);
char rpn_DynamicString_lastChar(rpn_DynamicString* dynamicString);
int rpn_DynamicString_length(rpn_DynamicString* dynamicString);
char* rpn_DynamicString_toString(const rpn_DynamicString* dynamicString);

#endif /* RPN_DYNAMIC_STRING */
