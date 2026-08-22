#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
int main()
{
    char command_history[2100][2100] ;
    int ptr=-1;
    char good_directory[6][2100] = {"/home/user", "/home/user/notes.txt", "/home/user/data.txt", "/home/user/records", "/home/user/records/entry.txt","/home/user/records/greet.sh"};
    char last_direc[2100];
    char current_direc[2100]="/home/user";
    chdir(current_direc);
    strcpy(last_direc, current_direc);
    char builtin[3][10]={"cd","exit","history"};
    while(1)
    {
        printf("MTL458 >");
        char input[2100];
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        if (input[0] == '\n' || input[0] == '\0')
        {
            continue;
        }

        input[strcspn(input, "\n")] = '\0';

        strcpy(command_history[++ptr], input);
        char *arguments[100];
        int argument_count = 0;
        char *token = strtok(input, " \t");

        while (token != NULL && argument_count < 99)
        {
            arguments[argument_count++] = token;
            token = strtok(NULL, " \t");
        }
        arguments[argument_count] = NULL;

        if (argument_count == 0)
        {
            continue;
        }

        if (strcmp(arguments[0], "exit") == 0)
        {
            break;
        }

        int flag = 1;
        for (int i = 0; i < 3; i++)
        {
            if (strcmp(arguments[0], builtin[i]) == 0)
            {
                flag = 0;
                break;
            }
        }
        // printf("%s\n",input);
        // printf("%d\n",flag);
        if(flag)
        {
            pid_t child_pid = fork();

            if (child_pid < 0)
            {
                perror("fork");
                continue;
            }

            if (child_pid == 0)
            {
                execvp(arguments[0], arguments);
                fprintf(stderr, "Invalid Command\n");
                exit(EXIT_FAILURE);
            }

            waitpid(child_pid, NULL, 0);
        }
        else
        {
            if(strcmp(arguments[0], "cd") == 0)
            {
                if(argument_count==1)
                {
                    chdir("/home/user");
                    strcpy(last_direc, current_direc);
                    strcpy(current_direc, "/home/user");
                }
                else if(argument_count==2)
                {
                    if(strcmp(arguments[1],"~")==0)
                    {
                        chdir("/home/user");
                    strcpy(last_direc, current_direc);
                    strcpy(current_direc, "/home/user");
                    }
                    else if(strcmp(arguments[1],"-")==0)
                    {
                        char temp[2100];
                        strcpy(temp, last_direc);
                        strcpy(last_direc, current_direc);
                       strcpy(current_direc, temp);
                       chdir(current_direc);
                        printf("%s\n",current_direc);
                    }
                    else if(strcmp(arguments[1],".")==0)
                    {
                        continue;
                    }
                    else if(strcmp(arguments[1],"..")==0)
                    {
                        chdir("..");
                        strcpy(last_direc, current_direc);
                        char *last_slash = strrchr(current_direc, '/');
                        if (last_slash != NULL)
                        {
                            *last_slash = '\0';
                        }
                        strcpy(current_direc, last_direc);
                    }
                    else
                    {
                        chdir(arguments[1]);
                        char temp[2100];
                        strcpy(temp, current_direc);
                        getcwd(current_direc, sizeof(current_direc));
                        int ok=0;
                        for(int i=0;i<6;i++)
                        {
                            if(strcmp(current_direc,good_directory[i])==0)
                            {
                                strcpy(last_direc, temp);
                                ok=1;
                                break;
                            }
                        }
                        if(ok==0)
                        {
                            printf("Invalid Command\n");
                            strcpy(current_direc, temp);
                            chdir(current_direc);
                        }
                    }
                }
                else
                {
                    printf("Invalid Command\n");
                }
            }
            else if(strcmp(arguments[0], "history") == 0)
            {
                if(argument_count==1)
                {
                    for(int i=0;i<=ptr;i++)
                    {
                        printf("%s\n",command_history[i]);
                    }
                    
                }
                else
                {
                    int no=atoi(arguments[1]);
                    for(int i=ptr-no+1;i<=ptr;i++)
                    {
                        printf("%s\n",command_history[i]);
                    }
                }
            }
    }
}
// printf("%s\n",cwd[0][0]);
}