target main:
	gcc -o geraboard main.c cJSON.c -lcurl -lc

target debug:
	gcc -o geraboard main.c cJSON.c -lcurl -lc  -g