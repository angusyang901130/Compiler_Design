# hw1 report

|Field|Value|
|-:|:-|
|Name|angusyang901130|
|ID|109550099|

## How much time did you spend on this project

I spend about 6 hours to finish this project

## Project overview

- For `Delimiters`, `Operators`, and `Reserved Words`, I use TOKEN_CHAR AND TOKEN_STRING to parse them out. The rules are set to be the same of those words. Take `return` and `+` for example, the rules look like:
    > "return"   { TOKEN_STRING(return, yytext); } <br>
    > "+"   { TOKEN_CHAR("+", yytext); }

- For `Oct_integer`, the regular expression describe a integer which can start with 0 and only contains 0 to 7

- For `Integer`, `Float`, `Scientific Notation`,  `Identifier`:
  - Define regular expression of `letter`, `digit` at first
  - Define `Integer` with digit
  - Define regular expression of `Identifier` with digit and letter
  - Define regular expression of `Float`, which includes an Integer, floating point, and a series of digits with no 0 at the end
  - Define regular expression of `Scientific Notation`, which includes a float or integer, one e or E, and a positive or negative integer
  
- For `String`, `Comment`, and `PsuedoComment`:
  - Use start condition to implement
  - `String`: Once read `"`, go to state `STRING`. When read the next `"` after a string is found, go back to `INITIAL`
  - `Comment`: 
    - `//`: Once read `//`, go to state `COMMENT1`. When read `\n`, go back to `INITIAL`, otherwise stay at current state
    - `/**/`: Once read `/*`, go to state `COMMENT0`. When read `*/`, go back to `INITIAL`, otherwise stay at current state
  - `PsuedoComment`: Once read `//&`, go to state `PCOMMENT`. When read `S+`, `S-`, `T+`, `T-`, adjust the value of opt_src and opt_tok, and then go to state `PCOMMENTGET`. Last, when read `\n`, go back to `INITIAL`, otherwise stay at current state

- If a input matches non of the rules above, detect it as bad character

## What is the hardest you think in this project

I think is implementing the string parsing because may have "" inside a string which indicates a single " in a string, increasing the complexity of dealing with string token.

## Feedback to T.A.s

I think the spec is quite detailed, I can clearly understand what I need to do. The diff.txt which compares the difference between the answer and my output is quite user-friendly to me, thanks for giving us a good tool to find out the bug. One thing I am concerned about is that when forming the result of parsing each file, I need to add `\r` before `\n` to output the right answer. I think it is because the OS system I am writing on is Windows and the execution is done in docker, which is Linux system. My answers are right after I add `\r` at printf functions, but I am not sure if this action leads to getting wrong answer in TA's enviornment. I have pushed a folder `109550099_test` which stores the result I output to avoid any misunderstanding about my code, thank you. 