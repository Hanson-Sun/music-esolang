#  Polyphony
esolang controlled by MIDI inputs

## TODO
- [ ] BRO LITERALLY EVERYTHING
- [ ] Grammar
- [ ] Tokenization
- [ ] MIDI parsing
- [ ] WRITING THE MF INTERPRETER!!


## Language Design Ideas

General idea: MIDI input will be converted to a "minimal" stack based language. The language will be executed and the output will be converted back to MIDI or printed to the console.

### Tokenization & Token Mapping
In general, there are certain chords reserved as keywords. For our purposes, a chord is defined as 2 or more notes played simultaneously. Outside of this, single notes are considered a literal and will be pushed to the stack. Chords that aren't recognized as a keyword will be read as a single note (the highest note of the chord). 

Speaking of literals, Polyphony operates on a base 12 number system. Each semitone represents a digit, with C being 0 and B being 11. By the way, the octave doesn't matter so both C#4 and C#5 represent 1. 

As opposed to being identified by the specific notes in a chord, chords are identified by the intervals between its notes. (More specifically, the amount of semitones between any 2 consective notes of a chord) For example, all octaves can be used to represent the end of a statement (equivalent to semi-colon in C++). From now on, we will represent a chord by its intervals. ex: an octave is written as (8) and the chord CC#E is written as (2,4) 

Some definitions listed below: 

- (5,4): wrappers to indicate the start/end of a function/the entire program
- (8): end of statement
- (3): comment wrappers
- 


### Language Specification
It will be a minimal stack based language (similar to Forth and uiua)

Here are some core operations the language will support. Note: binary operations pop two items off the stack and push one item back on the stack. Unary operations pop one item off the stack and push one item back on the stack. We also only support integer operations. Execution will be done in order from left to right, top to bottom, line by line.

**Arithmetic operations (binary)**:
- `+`: pops two items off the stack and pushes the sum back on the stack
- `-`: pops two items off the stack and pushes the difference back on the stack
- `*`: pops two items off the stack and pushes the product back on the stack
- `/`: pops two items off the stack and pushes the quotient back on the stack
- `%`: pops two items off the stack and pushes the remainder back on the stack

**Logical operations (binary)**:
- `=`: pops two items off the stack and pushes 1 if they are equal, 0 otherwise
- `<`: pops two items off the stack and pushes 1 if the first is less than the second, 0 otherwise
- `>`: pops two items off the stack and pushes 1 if the first is greater than the second, 0 otherwise

There are no boolean operations. Instead, we will use the following convention: 0 is false, 1 is true. Note that truthy and falsy values will not work since we only have bitwise logic operations.

**Logical operations**:
- `&`: pops *two* items off the stack and pushes result after bitwise and
- `|`: pops *two* items off the stack and pushes result after bitwise or
- `~`: pops *one* item off the stack and pushes result after bitwise not

**Stack manipulation**:
- `pop`: pops the top of the stack
- `dup`: duplicates the top of the stack
- `dup.`: duplicates the second item on the stack
- `swap`: swaps the top two items on the stack
- `size`: returns the size of the stack and pushes it onto the stack

a standalone literal will be pushed onto the stack by default

**I/O operations**:
- `input`: reads a value from the console and pushes it onto the stack
- `print`: pops and prints the top of the stack to the console
- `print-`: pops and prints the top of the stack as a character to the console
- `debug`: prints the entire stack to the console

**Definitions** (basic functions, similar to Forth):
- `def <name> <...> end`

No explicit return or parameter passing. Instead, the stack is used to pass parameters and return values.

**Conditionals and Branching**:
- `if <...> else <...> end`: pops the top of the stack, if it is 0, executes the second block, otherwise executes the first block
- `while <...> end`: pops the top of the stack, if it is 0, exit the loop. Otherwise, executes the block between the `while` and `end` commands


**Variables** (stolen from Forth):
- `var <name>`: creates an address in memory for a variable
- `!`: top of the stack is the value, second item is the address. Stores the value at the address
- `@`: gets the value at the address on the top of the stack and pushes it onto the the stack
- `^`: frees the memory at the address on the top of the stack

**Misc**:
- `# <...> #`: anything between `#` is a comment and is ignored


### Formal Grammar

```ebnf
<program>        ::= { <statement> }

<statement>      ::= <literal>
                   | <arithmetic-op>
                   | <logical-op>
                   | <stack-op>
                   | <io-op>
                   | <control-flow>
                   | <variable-op>
                   | <definition>
                   | <comment>

<literal>        ::= <number>
<number>         ::= "(" <pitch> { <pitch> } ")"
<pitch>          ::= "C" | "C#" | "D" | "D#" | "E" | "F" | "F#" | "G" | "G#" | "A" | "A#" | "B"

<arithmetic-op>  ::= "+" | "-" | "*" | "/" | "%"

<logical-op>     ::= "=" | "<" | ">" | "&" | "|" | "~"

<stack-op>       ::= "pop" | "dup" | "dup." | "swap" | "size"

<io-op>          ::= "input" | "print" | "print-" | "debug"

<control-flow>   ::= <if-else> | <while>
<if-else>        ::= "if" <block> [ "else" <block> ] "end"
<while>          ::= "while" <block> "end"
<block>          ::= { <statement> }

<variable-op>    ::= "var" <identifier>
                   | "!"       (* Store: top of stack is value, second is address *)
                   | "@"       (* Load: push value at address to the stack *)
                   | "^"       (* Free: free memory at the address on top of the stack *)
<identifier>     ::= <pitch> { <pitch> }

<definition>     ::= "def" <identifier> <block> "end"

<comment>        ::= "#" { <note> } "#"
```

Example program:
```
# This program demonstrates various features of the Polyphony #

# Variable declaration: Create a variable "A" #
var A  

# Define a function that adds two numbers and prints the result #
def C4
    input   # Take input (a number literal) #
    3 +     # Add 3 to the input #
    print   # Print the result (top of stack) #
end

# Another function that prints a message based on input comparison #
def D4
    input   # Take input (a number literal) #
    5 <     # Compare if input is less than 5 #
    if
        1   # Push number literal "1" (representing "Less than 5") #
        print
    else
        2   # Push number literal "2" (representing "Greater or equal to 5") #
        print
    end
end

# Main program flow #
( C )          # Push pitch C (representing number 1 in this case) onto the stack #
( E G )        # Push pitches E and G (representing a number literal) #
+              # Add them together (E + G) #
( F )          # Push F (representing another number literal) onto the stack #
*              # Multiply the result by F #

# Call the function C4 to add 3 to the result and print #
C4             # Function C4 gets called, prints input + 3 #

# Now call D4 with the result of the previous operation #
D4             # Function D4 compares the input and prints accordingly #

# Manipulating variables: Storing the result of the multiplication in "A" #
!              # Store top of stack into "A" #
@              # Load value of "A" back onto stack #
print          # Print value of "A" #

# A loop that will repeat 3 times, decrementing the counter each time #
var counter
3 !            # Store 3 in counter #
while
    counter @  # Get counter value #
    0 =        # Check if counter is 0 #
    if
        pop    # Exit the loop if counter is 0 #
    else
        counter @  # Get counter value again #
        1 -        # Subtract 1 from counter #
        !          # Store the updated value of counter #
        counter @  # Push counter value to stack #
        print      # Print counter value #
    end
end

counter ^      # Free the memory used by counter #

# End of program #
```



