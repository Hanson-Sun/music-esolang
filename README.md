# music-esolang
esolang controlled by MIDI inputs

## TODO
- [ ] BRO LITERALLY EVERYTHING


## Language Deisgn Ideas

General idea: MIDI input will be converted to a "minimal" stack based language. The language will be executed and the output will be converted back to MIDI or printed to the console.

### Language Specification
It will be a minimal stack based language (similar to Forth and uiua)

Here are some core operations the language will support. Note: binary operations pop two items off the stack and push one item back on the stack. Unary operations pop one item off the stack and push one item back on the stack. We also only support integer operations. Execution will be done in order from left to right, top to bottom, line by line.

Arithmetic operations (binary):
- `+`: pops two items off the stack and pushes the sum back on the stack
- `-`: pops two items off the stack and pushes the difference back on the stack
- `*`: pops two items off the stack and pushes the product back on the stack
- `/`: pops two items off the stack and pushes the quotient back on the stack
- `%`: pops two items off the stack and pushes the remainder back on the stack

Logical operations (binary):
- `=`: pops two items off the stack and pushes 1 if they are equal, 0 otherwise
- `<`: pops two items off the stack and pushes 1 if the first is less than the second, 0 otherwise
- `>`: pops two items off the stack and pushes 1 if the first is greater than the second, 0 otherwise
- `<=`: pops two items off the stack and pushes 1 if the first is less than or equal to the second, 0 otherwise
- `>=`: pops two items off the stack and pushes 1 if the first is greater than or equal to the second, 0 otherwise
- `!=`: pops two items off the stack and pushes 1 if the first is not equal to the second, 0 otherwise

Logical operations (unary):
- `&`: pops two items off the stack and pushes result after bitwise and
- `|`: pops two items off the stack and pushes result after bitwise or
- `~`: pops one item off the stack and pushes result after bitwise not

Stack manipulation:
- `push` (maybe this should be built in to not require an explicit operation):
- `pop`: pops the top of the stack
- `dup` (.): duplicates the top of the stack
- `dup.` (,): duplicates the second item on the stack
- `swap`: swaps the top two items on the stack
- `clear`: clears the stack

I/O operations:
- `input`: reads a value from the console and pushes it onto the stack
- `print`: pops and prints the top of the stack to the console
- `print.`: prints the entire stack to the console
- `show`: prints the entire stack as string to the console

Definitions (basic functions, similar to Forth):
- `def <name> <...> end`

Conditionals and branching (must be inside a definition, provides "scoping"):
- `if <...> else <...> end`: pops the top of the stack, if it is not 0, executes the first block, otherwise executes the second block
- `do <i keyword> end`: first and second items on the stack are the start and end of the loop. Pops both items and executes the block between the `do` and `loop` commands
- `while <...> end`: pops the top of the stack is not 0, if it is, exits the loop. Otherwise, executes the block between the `while` and `loop` commands

while/do commands might be redundant, so we can consider removing the do command

Variables (stolen from Forth):
- `variable <name>`: creates an address in memory for a variable
- `!`: top of the stack is the value, second item is the address. Stores the value at the address
- `@`: gets the value at the address on the top of the stack and pushes it onto the the stack

Misc:
- `#`: anything after a `#` is a comment and is ignored


### Tokenization and Token Mapping

Ok figure out how to convert MIDI to those commands.


Literals will be played by single notes. Every note in an octave will form the digits of a base 12 number system. Octaves are ignored.

We will use intervals/chords to represent keywords and operations. Here is the mapping:

### Formal Grammar

```ebnf
todo
```



