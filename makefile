all: derle calistir

derle:
	gcc -I./include/ -I./lib/libfdr/ -o ./lib/Module.o -c ./src/Module.c
	gcc -I./include/ -o ./bin/Main ./lib/Module.o ./src/Main.c ./lib/libfdr/fields.o ./lib/libfdr/dllist.o

calistir:
	./bin/Main
