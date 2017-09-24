#ifndef CGL_ASM
#define CGL_ASM

#define CGL_ASM_LEXICAL_EOF 1
#define CGL_ASM_LEXICAL_RA 3
#define CGL_ASM_LEXICAL_RB 4
#define CGL_ASM_LEXICAL_RC 5
#define CGL_ASM_LEXICAL_FA 6
#define CGL_ASM_LEXICAL_FB 7
#define CGL_ASM_LEXICAL_FC 8
#define CGL_ASM_LEXICAL_VA 9
#define CGL_ASM_LEXICAL_VB 10
#define CGL_ASM_LEXICAL_VC 11
#define CGL_ASM_LEXICAL_ID 300
#define CGL_ASM_LEXICAL_INT_MOV 2
#define CGL_ASM_LEXICAL_STRING 301
#define CGL_ASM_LEXICAL_INT 302
#define CGL_ASM_LEXICAL_HEX 303

typedef int (*cgl_asm_t)();

unsigned char* cgl_asm_read_file(char* file, long* size);

cgl_asm_t cgl_asm(unsigned char* text, long size);

#endif
