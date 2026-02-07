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

This reference covers everything needed to implement a functional Unix-like shell with built-in commands, external program execution, and interactive features.
