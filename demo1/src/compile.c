#include <stdlib.h>
#include <stdio.h>
#include "node.h"
#include "lexical.h"
#include "syntax.h"
#include "compile.h"
#include "vm.h"

unsigned char* cgl_compile(const unsigned char* text) {
	struct cgl_compile_t* self = (struct cgl_compile_t*) calloc(1, sizeof(struct cgl_compile_t));
	if (!self) {
		return 0;
	}
	struct cgl_syntax_t* syntax = self->syntax = cgl_syntax_new(text);
	if (!syntax) {
		free(self);
		return 0;
	}
	struct cgl_node_t* node = self->node = (struct cgl_node_t*) cgl_syntax_unit(syntax);
	if (!node) {
		cgl_syntax_free(syntax);
		free(self);
		return 0;
	}
	node->vtable->head(node, self);
	node->vtable->body(node, self);
	unsigned long size = node->vtable->size(node, self);
	unsigned char* bytes = self->bytes = self->bytes_next = (unsigned char*) malloc(size);
	node->vtable->build(node, self);
	// assert(self->bytes + size == self->bytes_next);
	free(node);
	cgl_syntax_free(syntax);
	free(self);
	return bytes;
}
