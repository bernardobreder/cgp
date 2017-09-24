#include "platform.h"
#include "cg_lib.h"

#ifdef __WIN32__

#include <windows.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

void* cgl_lib_open(const char* path) {
	return LoadLibrary(path);
}

void* cgl_lib_func(void* self, const char* name) {
	return GetProcAddress(library, name);
}

void cgl_lib_free(void* self) {
    FreeLibrary(self);
}

const char* cgl_lib_error() {
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 0, // Default language
                  (LPTSTR) & lpMsgBuf, 0, NULL);
	return (const char*) lpMsgBuf;
}

//void cgl_so_io_flush() {
//	_flushall();
//}
//
//int cgl_so_fs_dir() {
//	HANDLE hList;
//	TCHAR szDir[MAXSTRING];
//	WIN32_FIND_DATA FileData;
//	snprintf(szDir, MAXSTRING, "%s/*", name);
//	hList = FindFirstFile(szDir, &FileData);
//	if (hList != INVALID_HANDLE_VALUE) {
//		do {
//			if (b_char_compare(FileData.cFileName, ".") && b_char_compare(FileData.cFileName, "..")) {
//				b_bni_new_ostring_define(vm, opath, FileData.cFileName);
//				if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
//					b_array_add(dirs, b_char_dup(FileData.cFileName));
//				} else {
//					b_array_add(files, b_char_dup(FileData.cFileName));
//				}
//			}
//		}while (FindNextFile(hList, &FileData));
//		FindClose(hList);
//	}
//	return B_STATE_SUCCESS;
//}

#else

#include <dlfcn.h>

void* cgl_lib_open(const char* path) {
	return dlopen(path, RTLD_LAZY);
}

void* cgl_lib_func(void* self, const char* name) {
	return dlsym(self, name);
}

void cgl_lib_free(void* self) {
    dlclose (self);
}

const char* cgl_lib_error() {
    return dlerror ();
}

//void b_so_out_flush () {
//	fflush (stdout);
//	fflush (stderr);
//	fflush (stdin);
//}
//
//int b_so_dir_list (b_vm_t* vm, const char* name, b_array_t* dirs, b_array_t* files) {
//	DIR* dir = opendir (name);
//	if ( ! dir) {
//		struct dirent *dp;
//		while ( (dp = readdir (dir))) {
//			if (b_char_compare(dp->d_name, ".") && b_char_compare(dp->d_name, "..")) {
//				if (dp->d_type == DT_DIR) {
//					b_array_add (dirs, b_char_dup (dp->d_name));
//				} else if (dp->d_type == DT_REG) {
//					b_array_add (files, b_char_dup (dp->d_name));
//				}
//			}
//			closedir (dir);
//		}
//	}
//	return B_STATE_SUCCESS;
//}


#endif