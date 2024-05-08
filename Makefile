build:
	cc src/queue.c src/core.c src/main.c src/socketgenerator.c -o bin/server

run:
	./bin/server
