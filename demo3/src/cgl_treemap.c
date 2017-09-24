#include <stdlib.h>
#include <string.h>
#include "cgl_treemap.h"

struct cgl_treemap_entry_t {
	void* key;
	void* value;
	struct cgl_treemap_entry_t* left;
	struct cgl_treemap_entry_t* right;
	struct cgl_treemap_entry_t* parent;
	unsigned char color;
};

struct cgl_treemap_t {
	int (*compareFunc)(void*, void*);
	void (*freeKeyFunc)(void*);
	void (*freeValueFunc)(void*);
	struct cgl_treemap_entry_t* root;
	int size;
};

#define cgl_treemap_entry_red  0
#define cgl_treemap_entry_black 1
#define cgl_treemap_entry_set_color(p,c) if (p) { p->color = c; }
#define cgl_treemap_entry_color(p) (!p ? cgl_treemap_entry_black: p->color)
#define cgl_treemap_entry_right(p) (!p ? 0: p->right)
#define cgl_treemap_entry_left(p) (!p ? 0: p->left)
#define cgl_treemap_entry_parent(p) (!p ? 0: p->parent)

struct cgl_treemap_t* cgl_treemap_new(int (*compareFunc)(void*, void*), void (*freeKeyFunc)(void*), void (*freeValueFunc)(void*)) {
	struct cgl_treemap_t* data = (struct cgl_treemap_t*) calloc(1, sizeof(struct cgl_treemap_t));
	if (!data) {
		return 0;
	}
	data->compareFunc = compareFunc;
	data->freeKeyFunc = freeKeyFunc;
	data->freeValueFunc = freeValueFunc;
	return data;
}

void cgl_treemap_free(struct cgl_treemap_t* data) {
	cgl_treemap_clear(data);
	free(data);
}

static int cgl_treemap_int_compare(void* o1, void* o2) {
	return o1 - o2;
}

struct cgl_treemap_t* cgl_treemap_new_int(void (*freeValueFunc)(void*)) {
	return cgl_treemap_new(cgl_treemap_int_compare, 0, freeValueFunc);
}

static int cgl_treemap_vchar_compare(void* o1, void* o2) {
	return strcmp(o1, o2);
}

struct cgl_treemap_t* cgl_treemap_new_vchar(unsigned char freeKey, void (*freeValueFunc)(void*)) {
	return cgl_treemap_new(cgl_treemap_vchar_compare, freeKey ? free : 0, freeValueFunc);
}

static void cgl_treemap_rotate_left(struct cgl_treemap_t* data, struct cgl_treemap_entry_t* p) {
	if (p) {
		struct cgl_treemap_entry_t* r = p->right;
		p->right = r->left;
		if (r->left) {
			r->left->parent = p;
		}
		r->parent = p->parent;
		if (!p->parent) {
			data->root = r;
		} else if (p->parent->left == p) {
			p->parent->left = r;
		} else {
			p->parent->right = r;
		}
		r->left = p;
		p->parent = r;
	}
}

static void cgl_treemap_rotate_right(struct cgl_treemap_t* data, struct cgl_treemap_entry_t* p) {
	if (p) {
		struct cgl_treemap_entry_t* l = p->left;
		p->left = l->right;
		if (l->right) {
			l->right->parent = p;
		}
		l->parent = p->parent;
		if (!p->parent) {
			data->root = l;
		} else if (p->parent->right == p) {
			p->parent->right = l;
		} else {
			p->parent->left = l;
		}
		l->right = p;
		p->parent = l;
	}
}

