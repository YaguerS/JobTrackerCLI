#ifndef APPLICATION_H
#define APPLICATION_H

#include <stddef.h>

#define COMPANY_SIZE 100
#define POSITION_SIZE 100
#define LOCATION_SIZE 100
#define SOURCE_SIZE 80
#define DATE_SIZE 11
#define NOTES_SIZE 500

typedef enum {
    StatusLead = 1,
    StatusApplied,
    StatusViewed,
    StatusInterview,
    StatusTechnicalTest,
    StatusRejected,
    StatusOffer,
    StatusAccepted
} ApplicationStatus;

typedef struct {
    char company[COMPANY_SIZE];
    char position[POSITION_SIZE];
    char location[LOCATION_SIZE];
    char source[SOURCE_SIZE];
    char dateApplied[DATE_SIZE];
    ApplicationStatus status;
    char followUpDate[DATE_SIZE];
    char notes[NOTES_SIZE];
} Application;

typedef struct {
    Application *items;
    size_t count;
    size_t capacity;
} ApplicationList;

void initApplicationList(ApplicationList *applications);
void freeApplicationList(ApplicationList *applications);
size_t getApplicationCount(const ApplicationList *applications);
int addApplicationToList(ApplicationList *applications, const Application *application);
int isValidStatus(ApplicationStatus status);
const char *statusToString(ApplicationStatus status);
ApplicationStatus statusFromString(const char *statusText);
void printStatusOptions(void);

#endif
