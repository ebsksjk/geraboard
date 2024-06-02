#ifndef departures_h
    #define departures_h

#include "utils.h"
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

typedef struct Remark {
    char* type;
    char* code;
    char* text;
} Remark;

typedef struct Departure{
    char* when;
    char* tWhen;
    char* plannedWhen;
    char* tPlannedWhen;
    char* platform;
    char* plannedplatform;
    Line* line;
    char* direction;
    int delay;
    Remark** remarks;
    int rcount;
} Departure;

/////////////////////////////////////////////////////////////////

Departure** loadDepartures(const char* json_data, int* dCount) {
    cJSON* root = cJSON_Parse(json_data);
    //printf("json: %s\n",json_data);
    FILE* testout = fopen("out.json", "w");
    fprintf(testout, "%s", json_data);
    fclose(testout);

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
    *dCount = size;
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
        //printf(" VOR jWhen: %s\n", cJSON_Print(departure_json));
        cJSON* jWhen = cJSON_GetObjectItem(departure_json, "when");
        if(cJSON_IsNull(jWhen) || cJSON_IsInvalid(jWhen)) {
            departures[i]->when = strdup("?");
            departures[i]->tWhen = strdup("?");
        } else {
            departures[i]->when = strdup(jWhen->valuestring);
            departures[i]->tWhen = getTimeFromDateTime(departures[i]->when);
        }

        //printf("NACH jWhen%s\n", cJSON_Print(departure_json));
        cJSON* jPWhen = cJSON_GetObjectItem(departure_json, "plannedWhen");
        if(cJSON_IsNull(jPWhen) || cJSON_IsInvalid(jPWhen)) {
            departures[i]->plannedWhen = strdup("?");
            departures[i]->tPlannedWhen = strdup("?");
        } else {
            departures[i]->plannedWhen = strdup(jPWhen->valuestring);
            departures[i]->tPlannedWhen = getTimeFromDateTime(departures[i]->plannedWhen);
        }

        cJSON* jPlatform = cJSON_GetObjectItem(departure_json, "platform");

        if(cJSON_IsNull(jPlatform) || cJSON_IsInvalid(jPlatform)) {
            departures[i]->platform = strdup("?");
        } else {
            departures[i]->platform = strdup(jPlatform->valuestring);
        }

        cJSON* jPPlatform = cJSON_GetObjectItem(departure_json, "plannedPlatform");
        if(cJSON_IsNull(jPPlatform) || cJSON_IsInvalid(jPPlatform)) {
            departures[i]->plannedplatform = strdup("?");
        } else {
            //printf("%s\n", jPPlatform->valuestring);
            departures[i]->plannedplatform = strdup(jPPlatform->valuestring);
        }

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
            if(cJSON_GetObjectItem(line_json, "additionalName") == NULL) {
                departures[i]->line->additionalName = strdup("?");
            } else {
                departures[i]->line->additionalName = strdup(cJSON_GetObjectItem(line_json, "additionalName")->valuestring);
            }
            departures[i]->line->product = strdup(cJSON_GetObjectItem(line_json, "product")->valuestring);
        }

        //parse remarks
        cJSON* remarks_json = cJSON_GetObjectItem(departure_json, "remarks");
        int rsize = cJSON_GetArraySize(remarks_json);
        printf("remarks: %s\n", cJSON_Print(remarks_json));
        departures[i]->rcount = rsize;
        departures[i]->remarks = calloc(rsize, sizeof(Remark));
        if(departures[i]->remarks == NULL) {
            printf("aaaaaaaaa\n");
        }
        for (int j = 0; j < rsize; ++j) {
            departures[i]->remarks[j] = malloc(sizeof(Remark));
            cJSON* remark_json = cJSON_GetArrayItem(remarks_json, i);

            cJSON* jType = cJSON_GetObjectItem(remark_json, "type");
            cJSON* jCode = cJSON_GetObjectItem(remark_json, "code");
            cJSON* jText = cJSON_GetObjectItem(remark_json, "text");

            if(cJSON_IsString(jType) && !cJSON_IsNull(jType)) {
                //printf("jtype: %s\n", jType->valuestring);
                departures[i]->remarks[j]->type = strdup(jType->valuestring);
            } else {
                departures[i]->remarks[j]->type = strdup("?");
            }

            if(cJSON_IsString(jCode) && !cJSON_IsNull(jCode)) {
                //printf("jcode: %s\n", jCode->valuestring);
                departures[i]->remarks[j]->code = strdup(jCode->valuestring);
            } else {
                departures[i]->remarks[j]->code = strdup("?");
            }

            if(cJSON_IsString(jText) && !cJSON_IsNull(jText)) {
                //printf("jtext: %s\n", jText->valuestring);
                departures[i]->remarks[j]->text = strdup(jText->valuestring);
            } else {
                departures[i]->remarks[j]->text = NULL;
            }

            /*departures[i]->remarks[j]->type = cJSON_IsString(jType) ? strdup(jType->valuestring) : strdup("?");
            departures[i]->remarks[j]->code = cJSON_IsString(jCode) ? strdup(jCode->valuestring) : strdup("?");
            departures[i]->remarks[j]->text = cJSON_IsString(jText) ? strdup(jText->valuestring) : strdup("?");*/
        }
    }




    cJSON_Delete(root);
    return departures;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Departure** getDepartures(Station* station, int* sCount, int maxS, const char* lang) {
    Request req;
    asprintf(&req.URL, "https://v6.db.transport.rest/stops/%s/departures?duration=60&remarks=true&bus=false&tram=false&results=%d&language=\"%s\"", station->id, maxS, lang);
    //printf("req: %s\n", req.URL);
    makeRequest(&req);
    //printf("yayyyyyy");
    //printf("\n\n%s\n", req.response);
    int dSize;
    Departure** ret = loadDepartures(req.response, &dSize);
    //printf("%s (%s)", stat->name, stat->id);

    free(req.response);
    free(req.URL);

    *sCount = dSize;
    return ret;
}

////////////////////////////////////////////////////////////////////////////////////////

void freeDeparture(Departure* departure, int rsize) {
    if (departure == NULL) return;

    // Freeing fields of the Departure struct
    free(departure->when);
    free(departure->tWhen);
    free(departure->plannedWhen);
    free(departure->tPlannedWhen);
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

    if(departure->remarks != NULL) {
        for(int j = 0; j < rsize; j++) {
            if(departure->remarks[j] != NULL) {
                free(departure->remarks[j]->type);
                free(departure->remarks[j]->code);
                if(departure->remarks[j]->text != NULL) {
                    free(departure->remarks[j]->text);
                }
            }
        }
        free(departure->remarks);
    }

    // Freeing the Departure struct itself
    free(departure);
}


#endif