static void cgl_treemap_fix_after_insertion(struct cgl_treemap_t* data, struct cgl_treemap_entry_t* x) {
	x->color = cgl_treemap_entry_red;
	while (x && x != data->root && x->parent->color == cgl_treemap_entry_red) {
		if (cgl_treemap_entry_parent(x) == cgl_treemap_entry_left(cgl_treemap_entry_parent(cgl_treemap_entry_parent(x)))) {
			struct cgl_treemap_entry_t* y = cgl_treemap_entry_right(cgl_treemap_entry_parent(cgl_treemap_entry_parent(x)));
			if (cgl_treemap_entry_color(y) == cgl_treemap_entry_red) {
				cgl_treemap_entry_set_color(cgl_treemap_entry_parent(x), cgl_treemap_entry_black);
				cgl_treemap_entry_set_color(y, cgl_treemap_entry_black);
				cgl_treemap_entry_set_color( cgl_treemap_entry_parent(cgl_treemap_entry_parent(x)), cgl_treemap_entry_red);
				x = cgl_treemap_entry_parent(cgl_treemap_entry_parent(x));
			} else {
				if (x == cgl_treemap_entry_right(cgl_treemap_entry_parent(x))) {
					x = cgl_treemap_entry_parent(x);
					cgl_treemap_rotate_left(data, x);
				}
				cgl_treemap_entry_set_color(cgl_treemap_entry_parent(x), cgl_treemap_entry_black);
				cgl_treemap_entry_set_color( cgl_treemap_entry_parent(cgl_treemap_entry_parent(x)), cgl_treemap_entry_red);
				cgl_treemap_rotate_right(data, cgl_treemap_entry_parent(cgl_treemap_entry_parent(x)));
			}
		} else {
			struct cgl_treemap_entry_t* y = cgl_treemap_entry_left(cgl_treemap_entry_parent(cgl_treemap_entry_parent(x)));
			if (cgl_treemap_entry_color(y) == cgl_treemap_entry_red) {
				cgl_treemap_entry_set_color(cgl_treemap_entry_parent(x), cgl_treemap_entry_black);
				cgl_treemap_entry_set_color(y, cgl_treemap_entry_black);
				cgl_treemap_entry_set_color( cgl_treemap_entry_parent(cgl_treemap_entry_parent(x)), cgl_treemap_entry_red);
				x = cgl_treemap_entry_parent(cgl_treemap_entry_parent(x));
			} else {
				if (x == cgl_treemap_entry_left(cgl_treemap_entry_parent(x))) {
					x = cgl_treemap_entry_parent(x);
					cgl_treemap_rotate_right(data, x);
				}
				cgl_treemap_entry_set_color(cgl_treemap_entry_parent(x), cgl_treemap_entry_black);
				cgl_treemap_entry_set_color( cgl_treemap_entry_parent(cgl_treemap_entry_parent(x)), cgl_treemap_entry_red);
				cgl_treemap_rotate_left(data, cgl_treemap_entry_parent(cgl_treemap_entry_parent(x)));
			}
		}
	}
	data->root->color = cgl_treemap_entry_black;
}

cgl_treemap_error_t cgl_treemap_put(struct cgl_treemap_t* data, void* key, void* value) {
	struct cgl_treemap_entry_t* aux = data->root;
	if (!aux) {
		struct cgl_treemap_entry_t* entry = (struct cgl_treemap_entry_t*) calloc(1, sizeof(struct cgl_treemap_entry_t));
		if (!entry) {
			return 1;
		}
		entry->key = key;
		entry->value = value;
		data->root = entry;
		data->size = 1;
		return 0;
	}
	int cmp;
	struct cgl_treemap_entry_t* parent;
	do {
		parent = aux;
		cmp = data->compareFunc(key, aux->key);
		if (cmp < 0) {
			aux = aux->left;
		} else if (cmp > 0) {
			aux = aux->right;
		} else {
			void* result = aux->value;
			aux->value = value;
			if (data->freeValueFunc) {
				data->freeValueFunc(result);
			}
			return 0;
		}
	} while (aux);
	struct cgl_treemap_entry_t* e = (struct cgl_treemap_entry_t*) calloc(1, sizeof(struct cgl_treemap_entry_t));
	if (!e) {
		return 1;
	}
	e->key = key;
	e->value = value;
	e->parent = parent;
	if (cmp < 0) {
		parent->left = e;
	} else {
		parent->right = e;
	}
	cgl_treemap_fix_after_insertion(data, e);
	data->size++;
	return 0;
}

cgl_treemap_error_t cgl_treemap_put_int(struct cgl_treemap_t* data, int key, void* value) {
	void* p = 0;
	p += key;
	return cgl_treemap_put(data, p, value);
}

