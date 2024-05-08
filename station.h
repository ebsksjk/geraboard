#ifndef station

#define station

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "requests.h"
#include "cJSON.h"

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

Station* getStation(const char* name) {
    Request req;
    asprintf(&req.URL, "https://v6.db.transport.rest/locations?query=%s&results=1", name);
    printf("%s\n", req.URL);

    makeRequest(&req);

    printf("\n\n%s\n", req.response);

    free(req.response);
}

#endif