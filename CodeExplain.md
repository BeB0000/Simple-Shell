# BeboShell - Complete Function & Library Reference

## Core Libraries & Their Purposes

### **I/O Operations**
```c
#include <stdio.h>    // Standard Input/Output
```
- **printf()** - Print formatted output to stdout
- **fopen()/fclose()** - File open/close operations  
- **perror()** - Print system error messages
- **snprintf()** - Safe string formatting to buffer

### **Memory & System Control**
```c
#include <stdlib.h>   // Standard Library
```
- **exit()** - Terminate program with status code
- **getenv()** - Get environment variables (USER, HOME, PATH)
- **free()** - Release dynamically allocated memory

### **String Handling**
```c
#include <string.h>   // String Manipulation
```
- **strcmp()/strncmp()** - String comparison
- **strcpy()/strncpy()** - String copying
- **strtok()** - Tokenize strings by delimiter
- **strlen()** - Get string length

### **System Information**
```c
#include <sys/utsname.h>  // OS Info
#include <sys/sysinfo.h>  // System Stats
#include <unistd.h>       // POSIX API
```
- **uname()** - Get OS name, version, architecture
- **sysinfo()** - Get RAM usage, uptime
- **sysconf()** - Get CPU core count
- **getcwd()** - Get current working directory

### **File/Directory Operations**
```c
#include <dirent.h>    // Directory Reading
#include <sys/stat.h>  // File Status
```
- **opendir()/readdir()/closedir()** - Directory traversal
- **mkdir()** - Create directories
- **chdir()** - Change working directory

### **Process Management**
```c
#include <unistd.h>    // Process Control
#include <sys/wait.h>  // Process Waiting
#include <sys/types.h> // System Types
```
- **fork()** - Create child processes
- **execvp()** - Execute programs
- **wait()/waitpid()** - Wait for process completion

### **User Interface**
```c
#include <readline/readline.h>    // Line Editing
#include <readline/history.h>     // Command History
```
- **readline()** - Interactive input with editing
- **add_history()** - Maintain command history

---

## **Essential Function Categories for Shell Implementation**

### **1. Process Execution Pipeline**
```c
pid_t pid = fork();           // Create new process
if (pid == 0) {
    execvp(args[0], args);    // Child: replace with command
    exit(EXIT_FAILURE);       // If exec fails
} else {
    wait(&status);            // Parent: wait for completion
}
```

### **2. Built-in Command Handlers**
```c
// cd - Change directory
chdir(path);

// ls - List directory contents  
DIR *dir = opendir(".");
struct dirent *entry;
while ((entry = readdir(dir)) != NULL) {
    printf("%s\n", entry->d_name);
}
closedir(dir);

// mkdir - Create directory
mkdir(dirname, 0755);  // rwxr-xr-x permissions
```

### **3. System Info Commands**
```c
// uname - System information
struct utsname sysinfo;
uname(&sysinfo);
printf("OS: %s\n", sysinfo.sysname);

// free - Memory usage  
struct sysinfo meminfo;
sysinfo(&meminfo);
printf("Free RAM: %lu MB\n", 
       (meminfo.freeram * meminfo.mem_unit) / (1024*1024));

// cpuinfo - Processor count
long cores = sysconf(_SC_NPROCESSORS_ONLN);
```

### **4. String Parsing & Tokenization**
```c
// Split command line into arguments
char *token = strtok(input, " \t\n");
while (token != NULL) {
    args[argc++] = token;
    token = strtok(NULL, " \t\n");
}
args[argc] = NULL;  // NULL-terminate for execvp
```

### **5. Interactive Shell Loop**
```c
while (1) {
    char *input = readline("beboshell> ");  // Read with editing
    if (!input) break;  // Ctrl+D pressed
    
    add_history(input);  // Save to history
    
    // Parse and execute input...
    
    free(input);  // CRITICAL: Free readline's memory
}
```

---

## **Critical Memory Management Notes**

### **readline() Memory Responsibility**
```c
char *cmd = readline("prompt> ");  // ALLOCATES memory internally
// ... use cmd ...
free(cmd);  // YOU must free it! Forgetting causes memory leaks
```

### **String Handling Safety**
```c
// UNSAFE - buffer overflow risk
strcpy(dest, src);

// SAFE alternatives
strncpy(dest, src, sizeof(dest)-1);
dest[sizeof(dest)-1] = '\0';  // Ensure null termination

// Or use snprintf for formatting
snprintf(buffer, sizeof(buffer), "Value: %s", src);
```

---

## **Common Patterns in BeboShell**

### **1. Command Type Detection**
```c
if (strcmp(args[0], "cd") == 0) {
    // Built-in: change directory
    handle_cd(args[1]);
} else if (strcmp(args[0], "exit") == 0) {
    // Built-in: exit shell
    exit(0);
} else {
    // External command: fork & exec
    execute_external(args);
}
```

### **2. Error Handling**
```c
if (chdir(path) != 0) {
    perror("cd failed");  // Prints: "cd failed: No such file..."
    return;
}
```

### **3. Environment Variables**
```c
char *home = getenv("HOME");    // User's home directory
char *user = getenv("USER");    // Current username
char *path = getenv("PATH");    // Command search path
```

---

## **Quick Reference Cheat Sheet**

### **Essential Functions (90% of shell code):**
- `fork()` + `execvp()` + `wait()` - Run commands
- `readline()` + `free()` - User input
- `strtok()` - Parse arguments  
- `strcmp()` - Compare commands
- `chdir()`/`getcwd()` - Directory navigation
- `opendir()`/`readdir()` - List files

### **Common Issues to Avoid:**
1. **Forgetting to free() readline() input**
2. **Not NULL-terminating execvp() argument array**
3. **Using strcpy() without bounds checking**
4. **Missing error checking on system calls**

---

## Code Explanation

### Includes and Definitions

```c
#include <stdio.h>    // Standard Input/Output
#include <stdlib.h>   // Standard Library
#include <string.h>   // String Manipulation
#include <sys/utsname.h>  // OS Info
#include <sys/sysinfo.h>  // System Stats
#include <unistd.h>       // POSIX API
#include <dirent.h>    // Directory Reading
#include <sys/stat.h>  // File Status
#include <sys/types.h> // System Types
#include <sys/wait.h>  // Process Waiting
#include <readline/readline.h>    // Line Editing
#include <readline/history.h>     // Command History
```
These lines include various standard and system libraries necessary for the operations that will be performed. Each `#include` statement brings in different functionalities, such as input/output operations, memory management, string manipulations, system information retrieval, and command-line interaction.

```c
#define BUFF 1024
#define MAX_ARGS 32
```
Here, we define constants for buffer size (**BUFF**) and the maximum number of arguments (**MAX_ARGS**) that a command can take.

---

### Print Device Info Function

```c
void pdi() {
```
Declares a function named **`pdi`** (print device info) that will display system information.

```c
    struct utsname u;
    struct sysinfo s;
```
Two structures are declared: **`utsname`** to store system information, and **`sysinfo`** to hold details about system resources.

```c
    if (uname(&u) == 0) {
```
Calls **`uname`**, filling in the **`utsname`** structure **`u`**. If successful (returns 0), the following block prints various OS details.

```c
        printf("\n\033[47m\033[30mSystem:\033[0m %s\n", u.sysname);
        // Prints system name
        printf("\033[47m\033[30mNode Name:\033[0m %s\n", u.nodename);
        // Prints network node name
        printf("\033[47m\033[30mRelease:\033[0m %s\n", u.release);
        // Prints OS release
        printf("\033[47m\033[30mVersion:\033[0m %s\n", u.version);
        // Prints OS version
        printf("\033[47m\033[30mMachine:\033[0m %s\n", u.machine);
        // Prints machine hardware name
    }
```
These lines print various attributes of the system, using ANSI codes to format the output color.

```c
    if (sysinfo(&s) == 0) {
```
Calls **`sysinfo`**, filling in the **`sysinfo`** structure **`s`**. If successful, it retrieves system RAM and uptime.

```c
        unsigned long total = s.totalram * s.mem_unit / 1024 / 1024;
        unsigned long free = s.freeram * s.mem_unit / 1024 / 1024;
        printf("\033[47m\033[30mTotal RAM:\033[0m %lu MB\n", total);
        // Prints total RAM in MB
        printf("\033[47m\033[30mFree RAM:\033[0m %lu MB\n", free);
        // Prints free RAM in MB
        printf("\033[47m\033[30mUptime:\033[0m %ld seconds\n", s.uptime);
        // Prints system uptime
    }
```
Calculates total and free RAM, then prints these along with system uptime similarly formatted.

```c
    long cpu = sysconf(_SC_NPROCESSORS_ONLN);
    printf("\033[47m\033[30mCPU Cores:\033[0m %ld\033[0m\n\n", cpu);
}
```
Uses **`sysconf`** to get the number of CPU cores and prints this information.

---

### Command Parsing Function

```c
int parse(char *input, char **argv) {
    int argc = 0;
```
Declares a function **`parse`** that takes a string input and an array of strings (arguments). It initializes **`argc`** to count the number of arguments.

