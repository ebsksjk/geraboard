# Makefile

# Define the main target
all: geraboard

# Define how to build the main program
geraboard: main.c cJSON.c
	gcc -o geraboard main.c cJSON.c -lcurl -lc -g

# Define the test target
test: geraboard
	@echo "Running tests..."
	@./geraboard
	@rc=$$?; if [ $$rc -eq 0 ]; then echo "Program exited with code 0"; else echo "Program exited with code $$rc"; fi
	@./geraboard | grep -q "Abfahrten für Gera Hbf" && echo "Test passed!" || echo "Test failed!"
	@exit $$rc

# Define a clean target to remove generated files
clean:
	rm -f geraboard

debug:
	gcc -o geraboard main.c cJSON.c -lcurl -lc -g

memsafe:
	gcc -o geraboard main.c cJSON.c -lcurl -lc -fsanitize=address -g

strict:
	gcc -o geraboard main.c cJSON.c -lcurl -lc -g -Wall -Wpedantic
