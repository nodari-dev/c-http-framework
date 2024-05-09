build:
	cc src/queue.c src/http.c src/core.c src/main.c -o bin/server

run:
	./bin/server

send:
	curl -H "Content-Type: application/json" --data @test/data.json http://127.0.0.1:8080/coffee

