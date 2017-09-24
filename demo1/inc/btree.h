#ifndef BTREE_H
#define BTREE_H

#define long_btree_order 3

struct long_btree_node_t {
	struct long_btree_node_t* parent;
	int panret_child_index;
	unsigned long id;
	unsigned long children[long_btree_order];
	long keys[long_btree_order];
	unsigned char* values[long_btree_order];
	unsigned int size;
	unsigned char leaf;
	unsigned char changed;
};

struct long_btree_t {
	unsigned long size;
	unsigned char changed;
	unsigned long sequence;
	struct long_btree_node_t* root;
	struct long_btree_node_t* (*readNodeFunc)(unsigned long id);
	void (*writeNodeFunc)(struct long_btree_node_t* node);
	void (*valueFreeFunc)(unsigned char*);
};

struct long_btree_t* long_btree_new(struct long_btree_node_t* (*readNodeFunc)(unsigned long id), void (*writeNodeFunc)(struct long_btree_node_t* node), void (*freeValue)(unsigned char*));

void long_btree_free(struct long_btree_t* self);

unsigned char long_btree_add(struct long_btree_t* self, long key, unsigned char* bytes);

unsigned char* long_btree_get(struct long_btree_t* self, long key);

unsigned char* long_btree_remove(struct long_btree_t* self, long key);

unsigned long long_btree_size(struct long_btree_t* self);

void long_btree_clear(struct long_btree_t* self);

void long_btree_test();

#endif
