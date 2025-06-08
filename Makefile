CFILES = src/*.c
OUTPUT = build/main

linux:
	gcc $(CFILES) -o $(OUTPUT) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	find ./build -type f -executable -delete
