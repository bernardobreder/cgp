#include <stdlib.h>
#include "ihashmap.h"

struct ihashmap* cgl_ihashmap_new(void (*freeValue)(void*)) {
    struct ihashmap* self = (struct ihashmap*) calloc(1, sizeof(struct ihashmap));
    if (!self){
        return 0;
    }
    self->freeValue = freeValue;
    return self;
}

void cgl_ihashmap_free(struct ihashmap* self) {
    int a, b, c, d, e;
    void****** array1 = self->array;
    if (array1) {
        for (a = 0 ; a < 256 ; a++) {
            if (*array1) {
                void***** array2 = *array1;
                for (b = 0; b < 256; b++) {
                    if (*array2) {
                        void**** array3 = *array2;
                        for (c = 0; c < 256; c++) {
                            if (*array3) {
                                void*** array4 = *array3;
                                for (d = 0; d < 16; d++) {
                                    if (*array4) {
                                        void** array5 = *array4;
                                        for (e = 0; e < 16; e++) {
                                            void* data = *array5++;
                                            if (data) {
                                                if (self->freeValue) {
                                                    self->freeValue(data);
                                                }
                                            }
                                        }
                                        free(*array4);
                                        *array4 = 0;
                                    }
                                    array4++;
                                }
                                free(*array3);
                                *array3 = 0;
                            }
                            array3++;
                        }
                        free(*array2);
                        *array2 = 0;
                    }
                    array2++;
                }
                free(*array1);
                *array1 = 0;
            }
            array1++;
        }
        free(self->array);
        self->array = 0;
    }
    free(self);
}

unsigned char cgl_ihashmap_add(struct ihashmap* self, int key, void* value) {
    void****** array1 = self->array;
    if (!array1) {
        self->array = array1 = (void******) calloc(256, sizeof(void*));
        if (!self->array) {
            return 1;
        }
    }
    unsigned char index1 = key >> 24;
    void***** array2 = array1[index1];
    if (!array2) {
        array1[index1] = array2 = (void*****) calloc(256, sizeof(void*));
        if (!array2) {
            return 1;
        }
    }
    unsigned char index2 = (key >> 16) & 0xFF;
    void**** array3 = array2[index2];
    if (!array3) {
        array2[index2] = array3 = (void****) calloc(256, sizeof(void*));
        if (!array3) {
            return 1;
        }
    }
    unsigned char index3 = (key >> 8) & 0xFF;
    void*** array4 = array3[index3];
    if (!array4) {
        array3[index3] = array4 = (void***) calloc(256, sizeof(void*));
        if (!array4) {
            return 1;
        }
    }
    unsigned char index4 = (key >> 4) & 0x0F;
    void** array5 = (void**) array4[index4];
    if (!array5) {
        array4[index4] = array5 = (void**) calloc(16, sizeof(void*));
        if (!array5) {
            return 1;
        }
    }
    unsigned char index5 = key & 0x0F;
    void** array6 = array5 + index5;
    if (!*array6) {
        self->size++;
    }
    *array6 = value;
    return 0;
}

void* cgl_ihashmap_get(struct ihashmap* self, int key) {
    void****** array1 = self->array;
    if (!array1) {
        return 0;
    }
    unsigned char index1 = key >> 24;
    void***** array2 = array1[index1];
    if (!array2) {
        return 0;
    }
    unsigned char index2 = (key >> 16) & 0xFF;
    void**** array3 = array2[index2];
    if (!array3) {
        return 0;
    }
    unsigned char index3 = (key >> 8) & 0xFF;
    void*** array4 = array3[index3];
    if (!array4) {
        return 0;
    }
    unsigned char index4 = (key >> 4) & 0x0F;
    void** array5 = (void**) array4[index4];
    if (!array5) {
        return 0;
    }
    unsigned char index5 = key & 0x0F;
    return array5[index5];
}

void* cgl_ihashmap_del(struct ihashmap* self, int key) {
    void****** array1 = self->array;
    if (!array1) {
        return 0;
    }
    unsigned char index1 = key >> 24;
    void***** array2 = array1[index1];
    if (!array2) {
        return 0;
    }
    unsigned char index2 = (key >> 16) & 0xFF;
    void**** array3 = array2[index2];
    if (!array3) {
        return 0;
    }
    unsigned char index3 = (key >> 8) & 0xFF;
    void*** array4 = array3[index3];
    if (!array4) {
        return 0;
    }
    unsigned char index4 = (key >> 4) & 0x0F;
    void** array5 = (void**) array4[index4];
    if (!array5) {
        return 0;
    }
    unsigned char index5 = key & 0x0F;
    void* result = array5[index5];
    array5[index5] = 0;
    return result;
}

unsigned int cgl_ihashmap_size(struct ihashmap* self) {
    return self->size;
}
