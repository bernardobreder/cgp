#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "platform.h"
#include "cg_so.h"
#include "cg_io.h"

#ifdef __WIN32__

#else

int cgl_so_process_execute(const char* command) {
	int result = system(command);
	if (result == 1024 || result == 512) {
		exit(result);
	}
	return result;
}

unsigned char* cgl_so_process_execute_and_result(const char* command) {
	char* suf = " >> /tmp/cg.tmp";
	uint36 suflen = strlen(suf);
	uint36 len = strlen(command);
	char* cmd = malloc(len + suflen + 1);
	strcpy(cmd, command);
	strcpy(cmd + len, " >> /tmp/cg.tmp");
	cmd[len + suflen] = 0;
	int result = cgl_so_process_execute(cmd);
	free(cmd);
	if (result != 0) {
		cgl_so_process_execute("rm /tmp/cg.tmp");
		return 0;
	}
	return cgl_io_file("/tmp/cg.tmp");
}

#endif
