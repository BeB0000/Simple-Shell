#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>

#define BUFF 1024
#define MAX_ARGS 32

//print device info
        void pdi() {
                struct utsname u;
                struct sysinfo s;

                if (uname(&u) == 0) {
                        printf("\n\033[47m\033[30mSystem:\033[0m %s\n", u.sysname);
                        printf("\033[47m\033[30mNode Name:\033[0m %s\n", u.nodename);
                        printf("\033[47m\033[30mRelease:\033[0m %s\n", u.release);
                        printf("\033[47m\033[30mVersion:\033[0m %s\n", u.version);
                        printf("\033[47m\033[30mMachine:\033[0m %s\n", u.machine);
                }

                if (sysinfo(&s) == 0) {
                        unsigned long total = s.totalram * s.mem_unit / 1024 / 1024;
                        unsigned long free = s.freeram * s.mem_unit / 1024 / 1024;
                        printf("\033[47m\033[30mTotal RAM:\033[0m %lu MB\n", total);
                        printf("\033[47m\033[30mFree RAM:\033[0m %lu MB\n", free);
                        printf("\033[47m\033[30mUptime:\033[0m %ld seconds\n", s.uptime);
                }

                long cpu = sysconf(_SC_NPROCESSORS_ONLN);
                printf("\033[47m\033[30mCPU Cores:\033[0m %ld\033[0m\n\n", cpu);
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

                while (67) //you can make it 1 but i want it to be 67 'idk why?' :)
                {
                        char cwd[BUFF];
                        char *dir = "~";

                        if (getcwd(cwd, sizeof(cwd))) {
                                char *t = strrchr(cwd, '/');
                                if (t && *(t + 1)) dir = t + 1;
                        }

                        char prompt[BUFF];
                        snprintf(prompt, sizeof(prompt),
                                "\033[1;36m◄ %s ◎ \033[0m", dir);

                        char *res = readline(prompt);
                        if (!res) break;

                        res[strcspn(res, "\n")] = 0;
                        if (!*res) continue;

                        char tmp[BUFF];
                        strcpy(tmp, res);

                        char *argv[MAX_ARGS];
                        int argc = parse(tmp, argv);

                        //logic

                        if (strcmp(argv[0], "exit") == 0 || strcmp(argv[0], "quit") == 0) break;

                        else if (strcmp(argv[0], "cp") == 0) {
                                if (argc < 3) {
                                        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
                                } else {
                                        FILE *source = fopen(argv[1], "rb");
                                        if (source == NULL) {
                                                perror("Error opening source file");
                                        }

                                        FILE *dest = fopen(argv[2], "wb");
                                        if (dest == NULL) {
                                                perror("Error opening destination file");
                                                fclose(source);
                                        }

                                        char buffer[BUFF];
                                        size_t bytesRead;
                                        while ((bytesRead = fread(buffer, 1, sizeof(buffer), source)) > 0) {
                                                fwrite(buffer, 1, bytesRead, dest);
                                        }

                                        fclose(source);
                                        fclose(dest);
                                        printf("File copied successfully from %s to %s\n", argv[1], argv[2]);
                                }
                        } else if (strcmp(argv[0], "42") == 0) {
                                puts("you found the easter egg!!");
                        } else if (strcmp(argv[0], "rm") == 0) {
                                if (argc < 2) {
                                        puts("rm: missing operand");
                                } else if (remove(argv[1]) != 0) {
                                        perror("rm");
                                }
                        } else if (strcmp(argv[0], "touch") == 0) {
                                if (argc < 2) {
                                        puts("touch: missing file operand");
                                } else {
                                        FILE *f = fopen(argv[1], "a");
                                        if (f) fclose(f);
                                        else perror("touch");
                                }
                        } else if (strcmp(argv[0], "mkdir") == 0) {
                                if (argc < 2) {
                                        puts("mkdir: missing operand");
                                } else if (mkdir(argv[1], 0755) != 0) {
                                        perror("mkdir");
                                }
                        } else if (strcmp(argv[0], "help") == 0) {
                                puts("\n\033[1;36mBeboShell Commands:\033[0m");
                                puts("\033[1;34m  echo [text]\033[0m          - Print text to screen");
                                puts("\033[1;34m  whoami\033[0m               - Display the current user");
                                puts("\033[1;34m  ls/peek\033[0m              - List files in the current directory");
                                puts("\033[1;34m  pwd/path\033[0m             - Print working directory");
                                puts("\033[1;34m  clear/wipe\033[0m           - Clear the screen");
                                puts("\033[1;34m  exit/quit\033[0m            - Exit BeboShell");
                                puts("\033[1;34m  logo\033[0m                  - Display the shell logo");
                                puts("\033[1;34m  fetch\033[0m                 - Show device information");
                                puts("\033[1;34m  cat [fileName]\033[0m        - Display file content on screen");
                                puts("\033[1;34m  rm [(dir/file)Name]\033[0m   - Remove specified file or directory");
                                puts("\033[1;34m  mkdir [folderName]\033[0m    - Create a new directory");
                                puts("\033[1;34m  touch [fileName]\033[0m      - Create or update a file");
                                puts("\033[1;34m  cp [src] [dest]\033[0m      - Copy a file from source to destination\n");

                        } else if (strcmp(argv[0], "pwd") == 0 || strcmp(argv[0], "path") == 0) {
                                if (getcwd(cwd, sizeof(cwd))) puts(cwd);
                        } else if (strcmp(argv[0], "whoami") == 0) {
                                char *u = getenv("USER");
                                puts(u ? u : "unknown");
                        } else if (strcmp(argv[0], "echo") == 0) {
                                for (int i = 1; i < argc; i++) {
                                        printf("%s", argv[i]);
                                        if (i + 1 < argc) putchar(' ');
                                }
                                putchar('\n');
                        } else if (strcmp(argv[0], "clear") == 0 || strcmp(argv[0], "wipe") == 0) {
                                fputs("\033[H\033[2J\033[3J", stdout);
                                fflush(stdout);
                        } else if (strcmp(argv[0], "cat") == 0) {
                                char buffer[BUFF];
                                if (argc < 2) {
                                        puts("cat: missing file operand");
                                } else {
                                        FILE *file = fopen(argv[1], "r");

                                        if (file == NULL) {
                                                printf("cat: %s: No such file or directory\n", argv[1]);
                                        } else {
                                                size_t bytes;
                                                while ((bytes = fread(buffer, 1, sizeof(buffer), file)) > 0) {
                                                        fwrite(buffer, 1, bytes, stdout);
                                                }
                                                fclose(file);
                                        }
                                }
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
                        } else if (strcmp(argv[0], "cd") == 0 || strcmp(argv[0], "goto") == 0) {
                                char *p = argv[1];
                                if (!p || strcmp(p, "~") == 0) p = getenv("HOME");
                                if (!p || chdir(p) != 0) perror("cd");
                        } else if (strcmp(argv[0], "fetch") == 0) {
                                pdi();
                        } else if (strcmp(res, "logo") == 0) {
                                fputs("\n\033[1;96m╔════════════════════════════════════════════════════════════╗\n", stdout);
                                fputs("\033[1;96m║   \033[1;36m███████╗ ███████╗ ██████╗  ██████╗     ███████╗██╗  ██╗  \033[1;96m║\n", stdout);
                                fputs("\033[1;96m║   \033[1;36m██╔══██╗ ██╔════╝ ██╔══██╗██╔═══██╗    ██╔════╝██║  ██║  \033[1;96m║\n", stdout);
                                fputs("\033[1;96m║   \033[1;34m██████╔╝ █████╗   ██████╔╝██║   ██║    ███████╗███████║  \033[1;96m║\n", stdout);
                                fputs("\033[1;96m║   \033[1;34m██╔══██╗ ██╔══╝   ██╔══██╗██║   ██║    ╚════██║██╔══██║  \033[1;96m║\n", stdout);
                                fputs("\033[1;96m║   \033[1;35m██████╔╝ ███████╗ ██████╔╝╚██████╔╝    ███████║██║  ██║  \033[1;96m║\n", stdout);
                                fputs("\033[1;96m║   \033[1;35m╚═════╝  ╚══════╝ ╚═════╝  ╚═════╝     ╚══════╝╚═╝  ╚═╝  \033[1;96m║\n", stdout);
                                fputs("\033[1;96m╚════════════════════════════════════════════════════════════╝\033[0m\n\n", stdout);
                        } else {
                                pid_t pid = fork();
                                if (pid == 0) {
                                        execvp(argv[0], argv);
                                        fprintf(stderr, "BeboShell: command not found: %s\n", argv[0]);
                                        exit(1);
                                } else if (pid > 0) {
                                        wait(NULL);
                                }
                        }

                        if (*res) add_history(res);
                        free(res);
                }

                puts("Quitting...");
                return 0;
}
