# hw2 report

|Field|Value|
|-:|:-|
|Name|angusyang901130|
|ID|109550099|

## How much time did you spend on this project

> I spent about 1 day to finish this project.

## Project overview

> For my scanner, I add a "`return` <`token or character`>" at the end of each token rule. This return the token back to parser to find the rule to fit.

> For my parser, I have refered to the given README file of project to construct my rules. For example, the general form of grammer `zero or more`, `function definition`, `compound statement`, etc.
 
> I construct my parser structure from big non-terminal rules (ex. `Program`, `Statement`) to small non-terminal rules (ex. `expr`, `Type`).
 
> The rule of a bigger non-terminal may be much more general, for example, constant declaration is `KWvar ID ASSIGN Value;`, which is the exactly what we would write during coding. 

> I would then write the rules of the non-terminals such as `Value`, which includes number or string or bool, this small non-terminal rule is much more specific. 

> In this way, the code can be more readable and can be debugged more easily. 

## What is the hardest you think in this project

> During Debugging, I would try some parts of my code with some rules not written write yet , but this may cause the whole parser to fail parsing since it meets some wrong rules. I have a hard time finding the bug in my code.

## Feedback to T.A.s

> I think the explanation is quite obvious and specific in README file, which helps me a lot during constructing the rules, thanks for providing a good spec :)
