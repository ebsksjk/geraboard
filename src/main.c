#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_DEP_COUNT 10
#define MAX_STRING_W 100
#define DEF_LANG "de"
//other: en
#include "header/departures.h"

int main(const int c, char** v){
    int ret = 0;

    if(c == 1) {
        ret = printDepartures("Gera Hbf", false) | printDepartures("Gera Heinrichstraße", true);
        return ret;
    }

    bool localMode = true;

    for(int i = 1; i < c; i++) {
        if(strcmp(v[i], "-s") == 0) {
            localMode = false;
        } else if(strcmp(v[i], "-t") == 0) {
            localMode = true;
        } else if(strcmp(v[i], "--help") == 0 || strcmp(v[i], "-h") == 0) {
            printf("usage: geraboard -s [station 1, ... station n] -t [local station 1, ... local station n]");
            return 0;
        } else {
            ret = printDepartures(v[i], localMode);
        }
    }

    return ret;
}
