#Rshell 

Rshell is a program that uses bash commands and syscalls to replicate the bash terminal

##Install
To install and run rshell, enter the following into a terminal: 

```
1. git clone http://www.github.com/donei003/rshell.git

2. cd rshell

3. git checkout hw0

4. make

5. bin/rshell
```

##Features

Supports all bash commands and connectors such as ```;```, ```&&```, and ```||```

Supports the chaining of multiple commands using said connectors

Supports the use of comments in the terminal

Supports the exit command

Neglects whitespace characters in between commands, connectors, and arguments

##Bugs and Limitations

- Commands like ```cd``` are not supported currently

- Piping is also not supported currently as well

- Colors for files, directories, etc. are not supported in this make of Rshell

- Text navigation (such as arrow keys) is not supported by this make of Rshell

- Up arrow key does not run the previous command

- ```echo "hello world"``` will include the quotation marks:w

- Rshell does not accept commands after the first instance of ```#``` in that instruction

- ```&``` and ```|``` are not valid connectors and spaces in between valid connectors are not allowed

- The ```echo``` command will include every character until a valid connector is reached, a ```#```, or the endline

- Tab is treated as a character

