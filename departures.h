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
    char* line;
    char* direction;
    int delay;
    char* operator;
} Departure;

#endif