Formula parser receives a set of primitive tokens (or lexer tokens) and converts them into formula tokens.

## Resolving names

Lexer's name tokens need to be resolved in order to classify them into

  * cell reference
  * cell range reference
  * named range
  * built-in function
  * invalid name

based on the value of the name itself.