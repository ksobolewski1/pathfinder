#pragma once

struct binary_heap {
    int N;
    int data[256]; 
};

struct binary_heap* get_heap(); 

void enqueue(struct binary_heap* q, const int data);

int dequeue(struct binary_heap* q);

static inline int qsize(struct binary_heap* q) {
    return q->N - 1;
}
