#include "stats.h"

#include <stdio.h>

static int isPendingStatus(ApplicationStatus status){
    return status != StatusRejected && status != StatusAccepted;
}

static double calculateRate(size_t count, size_t total){
    if (total == 0){
        return 0.0;
    }

    return ((double)count / (double)total) * 100.0;
}

ApplicationStats calculateStats(const ApplicationList *applications){
    ApplicationStats stats = {0};
    size_t i;

    if (applications == NULL){
        return stats;
    }

    stats.totalApplications = applications->count;

    for (i = 0; i < applications->count; i++){
        ApplicationStatus status = applications->items[i].status;

        if (isPendingStatus(status)){
            stats.pendingApplications++;
        }

        switch (status){
        case StatusInterview:
            stats.interviews++;
            break;

        case StatusTechnicalTest:
            stats.technicalTests++;
            break;

        case StatusOffer:
            stats.offers++;
            break;

        case StatusAccepted:
            stats.offers++;
            break;

        case StatusRejected:
            stats.rejections++;
            break;

        case StatusLead:
        case StatusApplied:
        case StatusViewed:
            break;
        }
    }

    stats.interviewRate = calculateRate(stats.interviews, stats.totalApplications);
    stats.offerRate = calculateRate(stats.offers, stats.totalApplications);

    return stats;
}

void printStats(const ApplicationStats *stats){
    if (stats == NULL){
        return;
    }

    puts("\nStatistics");
    puts("----------");
    printf("Total applications:   %zu\n", stats->totalApplications);
    printf("Pending applications: %zu\n", stats->pendingApplications);
    printf("Interviews:           %zu\n", stats->interviews);
    printf("Technical tests:      %zu\n", stats->technicalTests);
    printf("Offers:               %zu\n", stats->offers);
    printf("Rejections:           %zu\n", stats->rejections);
    printf("Interview rate:       %.1f%%\n", stats->interviewRate);
    printf("Offer rate:           %.1f%%\n", stats->offerRate);
}