static struct cgl_treemap_entry_t* cgl_treemap_get_entry(struct cgl_treemap_t* data, void* key) {
	struct cgl_treemap_entry_t* p = data->root;
	while (p) {
		int cmp = data->compareFunc(key, p->key);
		if (cmp < 0) {
			p = p->left;
		} else if (cmp > 0) {
			p = p->right;
		} else {
			return p;
		}
	}
	return 0;
}

void* cgl_treemap_get(struct cgl_treemap_t* data, void* key) {
	struct cgl_treemap_entry_t* p = cgl_treemap_get_entry(data, key);
	return !p ? 0 : p->value;
}

void* cgl_treemap_get_int(struct cgl_treemap_t* data, int key) {
	void* p = 0;
	p += key;
	return cgl_treemap_get(data, p);
}

int cgl_treemap_size(struct cgl_treemap_t* data) {
	return data->size;
}

static struct cgl_treemap_entry_t* successor(struct cgl_treemap_entry_t* t) {
	if (!t) {
		return 0;
	} else if (t->right) {
		struct cgl_treemap_entry_t* p = t->right;
		while (p->left)
			p = p->left;
		return p;
	} else {
		struct cgl_treemap_entry_t* p = t->parent;
		struct cgl_treemap_entry_t* ch = t;
		while (p && ch == p->right) {
			ch = p;
			p = p->parent;
		}
		return p;
	}
}

void fixAfterDeletion(struct cgl_treemap_t* data, struct cgl_treemap_entry_t* x) {
	while (x != data->root && cgl_treemap_entry_color(x) == cgl_treemap_entry_black) {
		if (x == cgl_treemap_entry_left(cgl_treemap_entry_parent(x))) {
			struct cgl_treemap_entry_t* sib = cgl_treemap_entry_right(cgl_treemap_entry_parent(x));
			if (cgl_treemap_entry_color(sib) == cgl_treemap_entry_red) {
				cgl_treemap_entry_set_color(sib, cgl_treemap_entry_black);
				cgl_treemap_entry_set_color(cgl_treemap_entry_parent(x), cgl_treemap_entry_red);
				cgl_treemap_rotate_left(data, cgl_treemap_entry_parent(x));
				sib = cgl_treemap_entry_right(cgl_treemap_entry_parent(x));
			}
			if (cgl_treemap_entry_color(cgl_treemap_entry_left(sib)) == cgl_treemap_entry_black && cgl_treemap_entry_color(cgl_treemap_entry_right(sib)) == cgl_treemap_entry_black) {
				cgl_treemap_entry_set_color(sib, cgl_treemap_entry_red);
				x = cgl_treemap_entry_parent(x);
			} else {
				if (cgl_treemap_entry_color(cgl_treemap_entry_right(sib)) == cgl_treemap_entry_black) {
					cgl_treemap_entry_set_color(cgl_treemap_entry_left(sib), cgl_treemap_entry_black);
					cgl_treemap_entry_set_color(sib, cgl_treemap_entry_red);
					cgl_treemap_rotate_right(data, sib);
					sib = cgl_treemap_entry_right(cgl_treemap_entry_parent(x));
				}
				cgl_treemap_entry_set_color(sib, cgl_treemap_entry_color(cgl_treemap_entry_parent(x)));
				cgl_treemap_entry_set_color(cgl_treemap_entry_parent(x), cgl_treemap_entry_black);
				cgl_treemap_entry_set_color(cgl_treemap_entry_right(sib), cgl_treemap_entry_black);
				cgl_treemap_rotate_left(data, cgl_treemap_entry_parent(x));
				x = data->root;
			}
		} else { // symmetric
			struct cgl_treemap_entry_t* sib = cgl_treemap_entry_left(cgl_treemap_entry_parent(x));
			if (cgl_treemap_entry_color(sib) == cgl_treemap_entry_red) {
				cgl_treemap_entry_set_color(sib, cgl_treemap_entry_black);
				cgl_treemap_entry_set_color(cgl_treemap_entry_parent(x), cgl_treemap_entry_red);
				cgl_treemap_rotate_right(data, cgl_treemap_entry_parent(x));
				sib = cgl_treemap_entry_left(cgl_treemap_entry_parent(x));
			}
			if (cgl_treemap_entry_color(cgl_treemap_entry_right(sib)) == cgl_treemap_entry_black && cgl_treemap_entry_color(cgl_treemap_entry_left(sib)) == cgl_treemap_entry_black) {
				cgl_treemap_entry_set_color(sib, cgl_treemap_entry_red);
				x = cgl_treemap_entry_parent(x);
			} else {
				if (cgl_treemap_entry_color(cgl_treemap_entry_left(sib)) == cgl_treemap_entry_black) {
					cgl_treemap_entry_set_color(cgl_treemap_entry_right(sib), cgl_treemap_entry_black);
					cgl_treemap_entry_set_color(sib, cgl_treemap_entry_red);
					cgl_treemap_rotate_left(data, sib);
					sib = cgl_treemap_entry_left(cgl_treemap_entry_parent(x));
				}
				cgl_treemap_entry_set_color(sib, cgl_treemap_entry_color(cgl_treemap_entry_parent(x)));
				cgl_treemap_entry_set_color(cgl_treemap_entry_parent(x), cgl_treemap_entry_black);
				cgl_treemap_entry_set_color(cgl_treemap_entry_left(sib), cgl_treemap_entry_black);
				cgl_treemap_rotate_right(data, cgl_treemap_entry_parent(x));
				x = data->root;
			}
		}
	}

	cgl_treemap_entry_set_color(x, cgl_treemap_entry_black);
}

