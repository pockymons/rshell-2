# rshell
Purpose
====
rshell is a simple command shell created for my CS100 class at UCR.

How to install
====
```
$ git clone https://github.com/kzhen002/rshell.git
$ cd rshell
$ make
$ bin/rshell
```
Limits/Bugs/Comments
====
1. Currently rshell cannot handle arguments with any amount of connectors such as ||, &&, or ;
	*rshell will pass these connectors into execvp as a command which is a bug. Below is an example of this
	```
	ls || ps &&
	ls ||
	ls ; ls
	```
2.  The # comment command is also not supported
3. Ctr-c is not supported
4. Pipes are also not utilized.
5. The only valid exit command to enter into the shell is exit in that exact case
6. Finally, the code is a lot messier than I'd like it to be so I will clean it up soon and add in all of the missing features. My parsing method needs to be changed first to allow this to happen. 




