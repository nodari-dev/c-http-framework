run:
	cc src/utils/string_builder.c  src/http_types.c src/http_parser.c src/logger.c src/request_queue.c src/request_reader.c src/thread_pool.c src/main.c -o bin/server
	./bin/server

fun:
	cc src/utils/string_builder.c  src/playground.c -o bin/playground
	./bin/playground

ab:
	ab -c 10 -n 10 http://127.0.0.1:8080/
