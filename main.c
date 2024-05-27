#include <stdio.h>
#include "station.h"
#include "departures.h"
#include "utils.h"


#define MAX_RESPONSE_SIZE 1024


// Hilfsfunktion, um die Antwort von cURL zu verarbeiten


int main(int argc, char** argv) {
    /*for(int i = 1; i < argc; i++){
        Request req;
        req.URL = argv[i];
        req.response = NULL;

        makeRequest(&req);

        cJSON *json = cJSON_Parse(req.response);

        printf("%s\n", cJSON_Print(json));
        printf("request (unparsed):\n%s\n", req.response);

        free(req.response);
    }*/

    const char* unescapedStr = "Gera Süd";

    char* escStr = escapeString(unescapedStr);
    //printf("\n");

    Station* stat2 = getStation(escStr);
    printf("sued: %s (%s)\n", stat2->name, stat2->id);
    //free(stat);
    free(escStr);

    int dCount;
    Departure** det = getDepartures(stat2, &dCount);

    for(int i = 0; i < dCount; i++) {
        printf("line: %s (%s)\n", det[i]->line->name, det[i]->line->fahrtNr);
    }

    freeStation(stat2);
    for(int i = 0; i < dCount; i++) {
        freeDeparture(det[i]);
    }


    return 0;
}
