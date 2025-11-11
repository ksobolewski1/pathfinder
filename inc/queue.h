#pragma once


struct node;


struct binary_heap {
    int N;
    struct node** data; 
    int (*rating)(struct node* n1, struct node* n2);
};


void enqueue(struct binary_heap* q, struct node* n);

struct node* dequeue(struct binary_heap* q);

static inline int qsize(struct binary_heap* q) {
    return q->N - 1;
}
