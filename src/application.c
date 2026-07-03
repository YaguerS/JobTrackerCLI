#include "application.h"

#include <stdio.h>
#include <stdlib.h>

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

void printStatusOptions(void){
    ApplicationStatus status;

    for (status = StatusLead; status <= StatusAccepted; status++){
        printf("%d. %s\n", status, statusToString(status));
    }
}
