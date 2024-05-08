#include <stdio.h>
#include "station.h"
#include "requests.h"
#include "utils.h"
#include "cJSON.h"

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

    char* unescapedStr = "Gera Süd";

    escapeString(unescapedStr);
    printf("\n");

    return 0;
}
