#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "asm.h"
#include "vm.h"
#include "cg_io.h"
#include "lexical.h"
#include "syntax.h"
#include "compile.h"
#include "linker.h"
#include "ihashmap.h"
#include "lexasm.h"
#include "gpu.h"

static int main_help() {
	printf("Cpu Gpu Language v0.2\n\n");
	printf("cgp -test [-loop]\n");
	printf("\tMake test of this binary\n");
	printf("cgp -test -cuda\n");
	printf("\tCheck if there is CUDA\n");
	printf("cgp -asm ((-e <text>) | <filename.cga>)\n");
	printf("\tExecute a Virtual Machine Code\n");
//	printf("cgp -asm -l ((-e <text>) | <filename.cga>)\n");
//	printf("cgp -asm -c ((-e <text>) | <filename.cga>)\n");
	return 0;
}

static char** main_arg(int argc, char** argv, char* name) {
	int n;
	for (n = 0; n < argc; n++) {
		char* arg = *argv;
		if (!strcmp(arg, name)) {
			return argv;
		}
		argv++;
	}
	return 0;
}

static unsigned char* main_code(int argc, char** argv) {
	if (argc <= 1) {
		main_help();
		return 0;
	}
	if (main_arg(argc, argv, "-e")) {
		argv++;
		return (unsigned char*) strdup(*argv);
	} else {
		unsigned char* text = cgl_io_file(*argv);
		if (!text) {
			printf("Can not load: %s\n", *argv);
			return 0;
		}
		return text;
	}
}

static int main_asm(int argc, char** argv) {
	if (argc <= 0) {
		return main_help();
	}
//	byte loop_mode = main_arg(argc, argv, "-loop");
	byte disassembly = main_arg(argc, argv, "-d") != 0;
	if (disassembly) {
		return 0;
	} else {
		unsigned char* text = main_code(argc, argv);
		int result = cgl_asm_eval((const unsigned char*) text);
		printf("Finished: %d\n", result);
		free(text);
		return result;
	}
}

static int main_test(int argc, char** argv) {
	char** loop_mode = main_arg(argc, argv, "-loop");
	char** cuda_mode = main_arg(argc, argv, "-cuda");
	if (cuda_mode) {
		if (cgl_gpu_cuda_has()) {
			printf("CUDA was found\n");
		} else {
			printf("Can not found the CUDA\n");
		}
	} else {
		do {
			cgl_asm_test();
			cgl_lexical_test();
			cgl_syntax_test();
			cgl_compile_test();
			cgl_linker_test();
			cgl_vm_test();
			cgl_ihashmap_test();
			cgl_lexasm_test();
//			cgl_gpu_cuda_test();
			{
				int exp;
				long long frac;
				cgl_io_double_to_integer(1.23456789, &exp, &frac);
				assert(cgl_io_integer_to_double(exp, frac) == 1.23456789);
			}
		} while (loop_mode || 1);
	}
	return 0;
}

int main(int argc, char** argv) {
//	register size_t a = 2;
//	register size_t b = 0;
//	size_t flag = a == b;
//	unsigned char buffer[100];
//	memcpy(buffer, ((unsigned char*)main)+60, 66-60);
//	if (argc <= 1) {
//		return main_help();
//	}
	argc--;
	argv++;
//	if (main_arg(argc, argv, "-test")) {
		return main_test(argc - 1, argv + 1);
//	} else if (main_arg(argc, argv, "-asm")) {
//		return main_asm(argc - 1, argv + 1);
//	} else {
//		return main_help();
//	}
}
