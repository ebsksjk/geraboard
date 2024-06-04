#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "station.h"
#include "departures.h"
#include "utils.h"

#define MAX_DEP_COUNT 10
#define DEF_LANG "de"
//other: en


int main(int c, char** v){

    for(int j = 1; j < c; j++){
        char* stationname = escapeString(v[j]);
        //printf("unescaped: %s; escaped: %s\n", v[j], stationname);
        Station* station = getStation(stationname);
        //printf("bahnhof %d: %s (%s)\n", j, station->name, station->id);

        int dcount;
        Departure** det = getDepartures(station, &dcount, MAX_DEP_COUNT, DEF_LANG);
	
	if(det == NULL){
		printf("no departures could be loaded. aborting.\n");
		return -1;
	}
	printf("Abfahrten für %s:\n", station->name);
        for(int i = 0; i < dcount; i++) {
            if(det[i] == NULL) {
                printf("ja nun\n");
                continue;
            }
            int rcount = det[i]->rcount;
            bool cancelled = false;
            bool delayed = false;
            bool changedP = false;

            //printf("%s -> %s, %s (%s) - %s (%s)\n", det[i]->line->name, det[i]->direction, det[i]->tPlannedWhen, det[i]->tWhen, det[i]->plannedplatform, det[i]->platform);

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

            printf("Hinweise für %s: %d\n", det[i]->line->name, det[i]->rcount);
            for(int y = 0; y < det[i]->rcount; y++) {
                printf("hinweis nr: %d\n", y);
                /*if(!((det[i]->remarks[y] == NULL) || (det[i]->remarks[y]->text == NULL || !(strcmp(det[i]->remarks[y]->code, "cancelled"))))) {
                    printf("\tHinweis: %s\n", det[i]->remarks[y]->text);
                }*/
                if(det[i]->remarks[y] != NULL && det[i]->remarks[y]->type != NULL) {
                    printf("\tHinweis: %s\n", det[i]->remarks[y]->type);
                }
                if(det[i]->remarks[y] != NULL && det[i]->remarks[y]->code != NULL) {
                    printf("\tHinweis: %s\n", det[i]->remarks[y]->code);
                }
                if(det[i]->remarks[y] != NULL && det[i]->remarks[y]->text != NULL) {
                    printf("\tHinweis: %s\n", det[i]->remarks[y]->text);
                }
            }

        }
        for(int y = 0; y < dcount; y++) {
            freeDeparture(det[y], det[y]->rcount);
        }
        free(det);
        free(stationname);
        free(station);
    }

    /*

    int dCount;
    Departure** det = getDepartures(stat2, &dCount);

    for(int i = 0; i < dCount; i++) {
        printf("line: %s (%s) -> %s - %s (%s); Gleis %s (%s)\n", det[i]->line->name, det[i]->line->fahrtNr, det[i]->direction, det[i]->plannedWhen, det[i]->when, det[i]->plannedplatform, det[i]->platform);
    }

    freeStation(stat2);
    for(int i = 0; i < dCount; i++) {
        freeDeparture(det[i]);
    }
    */

    return 0;
}
