# Project Parser
A parser that transfers a given input structure into a parse tree.

## Structure of the Parse Table

The basic structure of the parse table would be:

> \<Category\>=Literal1\<Category2\>Literal2...
  
Separated by lines, each line represents a parse rule.

### Special Catagories
* `<EPS>` is the epsilon transition. Be sure nothing is after or before it.
* `<LITERAL_{X}>` is recognized as a string of terminals X. Their children will be removed due to performance.

## TO-DO
- [ ] Add description on how to build parse_table.
- [ ] Add rules for special literals internally.
- [ ] Add convenient methods to convert left factored tree.
