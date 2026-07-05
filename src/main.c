#include <stdio.h>

#include "application.h"
#include "file.h"
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
    FileResult result;

    result = saveApplications(APPLICATIONS_CSV_PATH, applications);
    if (result == FileResultOk){
        printf("Saved %zu application(s).\n", getApplicationCount(applications));
        return;
    }

    printf("Could not save applications: %s.\n", fileResultToString(result));
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
    FileResult loadResult;
    FileResult saveResult;
    MenuOption option;

    initApplicationList(&applications);

    puts("JobTrackerCLI");
    puts("-------------");

    loadResult = loadApplications(APPLICATIONS_CSV_PATH, &applications);
    if (loadResult == FileResultOk){
        printf("Loaded %zu application(s).\n", getApplicationCount(&applications));
    }else if (loadResult == FileResultNotFound){
        puts("No saved applications found. Starting with an empty list.");
    }else{
        printf("Could not load applications: %s. Starting with an empty list.\n",
               fileResultToString(loadResult));
    }

    do{
        printMenu();
        option = readMenuOption();

        if (option != MenuExit){
            handleMenuOption(option, &applications);
        }
    } while (option != MenuExit);

    saveResult = saveApplications(APPLICATIONS_CSV_PATH, &applications);
    if (saveResult != FileResultOk){
        printf("Could not save applications on exit: %s.\n", fileResultToString(saveResult));
    }

    freeApplicationList(&applications);

    puts("Goodbye.");

    return 0;
}
