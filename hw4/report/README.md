# hw4 report

|||
|-:|:-|
|Name|楊竣喆|
|ID|109550099|

## How much time did you spend on this project

> 3 days

## Project overview

> I use my own parser from hw3, and I construct my semantic analyzer base on the AST Dumper. 
> 
> I construct two new classes, one is for symbol manager, and one is for error message manager. Symbol Manager deals with the checking for each error types, and maintains the symbol table. Error Message Manager deals with the error messages and maintains the error messages storage. 
> 
> The semantic analyzer will traverse each node, symbol manager will check the semantic during traversing, and if there is error, error message manager will store the error message. During traversing, symbol manager will dump symbols when poping a scope from symbol table stack. All error messages will be dumped after traversing all AST. 

## What is the hardest you think in this project

> Dealing with some errors during traversing is quite hard because I have to think about whether multiple errors in same node should be stored. 

## Feedback to T.A.s

> Hope next homework can be easier, it's a time-consuming homework XD. ( But I learned a lot from the homework.
