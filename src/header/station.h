#ifndef STATION

#define STATION

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include "requests.h"
#include "../dep/cJSON.h"

/* example JSON:
[{
		"type":	"stop",
		"id":	"8010126",
		"name":	"Gera Süd",
		"location":	{
			"type":	"location",
			"id":	"8010126",
			"latitude":	50.870423,
			"longitude":	12.077403
		},
		"products":	{
			"nationalExpress":	false,
			"national":	false,
			"regionalExpress":	false,
			"regional":	true,
			"suburban":	false,
			"bus":	true,
			"ferry":	false,
			"subway":	false,
			"tram":	true,
			"taxi":	false
		}
	}]*/

typedef struct Location {
    char* type;
    char* id;
    double latitude;
    double longitude;
} Location;

typedef struct Products {
    bool nationalExpress;
    bool national;
    bool regionalExpress;
    bool regional;
    bool suburban;
    bool bus;
    bool ferry;
    bool subway;
    bool tram;
    bool taxi;
} Products;

typedef struct Station {
    char* type;
	char* id;
    char* name;
    Location* location;
    Products* products;
} Station;

////////////////////////////////////////////////////////////////////////////////////////////

Station* loadStation(const char* json_data) {
    setlocale(LC_ALL, "C");
    // Parse das JSON
    cJSON* root = cJSON_Parse(json_data);
    if (root == NULL) {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
        return NULL;
    }

    // Das erste Element im Array
    cJSON* stop = cJSON_GetArrayItem(root, 0);
    if (stop == NULL) {
        printf("Error: No stop found in JSON\n");
        cJSON_Delete(root);
        return NULL;
    }
    Station *station = malloc(sizeof(Station));
    if (station == NULL) {
        printf("Error: Could not allocate memory for station\n");
        cJSON_Delete(root);
        return NULL;
    }

    // Speicher für Location und Products allozieren
    station->location = (Location*)malloc(sizeof(Location));
    station->products = (Products*)malloc(sizeof(Products));
    if (station->location == NULL || station->products == NULL) {
        printf("Error: Could not allocate memory for location or products\n");
        free(station);
        cJSON_Delete(root);
        return NULL;
    }

    // Extrahiere "type"
    cJSON* type = cJSON_GetObjectItem(stop, "type");
    if (cJSON_IsString(type) && (type->valuestring != NULL)) {
        station->type = strdup(type->valuestring);
    }

    // Extrahiere "id"
    cJSON* id = cJSON_GetObjectItem(stop, "id");
    if (cJSON_IsString(id) && (id->valuestring != NULL)) {
        station->id = strdup(id->valuestring);
    }

    // Extrahiere "name"
    cJSON* name = cJSON_GetObjectItem(stop, "name");
    if (cJSON_IsString(name) && (name->valuestring != NULL)) {
        station->name = strdup(name->valuestring);
    }

    // Extrahiere "location"
    cJSON* location = cJSON_GetObjectItem(stop, "location");
    if (location != NULL) {
        // Extrahiere "type"
        cJSON* loc_type = cJSON_GetObjectItem(location, "type");
        if (cJSON_IsString(loc_type) && (loc_type->valuestring != NULL)) {
            station->location->type = strdup(loc_type->valuestring);
        }

        // Extrahiere "id"
        cJSON* loc_id = cJSON_GetObjectItem(location, "id");
        if (cJSON_IsString(loc_id) && (loc_id->valuestring != NULL)) {
            station->location->id = strdup(loc_id->valuestring);
        }

        // Extrahiere "latitude"
        cJSON* latitude = cJSON_GetObjectItem(location, "latitude");
        if (cJSON_IsNumber(latitude)) {
            station->location->latitude = latitude->valuedouble;
        }

        // Extrahiere "longitude"
        cJSON* longitude = cJSON_GetObjectItem(location, "longitude");
        if (cJSON_IsNumber(longitude)) {
            station->location->longitude = longitude->valuedouble;
        }
    }

    // Extrahiere "products"
    cJSON* products = cJSON_GetObjectItem(stop, "products");
    if (products != NULL) {
        #define EXTRACT_BOOL(product_name) \
            cJSON* product_name = cJSON_GetObjectItem(products, #product_name); \
            if (cJSON_IsBool(product_name)) { \
                station->products->product_name = cJSON_IsTrue(product_name); \
            }

        EXTRACT_BOOL(nationalExpress);
        EXTRACT_BOOL(national);
        EXTRACT_BOOL(regionalExpress);
        EXTRACT_BOOL(regional);
        EXTRACT_BOOL(suburban);
        EXTRACT_BOOL(bus);
        EXTRACT_BOOL(ferry);
        EXTRACT_BOOL(subway);
        EXTRACT_BOOL(tram);
        EXTRACT_BOOL(taxi);

        #undef EXTRACT_BOOL
    }

    // Bereinigen Sie die JSON-Struktur
    cJSON_Delete(root);

    return station;
}

//////////////////////////////////////////////////////////////////////////////////

void freeStation(Station* station) {
    if (station) {
        free(station->type);
        free(station->id);
        free(station->name);
        free(station->location->type);
        free(station->location->id);
        free(station->location);
        free(station->products);
        free(station);
    }
}

//////////////////////////////////////////////////////////////////////////////////
Station* getStation(const char* name) {
    Request req;
    asprintf(&req.URL, "https://v6.db.transport.rest/locations?query=%s&results=1", name);
    req.response = calloc(1, sizeof(char));
    //printf("%s\n", req.URL);

    makeRequest(&req);
    //printf("yayyyyyy");
    //printf("\n\n%s\n", req.response);

    Station* stat = loadStation(req.response);
    //printf("%s (%s)", stat->name, stat->id);

    free(req.response);
    free(req.URL);

    return stat;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif