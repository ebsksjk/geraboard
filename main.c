#include <stdio.h>
#include "station.h"
#include "departures.h"
#include "utils.h"

#define MAX_DEP_COUNT 10
#define DEF_LANG "de"
//other: en


int main(int c, char** v){

    for(int j = 1; j < c; j++){
        char* stationname = escapeString(v[j]);
        printf("unescaped: %s; escaped: %s\n", v[j], stationname);

        Station* station = getStation(stationname);
        printf("bahnhof %d: %s (%s)\n", j, station->name, station->id);

        int dcount, rcount;
        Departure** det = getDepartures(station, &dcount, &rcount, MAX_DEP_COUNT, DEF_LANG);

        for(int i = 0; i < dcount; i++) {
            printf("line: %s (%s) -> %s - um %s", det[i]->line->name, det[i]->line->fahrtNr, det[i]->direction, det[i]->tPlannedWhen);
            if(det[i]->delay/60) {
                printf("; heute um: %s; (dh, eine Verspätung von +%d)", det[i]->tWhen, det[i]->delay/60);

            }
            printf(" von Gleis %s (%s) (lustige infos: %s)\n", det[i]->plannedplatform, det[i]->platform, det[i]->line->additionalName);
            for(int y = 0; y <= rcount; y++) {
                if(!((det[i]->remarks[y] == NULL) || (det[i]->remarks[y]->text == NULL))) {
                    printf("Hinweis: (type)%s (code)%s (text)%s\n", det[i]->remarks[y]->type, det[i]->remarks[y]->code, det[i]->remarks[y]->text);
                }
            }
        }
        for(int i = 0; i < dcount; i++) {
            freeDeparture(det[i], rcount);
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
