CFILES = src/*.c
OUTPUT = build/main

linux:
	gcc $(CFILES) -o $(OUTPUT) -lSDL2 -lSDL2_ttf

clean:
	find ./build -type f -executable -delete
