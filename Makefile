.IGNORE:

all:
	gcc main.c game.c ui.c vector.c stage.c card.c ui_stage.c -o main

clean:
	find . -type f -executable -delete


