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

    Station* stat = getStation("Gera%20Hbf");
    printf("hbf: %s (%s)\n", stat->name, stat->id);

    const char* unescapedStr = "Gera Süd";

    char* escStr = escapeString(unescapedStr);
    //printf("\n");

    Station* stat2 = getStation(escStr);
    printf("sued: %s (%s)\n", stat2->name, stat2->id);
    //free(stat);
    free(escStr);

    return 0;
}
