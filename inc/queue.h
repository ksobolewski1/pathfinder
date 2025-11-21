#pragma once



struct binary_heap {
    int N;
    void** data; 
    int (*rating)(void* d1, void*d2);
};


void enqueue(struct binary_heap* q, void* d);

void* dequeue(struct binary_heap* q);

static inline int qsize(struct binary_heap* q) {
    return q->N - 1;
}
