build:
	cc src/request_queue.c src/request_reader.c src/http_parser.c src/core.c src/main.c -o bin/server
run:
	./bin/server

fun:
	cc src/http_parser.c src/playground.c -o bin/playground
	./bin/playground


send:
	curl -H "Content-Type: application/json" --data @test/data.json http://127.0.0.1:8080/coffee

