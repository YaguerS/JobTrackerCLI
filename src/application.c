#include "application.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 8

void initApplicationList(ApplicationList *applications){
    if (applications == NULL){
        return;
    }

    applications->items = NULL;
    applications->count = 0;
    applications->capacity = 0;
}

void freeApplicationList(ApplicationList *applications){
    if (applications == NULL){
        return;
    }

    free(applications->items);
    applications->items = NULL;
    applications->count = 0;
    applications->capacity = 0;
}

size_t getApplicationCount(const ApplicationList *applications){
    if (applications == NULL){
        return 0;
    }

    return applications->count;
}

int addApplicationToList(ApplicationList *applications, const Application *application){
    Application *items;
    size_t newCapacity;

    if (applications == NULL || application == NULL){
        return 0;
    }

    if (applications->count == applications->capacity){
        if (applications->capacity == 0){
            newCapacity = INITIAL_CAPACITY;
        }else{
            if (applications->capacity > ((size_t)-1) / 2){
                return 0;
            }

            newCapacity = applications->capacity * 2;
        }

        items = realloc(applications->items, newCapacity * sizeof(applications->items[0]));
        if (items == NULL){
            return 0;
        }

        applications->items = items;
        applications->capacity = newCapacity;
    }

    applications->items[applications->count] = *application;
    applications->count++;

    return 1;
}

int isValidStatus(ApplicationStatus status){
    return status >= StatusLead && status <= StatusAccepted;
}

const char *statusToString(ApplicationStatus status){
    switch (status){
    case StatusLead:
        return "Lead";

    case StatusApplied:
        return "Applied";

    case StatusViewed:
        return "Viewed";

    case StatusInterview:
        return "Interview";

    case StatusTechnicalTest:
        return "Technical Test";

    case StatusRejected:
        return "Rejected";

    case StatusOffer:
        return "Offer";

    case StatusAccepted:
        return "Accepted";

    default:
        return "Unknown";
    }
}

ApplicationStatus statusFromString(const char *statusText){
    ApplicationStatus status;

    if (statusText == NULL){
        return 0;
    }

    for (status = StatusLead; status <= StatusAccepted; status++){
        if (strcmp(statusText, statusToString(status)) == 0){
            return status;
        }
    }

    return 0;
}

void printStatusOptions(void){
    ApplicationStatus status;

    for (status = StatusLead; status <= StatusAccepted; status++){
        printf("%d. %s\n", status, statusToString(status));
    }
}