static void deleteEntry(struct cgl_treemap_t* data, struct cgl_treemap_entry_t* p) {
	data->size--;
	if (p->left && p->right) {
		struct cgl_treemap_entry_t* s = successor(p);
		p->key = s->key;
		p->value = s->value;
		p = s;
	}
	struct cgl_treemap_entry_t* replacement = (p->left ? p->left : p->right);
	if (replacement) {
		replacement->parent = p->parent;
		if (!p->parent) {
			data->root = replacement;
		} else if (p == p->parent->left) {
			p->parent->left = replacement;
		} else {
			p->parent->right = replacement;
		}
		p->left = p->right = p->parent = 0;
		if (p->color == cgl_treemap_entry_black) {
			cgl_treemap_fix_after_insertion(data, replacement);
		}
	} else if (!p->parent) {
		data->root = 0;
	} else {
		if (p->color == cgl_treemap_entry_black) {
			cgl_treemap_fix_after_insertion(data, p);
		}
		if (p->parent) {
			if (p == p->parent->left) {
				p->parent->left = 0;
			} else if (p == p->parent->right) {
				p->parent->right = 0;
			}
			p->parent = 0;
		}
	}
}

void cgl_treemap_remove(struct cgl_treemap_t* data, void* key) {
	struct cgl_treemap_entry_t* p = cgl_treemap_get_entry(data, key);
	if (p) {
		deleteEntry(data, p);
		if (data->freeKeyFunc) {
			data->freeKeyFunc(p->key);
		}
		if (data->freeValueFunc) {
			data->freeValueFunc(p->value);
		}
		free(p);
	}
}

void cgl_treemap_remove_int(struct cgl_treemap_t* data, int key) {
	void* p = 0;
	p += key;
	cgl_treemap_remove(data, p);
}

static void cgl_treemap_remove_entry(struct cgl_treemap_t* data, struct cgl_treemap_entry_t* e) {
	if (data->freeKeyFunc) {
		data->freeKeyFunc(e->key);
	}
	if (data->freeValueFunc) {
		data->freeValueFunc(e->value);
	}
	if (e->left) {
		cgl_treemap_remove_entry(data, e->left);
	}
	if (e->right) {
		cgl_treemap_remove_entry(data, e->right);
	}
	free(e);
}

void cgl_treemap_clear(struct cgl_treemap_t* data) {
	if (data->root) {
		cgl_treemap_remove_entry(data, data->root);
	}
	data->root = 0;
	data->size = 0;
}
