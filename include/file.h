#ifndef FILE_H
#define FILE_H

#include "application.h"

#define APPLICATIONS_CSV_PATH "data/applications.csv"

typedef enum {
    FileResultOk,
    FileResultNotFound,
    FileResultInvalidData,
    FileResultMemoryError,
    FileResultIoError
} FileResult;

FileResult loadApplications(const char *filePath, ApplicationList *applications);
FileResult saveApplications(const char *filePath, const ApplicationList *applications);
const char *fileResultToString(FileResult result);

#endif
