# hw5 report

|||
|-:|:-|
|Name|楊竣喆|
|ID|109550099|

## How much time did you spend on this project

>  A week

## Project overview

> In General, each node has its basic assembly code. With some special cases, some nodes may add additional assembly codes or just different code from the general code of each node.
> 
> For local variables that is used in function or main function, the value will be push to a certain place of stack and this position won't be changed during the scope.
>
> For expressions, I push the value of the expression to stack after calculation, and its parent node can make use of the previously pushed value or expression result to finish the expression value calculation.
>
> For loops and condtions, I need to generate some new labels, and the labels will be stored in true / false stack, which indicates which label the code should jump to after knowing the result of condition.

## What is the hardest you think in this project

> The bonus part, condition and loop are the hardest. Bonus part contains array, which is quite different than basic cases. Float and string needs additional section to store value. For condition and loop, I have to maintain the sequence of labels, which takes a lot of time to implement the code flow.

## Feedback to T.A.s

> Thanks for the TAs providing code structure for us in each lab, makes me knowing what to do quicker. Actually, I have updated my knowledge of C++ in these homework since the way you write is quite new to me, a lot of cool functions and objects I have never used before (I use the code structure you provided until hw5, in hw4 I use my own structure in hw3).  
