#ifndef LIST_H
#define LIST_H

struct list_t {
	unsigned int size;
	unsigned int max;
	void* data;
};

struct list_t* list_new(unsigned int max);

struct list_t* list_new_full(unsigned int size);

const struct list_t* list_new_empty();

void list_free(struct list_t* self, void* freeFunc);

unsigned char list_add(struct list_t** pself, void* value);

unsigned int list_size(struct list_t* self);

void* list_get(struct list_t* self, unsigned int index);

void list_set(struct list_t* self, unsigned int index, void* value);

const void** list_iterator(struct list_t* self);

void* list_remove(struct list_t* self, unsigned int index);

void list_clear(struct list_t* self, void* freeFunc);

#endif
