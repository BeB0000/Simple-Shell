#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <dirent.h>

#define BUFF 1024

//print device info
void pdi() {
    struct utsname system_info;
    struct sysinfo mem_info;
    
    if (uname(&system_info) == 0) {
        printf("\nSystem: %s\n", system_info.sysname);
        printf("Node Name: %s\n", system_info.nodename);
        printf("Release: %s\n", system_info.release);
        printf("Version: %s\n", system_info.version);
        printf("Machine: %s\n", system_info.machine);
    }
    
    if (sysinfo(&mem_info) == 0) {
        printf("Total RAM: %lu MB\n", mem_info.totalram / 1024 / 1024);
        printf("Free RAM: %lu MB\n", mem_info.freeram / 1024 / 1024);
        printf("Uptime: %ld seconds\n", mem_info.uptime);
    }
    long num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    printf("CPU Cores: %ld\n\n", num_cpus);
}



int main(void){
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

	while(67) //you can make it 1 but i want it to be 67 idk why :)
	{
		char *dirN = "~";
		char cwd[BUFF];
		if (getcwd(cwd, sizeof(cwd))) {
       			char *temp = strrchr(cwd, '/');
			if (temp) {
            			dirN = temp + 1;
        		}
		}

		printf("\033[1;36m◄ %s ◎ \033[0m", dirN);
		
		if (fgets(res, BUFF, stdin) == NULL){printf("Quitting... \n\n"); break; };
		
		res[strcspn(res, "\n")] = 0;





		if (strncmp(res, "echo ", 5) == 0 || strcmp(res, "echo") == 0)
		{
			int i = 5;
			while(res[i] == ' ')
			{
				i++;
			}
			while(res[i] != '\0'){
				putchar(res[i]);
				i++;
			}
			puts("");
		}



		else if (strcmp(res, "whoami") == 0)
		{
			char *usr = getenv("USER");
			puts(usr ? usr : "Unknown");
		}
		


		else if (strcmp(res, "ls") == 0 || strcmp(res, "peek") == 0)
		{
			DIR *d = opendir(".");
			if(!d) {
				perror("ls");
			}
			else
			{
				struct dirent *dir;
				while((dir = readdir(d)))
				{
					if (dir->d_name[0] != '.'){
						printf("%s  ", dir->d_name);
					}
				}
				puts("");
				closedir(d);
			}
		}
	


		else if (strcmp(res, "pwd") == 0 || strcmp(res, "path") == 0)
		{
			char cwd[BUFF];
			if(getcwd(cwd, sizeof(cwd))){
				puts(cwd);
			}

			else{
			perror("pwd");
			}
		}



		else if (strcmp(res, "help") == 0){
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



		else if (strncmp(res, "cd ", 3) == 0 || strcmp(res, "cd") == 0 || strcmp(res, "goto") == 0 || strncmp(res, "goto ", 5) == 0)
		{
			char *path;
                        
			if (strcmp(res, "cd") == 0 || strcmp(res, "goto") == 0) {
        			path = getenv("HOME");
        			if (!path) {
            				fprintf(stderr, "cd: HOME not set\n");
            				continue;
        			}
    			}
	    		else {
				if (strcmp(res, "goto") == 0){
        				path = res + 5;
				}
				else{
        				path = res + 3;
        			}

        			while (*path == ' ') {
            				path++;
        			}

        			if (strcmp(path, "~") == 0) {
            				path = getenv("HOME");
            				if (!path) {
                				fprintf(stderr, "cd: HOME not set\n");
                				continue;
            				}
        			}
    			}
    			if (chdir(path) != 0) {
        			perror("cd");
    			}
		}



		else if (strcmp(res, "") == 0){ }// do nothing
		


		else if (strcmp(res, "42") == 0){puts("you found the eastter egg!!");}



		else if (strcmp(res, "clear") == 0 || strcmp(res, "wipe") == 0){fputs("\033[H\033[2J", stdout);}



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




		else if (strcmp(res, "fetch") == 0){
			pdi();
		}



		else if (strcmp(res, "exit") == 0 || strcmp(res, "quit") == 0){puts("Quitting... "); break;}
		


		else {printf("BeboShell: Unknown command: %s\n\n", res);}
	}
	return 0;
}
