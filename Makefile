# Makefile

# Define the main target
all: geraboard

# Define how to build the main program
geraboard: src/main.c src/dep/cJSON.c
	@mkdir bin
	@gcc -o bin/geraboard src/main.c src/dep/cJSON.c -lcurl -lc -g

# Define the test target
test: clean geraboard
	@echo "Running tests..."
	@./bin/geraboard
	@rc=$$?; if [ $$rc -eq 0 ]; then echo "Program exited with code 0"; else echo "Program exited with code $$rc"; fi
	@./bin/geraboard | grep -q "Abfahrten für Gera Hbf" && echo "Test passed!" || echo "Test failed!"
	@echo "running test #2"
	@./bin/geraboard "Plochingen"
	@rc=$$?; if [ $$rc -eq 0 ]; then echo "Program exited with code 0"; else echo "Program exited with code $$rc"; fi
	@./bin/geraboard "Plochingen" | grep -q "Abfahrten für Plochingen" && echo "Test passed!" || echo "Test failed!"
	@-./bin/geraboard "00"
	@rc=$$?; if [ $$rc -eq 1 ]; then echo "Program exited with code 1"; else echo "Program exited with code $$rc"; fi
	@-./bin/geraboard "00" | grep -q "aborting" && echo "Test passed!" || echo "Test failed!"
	@exit $$rc

# Define a clean target to remove generated files
clean:
	@rm -f geraboard
	@rm -r bin

debug: src/main.c src/dep/cJSON.c
	@mkdir bin
	@gcc -o bin/geraboard src/main.c src/dep/cJSON.c -lcurl -lc -g

memsafe: src/main.c src/dep/cJSON.c
	@mkdir bin
	@gcc -o bin/geraboard src/main.c src/dep/cJSON.c -lcurl -lc -fsanitize=address -g

strict: src/main.c src/dep/cJSON.c
	@mkdir bin
	@gcc -o bin/geraboard src/main.c src/dep/cJSON.c -lcurl -lc -g -Wall -Wpedantic

run:
	@./bin/geraboard

