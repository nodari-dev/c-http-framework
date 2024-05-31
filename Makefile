build:
	cc src/request_queue.c src/request_reader.c src/http_parser.c src/main.c -o bin/server
run:
	./bin/server

fun:
	cc src/http_parser.c src/request_reader.c src/request_queue.c  src/playground.c -o bin/playground
	./bin/playground

ab:
	ab -c 10 -n 10000 http://127.0.0.1:8080/
