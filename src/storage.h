#include <pebble.h>

typedef struct {
    char selectedVersion[64];
    int bottomspace;
} storage_t;
extern storage_t storage;

void storage_init(void);
void storage_persist(void);
void storage_deinit(void);
