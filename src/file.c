#include "file.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#define CSV_FIELD_COUNT 8
#define CSV_FIELD_SIZE NOTES_SIZE
#define CSV_LINE_SIZE 2048

static int appendCsvChar(char *field, size_t *length, char ch){
    if (*length >= CSV_FIELD_SIZE - 1){
        return 0;
    }

    field[*length] = ch;
    (*length)++;
    field[*length] = '\0';

    return 1;
}

static int parseCsvLine(const char *line, char fields[CSV_FIELD_COUNT][CSV_FIELD_SIZE], size_t *fieldCount){
    size_t fieldIndex = 0;
    size_t fieldLength = 0;
    size_t i = 0;
    int inQuotes = 0;

    if (line == NULL || fields == NULL || fieldCount == NULL){
        return 0;
    }

    fields[fieldIndex][0] = '\0';

    while (line[i] != '\0'){
        char ch = line[i];

        if (inQuotes){
            if (ch == '"'){
                if (line[i + 1] == '"'){
                    if (!appendCsvChar(fields[fieldIndex], &fieldLength, '"')){
                        return 0;
                    }
                    i += 2;
                    continue;
                }

                inQuotes = 0;
                i++;
                continue;
            }

            if (!appendCsvChar(fields[fieldIndex], &fieldLength, ch)){
                return 0;
            }
            i++;
            continue;
        }

        if (ch == '"'){
            if (fieldLength != 0){
                return 0;
            }

            inQuotes = 1;
            i++;
            continue;
        }

        if (ch == ','){
            fieldIndex++;
            if (fieldIndex >= CSV_FIELD_COUNT){
                return 0;
            }

            fieldLength = 0;
            fields[fieldIndex][0] = '\0';
            i++;
            continue;
        }

        if (ch == '\n' || ch == '\r'){
            break;
        }

        if (!appendCsvChar(fields[fieldIndex], &fieldLength, ch)){
            return 0;
        }
        i++;
    }

    if (inQuotes){
        return 0;
    }

    *fieldCount = fieldIndex + 1;
    return 1;
}

static int copyField(char *destination, size_t destinationSize, const char *source){
    if (destination == NULL || source == NULL || strlen(source) >= destinationSize){
        return 0;
    }

    strcpy(destination, source);
    return 1;
}

static int fieldsAreHeader(char fields[CSV_FIELD_COUNT][CSV_FIELD_SIZE]){
    return strcmp(fields[0], "Company") == 0 &&
           strcmp(fields[1], "Position") == 0 &&
           strcmp(fields[2], "Location") == 0 &&
           strcmp(fields[3], "Source") == 0 &&
           strcmp(fields[4], "Date Applied") == 0 &&
           strcmp(fields[5], "Status") == 0 &&
           strcmp(fields[6], "Follow-up Date") == 0 &&
           strcmp(fields[7], "Notes") == 0;
}

static int applicationFromFields(char fields[CSV_FIELD_COUNT][CSV_FIELD_SIZE], Application *application){
    ApplicationStatus status;

    if (application == NULL){
        return 0;
    }

    status = statusFromString(fields[5]);
    if (!isValidStatus(status)){
        return 0;
    }

    memset(application, 0, sizeof(*application));

    return copyField(application->company, sizeof(application->company), fields[0]) &&
           copyField(application->position, sizeof(application->position), fields[1]) &&
           copyField(application->location, sizeof(application->location), fields[2]) &&
           copyField(application->source, sizeof(application->source), fields[3]) &&
           copyField(application->dateApplied, sizeof(application->dateApplied), fields[4]) &&
           copyField(application->followUpDate, sizeof(application->followUpDate), fields[6]) &&
           copyField(application->notes, sizeof(application->notes), fields[7]) &&
           (application->status = status, 1);
}

static int writeCsvField(FILE *file, const char *field){
    int needsQuotes;
    size_t i;

    if (file == NULL || field == NULL){
        return 0;
    }

    needsQuotes = strpbrk(field, ",\"\n\r") != NULL;

    if (!needsQuotes){
        return fputs(field, file) >= 0;
    }

    if (fputc('"', file) == EOF){
        return 0;
    }

    for (i = 0; field[i] != '\0'; i++){
        if (field[i] == '"' && fputc('"', file) == EOF){
            return 0;
        }

        if (fputc(field[i], file) == EOF){
            return 0;
        }
    }

    return fputc('"', file) != EOF;
}

