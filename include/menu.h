#ifndef MENU_H
#define MENU_H

#include <stddef.h>

typedef enum {
    MenuAddApplication = 1,
    MenuViewApplications,
    MenuUpdateStatus,
    MenuSearchByCompany,
    MenuStatistics,
    MenuSave,
    MenuExit
} MenuOption;

void printMenu(void);
MenuOption readMenuOption(void);
int readText(const char* prompt, char* buffer, size_t size, int required);
int readIntInRange(const char* prompt, int min, int max, int* value);

#endif
