#include <stdio.h>
#include "station.h"
#include "departures.h"
#include "utils.h"

#define MAX_DEP_COUNT 10



int main(int c, char** v){

    for(int j = 1; j < c; j++){
        char* stationname = escapeString(v[j]);
        printf("unescaped: %s; escaped: %s\n", v[j], stationname);

        Station* station = getStation(stationname);
        printf("bahnhof %d: %s (%s)\n", j, station->name, station->id);

        int dcount;
        Departure** det = getDepartures(station, &dcount, MAX_DEP_COUNT);

        for(int i = 0; i < dcount; i++) {
            printf("line: %s (%s) -> %s - um %s (actual: %s; Verspätung von %d m.); Gleis %s (%s) (lustige infos: %s)\n", det[i]->line->name, det[i]->line->fahrtNr, det[i]->direction, det[i]->tPlannedWhen, det[i]->tWhen, det[i]->delay/60, det[i]->plannedplatform, det[i]->platform, det[i]->line->additionalName);
        }
        for(int i = 0; i < dcount; i++) {
            freeDeparture(det[i]);
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