```c
    char *tok = strtok(input, " ");
```
Uses **`strtok`** to tokenize the input string, breaking it based on spaces.

```c
    while (tok && argc < MAX_ARGS - 1) {
```
Enters a loop to process tokens as long as **`tok`** is not NULL and the argument count does not exceed the limit.

```c
        argv[argc++] = tok;
        tok = strtok(NULL, " ");
    }
```
Stores each token in the **`argv`** array and fetches the next token.

```c
    argv[argc] = NULL;
    return argc;
}
```
Sets the last argument to NULL for proper formatting for functions like **`execvp`** and returns the count of arguments.

---

### Main Function

```c
int main(void) {
    fputs("\033[H\033[2J", stdout);
```
Clears the terminal screen using ANSI escape codes.

```c
    char res[BUFF];
```
Creates a buffer to hold input.

```c
    puts("HELLO in \"BeboShell\" \n\ntype help for more info\n");
```
Prints a welcome message to the user.

```c
    fputs("\n\033[1;96m╔════════════════════════════════════════════════════════════╗\n", stdout);
    // Print ASCII art branding
```
Prints some stylized branding for the shell.

```c
    while (67) //you can make it 1 but I want it to be 67 'idk why?' 
```
A loop that continues indefinitely because the condition is always true (67 is a non-zero value).

```c
    {
        char cwd[BUFF];
        char *dir = "~";
```
Declares a buffer for current working directory and initializes **`dir`** to a default value.

```c
        if (getcwd(cwd, sizeof(cwd))) {
            char *t = strrchr(cwd, '/');
            if (t && *(t + 1)) dir = t + 1;
        }
```
Fetches the current working directory with **`getcwd`** and extracts the last component of the directory path.

```c
        char prompt[BUFF];
        snprintf(prompt, sizeof(prompt), "\033[1;36m◄ %s ◎ \033[0m", dir);
```
Formats the shell prompt using ANSI colors, setting it to show the current directory.

```c
        char *res = readline(prompt);
        if (!res) break;
```
Displays the prompt and reads user input. If input is empty (Ctrl+D), the loop breaks.

```c
        res[strcspn(res, "\n")] = 0;
        if (!*res) continue;
```
Removes the newline character and skips empty inputs.

```c
        char tmp[BUFF];
        strcpy(tmp, res);
```
Copies user input to another buffer for tokenization.

```c
        char *argv[MAX_ARGS];
        int argc = parse(tmp, argv);
```
Parses the command input into arguments.

---

### Command Execution Logic

```c
        //logic
        if (strcmp(argv[0], "exit") == 0 || strcmp(argv[0], "quit") == 0) break;
```
Checks for exit commands to break the loop and end the shell.

```c
        else if (strcmp(argv[0], "cp") == 0) {
```
Handles the copy command.

```c
            if (argc < 3) {
                fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
            } else {
```
Verifies necessary arguments are present; if not, prints usage information.

```c
                FILE *source = fopen(argv[1], "rb");
                if (source == NULL) {
                    perror("Error opening source file");
                }
```
Opens the source file for reading in binary mode.

```c
                FILE *dest = fopen(argv[2], "wb");
                if (dest == NULL) {
                    perror("Error opening destination file");
                    fclose(source);
                }
```
Attempts to open the destination file for writing.

```c
                char buffer[BUFF];
                size_t bytesRead;
                while ((bytesRead = fread(buffer, 1, sizeof(buffer), source)) > 0) {
                    fwrite(buffer, 1, bytesRead, dest);
                }
```
Reads data from the source file in chunks and writes it to the destination file.

```c
                fclose(source);
                fclose(dest);
                printf("File copied successfully from %s to %s\n", argv[1], argv[2]);
```
Closes both files and prints a success message.

```c
        } else if (strcmp(argv[0], "42") == 0) {
            puts("you found the easter egg!!");
```
Checks for a specific "easter egg" command.

```c
        } else if (strcmp(argv[0], "rm") == 0) {
```
Handles the remove command.

```c
            if (argc < 2) {
                puts("rm: missing operand");
            } else if (remove(argv[1]) != 0) {
                perror("rm");
            }
```
Removes the specified file if enough arguments are provided.

```c
        } else if (strcmp(argv[0], "touch") == 0) {
```
Handles the `touch` command to create/update a file.

```c
            if (argc < 2) {
                puts("touch: missing file operand");
            } else {
                FILE *f = fopen(argv[1], "a");
                if (f) fclose(f);
                else perror("touch");
            }
```
Opens the specified file in append mode. If it doesn't exist, it creates it.

