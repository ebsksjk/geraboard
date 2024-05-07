#include <stdio.h>
#include "requests.h"

#define MAX_RESPONSE_SIZE 1024

typedef struct departure{
    char* time;
    char* line; 
    char* to;
} departure;


// Hilfsfunktion, um die Antwort von cURL zu verarbeiten


int main(int argc, char** argv) {
    for(int i = 1; i < argc; i++){
        Request req;
        req.URL = argv[i];
        req.response = NULL;

        makeRequest(&req);

        printf("%s\n", req.response);

        free(req.response);
    }

    return 0;
}
