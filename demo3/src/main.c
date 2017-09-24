#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cgl_test.h"
#include "cgl_vm.h"
#include "cgl_asm.h"

void main_help(int argc, char** argv);
void main_version(int argc, char** argv);
int main_test(int argc, char** argv);
int main_execute(int argc, char** argv);
int main_compile(int argc, char** argv);
//void main_asm_disassemble(int argc, char** argv);

//void main_asm_disassemble(int argc, char** argv) {
//	if (argc == 0) {
//		main_help(argc, argv);
//		return;
//	}
//}

int main_execute(int argc, char** argv) {
	if (argc == 0) {
		main_help(argc, argv);
		return 0;
	}
	long size;
	unsigned char* text = cgl_asm_read_file(argv[0], &size);
	if (!text) {
		printf("file not found: \"%s\"\n", argv[0]);
		return 0;
	}
	cgl_asm(text, size);
}

int main_compile(int argc, char** argv) {
	return 0;
}

int main_test(int argc, char** argv) {
	cgl_test();
	return 0;
}

void main_version(int argc, char** argv) {
	printf("cgl version \"0.1\"\n");
	printf("Cpu Gpu Assemble version \"0.1\"\n");
	printf("Cpu Gpu Language version \"0.1\"\n");
}

void main_help(int argc, char** argv) {
	printf("cgl <option>\n");
	printf("option:\n");
	printf("  -h, -help             Help to use the cgl command.\n");
	printf("  -v, -version          Show the version of the cgl.\n");
	printf("  -t, -test             Execute all tests of cgl.\n");
	printf("  -e <file>...         	Execute a assembler program.\n");
//	printf("  -c <file.cgl>...      Compile a Cpu Gpu Language program.\n");
//	printf("  -asm -d <file>...     Disassemble a assembler program.\n");
}

int main(int argc, char** argv) {
	if (argc < 2 || !strcmp("-help", argv[1]) || !strcmp("--help", argv[1]) || !strcmp("help", argv[1]) || !strcmp("h", argv[1]) || !strcmp("-h", argv[1]) || !strcmp("--h", argv[1])) {
		main_help(argc - 2, argv + 2);
	} else if (!strcmp("-v", argv[1]) || !strcmp("-version", argv[1])) {
		main_version(argc - 2, argv + 2);
	} else if (!strcmp("-t", argv[1]) || !strcmp("-test", argv[1])) {
		return main_test(argc - 2, argv + 2);
	} else if (!strcmp("-e", argv[1])) {
		return main_execute(argc - 2, argv + 2);
	} else if (!strcmp("-c", argv[1])) {
		return main_compile(argc - 2, argv + 2);
	} else {
		main_help(argc, argv);
	}
	return EXIT_SUCCESS;
}
