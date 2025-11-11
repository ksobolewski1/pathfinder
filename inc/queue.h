#pragma once

struct node_data {

    int id; 
    float hcost;
    float gcost;
    int parent;
    int index;

};

struct binary_heap {
    int N;
    struct node_data* data; 
    int (*rating)(struct node_data n1, struct node_data n2);
};


void enqueue(struct binary_heap* q, struct node_data n);

struct node_data dequeue(struct binary_heap* q);

static inline int qsize(struct binary_heap* q) {
    return q->N - 1;
}
