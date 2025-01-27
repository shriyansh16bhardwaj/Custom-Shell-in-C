#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>

#define MAX_INPUT 1024
#define MAX_ARG_COUNT 64

char current_directory[PATH_MAX];
char previous_directory[PATH_MAX];

void help_menu()
{
    printf("Custom Shell Help:\n");
    printf("  cd <directory> - Change the current directory\n");
    printf("  ls             - List files in the current directory\n");
    printf("  exit           - Exit the shell\n");
    printf("  help           - Display this help message\n");
}

void input_parsing(char *input, char **args)
{
    int i = 0;
    args[i] = strtok(input, " ");
    while (args[i] != NULL)
    {
        i++;
        args[i] = strtok(NULL, " ");
    }
}

void command_executioner(char **args)
{
    char new_directory[PATH_MAX];


    int input_fd = -1, output_fd = -1;
    char *input_file = NULL, *output_file = NULL;
    int append_mode = 0; 


    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "<") == 0) 
        {
            input_file = args[i + 1];
            args[i] = NULL; 
        }
        else if (strcmp(args[i], ">") == 0) 
        {
            output_file = args[i + 1];
            args[i] = NULL; 
            append_mode = 0;
        }
        else if (strcmp(args[i], ">>") == 0) 
        {
            output_file = args[i + 1];
            args[i] = NULL; 
            append_mode = 1;
        }
    }

    if (strcmp(args[0], "cd") == 0)
    {
        if (args[1] == NULL)
        {
            fprintf(stderr, "cd: Missing argument\n");
            return;
        }

        if (strcmp(args[1], "-") == 0)
        {
            if (previous_directory[0] == '\0')
            {
                fprintf(stderr, "cd: No previous directory found\n");
                return;
            }
            printf("%s\n", previous_directory);
            if (chdir(previous_directory) != 0)
            {
                perror("cd");
                return;
            }

            strcpy(new_directory, current_directory);
            strcpy(current_directory, previous_directory);
            strcpy(previous_directory, new_directory);
        }
        else
        {
            if (getcwd(new_directory, sizeof(new_directory)) == NULL)
            {
                perror("getcwd");
                return;
            }
            if (chdir(args[1]) != 0)
            {
                perror("cd");
            }
            else
            {
                strcpy(previous_directory, current_directory);
                strcpy(current_directory, args[1]);
            }
        }
        return;
    }

    if (strcmp(args[0], "help") == 0)
    {
        help_menu();
        return;
    }

    pid_t process_id = fork();
    if (process_id < 0)
    {
        perror("Fork failed");
    }
    else if (process_id == 0)
    {

        if (input_file != NULL)
        {
            input_fd = open(input_file, O_RDONLY);
            if (input_fd < 0)
            {
                perror("Input file");
                exit(EXIT_FAILURE);
            }
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }


        if (output_file != NULL)
        {
            if (append_mode)
            {
                output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            }
            else
            {
                output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            if (output_fd < 0)
            {
                perror("Output file");
                exit(EXIT_FAILURE);
            }
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        if (execvp(args[0], args) == -1)
        {
            perror("Execution failed");
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        wait(NULL);
    }
}

int main()
{
    char input[MAX_INPUT];
    char *args[MAX_ARG_COUNT];

    previous_directory[0] = '\0';

    if (getcwd(current_directory, sizeof(current_directory)) == NULL)
    {
        perror("getcwd");
        return 1;
    }
    
    while (1)
    {
        printf("%s$ ", current_directory);

        if (fgets(input, MAX_INPUT, stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = '\0';

        input_parsing(input, args);

        if (args[0] != NULL)
        {
            if (strcmp(args[0], "exit") == 0)
            {
                break;
            }

            command_executioner(args);
        }
    }
    return 0;
}
