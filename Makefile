build:
	cc src/request_queue.c src/http_parser.c src/core.c src/main.c -o bin/server

run:
	./bin/server

send:
	curl -H "Content-Type: application/json" --data @test/data.json http://127.0.0.1:8080/coffee

