#Christopher Carlisle
#Python project
#Survey of Programming Languages
#11/03/10

import sys
from hw import Stack, Queue

#Returns the priority of the operator
def getPriority(operator):
    if operator in "*/":
        return 2
    elif operator in "+-":
        return 1
    else:
        return 0

stack = Stack() #general stack used to convert infix to prefix
output = Stack() #stack used to construct the output string

print "\n======================================"
print "====                              ===="
print "====   Infix to Prefix Converter  ===="
print "====                              ===="
print "======================================\n"

print "Please put your infix equation in: ",
x = sys.stdin.readline().strip()

#1) Reverse the input string.
x = x[::-1]

#2) Examine the next element in the input.
for c in x:

    #3) If it is operand, add it to output string.
    if c not in "+-*/()":
        output.push(c)

    #4) If it is Closing parenthesis, push it on stack.
    elif c == ")":
        stack.push(c)

    #5) If it is an operator, then
    elif c in "+-*/":
        while True:

            #i) If stack is empty, push operator on stack.
            if stack.isEmpty():
                stack.push(c)
                break

            #ii) If the top of stack is closing parenthesis, push operator on stack.
            elif stack.top() == ")":
                stack.push(c)
                break

            #iii) If it has same or higher priority than the top
            #     of stack, push operator on stack.
            elif getPriority(c) >= getPriority(stack.top()):
                stack.push(c)
                break

            #iv) Else pop the operator from the stack
            #    and add it to output string, repeat step 5.
            else:
                output.push(stack.pop())

    #6) If it is a opening parenthesis, pop operators from stack &
    #   add to output string until a closing parenthesis is
    #   encountered. Pop & discard the closing parenthesis.
    elif c == "(":
        while True:
            if stack.top() == ")":
                stack.pop()
                break
            else:
                output.push(stack.pop())
    else:
        print "Something went wrong people, maybe some bad input."

#7) If there is more input go to step 2

#8) If there is no more input, unstack the remaining operators and add them to output string.
while not stack.isEmpty():
    output.push(stack.pop())

#print the string
while not output.isEmpty():
    print output.pop(),
