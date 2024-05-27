#ifndef departures_h
    #define departures_h
/* example:
* "departures": [
{
"tripId": "1|401341|1|80|27052024",
"when": "2024-05-27T12:48:00+02:00",
"plannedWhen": "2024-05-27T12:48:00+02:00",
"delay": 0,
"platform": "1",
"plannedPlatform": "1",
"prognosisType": "prognosed",
"direction": "Gera Hbf",
"provenance": null,
"line": {
"type": "line",
"id": "vbg-rb4",
"fahrtNr": "80270",
"name": "VBG RB4",
"public": true,
"adminCode": "RD____",
"productName": "VBG",
"mode": "train",
"product": "regional",
"operator": {
"type": "operator",
"id": "vogtlandbahn-die-landerbahn-gmbh-dlb",
"name": "vogtlandbahn - Die Länderbahn GmbH DLB"
},
"additionalName": "VBG RB4"
}
}]

*/


typedef struct Line {
    char* fahrtNr;
    char* name;
    char* id;
    char* additionalName;
    char* product;
} Line;

typedef struct Departure{
    char* when;
    char* plannedWhen;
    char* platform;
    char* plannedplatform;
    Line* line;
    char* direction;
    int delay;
} Departure;

/////////////////////////////////////////////////////////////////

Departure** loadDepartures(const char* json_data, int* count) {
    cJSON* root = cJSON_Parse(json_data);
    printf("json: %s\n",json_data);
    if (root == NULL) {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
        return NULL;
    }

    cJSON* departures_json = cJSON_GetObjectItem(root, "departures");
    if (departures_json == NULL) {
        cJSON_Delete(root);
        return NULL;
    }

    int size = cJSON_GetArraySize(departures_json);
    *count = size;
    Departure** departures = (Departure**)malloc(sizeof(Departure*) * size);
    if (departures == NULL) {
        cJSON_Delete(root);
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        cJSON* departure_json = cJSON_GetArrayItem(departures_json, i);

        departures[i] = (Departure*)malloc(sizeof(Departure));
        if (departures[i] == NULL) {
            // Free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(departures[j]->when);
                free(departures[j]->plannedWhen);
                free(departures[j]->platform);
                free(departures[j]->plannedplatform);
                free(departures[j]->direction);
                free(departures[j]->line->fahrtNr);
                free(departures[j]->line->name);
                free(departures[j]->line->id);
                free(departures[j]->line->additionalName);
                free(departures[j]->line->product);
                free(departures[j]->line);
                free(departures[j]);
            }
            free(departures);
            cJSON_Delete(root);
            return NULL;
        }

        // Initialize Departure fields
        departures[i]->when = strdup(cJSON_GetObjectItem(departure_json, "when")->valuestring);
        departures[i]->plannedWhen = strdup(cJSON_GetObjectItem(departure_json, "plannedWhen")->valuestring);
        departures[i]->platform = strdup(cJSON_GetObjectItem(departure_json, "platform")->valuestring);
        departures[i]->plannedplatform = strdup(cJSON_GetObjectItem(departure_json, "plannedPlatform")->valuestring);
        departures[i]->direction = strdup(cJSON_GetObjectItem(departure_json, "direction")->valuestring);
        departures[i]->delay = cJSON_GetObjectItem(departure_json, "delay")->valueint;

        // Parse "line"
        cJSON* line_json = cJSON_GetObjectItem(departure_json, "line");
        if (line_json != NULL) {
            departures[i]->line = (Line*)malloc(sizeof(Line));
            if (departures[i]->line == NULL) {
                // Free previously allocated memory
                for (int j = 0; j <= i; j++) {
                    free(departures[j]->when);
                    free(departures[j]->plannedWhen);
                    free(departures[j]->platform);
                    free(departures[j]->plannedplatform);
                    free(departures[j]->direction);
                    free(departures[j]->line);
                    free(departures[j]);
                }
                free(departures);
                cJSON_Delete(root);
                return NULL;
            }

            // Initialize Line fields
            departures[i]->line->fahrtNr = strdup(cJSON_GetObjectItem(line_json, "fahrtNr")->valuestring);
            departures[i]->line->name = strdup(cJSON_GetObjectItem(line_json, "name")->valuestring);
            departures[i]->line->id = strdup(cJSON_GetObjectItem(line_json, "id")->valuestring);
            departures[i]->line->additionalName = strdup(cJSON_GetObjectItem(line_json, "additionalName")->valuestring);
            departures[i]->line->product = strdup(cJSON_GetObjectItem(line_json, "product")->valuestring);
        }
    }

    cJSON_Delete(root);
    return departures;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Departure** getDepartures(Station* station, int* count) {
    Request req;
    asprintf(&req.URL, "https://v6.db.transport.rest/stops/%s/departures?duration=60&remarks=true&language=en&bus=false&tram=false", station->id);

    makeRequest(&req);
    printf("yayyyyyy");
    printf("\n\n%s\n", req.response);
    int size;
    Departure** ret = loadDepartures(req.response, &size);
    //printf("%s (%s)", stat->name, stat->id);

    free(req.response);
    free(req.URL);

    *count = size;
    return ret;
}

////////////////////////////////////////////////////////////////////////////////////////

void freeDeparture(Departure* departure) {
    if (departure == NULL) return;

    // Freeing fields of the Departure struct
    free(departure->when);
    free(departure->plannedWhen);
    free(departure->platform);
    free(departure->plannedplatform);
    free(departure->direction);

    // Freeing the Line struct and its fields
    if (departure->line != NULL) {
        free(departure->line->fahrtNr);
        free(departure->line->name);
        free(departure->line->id);
        free(departure->line->additionalName);
        free(departure->line->product);
        free(departure->line);
    }

    // Freeing the Departure struct itself
    free(departure);
}


#endif