```c
        } else if (strcmp(argv[0], "mkdir") == 0) {
```
Handles the `mkdir` command to create directories.

```c
            if (argc < 2) {
                puts("mkdir: missing operand");
            } else if (mkdir(argv[1], 0755) != 0) {
                perror("mkdir");
            }
```
Creates a directory with specified permissions.

---

### Help Command

```c
        } else if (strcmp(argv[0], "help") == 0) {
```
Handles the help command.

```c
            puts("\n\033[1;36mBeboShell Commands:\033[0m");
            // List commands and their descriptions
```
Prints the list of all available commands in a formatted way.

---

### Miscellaneous Commands

```c
        } else if (strcmp(argv[0], "pwd") == 0 || strcmp(argv[0], "path") == 0) {
            if (getcwd(cwd, sizeof(cwd))) puts(cwd);
```
Prints the current working directory.

```c
        } else if (strcmp(argv[0], "whoami") == 0) {
            char *u = getenv("USER");
            puts(u ? u : "unknown");
```
Displays the current username from environment variables.

```c
        } else if (strcmp(argv[0], "echo") == 0) {
            for (int i = 1; i < argc; i++) {
                printf("%s", argv[i]);
                if (i + 1 < argc) putchar(' ');
            }
            putchar('\n');
```
Prints the arguments following **`echo`**.

```c
        } else if (strcmp(argv[0], "clear") == 0 || strcmp(argv[0], "wipe") == 0) {
            fputs("\033[H\033[2J\033[3J", stdout);
            fflush(stdout);
```
Clears the terminal screen.

```c
        } else if (strcmp(argv[0], "cat") == 0) {
```
Handles the `cat` command to display file contents.

```c
            char buffer[BUFF];
            if (argc < 2) {
                puts("cat: missing file operand");
            } else {
                FILE *file = fopen(argv[1], "r");
```
Opens the specified file for reading.

```c
                if (file == NULL) {
                    printf("cat: %s: No such file or directory\n", argv[1]);
                } else {
                    size_t bytes;
                    while ((bytes = fread(buffer, 1, sizeof(buffer), file)) > 0) {
                        fwrite(buffer, 1, bytes, stdout);
                    }
                    fclose(file);
                }
```
Reads the contents of the file and outputs it to the console.

---

### List Files Command

```c
        } else if (strcmp(argv[0], "ls") == 0 || strcmp(argv[0], "peek") == 0) {
            DIR *d = opendir(".");
            if (!d) perror("ls");
            else {
                struct dirent *e;
                while ((e = readdir(d))) {
                    if (e->d_name[0] != '.')
                        printf("%s  ", e->d_name);
                }
                putchar('\n');
                closedir(d);
            }
```
Lists the contents of the current directory, ignoring hidden files (those starting with a dot).

---

### Change Directory Command

```c
        } else if (strcmp(argv[0], "cd") == 0 || strcmp(argv[0], "goto") == 0) {
            char *p = argv[1];
            if (!p || strcmp(p, "~") == 0) p = getenv("HOME");
            if (!p || chdir(p) != 0) perror("cd");
```
Changes the working directory. If no directory is given, it defaults to the user's home directory.

---

### System Information Command

```c
        } else if (strcmp(argv[0], "fetch") == 0) {
            pdi();
```
Calls the **`pdi`** function to fetch and print system information.

---

### Logo Command

```c
        } else if (strcmp(res, "logo") == 0) {
            fputs("\n\033[1;96m╔════════════════════════════════════════════════════════════╗\n", stdout);
            // Prints ascii art logo
```
Displays the ASCII art logo.

---

### External Command Execution

```c
        } else {
            pid_t pid = fork();
            if (pid == 0) {
                execvp(argv[0], argv);
                fprintf(stderr, "BeboShell: command not found: %s\n", argv[0]);
                exit(1);
            } else if (pid > 0) {
                wait(NULL);
            }
```
Handles execution of external commands. It forks a new process, and in the child process, it tries to execute the command. If it fails, it prints an error message. The parent waits for the child process to finish.

---

### Adding to Command History

```c
        if (*res) add_history(res);
        free(res);
    }
```
Adds the input command to the command history and frees the memory allocated by **`readline`**.

---

### Exiting the Shell

```c
    puts("Quitting...");
    return 0;
}
```
Prints a quitting message and returns 0 to indicate successful termination of the program.


This reference covers everything needed to implement a functional Unix-like shell with built-in commands, external program execution, and interactive features.
