#include "menu.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MENU_INPUT_SIZE 64

static int parseMenuChoice(const char *input, int *choice){
    char *end = NULL;
    long parsed;

    if (input == NULL || choice == NULL){
        return 0;
    }

    errno = 0;
    parsed = strtol(input, &end, 10);

    if (end == input || errno == ERANGE || parsed < INT_MIN || parsed > INT_MAX){
        return 0;
    }

    while (*end != '\0'){
        if (*end != '\n' && *end != ' ' && *end != '\t'){
            return 0;
        }
        end++;
    }

    *choice = (int)parsed;
    return 1;
}

void printMenu(void){
    putchar('\n');
    puts("Main menu");
    puts("1. Add application");
    puts("2. View applications");
    puts("3. Update application status");
    puts("4. Search by company");
    puts("5. Statistics");
    puts("6. Save");
    puts("7. Exit");
}

MenuOption readMenuOption(void){
    char input[MENU_INPUT_SIZE];
    int choice = 0;

    for (;;){
        fputs("Choose an option (1-7): ", stdout);

        if (fgets(input, sizeof(input), stdin) == NULL){
            putchar('\n');
            return MenuExit;
        }

        if (strchr(input, '\n') == NULL){
            int ch;

            while ((ch = getchar()) != '\n' && ch != EOF){
            }
        }

        if (parseMenuChoice(input, &choice) && choice >= MenuAddApplication &&
            choice <= MenuExit){
            return (MenuOption)choice;
        }

        puts("Invalid option. Please enter a number from 1 to 7.");
    }
}
