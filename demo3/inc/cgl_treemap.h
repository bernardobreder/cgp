#ifndef BREDER_TREEMAP_H
#define BREDER_TREEMAP_H

/**
 * Define the type of treemap
 */
typedef struct cgl_treemap_t cgl_treemap_t;

/**
 * Define the type of error
 */
typedef unsigned char cgl_treemap_error_t;

/**
 * Create a new treemap
 */
struct cgl_treemap_t* cgl_treemap_new(int (*compareFunc)(void*, void*),
		void (*freeKeyFunc)(void*), void (*freeValueFunc)(void*));

/**
 * Free the treemap struct
 */
void cgl_treemap_free(struct cgl_treemap_t* data) ;

/**
 * Create a new treemap with int key
 */
struct cgl_treemap_t* cgl_treemap_new_int(void (*freeValueFunc)(void*));

/**
 * Create a new treemap with char* key
 */
struct cgl_treemap_t* cgl_treemap_new_vchar(unsigned char freeKey,
		void (*freeValueFunc)(void*));

/**
 * Add a new element.
 */
cgl_treemap_error_t cgl_treemap_put(struct cgl_treemap_t* data, void* key,
		void* value);

/**
 * Add a new element.
 * The key is a int.
 */
cgl_treemap_error_t cgl_treemap_put_int(struct cgl_treemap_t* data, int key,
		void* value);

/**
 * Return a element of treemap
 */
void* cgl_treemap_get(struct cgl_treemap_t* data, void* key);

/**
 * Return a element of treemap
 */
void* cgl_treemap_get_int(struct cgl_treemap_t* data, int key);

/**
 * Return the size of treemap
 */
int cgl_treemap_size(struct cgl_treemap_t* data);

/**
 * Remove a element of treemap.
 */
void cgl_treemap_remove(struct cgl_treemap_t* data, void* key);

/**
 * Remove a element of treemap.
 */
void cgl_treemap_remove_int(struct cgl_treemap_t* data, int key);

/**
 * Remove and Return a element of treemap
 */
void cgl_treemap_clear(struct cgl_treemap_t* data);

#endif
