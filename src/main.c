#include <stdio.h>

#include "application.h"
#include "menu.h"

static void handleAddApplication(const ApplicationList *applications){
    (void)applications;

    puts("Add application is not implemented yet.");
}

static void handleViewApplications(const ApplicationList *applications){
    if (getApplicationCount(applications) == 0){
        puts("No applications loaded yet.");
        return;
    }

    puts("View applications is not implemented yet.");
}

static void handleUpdateStatus(const ApplicationList *applications){
    if (getApplicationCount(applications) == 0){
        puts("No applications available to update.");
        return;
    }

    puts("Available statuses:");
    printStatusOptions();
    puts("Update application status is not implemented yet.");
}

static void handleSearchByCompany(const ApplicationList *applications){
    if (getApplicationCount(applications) == 0){
        puts("No applications available to search.");
        return;
    }

    puts("Search by company is not implemented yet.");
}

static void handleStatistics(const ApplicationList *applications){
    printf("Applications in memory: %zu\n", getApplicationCount(applications));
    puts("Detailed statistics are not implemented yet.");
}

static void handleSave(const ApplicationList *applications){
    printf("Applications ready to save: %zu\n", getApplicationCount(applications));
    puts("CSV saving is not implemented yet.");
}

static void handleMenuOption(MenuOption option, const ApplicationList *applications){
    switch (option){
    case MenuAddApplication:
        handleAddApplication(applications);
        break;

    case MenuViewApplications:
        handleViewApplications(applications);
        break;

    case MenuUpdateStatus:
        handleUpdateStatus(applications);
        break;

    case MenuSearchByCompany:
        handleSearchByCompany(applications);
        break;

    case MenuStatistics:
        handleStatistics(applications);
        break;

    case MenuSave:
        handleSave(applications);
        break;

    case MenuExit:
        break;
    }
}

int main(void){
    ApplicationList applications;
    MenuOption option;

    initApplicationList(&applications);

    puts("JobTrackerCLI");
    puts("-------------");

    do{
        printMenu();
        option = readMenuOption();

        if (option != MenuExit){
            handleMenuOption(option, &applications);
        }
    } while (option != MenuExit);

    freeApplicationList(&applications);

    puts("Goodbye.");

    return 0;
}
