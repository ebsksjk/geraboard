#ifndef utils
#define utils

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include <locale.h>

char* restrcat(char *s1, const char *s2){
    const size_t a = strlen(s1);
    const size_t b = strlen(s2);
    const size_t size_ab = a + b + 1;

    s1 = realloc(s1, size_ab);

    memcpy(s1 + a, s2, b + 1);

    return s1;
}


const char* escapeWChar(wchar_t symbol){
    static char result[2];

    switch (symbol) {
        case L' ':
            return "%20";
        case L'ü':
            return "%C3%BC";
        case L'ö':
            return "%C3%B6";
        case L'ä':
            return "%C3%A4";
        case L'Ä':
            return "%C3%84";
        case L'Ü':
            return "%C3%9C";
        case L'Ö':
            return "%C3%96";
        case L'ß':
            return "%C3%9F";
        default:
            result[0] = (char)symbol;
            result[1] = '\0';
            return result;
    }
} 

char* escapeWideCharString(wchar_t* wstr){
    char* escStr = calloc(1, sizeof(char));
    for (wchar_t* s = wstr; s != NULL && *s != L'\0'; s++) {
        printf("escaped: %s (%c - %x)\n", escapeWChar(*s), *s, *s);
        escStr = restrcat(escStr, escapeWChar(*s));
    }

    printf("escaped String: %s (%ld chars)\n", escStr, strlen(escStr));

    return escStr;
}

char* escapeString(const char* str){

    char* ret;
    wchar_t *wstr;
    int length = strlen(str);

    setlocale(LC_ALL, "");
    
    // Speicher für den wchar_t*-String zuweisen (jedes Zeichen benötigt mehr Speicherplatz als in der char*-Version)
    wstr = calloc(length + 1, sizeof(wchar_t));
    if (wstr == NULL) {
        perror("Could not allocate memory");
        exit(EXIT_FAILURE);
    }
    
    // Den char*-String in einen wchar_t*-String umwandeln
    if (mbstowcs(wstr, str, length + 1) == (size_t)-1) {
        perror("mbstowcs failure :( - ");
        exit(EXIT_FAILURE);
    }
    
    
    // Speicher freigeben
    ret = escapeWideCharString(wstr);
    free(wstr);

    return ret;
}

#endif