static int writeApplication(FILE *file, const Application *application){
    return writeCsvField(file, application->company) &&
           fputc(',', file) != EOF &&
           writeCsvField(file, application->position) &&
           fputc(',', file) != EOF &&
           writeCsvField(file, application->location) &&
           fputc(',', file) != EOF &&
           writeCsvField(file, application->source) &&
           fputc(',', file) != EOF &&
           writeCsvField(file, application->dateApplied) &&
           fputc(',', file) != EOF &&
           writeCsvField(file, statusToString(application->status)) &&
           fputc(',', file) != EOF &&
           writeCsvField(file, application->followUpDate) &&
           fputc(',', file) != EOF &&
           writeCsvField(file, application->notes) &&
           fputc('\n', file) != EOF;
}

FileResult loadApplications(const char *filePath, ApplicationList *applications){
    ApplicationList loadedApplications;
    FILE *file;
    char line[CSV_LINE_SIZE];
    size_t lineNumber = 0;

    if (filePath == NULL || applications == NULL){
        return FileResultInvalidData;
    }

    file = fopen(filePath, "r");
    if (file == NULL){
        if (errno == ENOENT){
            return FileResultNotFound;
        }

        return FileResultIoError;
    }

    initApplicationList(&loadedApplications);

    while (fgets(line, sizeof(line), file) != NULL){
        char fields[CSV_FIELD_COUNT][CSV_FIELD_SIZE];
        size_t fieldCount = 0;
        Application application;

        lineNumber++;

        if (strchr(line, '\n') == NULL && !feof(file)){
            freeApplicationList(&loadedApplications);
            fclose(file);
            return FileResultInvalidData;
        }

        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0'){
            continue;
        }

        if (!parseCsvLine(line, fields, &fieldCount) || fieldCount != CSV_FIELD_COUNT){
            freeApplicationList(&loadedApplications);
            fclose(file);
            return FileResultInvalidData;
        }

        if (lineNumber == 1 && fieldsAreHeader(fields)){
            continue;
        }

        if (!applicationFromFields(fields, &application)){
            freeApplicationList(&loadedApplications);
            fclose(file);
            return FileResultInvalidData;
        }

        if (!addApplicationToList(&loadedApplications, &application)){
            freeApplicationList(&loadedApplications);
            fclose(file);
            return FileResultMemoryError;
        }
    }

    if (ferror(file)){
        freeApplicationList(&loadedApplications);
        fclose(file);
        return FileResultIoError;
    }

    if (fclose(file) != 0){
        freeApplicationList(&loadedApplications);
        return FileResultIoError;
    }

    freeApplicationList(applications);
    *applications = loadedApplications;

    return FileResultOk;
}

FileResult saveApplications(const char *filePath, const ApplicationList *applications){
    FILE *file;
    size_t i;

    if (filePath == NULL || applications == NULL){
        return FileResultInvalidData;
    }

    file = fopen(filePath, "w");
    if (file == NULL){
        return FileResultIoError;
    }

    if (fputs("Company,Position,Location,Source,Date Applied,Status,Follow-up Date,Notes\n", file) < 0){
        fclose(file);
        return FileResultIoError;
    }

    for (i = 0; i < applications->count; i++){
        if (!writeApplication(file, &applications->items[i])){
            fclose(file);
            return FileResultIoError;
        }
    }

    if (fclose(file) != 0){
        return FileResultIoError;
    }

    return FileResultOk;
}

const char *fileResultToString(FileResult result){
    switch (result){
    case FileResultOk:
        return "ok";

    case FileResultNotFound:
        return "file not found";

    case FileResultInvalidData:
        return "invalid CSV data";

    case FileResultMemoryError:
        return "memory allocation failed";

    case FileResultIoError:
        return "file input/output error";

    default:
        return "unknown file error";
    }
}
