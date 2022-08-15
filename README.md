# Operating System with Multi-process Scheduling
 
## Description
A simple operating system implemented with C. 
- To compile, use command `make mysh`. 
- To run the OS in interactive mode, use `./mysh`. 
- The OS also supports batch mode that takes a file as input, use `./mysh < input.txt`.
- Supports basic command, including `help` `quit` `print` `set` `echo` `run` `exec` `my_ls`(list all files in the current directory in alphabetical order), and the commands are case sensitive.

## Command
### `set`
- `set VAR STRING`
- If `VAR` already exists, then overwrite the previously assigned one with `STRING`.
- If not, assign `STRING` to variable `VAR`.
### `print`
- `print VAR`
- Display the value assigned to `VAR`.
- If `VAR` does not exist in memory, then error message will be displayed as `Variable does not exist`
### `run`
