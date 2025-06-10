CFILES = src/*.c
OUTPUT = build/main

linux:
	gcc $(CFILES) -o $(OUTPUT) -lSDL2 -lSDL2_ttf
	
macos:
	gcc $(CFILES) -o $(OUTPUT) -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lSDL2_ttf

clean:
	find ./build -type f -executable -delete