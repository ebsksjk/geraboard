//
// Created by mara on 05.06.24.
//

#ifndef LDEPARTURE_H
#define LDEPARTURE_H

#include "station.h"
#include "departures.h"

Departure** getlDepartures(Station* station, int* sCount, int maxS, const char* lang) {
    Request req;
    /*"nationalExpress":	false,
            "national":	false,
            "regionalExpress":	false,
            "regional":	true,
            "suburban":	false,*/
    asprintf(&req.URL, "https://v6.db.transport.rest/stops/%s/departures?duration=60&remarks=true&suburban=false&regional=false&regionalExpress=false&national=false&results=%d&language=\"%s\"", station->id, maxS, lang);
    //&tram=false
    //printf("req: %s\n", req.URL);
    makeRequest(&req);
    //printf("yayyyyyy");
    //printf("\n\n%s\n", req.response);
    Departure** det =  loadDepartures(req.response, sCount);

    free(req.response);
    free(req.URL);

    return det;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int printlDepartures(const char* sname) {
    const char* stationname = escapeString(sname);
    Station* station = getStation(stationname);
    int dcount;
    Departure** det = getlDepartures(station, &dcount, MAX_DEP_COUNT, DEF_LANG);

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

        for(int y = 0; y < rcount; y++) {
            if(!(strcmp(det[i]->remarks[y]->code, "cancelled"))) {
                cancelled = true;
                break;
            }
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
            printf("heute um %s (ursprünglich %s), ca. %d Minuten verspätet\n", det[i]->tWhen, det[i]->tPlannedWhen, det[i]->delay/60);
        } else {
            printf("heute um %s\n", det[i]->tPlannedWhen);
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

#endif //LDEPARTURE_H
