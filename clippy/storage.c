#include "storage.h"

storage_t *new_storage() {
    storage_t *nouveau = (storage_t *)malloc(sizeof(storage_t));
    if (nouveau == NULL) {
        log_fatal("Unable to malloc storage_t");
        return NULL;
    }

    nouveau->elements = (element_t **)malloc(sizeof(element_t *) * MAX_ELEMENTS);
    for (int i = 0; i < MAX_ELEMENTS; i++) {
        nouveau->elements[i] = (element_t *)malloc(sizeof(element_t));
        nouveau->elements[i]->buf = (char *)calloc(MAX_MESSAGE_SIZE, sizeof(char));
        nouveau->elements[i]->buf[0] = 'E';
        nouveau->elements[i]->buf[1] = '\0';
        nouveau->elements[i]->len = 1;
        nouveau->elements[i]->timestamp = 0;
    }
    return nouveau;
}

void free_storage(storage_t *to_free) {

    for (int i = 0; i < MAX_ELEMENTS; i++) {
        free(to_free->elements[i]->buf);
        free(to_free->elements[i]);
    }
    free(to_free->elements);
    free(to_free);
}

void print_storage() {
    puts("Storage dump:");
    for (int i = 0; i < MAX_ELEMENTS; i++) {
        printf("i:%d len:%zu b:%s time:%ld\n", i, msg_store->elements[i]->len,
               msg_store->elements[i]->buf, msg_store->elements[i]->timestamp);
    }
    fflush(stdout);
}

int put_message(int region, unsigned long timestamp, int hash, size_t len,
                char *buf) {
    if (region > MAX_ELEMENTS - 1) {
        log_error("Naughty tried to put message in excess position");
        return -1;
    }

    pthread_mutex_lock(&m[region]); // start of Critical Section
    if (timestamp > msg_store->elements[region]->timestamp ||
            (timestamp == msg_store->elements[region]->timestamp &&
             hash > msg_store->elements[region]->hash)) {
        msg_store->elements[region]->timestamp = timestamp;
        msg_store->elements[region]->hash = hash;
        strncpy(msg_store->elements[region]->buf, buf, MAX_MESSAGE_SIZE);
        msg_store->elements[region]->len = len;
        log_trace("New Element[%d] Value=\"%s\"", region,
                  msg_store->elements[region]->buf);
    }
    pthread_cond_broadcast(&c[region]);
    pthread_mutex_unlock(&m[region]);

    return 0;
}

element_t *get_message(int region) {
    if (region > MAX_ELEMENTS - 1 || region < 0) {
        log_error("Naughty tried to get message in excess position");
        return NULL;
    }
    return msg_store->elements[region];
}