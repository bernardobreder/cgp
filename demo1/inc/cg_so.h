#ifndef CGL_SO_H
#define CGL_SO_H

#include "platform.h"

int cgl_so_process_execute(const char* command);

unsigned char* cgl_so_process_execute_and_result(const char* command);

#endif
