#ifndef STATS_H
#define STATS_H

#include <stddef.h>

#include "application.h"

typedef struct {
    size_t totalApplications;
    size_t pendingApplications;
    size_t interviews;
    size_t technicalTests;
    size_t offers;
    size_t rejections;
    double interviewRate;
    double offerRate;
} ApplicationStats;

ApplicationStats calculateStats(const ApplicationList* applications);
void printStats(const ApplicationStats* stats);

#endif
