RPN Converter
=============

A library to convert arithmetic expressions between infix and postfix notation.

Installation
------------

* Download the contents of this repository.
* Install [Check](https://libcheck.github.io/check/) if necessary.
* Type `make test` within the repository directory to build and run the tests. 

Derivation of the Infix to Postfix algorithm used
--------------------------------------------------

First, observe that a mathematically valid conversion of `a+b+c+d` is `abcd+++` (The requirements do not accept this, however. More on that later.) 
Similarly, `a+b*c^d` converts to `abcd^*+`. Thus, in the default case in which a prior operator does not take precedence over a later operator 
(that is, the expression is entirely left-associative), the conversion is simple: We output each operand in order, and then output each 
operator in reverse order. Our basic pattern will therefore be to proceed through the string, outputting each operand immediately, and putting 
each operator in a stack. Once the end of the string is reached, we output each operator in reverse order.

Now let's look at a case where a prior operator *does* take precedence over a later operator: `a*b+c`. This converts to `ab*c+`. The difference
here occurs once we reach the `+` in the infix string. Because the higher precedence of `*` demands that it occurs prior to the `+`, we need
to perform the `*` operation immediately, so that the section `a*b` will be consumed, to be later operated on by the `+` we've just encountered.
Thus, we insert the `*` at the top of our operator stack immediately, and then continue as normal: We put the `+` on top of the stack, and
continue. 

A more complex situation is: `a*b^c+d`. `*` does not precede `^`, so by the time we get to `+` we haven't had any interruptions in our default
pattern: That is, we've thus far outputted `abc` and our operator stack has `^` at the top, followed by `*`. However, now `+` is preceded by `^`,
so we have to pop out the `^`, effectively composing `(b^c)` into a single operand. The *new* prior operand is now `*`. Since this still takes priority
over `+`, we have to pop it as well, creating the composite operand `(a*(b^c))`. Finally, we reach `d`. Thus far, we've outputted `abc^*`, to 
which we add `d`, and finally the remaining contents of our operand stack, `+`, resulting in `abc^*d+`, the correct result.

All that remains is to handle parentheses. Originally, I handled these recursively, treating each parenthetical as an expression to be converted itself.
For this second implementation, I learned of a refinement to this, in which an open parenthesis is pushed to the operator stack, and when a closed parenthesis
is encountered, all operators are immediately popped off of the stack and outputted until the corresponding open parenthesis is reached (which is itself popped 
off the stack, though not outputted). This is simiar in concept (the parenthesis in the stack acts as a delimiter for what is effectively recursion depth),
but gets rid of some of the more clunkly aspects of keeping track of position in the infix string being parsed.

The final thing to note is that the requirements specify that equal operators should associate left rather than right. That is, `a+b+c+d` should
convert to `ab+c+d+`. This just means we need to break ties by assuming that the prior operator takes precedence over the current operator.

Thus, our algorithm is:

1. Set our current index to the start of the infix string
2. Read the character at the current index of the string
    * If the character is an operand, output it and increment the current index
    * If the character is an an operator...
        * If our operator stack is empty, or the new operator has higher precedence than the operator at the top of the stack, put the new operator on top of the stack
        * Otherwise, pop the operator off the top of the stack and output it. Repeat this step until the operator stack is empty or an operator of lower or equal precedence is reached.
    * If the character is an open parenthesis, push it to the stack
    * If the character is a closed parenthesis, pop operators off of the operator stack and output them in turn until an open parenthesis is reached. The open parenthesis should be popped off, but not outputted.
3. Increment the current index. If we've reached the end of the string, we are finished. Otherwise repeat step 2.

Derivation of the Postfix to Infix algorithm used:
--------------------------------------------------

Fortunately, Postfix notation maps quite easily to familiar data structures: Each operand is pushed onto a stack, and each operator operates on the top
two entries in the operand stack, creating a new parenthetical expression, itself a new operand, which is then pushed to the operand stack. Unlike
with Infix notation, there's never a need to "look ahead" in the expression.

Our algorithm is simply:

1. Set our current index to the start of the postfix string
2. If the current index contains an operand, push it to the operand stack
3. If the current index contains an operator `o`, pop `y` and then `x` off the operand stack, and then push `xoy` to the operand stack. Multi-character operands should be surrounded by parentheses, e.g.
If `x` is `a+b`, `y` is `c`, and `o` is `*`, this results in `(a+b)*c`
4. Increment the current index.
5. If we have reached the end of the postfix string, pop the operand off the top of the operand stack and output it. Otherwise, return to 2.

