CFILES = src/*.c
OUTPUT = build/main

linux:
	gcc $(CFILES) -o $(OUTPUT) -lraylib

clean:
	find ./build -type f -executable -delete
