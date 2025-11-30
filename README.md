# C-Shell

A custom shell implementation in C supporting multiple specifications.

## Specifications Implemented

### Spec 1: Display Prompt
- Displays `<username@systemname:path>` with color formatting
- Replaces home directory with `~` in path display
- Green color prompt using ANSI escape codes

### Spec 2: Input Parsing
- Supports `;` separated commands (sequential execution)
- Supports `&` for background process execution
- Trims whitespace from commands
- Validates command syntax

### Spec 3: hop Command
- Changes directory with support for `~`, `-`, `.`, `..`
- Maintains previous directory for `-` flag
- Prints absolute path after successful hop

### Spec 4: reveal Command
- Lists directory contents with color coding
  - Blue for directories
  - Green for executables
  - White for regular files
- Supports `-a` flag (show hidden files)
- Supports `-l` flag (show file details)
- Lexicographically sorted output

### Spec 5: log Command
- Stores command history (up to 14 commands)
- Avoids consecutive duplicate entries
- Supports `log purge` to clear history
- Supports `log execute <index>` to re-execute commands
- Uses file-based persistence

### Spec 6: System Commands
- Executes system commands via fork + execvp
- Foreground execution with time monitoring
- Background execution with PID display
- Reports commands taking >2 seconds

### Spec 7: proclore Command
- Displays process information
- Shows PID, status, process group, virtual memory, executable path
- Can query any PID or current process

### Spec 8: seek Command
- Recursive directory search
- Supports `-d` flag (search directories only)
- Supports `-f` flag (search files only)
- Supports `-e` flag (open file or enter directory on unique match)
- Color-coded output (blue for directories, green for files)

### Spec 9: .myshrc Configuration
- Supports aliases via `alias name = command`
- Supports functions via `func name() { body }`
- Functions support parameter substitution with `$1`
- Auto-loads on shell startup

### Spec 10: I/O Redirection
- `>` for output redirection (overwrite)
- `>>` for output redirection (append)
- `<` for input redirection
- Combined redirections supported

### Spec 11: Pipes
- Supports command chaining with `|`
- Multiple pipes supported
- Pipe with I/O redirection

### Spec 12: Pipes with Redirection
- Combined piping and I/O redirection in the same command
- Handles complex command pipelines

### Spec 13: activities Command
- Lists all running/stopped background processes
- Sorted alphabetically by process name
- Shows PID, command name, and state

### Spec 14: Signal Handling
- `Ctrl+C` (SIGINT) - Interrupt foreground process
- `Ctrl+Z` (SIGTSTP) - Stop foreground process
- `Ctrl+D` - Ignored (shell does not exit)
- `Ctrl+\` (SIGQUIT) - Logout and kill all processes
- `ping <pid> <signal>` - Send arbitrary signals

### Spec 15: fg and bg Commands
- `fg <pid>` - Bring process to foreground
- `bg <pid>` - Resume stopped process in background
- Proper terminal control management

### Spec 16: neonate Command
- `neonate -n <time>` - Prints most recent PID every `<time>` seconds
- Uses `/proc` filesystem
- Exits on `x` key press (non-blocking input)

### Spec 17: iMan Command
- `iMan <command>` - Fetches man page via socket connection
- Connects to man.he.net on port 80
- Strips HTML tags from response
- URL encoding for special characters

## Build Instructions
```
make
./a.out
```

## File Structure
```
C-Shell/
├── src/
│   ├── main.c          # Main shell loop
│   ├── prompt.c/h      # Spec 1: Display prompt
│   ├── spec2.c/h       # Spec 2: Input parsing
│   ├── hop.c/h         # Spec 3: Directory navigation
│   ├── reveal.c/h      # Spec 4: Directory listing
│   ├── log.c/h         # Spec 5: Command history
│   ├── spec6.c/h       # Spec 6: System commands
│   ├── proc.c/h        # Spec 7: Process info
│   ├── seek.c/h        # Spec 8: File search
│   ├── spec9.c/h       # Spec 9: Configuration
│   ├── ior.c/h         # Spec 10-12: I/O redirection
│   ├── activities.c/h  # Spec 13: Process listing
│   ├── keyboard.c/h    # Spec 14: Signal handling
│   ├── fgbg.c/h        # Spec 15: Foreground/background
│   ├── neonate.c/h     # Spec 16: PID monitoring
│   ├── iman.c/h        # Spec 17: Man page fetcher
│   ├── ping.c/h        # Signal sending
│   ├── makefile        # Build file
│   └── .myshrc         # User configuration
├── README.md
└── .gitignore
```

## Author
Blotify
