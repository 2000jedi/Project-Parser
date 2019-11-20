# yparser
A parser that transfers a given input structure into a parse tree.

## Structure of the Parse Table

### Special Catagories
* `<EPS>` is the epsilon transition. Be sure nothing is after or before it.
* `<LITERAL_{X}>` is recognized as a string of terminals X. Their children will be removed due to performance.

## TO-DO
- [ ] Add description on how to build parse_table.
- [ ] Add rules for special literals internally.
- [ ] Add convenient methods to turn left factored tree into normal parse tree.
