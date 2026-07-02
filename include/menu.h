#ifndef MENU_H
#define MENU_H

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

#endif
