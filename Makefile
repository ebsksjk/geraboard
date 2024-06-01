target main:
	gcc -o geraboard main.c cJSON.c -lcurl -lc

target debug:
	gcc -o geraboard main.c cJSON.c -lcurl -lc -g

target all:
	gcc -o geraboard main.c cJSON.c -lcurl -lc

target memsafe:
	gcc -o geraboard main.c cJSON.c -lcurl -lc -fsanitize=address -g

target strict:
	gcc -o geraboard main.c cJSON.c -lcurl -lc -g -Wall -Wpedantic