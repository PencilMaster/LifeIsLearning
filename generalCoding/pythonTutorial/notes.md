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
- `**` calculates powers. (base ** power).
- `=` assigns a RHS value to a LHS variable. Afterwards no result is displayed before the next interactive prompt.
- `_` in interactive mode, the last printed expression is assigned to this variable. Easy to use for calculations. Never assign this variable in interactive mode, it will mask the built-in variable.
- Operators with mixed type operands, convert the integer operand to floating point -> Full support for floating point.<br>

Python also supports other number types, such as [Decimal](https://docs.python.org/3/library/decimal.html#decimal.Decimal) and [Fraction](https://docs.python.org/3/library/fractions.html#fractions.Fraction) and built-in support for [Complex Numbers](https://docs.python.org/3/library/stdtypes.html#typesnumeric), using j or J suffix (e.g. 3+5j).

### 3.1.2. Text


