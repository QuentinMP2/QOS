#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <n7OS/keyboard.h>
#include <n7OS/cpu.h>
#include <n7OS/process.h>

extern process_t process_table[NB_PROC];

/**
 * @brief Display the usage of the minishell.
 */
void print_help() {
    printf("Minishell - Available commands:\n");
    printf("    help - display this help text\n");
    printf("    clear - clear the screen\n");
    printf("    ps - list processes that run on the system\n");
    printf("    block <pid_process> - block the process\n");
    printf("    unblock <pid_process> - unblock the process\n");
    printf("    kill <pid_process> - kill the process\n");
    printf("    poweroff - shutdown the system\n");
    printf("    exit - quit the minishell\n");
}

int minishell() {
    char c = 0;

    /** Buffer to store current command. */
    char buff_cmd[BUFFER_SIZE];

    /** Current index of the buffer; */
    uint16_t buff_i = 0;

    /* Initialize the command buffer. */
    for (int i = 0; i < BUFFER_SIZE; i++)
        buff_cmd[i] = 0;

    while (1) {
        // for (int i = 0; i < 100000000; i++) {
        // }
        // printf("Hello world from MINISHELL\n");
        printf("quentin@QOS > ");

        while((c = kgetch()) != '\n') {
            if (buff_i == 0 && c == '\b') {
                /* Do nothing to avoid delete prompt display in the console. */
                continue;
            } 
            
            if (c != 0 && buff_i < BUFFER_SIZE) {
                /* Stop write char in the buffer if it's full
                 * to avoid segmentation fault. 
                 */

                buff_cmd[buff_i] = c;
                
                if (c == '\b') {
                    /* If backspace then reduce the buffer index. */
                    buff_i--;
                } else {
                    buff_i++;
                }

                printf("%c", c);
            }
        }

        /* Finish the command. */
        buff_cmd[buff_i] = '\0';
        printf("\n");

        char *cmd = strtok(buff_cmd, " ");
        char *arg1 = strtok(NULL, " ");

        if (strcmp(buff_cmd, "help") == 0) {
            print_help();
        } else if (strcmp(buff_cmd, "clear") == 0) {
            printf("\f");
        } else if (strcmp(buff_cmd, "ps") == 0) {
            process_t *process;
            printf("PID\t\tSTATE  \t\tNAME\n");
            for (int i = 0; i < NB_PROC; i++) {
                if (is_process(i)) {
                    process = get_process(i);
                    if (process->state == ELECTED)
                        printf("%03d\t\tELECTED\t\t%s\n", process->pid, process->name);
                    else if (process->state == READY)
                        printf("%03d\t\tREADY  \t\t%s\n", process->pid, process->name);
                    else if (process->state == BLOCKED)
                        printf("%03d\t\tBLOCKED\t\t%s\n", process->pid, process->name);
                }
            }
        } else if (strcmp(cmd, "block") == 0) {
            pid_t pid_proc = atoi(arg1);
            uint32_t pos_proc = find_process(pid_proc);
            if (is_process(pos_proc)) {
                block_process(pid_proc);
            } else {
                printf("The given PID is not a process.\n");
            }
        } else if (strcmp(cmd, "unblock") == 0) {
            pid_t pid_proc = atoi(arg1);
            uint32_t pos_proc = find_process(pid_proc);
            if (is_process(pos_proc)) {
                unblock_process(pid_proc);
            } else {
                printf("The given PID is not a process.\n");
            }
        } else if (strcmp(cmd, "kill") == 0) {
            pid_t pid_proc = atoi(arg1);
            uint32_t pos_proc = find_process(pid_proc);
            if (is_process(pos_proc)) {
                kill_process(pid_proc);
            } else {
                printf("The given PID is not a process.\n");
            }
        } else if (strcmp(buff_cmd, "poweroff") == 0) {
            shutdown(1);
        } else if (strcmp(buff_cmd, "exit") == 0) {
            printf("Goodbye!\n");

            while(1)
                hlt();
        } else {
            printf("Unknown command.\n");
            print_help();
        }
        
        buff_i = 0;
    }
}