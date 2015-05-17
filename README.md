#Rshell 

Rshell is a program that uses bash commands and syscalls to replicate the bash terminal

##Install
To install and run rshell, enter the following into a terminal: 

```
1. git clone http://www.github.com/donei003/rshell.git

2. cd rshell

3. git checkout hw2

4. make

5. bin/rshell
```

##Features

Supports all bash commands and connectors such as ```;```, ```&&```, and ```||```

Supports piping and input/output redirection (```|```, ```>```, ```>>```, ```<```)

Supports the chaining of multiple commands using said connectors and piping

Supports the use of comments in the terminal

Supports the exit command

Neglects whitespace characters in between commands, connectors, and arguments

##Bugs and Limitations

- Commands like ```cd``` are not supported currently

- Colors for files, directories, etc. are not supported in this make of Rshell

- Text navigation (such as arrow keys) is not supported by this make of Rshell

- Up arrow key does not run the previous command

- ```echo "hello world"``` will include the quotation marks:w

- Rshell does not accept commands after the first instance of ```#``` in that instruction

- ```&``` and ```|``` are not valid connectors and spaces in between valid connectors are not allowed

- In the occurrence of a invalid command followed by the ```||``` the proceeding instruction will not execute

- The ```echo``` command will include every character until a valid connector is reached, a ```#```, or the endline

- Tab is treated as a character

#ls

Replicating the UNIX command to display the contents of a directory(ies)

##Download and Install
```
$ git clone  http://github.com/donei003/rshell.git
$ cd rshell
$ git checkout hw1
$ make
$ bin/ls
```

##Features
- Working -a flag to display all files, including hidden ones

- Working -l flag to list file descriptions(last modified, size, permissions, etc...)

- Working -R flag with recursive search, refer to Bugs and Limitations

- Allows multiple paths and flags to be used

- Absolute paths and relatives paths supported, including '~'

- If the user enters an invalid flag, an error message will be output but the program will still run

- The absolute path of the folder being displayed is always displayed to give the user more information


##Bugs and Limitations

- When executing ls with certain passed in arguments, an error message will appear saying 
  the operation cannot be performed. This is a problem with the hammer server and does not affect
  the program.

- When using the -R flag, the program will occasionally seg fault. The seg fault will not always happen and if it occurs
  it may not be in the same place. Running the same commands twice could result in one seg fault and one success. This is likely
  due to the memory allocated to each user and exceeding it will cause the seg fault.

- When -a is passed in, all hidden files will appear before regular files/folders. The '.' is given higher precedence.
