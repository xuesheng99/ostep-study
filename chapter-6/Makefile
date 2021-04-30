all: syscall_benchmark context_switch_benchmark

clean:
	rm -rf ./dist/*

syscall_benchmark:
	gcc -o ./dist/syscall_benchmark syscall_benchmark.c -Wall

context_switch_benchmark:
	gcc -o ./dist/context_switch_benchmark -g context_switch_benchmark.c -Wall	