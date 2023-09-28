# hw3 report

|||
|-:|:-|
|Name|yourname|
|楊竣喆|109550099|

## How much time did you spend on this project

> About 4 to 5 days

## Project overview

> I construct my code from top to bottom. Starting with `ProgramNode` (`root`), do dfs to write the rules of each child nodes, so that in the end I can write rule of current node with the returned objects from child nodes.

> To deal with nodes with `list` in its name, I use linked list to store multiple nodes, and then store the nodes into a vector during constructor of parent node. For example, declaration may need multiple variables (IdList), so I use linked list to store VariableNodes. 

> For each node, I add a `next_node` member, and create two virtual function: `getNextNode` and `setNextNode` to access private member `next_node`.

> I print the result by the visitor pattern, start from the top of the node, do dfs and print the information of each node. During traversal, maintain the indentation to match the requirement of output visit pattern. 

> In `visitChildNode` function, in order to print child node information, I implement `accept` function for each node to accept `visitor` object and print its information. Both `visitChildNode` and `accept` are virtual function so that it will automatically look at what object type the node is, and then call its own `visitChildNode` and `accept`.

## What is the hardest you think in this project

> The last time I use c++ to write a OOP program is about two years ago, so I have to review the pattern of inheritence. Also, AST is kind of complex, I spent some time understanding how it works.

## Feedback to T.A.s

> The spec and guideline are helpful, they help me understand how should I implement my functions.
