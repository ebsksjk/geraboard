#ifndef utils
#define utils

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include <locale.h>


const char* escapeString(const char* str){

    setlocale(LC_ALL, "");

    int length = strlen(str);
    printf("string %s with length %d\n", str, length);
    
    // Speicher für den wchar_t*-String zuweisen (jedes Zeichen benötigt mehr Speicherplatz als in der char*-Version)
    wchar_t *wstr = calloc(length + 1, sizeof(wchar_t));
    if (wstr == NULL) {
        perror("Could not allocate memory");
        exit(EXIT_FAILURE);
    }
    
    // Den char*-String in einen wchar_t*-String umwandeln
    if (mbstowcs(wstr, str, length + 1) == (size_t)-1) {
        perror("mbstowcs failure :( - ");
        exit(EXIT_FAILURE);
    }

    int wlen = wcslen(wstr);
    printf("len: %d\n", wlen);

    if(wstr == NULL){
        printf("NULLLLL :((((\n");
    }

    wprintf(L"aaaaaaaaaa\n");
    
    // Den resultierenden wchar_t*-String ausgeben
    wprintf(L"Der Wide-String: %ls\n", wstr);
    for (wchar_t* s = wstr; s != NULL && *s != L'\0'; s++) {
        wprintf(L"%lc\n", *s);
    }
    
    // Speicher freigeben
    free(wstr);

    return "";

    //return escapeWideCharString(str);
}

const char* escapeWideCharString(const wchar_t* str){

}


const char* escapeSymbol(const wchar_t symbol){
    switch (symbol) {
        case ' ':
            return "%20";
        case 'ü':
            return "%C3%BC";
        case 'ö':
            return "%C3%B6";
        case 'ä':
            return "%C3%A4";
        case 'Ä':
            return "%C3%84";
        case 'Ü':
            return "%C3%9C";
        case 'Ö':
            return "%C3%96";
        case 'ß':
            return "%C3%9F";
    }
} 

#endif