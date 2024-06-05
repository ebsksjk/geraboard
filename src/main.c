#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_DEP_COUNT 10
#define MAX_STRING_W 100
#define DEF_LANG "de"
//other: en
#include "header/departures.h"
#include "header/lDeparture.h"

int main(const int c, char** v){
    int ret = 0;

    if(c == 1) {
        ret = printDepartures("Gera Hbf") | printlDepartures("Gera Heinrichstraße");
        return ret;
    }

    bool stationMode = true;

    for(int i = 1; i < c; i++) {
        if(strcmp(v[i], "-s") == 0) {
            stationMode = true;
        } else if(strcmp(v[i], "-t") == 0) {
            stationMode = false;
        } else if(strcmp(v[i], "--help") == 0 || strcmp(v[i], "-h") == 0) {
            printf("usage: geraboard -s [station 1, ... station n] -t [local station 1, ... local station n]");
            return 0;
        } else {
            if(stationMode) {
                ret = printDepartures(v[i]);
            } else {
                ret = printlDepartures(v[i]);
            }
        }
    }

    return ret;
}
