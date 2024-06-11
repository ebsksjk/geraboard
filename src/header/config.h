//
// Created by mara on 07.06.24.
//

#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>

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
    Config* ret = NULL;

    return ret;
}

Config* getConfig(const char* path){
    printf("path!: %s\n", path);

    FILE* cfg = fopen(path, "r");
    if(cfg == NULL) {
        printf(":((((((\n");
    }
    fseek (cfg, 0, SEEK_END);
    long length = ftell(cfg);

    char* buffer= calloc(length, sizeof(char));
    fread (buffer, 1, length, cfg);

    fclose(cfg);

    printf("%s\n", buffer);
}



#endif //CONFIG_H
