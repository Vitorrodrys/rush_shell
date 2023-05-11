This work is a project of the Operating Systems discipline, it aims to build a basic shell with functions for communication between child processes (pipes), control of multiple child processes, manipulation of file descriptors and redirection of file descriptors from standard outputs to other files (duplicate the file descriptors for descriptors 0 " stdin", or for 1 "stdout").
The following functions have been implemented:

  Use of multiple pipes. example: "commandx | command | y | command z | ...".
  
  redirecting standard output to a file to create a new file or overwriting an old one ">", redirecting standard output to adding content to a file ">>" and       redirecting standard input to a command to read from a file "<".
  
  functions like cd, and aliases for the home directory "~" are present.
  
  
