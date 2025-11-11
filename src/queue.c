#include "queue.h"

#include <stdio.h>
#include <stdlib.h> 

#include "core.h"


static inline void swap(struct node** data, int i, int j) {
    struct node* temp = data[i];
    data[i] = data[j];
    data[j] = temp; 
}


void sift_up(struct binary_heap* q, const int N, int i) {
    
    if (i > N || i == 1) return;
	
    int parent_index = i >> 1;
    struct node* n1 = q->data[parent_index];
    struct node* n2 = q->data[i]; 
    
    if (q->rating(n2, n1)) swap(q->data, parent_index, i);
    sift_up(q, N, parent_index);    
}

void sift_down(struct binary_heap* q, const int N, int i) {
    
    int prev = i;
    int left_index = i << 1;
    int right_index = (i << 1) + 1; 
    struct node* current_node = q->data[i]; 
    
    if (q->rating(current_node, q->data[left_index]))
	prev = left_index;
    if (q->rating(q->data[right_index], q->data[prev]))
	prev = right_index;
    
    if (prev != i && prev <= N) {
	swap(q->data, i, prev);
	sift_down(q, N, prev);
    }
}

struct node* dequeue(struct binary_heap* q) {
    int last = q->N - 1;
    q->N--;
    swap(q->data, 1, last);
    struct node* n = q->data[last];
    sift_down(q, q->N, 1);
    return n;
}

void enqueue(struct binary_heap* q, struct node* node) {
    
    int n = q->N;
    q->data[q->N++] = node;
    sift_up(q, n, n);
}


