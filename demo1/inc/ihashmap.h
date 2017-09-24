#ifndef CGL_IHASHMAP_H
#define CGL_IHASHMAP_H

struct ihashmap {
    unsigned int size;
    void****** array;
    void (*freeValue)(void*);
};

struct ihashmap* cgl_ihashmap_new(void (*freeValue)(void*));

void cgl_ihashmap_free(struct ihashmap* self);

unsigned char cgl_ihashmap_add(struct ihashmap* self, int key, void* value);

void* cgl_ihashmap_get(struct ihashmap* self, int key);

void* cgl_ihashmap_del(struct ihashmap* self, int key);

unsigned int cgl_ihashmap_size(struct ihashmap* self);

void cgl_ihashmap_test();

#endif
