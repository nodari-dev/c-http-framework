run:
	cc src/utils/* src/logger.c src/http/*  src/core/* src/playground.c -o bin/server
	./bin/server

fun:
	cc src/utils/string_builder.c src/utils/array_list.c src/router/router.c src/playground.c -o bin/playground
	./bin/playground

ab:
	ab -c 10 -n 10 http://127.0.0.1:8080/
