#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>

#define BUFF 1024
#define MAX_ARGS 32

//print device info
void pdi() {
    struct utsname u;
    struct sysinfo s;

    if (uname(&u) == 0) {
        printf("\nSystem: %s\n", u.sysname);
        printf("Node Name: %s\n", u.nodename);
        printf("Release: %s\n", u.release);
        printf("Version: %s\n", u.version);
        printf("Machine: %s\n", u.machine);
    }

    if (sysinfo(&s) == 0) {
        unsigned long total = s.totalram * s.mem_unit / 1024 / 1024;
        unsigned long free = s.freeram * s.mem_unit / 1024 / 1024;
        printf("Total RAM: %lu MB\n", total);
        printf("Free RAM: %lu MB\n", free);
        printf("Uptime: %ld seconds\n", s.uptime);
    }

    long cpu = sysconf(_SC_NPROCESSORS_ONLN);
    printf("CPU Cores: %ld\n\n", cpu);
}

int parse(char *input, char **argv) {
    int argc = 0;
    char *tok = strtok(input, " ");
    while (tok && argc < MAX_ARGS - 1) {
        argv[argc++] = tok;
        tok = strtok(NULL, " ");
    }
    argv[argc] = NULL;
    return argc;
}

int main(void) {
    fputs("\033[H\033[2J", stdout);
    char res[BUFF];

    puts("HELLO in \"BeboShell\" \n\ntype help for mare info\n");

    fputs("\n\033[1;96m╔════════════════════════════════════════════════════════════╗\n", stdout);
    fputs("\033[1;96m║  \033[1;36m███████╗ ███████╗ ██████╗  ██████╗     ███████╗██╗  ██╗   \033[1;96m║\n", stdout);
    fputs("\033[1;96m║  \033[1;36m██╔══██╗ ██╔════╝ ██╔══██╗██╔═══██╗    ██╔════╝██║  ██║   \033[1;96m║\n", stdout);
    fputs("\033[1;96m║  \033[1;34m██████╔╝ █████╗   ██████╔╝██║   ██║    ███████╗███████║   \033[1;96m║\n", stdout);
    fputs("\033[1;96m║  \033[1;34m██╔══██╗ ██╔══╝   ██╔══██╗██║   ██║    ╚════██║██╔══██║   \033[1;96m║\n", stdout);
    fputs("\033[1;96m║  \033[1;35m██████╔╝ ███████╗ ██████╔╝╚██████╔╝    ███████║██║  ██║   \033[1;96m║\n", stdout);
    fputs("\033[1;96m║  \033[1;35m╚═════╝  ╚══════╝ ╚═════╝  ╚═════╝     ╚══════╝╚═╝  ╚═╝   \033[1;96m║\n", stdout);
    fputs("\033[1;96m╚════════════════════════════════════════════════════════════╝\033[0m\n\n", stdout);

    while (67) //you can make it 1 but i want it to be 67 idk why :)
    {
        char cwd[BUFF];
        char *dir = "~";

        if (getcwd(cwd, sizeof(cwd))) {
            char *t = strrchr(cwd, '/');
            if (t && *(t + 1)) dir = t + 1;
        }

        printf("\033[1;36m◄ %s ◎ \033[0m", dir);

        if (!fgets(res, BUFF, stdin)) break;
        res[strcspn(res, "\n")] = 0;
        if (!*res) continue;

        char tmp[BUFF];
        strcpy(tmp, res);

        char *argv[MAX_ARGS];
        int argc = parse(tmp, argv);

        
        
        //logic
        
        
        if (strcmp(argv[0], "exit") == 0 || strcmp(argv[0], "quit") == 0) break;

        else if (strcmp(argv[0], "help") == 0) {
            puts("BeboShell Commands:");
            puts("  echo [text]  - Print text to screen");
            puts("  whoami       - Display current user");
            puts("  ls/peek      - List files in current directory");
            puts("  pwd/path     - Print working directory");
            puts("  clear/wipe        - Clear the screen");
            puts("  exit/quit    - Exit BeboShell");
            puts("  logo         - show the \"logo\"");
            puts("  fetch        - Show the \"fetch device iinfo\"");
        }

        else if (strcmp(argv[0], "pwd") == 0 || strcmp(argv[0], "path") == 0) {
            if (getcwd(cwd, sizeof(cwd))) puts(cwd);
        }

        else if (strcmp(argv[0], "whoami") == 0) {
            char *u = getenv("USER");
            puts(u ? u : "unknown");
        }

        else if (strcmp(argv[0], "echo") == 0) {
            for (int i = 1; i < argc; i++) {
                printf("%s", argv[i]);
                if (i + 1 < argc) putchar(' ');
            }
            putchar('\n');
        }

        else if (strcmp(argv[0], "clear") == 0 || strcmp(argv[0], "wipe") == 0) {
            fputs("\033[H\033[2J", stdout);
        }

        else if (strcmp(argv[0], "ls") == 0 || strcmp(argv[0], "peek") == 0) {
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
        }

        else if (strcmp(argv[0], "cd") == 0 || strcmp(argv[0], "goto") == 0) {
            char *p = argv[1];
            if (!p || strcmp(p, "~") == 0) p = getenv("HOME");
            if (!p || chdir(p) != 0) perror("cd");
        }

        else if (strcmp(argv[0], "fetch") == 0) {
            pdi();
        }
        
        else if (strcmp(res, "logo") == 0)
        {
            fputs("\n\033[1;96m╔════════════════════════════════════════════════════════════╗\n", stdout);
            fputs("\033[1;96m║   \033[1;36m███████╗ ███████╗ ██████╗  ██████╗     ███████╗██╗  ██╗  \033[1;96m║\n", stdout);
            fputs("\033[1;96m║   \033[1;36m██╔══██╗ ██╔════╝ ██╔══██╗██╔═══██╗    ██╔════╝██║  ██║  \033[1;96m║\n", stdout);
            fputs("\033[1;96m║   \033[1;34m██████╔╝ █████╗   ██████╔╝██║   ██║    ███████╗███████║  \033[1;96m║\n", stdout);
            fputs("\033[1;96m║   \033[1;34m██╔══██╗ ██╔══╝   ██╔══██╗██║   ██║    ╚════██║██╔══██║  \033[1;96m║\n", stdout);
            fputs("\033[1;96m║   \033[1;35m██████╔╝ ███████╗ ██████╔╝╚██████╔╝    ███████║██║  ██║  \033[1;96m║\n", stdout);
            fputs("\033[1;96m║   \033[1;35m╚═════╝  ╚══════╝ ╚═════╝  ╚═════╝     ╚══════╝╚═╝  ╚═╝  \033[1;96m║\n", stdout);
            fputs("\033[1;96m╚════════════════════════════════════════════════════════════╝\033[0m\n\n", stdout);
        }
        
        else {
            pid_t pid = fork();
            if (pid == 0) {
                execvp(argv[0], argv);
                perror("exec");
                exit(1);
            } else if (pid > 0) {
                wait(NULL);
            } else {
                perror("fork");
            }
        }
    }

    puts("Quitting...");
    return 0;
}
