target main:
	gcc -o geraboard main.c cJSON.c -lcurl -lc

target debug:
	gcc -o geraboard main.c cJSON.c -lcurl -lc -g

target all:
	gcc -o geraboard main.c cJSON.c -lcurl -lc

target memsafe:
	gcc -o geraboard main.c cJSON.c -lcurl -lc -fsanitize=address -g