#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "station.h"
#include "departures.h"
#include "utils.h"

#define MAX_DEP_COUNT 10
#define MAX_STRING_W 100
#define DEF_LANG "de"
//other: en

int printWString(char* s) {
    bool shouldWrap = false;
    for(int c = 0; *s != 0; s++, c++) {
        if(*s == '\n') {
            continue;
        }
        if(c == MAX_STRING_W) {
            shouldWrap = true;
            c = 0;
        }
        if(shouldWrap && isspace(*s)) {
            putc('\n', stdout);
            putc('\t', stdout);
            putc('\t', stdout);
            shouldWrap = false;
        } else {
            putc(*s, stdout);
        }
    }

    return 0;
}

int printRemarks(Departure *det) {
    for(int y = 0; y < det->rcount; y++) {
        if(!((det->remarks[y] == NULL) || (det->remarks[y]->text == NULL || !(strcmp(det->remarks[y]->code, "cancelled"))))) {
            printf("\t - Hinweis bezüglich %s:\n\t\t", det->remarks[y]->type);
            char* s = det->remarks[y]->text;
            printWString(s);
            printf(" (%s)\n", det->remarks[y]->code);
        }
    }
    return 0;
}

int printDepartures(const char* sname) {
    const char* stationname = escapeString(sname);
    Station* station = getStation(stationname);
    int dcount;
    Departure** det = getDepartures(station, &dcount, MAX_DEP_COUNT, DEF_LANG);

    if(det == NULL){
        printf("no departures could be loaded. aborting.\n");
        return 1;
    }
    printf("Abfahrten für %s:\n", station->name);
    for(int i = 0; i < dcount; i++) {
        if(det[i] == NULL) {
            printf("ja nun\n");
            continue;
        }
        const int rcount = det[i]->rcount;
        bool cancelled = false;
        bool delayed = false;
        bool changedP = false;

        for(int y = 0; y < rcount; y++) {
            if(!(strcmp(det[i]->remarks[y]->code, "cancelled"))) {
                cancelled = true;
                break;
            }
        }
        if((strcmp(det[i]->plannedplatform, det[i]->platform))) {
            changedP = true;
        }
        if(det[i]->delay/60) {
            delayed = true;
        }

        if(cancelled) {
            printf(" - %s (%s) nach (%s) um (%s) fällt heute aus.\n", det[i]->line->name, det[i]->line->fahrtNr, det[i]->direction, det[i]->tPlannedWhen);
            printRemarks(det[i]);
            continue;
        }

        printf(" - %s (%s) nach %s ", det[i]->line->name, det[i]->line->fahrtNr, det[i]->direction);
        if(delayed) {
            printf("heute um %s (ursprünglich %s), ca. %d Minuten verspätet ", det[i]->tWhen, det[i]->tPlannedWhen, det[i]->delay/60);
        } else {
            printf("heute um %s ", det[i]->tPlannedWhen);
        }
        if(changedP) {
            printf("abweichend auf Gleis %s.\n", det[i]->platform);
        } else {
            printf("auf Gleis %s\n", det[i]->platform);
        }
        printRemarks(det[i]);
    }
    for(int y = 0; y < dcount; y++) {
        freeDeparture(det[y], det[y]->rcount);
    }
    free(det);
    free(station);

    return 0;
}


int main(const int c, char** v){
    int ret = 0;

    if(c == 1) {
        ret = printDepartures("Gera Hbf");
    } else {
        for(int j = 1; j < c; j++){
            ret = printDepartures(v[j]);
        }
    }

    return ret;
}
