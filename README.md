## Introduction

This project involved the construction of a shell, which prompts the user for input, executes the command, then prompts the user once more. The shell recognizes system commands and also special commands such as, but not limited to, hop, log, ping, proclore, 
eonate, and eveal.

### Specification 1: Display
Prompt format: <Username@SystemName:~>

### Specification 2: Input
Supports ; separated commands and & for background processes.

### Specification 3: hop
Changes directory. Supports ., .., ~, and - flags.

### Specification 4: reveal
Lists files with -a and -l flags. Color-coded output.

### Specification 5: log
Stores up to 15 commands. Supports purge and execute.

### Specification 6: System Commands
Executes via execvp. Foreground and background support.

### Specification 7: proclore
Process info: PID, status, memory, executable path.

### Specification 8: seek
Recursive search with -d, -f, -e flags.

### Specification 9: .myshrc
Aliases and functions support.

### Specification 10: I/O Redirection
>, >>, < support.

### Specification 11: Pipes
Any number of pipes supported.

### Specification 12: Pipes + Redirection
Combined piping and I/O redirection.

### Specification 13: activities
Lists spawned processes with state.

### Specification 14: Signals
ping command, Ctrl+C, Ctrl+Z, Ctrl+D.

### Specification 15: fg and bg
Process foreground/background control.

### Specification 16: neonate (BONUS)
Prints most recent PID every n seconds.

### Specification 17: iMan
Fetches man pages via sockets.

## Running
1. make -C src
2. ./src/a.out
