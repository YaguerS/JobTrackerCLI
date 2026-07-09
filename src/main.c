#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "application.h"
#include "file.h"
#include "menu.h"
#include "stats.h"

static int parseDatePart(const char *text, size_t start, size_t length){
    int value = 0;
    size_t i;

    for (i = 0; i < length; i++){
        unsigned char ch = (unsigned char)text[start + i];

        if (!isdigit(ch)){
            return -1;
        }

        value = (value * 10) + (int)(ch - '0');
    }

    return value;
}

static int isValidDate(const char *dateText, int required){
    int month;
    int day;

    if (dateText == NULL){
        return 0;
    }

    if (!required && dateText[0] == '\0'){
        return 1;
    }

    if (strlen(dateText) != 10 || dateText[4] != '-' || dateText[7] != '-'){
        return 0;
    }

    month = parseDatePart(dateText, 5, 2);
    day = parseDatePart(dateText, 8, 2);

    return month >= 1 && month <= 12 && day >= 1 && day <= 31;
}

static int readDate(const char *prompt, char *buffer, size_t size, int required){
    for (;;){
        if (!readText(prompt, buffer, size, required)){
            return 0;
        }

        if (isValidDate(buffer, required)){
            return 1;
        }

        puts("Invalid date. Use YYYY-MM-DD.");
    }
}

static const char *displayText(const char *text){
    if (text == NULL || text[0] == '\0'){
        return "-";
    }

    return text;
}

static int charsEqualIgnoreCase(char left, char right){
    unsigned char leftChar = (unsigned char)left;
    unsigned char rightChar = (unsigned char)right;

    return tolower(leftChar) == tolower(rightChar);
}

static int startsWithIgnoreCase(const char *text, const char *query){
    size_t i;

    if (text == NULL || query == NULL){
        return 0;
    }

    for (i = 0; query[i] != '\0'; i++){
        if (text[i] == '\0' || !charsEqualIgnoreCase(text[i], query[i])){
            return 0;
        }
    }

    return 1;
}

static int containsIgnoreCase(const char *text, const char *query){
    size_t i;

    if (text == NULL || query == NULL || query[0] == '\0'){
        return 0;
    }

    for (i = 0; text[i] != '\0'; i++){
        if (startsWithIgnoreCase(&text[i], query)){
            return 1;
        }
    }

    return 0;
}

static void printApplication(const Application *application, size_t number){
    if (application == NULL){
        return;
    }

    printf("\nApplication %zu\n", number);
    puts("----------------");
    printf("Company:        %s\n", displayText(application->company));
    printf("Position:       %s\n", displayText(application->position));
    printf("Location:       %s\n", displayText(application->location));
    printf("Source:         %s\n", displayText(application->source));
    printf("Date applied:   %s\n", displayText(application->dateApplied));
    printf("Status:         %s\n", statusToString(application->status));
    printf("Follow-up date: %s\n", displayText(application->followUpDate));
    printf("Notes:          %s\n", displayText(application->notes));
}

static void handleAddApplication(ApplicationList *applications){
    Application application;
    int statusChoice;

    memset(&application, 0, sizeof(application));

    puts("\nAdd application");

    if (!readText("Company: ", application.company, sizeof(application.company), 1) ||
        !readText("Position: ", application.position, sizeof(application.position), 1) ||
        !readText("Location: ", application.location, sizeof(application.location), 0) ||
        !readText("Source: ", application.source, sizeof(application.source), 0) ||
        !readDate("Date applied (YYYY-MM-DD): ", application.dateApplied,
                  sizeof(application.dateApplied), 1)){
        puts("Add application cancelled.");
        return;
    }

    puts("Status:");
    printStatusOptions();
    if (!readIntInRange("Choose status (1-8): ", StatusLead, StatusAccepted, &statusChoice)){
        puts("Add application cancelled.");
        return;
    }

    application.status = (ApplicationStatus)statusChoice;

    if (!readDate("Follow-up date (YYYY-MM-DD, optional): ", application.followUpDate,
                  sizeof(application.followUpDate), 0) ||
        !readText("Notes: ", application.notes, sizeof(application.notes), 0)){
        puts("Add application cancelled.");
        return;
    }

    if (!addApplicationToList(applications, &application)){
        puts("Could not add application. Memory allocation failed.");
        return;
    }

    printf("Added application for %s.\n", application.company);
}

static void handleViewApplications(const ApplicationList *applications){
    size_t i;

    if (getApplicationCount(applications) == 0){
        puts("No applications loaded yet.");
        return;
    }

    printf("\nApplications (%zu)\n", getApplicationCount(applications));

    for (i = 0; i < applications->count; i++){
        printApplication(&applications->items[i], i + 1);
    }
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
    char query[COMPANY_SIZE];
    size_t i;
    size_t matches = 0;

    if (getApplicationCount(applications) == 0){
        puts("No applications available to search.");
        return;
    }

    if (!readText("Company search: ", query, sizeof(query), 1)){
        puts("Search cancelled.");
        return;
    }

    for (i = 0; i < applications->count; i++){
        if (containsIgnoreCase(applications->items[i].company, query)){
            if (matches == 0){
                printf("\nSearch results for \"%s\"\n", query);
            }

            printApplication(&applications->items[i], i + 1);
            matches++;
        }
    }

    if (matches == 0){
        printf("No applications found for \"%s\".\n", query);
        return;
    }

    printf("\nFound %zu matching application(s).\n", matches);
}

static void handleStatistics(const ApplicationList *applications){
    ApplicationStats stats = calculateStats(applications);

    printStats(&stats);
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

static void handleMenuOption(MenuOption option, ApplicationList *applications){
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
