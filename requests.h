#ifndef REQUESTS_H
    #define REQUESTS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

#define GET_VARIABLE_NAME(Variable) (#Variable)

typedef struct Request{
    char* URL;
    char** argv;
    int argc;

    char* response;

} Request;

size_t write_callback(void *ptr, size_t size, size_t nmemb, char **response) {
    size_t new_size = size * nmemb;
    *response = realloc(*response, new_size + 1); // +1 für das Nullzeichen
    if (*response == NULL) {
        fprintf(stderr, "Fehler beim Zuweisen von Speicher\n");
        return 0;
    }
    memcpy(*response, ptr, new_size);
    (*response)[new_size] = '\0'; // Nullzeichen am Ende setzen
    printf("response: %s\n", *response);
    return new_size;
}


void makeRequest(Request* req){
    CURL *curl;
    CURLcode res;
    char* url = req->URL; // Die URL, von der Sie die Antwort abrufen möchten

    curl = curl_easy_init();
    if (curl) {

        curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 256000);

        // Setzen der URL für die Anfrage
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Festlegen der Funktion zum Verarbeiten der Antwort
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        // Erstellen Sie ein leeres Antwort-Array
        req->response = malloc(1); // Ein Byte für das Nullzeichen
        req->response[0] = '\0'; // Nullterminierung

        // Übergeben des Zielbereichs für die Antwort
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &req->response);


        // Ausführen der Anfrage
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "cURL-Fehler: %s\n", curl_easy_strerror(res));
        }

        // Freigeben von cURL-Ressourcen
        curl_easy_cleanup(curl);
    }

    printf("response in makeRequest: %s\n", req->response);
}

#endif