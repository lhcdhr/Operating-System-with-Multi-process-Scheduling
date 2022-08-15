# Operating System with Multi-process Scheduling
 
## Description
A simple operating system implemented with C. 
- To compile, use command `make mysh`. 
- To run the OS in interactive mode, use `./mysh`. 
- The OS also supports batch mode that takes a file as input, use `./mysh < input.txt`.
- Supports basic commands, including `help` `quit` `print` `set` `echo` `run` `exec` `my_ls`, and the commands are case sensitive.
- Scripts can be executed concurrently following different scheduling policies, including First Come First Serve (FCFS), Shortest Job First (SJF), Round Robin (RR), and SJF with job Aging.

## Command
### `help`
- `help`
- Display all the commands and how to use them.
### `quit`
- `quit`
- Terminate the OS Shell, and delete the backing store directory and all its contents.
### `set`
- `set VAR STRING`
- If `VAR` already exists, then overwrite the previously assigned one with `STRING`.
- If not, assign `STRING` to variable `VAR`.
### `print`
- `print VAR`
- Display the value assigned to `VAR`.
- If `VAR` does not exist in memory, then error message will be displayed as `Variable does not exist`
### `echo`
- `echo STRING` or `echo $VAR`
- If the token is a `STRING`, then it will be displayed in the next line.
- If the token is preceded by a `$`, then the assigned value of `VAR` will be displayed. If no such `VAR` exists in the OS Shell memory, then it will display an empty line.
### `run`
- `run SCRIPT`
- Assuming the file `SCRPIT` exists in the current directory, this command sends each line of the file to the intepreter.
- Each line of the command is treated as a command.
- If error occurs when running, the error will be displayed and the script continue running.
- The code of `SCRIPT` is loaded in to the OS Shell memory line by line.
- A PCB keeps track of the PID, the location in memory where the script is stored, and the current inscruction to execute.
- The program is added to the ready queue.
- When `SCRIPT` terminates, the stored code is cleaned from the OS Shell memory.
### `exec`
- `exec P1 P2 P3 POLICY`
- This command can execute up to 3 programs concurrently following the given scheduling policy `POLICY`.
- The scheduling policies include `FCFS`, `SJF`, `RR`, and `AGING` (SJF with aging technique).
- The code of each process is loaded into the OS Shell memory, and PCBs are created just like in `run` command.
- PCBs are added to the ready queue, and the order and updating of the queue is determined by the scheduling policies.
- The program that finished executing will be removed from the queue, and the stored code in the memory will be cleaned.
### `my_ls`
- `my_ls`
- List all files in the current directory in alphabetical order.
