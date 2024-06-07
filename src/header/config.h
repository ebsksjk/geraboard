//
// Created by mara on 07.06.24.
//

#ifndef CONFIG_H
#define CONFIG_H

/*
Beispiel json:

{
    "config": {
        "tstoplist": [
            "Gera Hbf",
            "Gera Süd"
        ],
        "lstoplist": [
            "Gera Heinrichstraße",
            "Gera Friedrich-Engels-Straße"
        ]
    }
}
 */

typedef struct config {
    char** tstoplist;
    int tstopcount;
    char** lstoplist;
    int lstopcount;
} Config;

Config* loadConfig(const char* config){

}

#endif //CONFIG_H
