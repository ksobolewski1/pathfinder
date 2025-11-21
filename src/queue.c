#include "queue.h"

#include <stdio.h>
#include <stdlib.h> 

#include "core.h"


static inline void swap(void** data, int i, int j) {
    void* temp = data[i];
    data[i] = data[j];
    data[j] = temp; 
}


void sift_up(struct binary_heap* q, const int last, int i) {

    if (i > last || i == 1) return;
	
    int parent_index = i >> 1;
    void* d1 = q->data[parent_index];
    void* d2 = q->data[i]; 
    
    if (q->rating(d2, d1)) swap(q->data, parent_index, i);
    sift_up(q, last, parent_index);    
}

void sift_down(struct binary_heap* q, const int N, int i) {
    
    int prev = i;
    int left_index = i << 1;
    int right_index = (i << 1) + 1; 
    void* cd = q->data[i]; 
    
    if (q->rating(cd, q->data[left_index]))
	prev = left_index;
    if (q->rating(q->data[right_index], q->data[prev]))
	prev = right_index;
    
    if (prev != i && prev <= N) {
        swap(q->data, i, prev);
        sift_down(q, N, prev);
    }
}

void* dequeue(struct binary_heap* q) {
    int last = q->N - 1;
    swap(q->data, 1, last);
    void* d = q->data[last];
    sift_down(q, last, 1);
    q->N--;
    return d;
}

void enqueue(struct binary_heap* q, void* d) {
    
    q->data[q->N] = d;
    sift_up(q, q->N, q->N);
    q->N++;
}


