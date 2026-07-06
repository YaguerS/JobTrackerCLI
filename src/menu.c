#include "menu.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MENU_INPUT_SIZE 64

static int parseInt(const char *input, int *value){
    char *end = NULL;
    long parsed;

    if (input == NULL || value == NULL){
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

    *value = (int)parsed;
    return 1;
}

static void clearInputLine(void){
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF){
    }
}

static int inputLineIsComplete(void){
    int ch = getchar();

    if (ch == '\n' || ch == EOF){
        return 1;
    }

    clearInputLine();
    return 0;
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
            clearInputLine();
        }

        if (parseInt(input, &choice) && choice >= MenuAddApplication &&
            choice <= MenuExit){
            return (MenuOption)choice;
        }

        puts("Invalid option. Please enter a number from 1 to 7.");
    }
}

int readText(const char *prompt, char *buffer, size_t size, int required){
    size_t length;

    if (prompt == NULL || buffer == NULL || size == 0){
        return 0;
    }

    for (;;){
        fputs(prompt, stdout);

        if (fgets(buffer, size, stdin) == NULL){
            putchar('\n');
            return 0;
        }

        if (strchr(buffer, '\n') == NULL && !inputLineIsComplete()){
            printf("Input is too long. Maximum length is %zu character(s).\n", size - 1);
            continue;
        }

        length = strlen(buffer);
        if (length > 0 && buffer[length - 1] == '\n'){
            buffer[length - 1] = '\0';
            length--;
        }

        if (required && length == 0){
            puts("This field is required.");
            continue;
        }

        return 1;
    }
}

int readIntInRange(const char *prompt, int min, int max, int *value){
    char input[MENU_INPUT_SIZE];
    int parsed;

    if (prompt == NULL || value == NULL || min > max){
        return 0;
    }

    for (;;){
        if (!readText(prompt, input, sizeof(input), 1)){
            return 0;
        }

        if (parseInt(input, &parsed) && parsed >= min && parsed <= max){
            *value = parsed;
            return 1;
        }

        printf("Invalid option. Please enter a number from %d to %d.\n", min, max);
    }
}
