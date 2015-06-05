GDB Cheatsheet
===
GDB Cheatsheet is a cheatsheet designed for UCR's CS100 class to help students use and understand gdb.

##Topics Discussed

###List of Commands

| command | what it does |
| break | Suspends program at line number or function |
| tbreak | Creates a temporary breakpoint |
| info | Lists information about it's argument |
| clear | Clears all breakpoints |
| delete | Deletes specific breakpoints |
| disable | Disables a breakpoint |
| enable | Enables a breakpoint |
| list | Lists source code |
| run | Runs the program until end or breakpoint |
| watch | Pauses program when a variable's value changes |
| continue | Continues executing the halted program |
| step | Steps line by line through code |
| next | Similar to step but, automates function calls |
| until | Runs program until a line number or function |
| finish | Run until the end of function |
| print | Prints the contents of a variable |
| backtrace full | Prints all local variables |
| display | Prints variable after every step/next |
| undisplay | Disables display of variable |
| whatis | Display variable's data type |
| where | Displays current line number and function |
| set var | Changes contents of a variable |
| set follow-fork-mode | Follows child or parent after fork |
| record | Enables reverse commands |
| reverse-step | Steps backwards |
| reverse-next | Nexts backwards |
| reverse-continue | Continues backwards |
| signal | Sends signal to program |
| kill | Kills program without exiting gdb |
| quit | Exits gdb |
