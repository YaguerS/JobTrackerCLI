#include <stdio.h>

#include "menu.h"

static void printNotImplemented(MenuOption option){
    switch (option){
    case MenuAddApplication:
        puts("Add application is not implemented yet.");
        break;

    case MenuViewApplications:
        puts("View applications is not implemented yet.");
        break;

    case MenuUpdateStatus:
        puts("Update application status is not implemented yet.");
        break;

    case MenuSearchByCompany:
        puts("Search by company is not implemented yet.");
        break;

    case MenuStatistics:
        puts("Statistics are not implemented yet.");
        break;

    case MenuSave:
        puts("Save is not implemented yet.");
        break;

    case MenuExit:
        break;
    }
}

int main(void){
    MenuOption option;

    puts("JobTrackerCLI");
    puts("-------------");

    do{
        printMenu();
        option = readMenuOption();

        if (option != MenuExit){
            printNotImplemented(option);
        }
    } while (option != MenuExit);

    puts("Goodbye.");

    return 0;
}
