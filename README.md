# PostfixProject
Data structures and algorithms term project

# Input and Output Type

Variables must be one character, every part of function must be divided by whitespace

a = 2 ;

b = 3 ;

c = a * ( b + 4 ) ;

b = c + b * 2 ;

d = a * ( b - c ) / 2 ;

#Example
	Reading Line is : d = a * ( b - c ) / 2 ;

        STEP 1 : CREATE POSTFIX

Postfix:                                          Stack: Empty

Postfix: a                                        Stack: Empty
Postfix: a                                        Stack: *
Postfix: a                                        Stack: * (
Postfix: a b                                      Stack: * (
Postfix: a b                                      Stack: * ( -
Postfix: a b c                                    Stack: * ( -
Postfix: a b c -                                  Stack: *
Postfix: a b c - *                                Stack: /
Postfix: a b c - * 2                              Stack: /
Postfix: a b c - * 2 /                            Stack: Empty

Postfix version of function: a b c - * 2 /

         Step 2: SOLVING POSTFIX

Stack: 3.00
Stack: 3.00 15.00
Stack: 3.00 15.00 11.00
Stack: 3.00 4.00
Stack: 12.00
Stack: 12.00 2
Stack: 6.00

Variables : a=3.00 b=15.00 c=11.00 d=6.00