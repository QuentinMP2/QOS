#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <n7OS/keyboard.h>
#include <n7OS/cpu.h>

void print_help() {
    printf("\nMinishell - Available commands:\n");
    printf("    help - display this help text\n");
    printf("    clear - clear the screen\n");
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
        printf("\nquentin@QOS > ");

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

        if (strcmp(buff_cmd, "help") == 0) {
            print_help();
        } else if (strcmp(buff_cmd, "clear") == 0) {
            printf("\f");
        } else if (strcmp(buff_cmd, "poweroff") == 0) {
            shutdown(1);
        } else if (strcmp(buff_cmd, "exit") == 0) {
            printf("\nGoodbye!\n");
            
            while(1)
                hlt();
            // return 1;
        } else {
            printf("\nUnknown command.");
            print_help();
        }
        
        buff_i = 0;
    }
}