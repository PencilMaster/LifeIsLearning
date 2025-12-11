Here we will write our first notes to the Python Tutorial at: "https://docs.python.org/3/tutorial"

# 1. Whetting Your Appetite

Python allows you to split your program into modules that can be reused in other Python programs. It comes with a large collection of standard modules that you can use as the basis of your programs.<br>
Python is extensible: if you know how to program in C it is easy to add a new built-in function or module to the interpreter, either to perform critical operations at maximum speed, or to link Python programs to libraries that may only be available in binary form (such as a vendor-specific graphics library). Once you are really hooked, you can link the Python interpreter into an application written in C and use it as an extension or command language for that application.<br>

# 2. Using the Python Interpreter

## 2.1. Invoking the Interpreter

Execute the interpreter by running `python3.14`.<br>
Exit it with `Ctrl+d` on UNIX or `quit()`.<br>

Interpreter's line editing features include:
- Interactive Editing
- History Substitution
- Code Completion<br>
This only on systems that support [GNU Readline](https://tiswww.case.edu/php/chet/readline/rltop.html) library.

Different versions of calling the Interpretor:<br>
1. Calling it with standard input, connected to a tty device, makes it read and execute commands interactively.
2. Calling it with a file name argument or with a file as std::in, makes it read and execute a script from that file.
3. `python -c command [arg] ...` executes the statement(s) in command, analogous to the shell's [-c](https://docs.python.org/3/using/cmdline.html#cmdoption-c) option. Because of python indentation and other specialness, best to quote the command in its entirety.
4. To invoke Python modules as scripts, use `python -m module [arg] ...`. This executes the source file for module as if you had spelled its full name on the command line.
5. To enter interactive mode after using a script file, use the flag [-i](https://docs.python.org/3/using/cmdline.html#cmdoption-i) before the script.

All command line options are described in [Command line and environment](https://docs.python.org/3/using/cmdline.html#using-on-general).<br>

### 2.1.1. Argument Passing

The `sys` module is used to interact with the system. One use of it is: If the interpreter is called with a script name and additional arguments afterwards, then the name is assigned to `sys.argv[0]` and the additional arguments are assigned to `sys.argv[1,...]`.<br>
When it is invoked with `-c`, then `sys.argv[0]` is `-c`. With `-m` it becomes the full name of the located module.<br>
Other options after `-c` command or `-m` module, are not consumed by interpreter's option processing but left in `sys.argv[1,...]`<br>

### 2.1.2. Interactive Mode

Interpreter is in `Interactive Mode` when it reads commands from a tty (texttypewriter).<br>
In this mode it prompts for the next command with the primary prompt `>>>`.<br>
For continuation lines it prompts with the `secondary prompt` `...`.<br>
It also prints a welcome message stating its version number and a copyright notice before printing the first prompt.<br>

`...` is used inside if statements for example:
```{source=python3.14, include=TRUE, echo=FALSE}
>>> theWorldIsFlat = True
>>> if theWorldIsFlat:
...    print("Be careful not to fall off!")
```

## 2.2. The Interpreter and Its Environment

### 2.2.1. Source Code Encoding

By default, source files are treated as if they are encoded in `UTF-8`.<br>
In `UTF-8` most language characters can be used simultaneously in string literals, identifiers and comments.<br>
Important: Any portable code should only use ASCII characters for identifiers. The standard library does the same.<br>

To declare an encoding different than the default, add a special line as the first line of the file.<br>
```{source=python3.14, include=TRUE, echo=FALSE}
# -*- coding: <encoding>  -*-

# ex.
# -*- coding: cp1252 -*-

# Exception to the rule is a [UNIX "shebang" line](https://docs.python.org/3/tutorial/appendix.html#tut-scripts)
# Here the encoding should be added as the second line
#!/usr/bin/env python3
# -*- coding: cp1252 -*-
```

Fun Fact: On Unix, the Python 3.x interpreter is by default not installed with the executable named python, so that it does not conflict with a simultaneously installed Python 2.x executable.

## 2.3. Links
[GNU Readline](https://tiswww.case.edu/php/chet/readline/rltop.html)<br>
[-c](https://docs.python.org/3/using/cmdline.html#cmdoption-c)<br>
[-i](https://docs.python.org/3/using/cmdline.html#cmdoption-i)<br>
[Command line and environment](https://docs.python.org/3/using/cmdline.html#using-on-general)<br>
[UNIX "shebang" line](https://docs.python.org/3/tutorial/appendix.html#tut-scripts)


# 3. An Informal Introduction to Python

Comments in Python start with the hash character \#, and extend to the end of the line.<br>
Write a comment at the start of a line, following whitespace or code, but not within a string literal.<br>
Within a string literal, the hash character is just that, a hash character.<br>

Example:<br>
```{source=python3.14, include=TRUE, echo=FALSE}
# This is the first comment
spam = 1 # and this is the second comment
         # ... and now a third
text = "# This is not a comment because it's inside quotes."
```

## 3.1. Using Python as a Calculator

### 3.1.1. Numbers

Expression syntax is straight forward. `+` `-` `*` `/` are used to perform arithmetic; parantheses `()` can be used for grouping.
- `int` is integer numbers (e.g. natural numbers 0, 2, 4, 20). Do not worry about unsigned or signed. This is python and you have(need) no control over it.
- `float` is numbers with a fractional part (e.g. 5.0, 1.6).
- `\` always returns a float.
- `\\` does a floor division.
- `%` calculates the remainder.
- `**` calculates powers. (base ** power). IMPORTANT: Precedence of `**` is higher than `-`.
- `=` assigns a RHS value to a LHS variable. Afterwards no result is displayed before the next interactive prompt.
- `_` in interactive mode, the last printed expression is assigned to this variable. Easy to use for calculations. Never assign this variable in interactive mode, it will mask the built-in variable.
- Operators with mixed type operands, convert the integer operand to floating point -> Full support for floating point.<br>

Python also supports other number types, such as [Decimal](https://docs.python.org/3/library/decimal.html#decimal.Decimal) and [Fraction](https://docs.python.org/3/library/fractions.html#fractions.Fraction) and built-in support for [Complex Numbers](https://docs.python.org/3/library/stdtypes.html#typesnumeric), using j or J suffix (e.g. 3+5j).

### 3.1.2. Text

Text can be characters, (data type [str](https://docs.python.org/3/library/stdtypes.html#str) or string) and numbers, can be enclosed in single or double quotes `'...'` or `"..."` with the same result.<br>
Watch out, special characters exist when using `\` followed by other characters.<br>
For Example:
- To quote a quote, we need to escape it `'doesn\'t'` will be shown correctly. Trick: inside `"` we don't need to escape `'` and vice-versa.
- In Python shell, without using `print()`, it shows the raw string without transforming special characters.
- In Python shell, using `print()`, special characters are transformed. `\n` for example will create a new line in the output.
- Raw strings can be used in `print()` by prefacing the string with an `r`. `print(r'C:\some\name')` will not activate `\n`.
- Rule: Raw strings are not allowed to end in an odd number of `\` characters -> [FAQ entry](https://docs.python.org/3/faq/programming.html#faq-programming-raw-string-backslash).
- Create string literals that span multiple lines, using triple-quotes `"""..."""` or `'''...'''`. EOL characters `\n` are automatically included in the string. To prevent the automatic addition of an `\n`, write `\` at the end of a line inside the string.<br>

Concatenation of strings:
- They can be glued together using the `+` operator.
- They can be repeater with the `*` operator.
- Two or more string literals next to each other are automatically concatenated.
- Automatic concatenation only works with literals and not with variables or expressions. When mixing types, use `+`.<br>

Indexing (used to obtain individual characters) and Slicing (used to obtain substrings):
- Indexing (subscripting) of strings works with \<variable\_name\>[\<index\>]. A character is simply a string of size one in Python.
- Indexing also works with negative numbers, where `-1` denotes the position of the last character in the string.
- Syntax: \<variable\>[\<inclusive\_start\>:\<exlusive\_end\>].
- Default value of slice indices it either zero for the first index, or the size of the string being sliced for the second index.
- Syntax: \<variable\>[:\<exlusive\_end\>]. This also works with negative indices, ex. `string[-2:]` contains the last two characters.
- Error: When using an Index that is too large. len(word) == 5 -> word[42] throws an error.
- No Error: Out of range slice indexes always work. word[4:42], word[42:] == ''.
- Error: Trying to mutate a string / overwrite parts of it. Always throws an error. Strings are [immutable](https://docs.python.org/3/glossary.html#term-immutable).
- When you need a different string, create a new one and use slicing to your advantage. word[:2] + 'J' + word[3:].
- Built in function `len()` returns the length of a string.<br>

Useful Links:
- [Text Sequence Type](https://docs.python.org/3/library/stdtypes.html#textseq)
- [String Methods](https://docs.python.org/3/library/stdtypes.html#string-methods)
- [f-strings](https://docs.python.org/3/reference/lexical_analysis.html#f-strings)
- [Format String Syntax](https://docs.python.org/3/library/string.html#formatstrings)
- [printf-style String Formatting](https://docs.python.org/3/library/stdtypes.html#old-string-formatting)<br>

### 3.1.3. Lists

There are many compound data types in python. They are used to group together other values. The list can be written as a list of comma-seperated values (items) between square brackets. They may contain different types, but usually the items all have the same type. And they can be indexed and sliced just like strings (and all other built-in [sequence](https://docs.python.org/3/glossary.html#term-sequence) types).<br>
Example: `squares = [1, 4, 9, 16, 25]`<br>
- Lists also support concatenation with the `+` operator.
- Lists are [mutable](https://docs.python.org/3/glossary.html#term-mutable) unlike strings.
- Add new items at the end of a list using the method `list.append()`.

Important difference to other languages:
Assignment in Python using the `=` operator always assigns by reference. All changes to one reference will also be visible from the other variables that refer to it.<br>
This is easily visible calling `id()` function on list variables and comparing them.<br>

Shallow copies:
- All slice operations return a new list containing the requested elements.
- Trick: To copy assign, do `copy_letters = letters[:]` and any changes to copy\_letters will be local to that variable.
- Allowed: Assigning to a slice, use the following syntax:
- `Syntax[2:4] = ['C', 'D', 'E']` or even `letters[2:4] = []`.
- `Syntax[2] = 'C', 'D', 'E'`. In the output, this section will be saved in entry `2` as a tuple ('C', 'D', 'E'). Tuple are immutable.
- Built in function `len()` also works on lists.
- We can nest lists: `a = [1, 2]`, `n = ['a', 'b']`, `x = [a, n]`.<br>

## 3.2. First Steps Towards Programming

```{source=python3.14, include=TRUE, echo=FALSE}
# Fibonacci series:
a, b = 0, 1
while a<10:
    print(a)
    a, b = b, a+b
```

**Key points here:**

1.  `Multiple assignment` in first line - variables a und b get values 0 and 1 assigned.<br>
    This shows, that the RHS expressions are evaluated first, before any assignments take place.<br>
    The RHS expressions are assigned from the left to right.<br>
2.  [while](https://docs.python.org/3/reference/compound_stmts.html#while) loop executes as long as condition remains true.<br>
    Any non-zero integer value is true; zero is false. The condition may be any sequence; anything with a non-zero length is true, empty is false.<br>
    Standard comparison operators are the same as in C. <br>
3.  Indentation groups statements in Python.
4.  [print()](https://docs.python.org/3/library/functions.html#print) function writes values of the argument(s) given.<br>
    String are printed without quotes, and a space is inserted between items. `print('Hello', i)` -> `Hello World!` when i='World!'.<br>

# 4. More Control Flow Tools


