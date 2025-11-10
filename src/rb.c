#include "rb.h"

#include "core.h"

#include <stdlib.h>
#include <time.h>

#include <stdio.h>


int* visited;
struct node** stack;


struct node** get_neighbours(int id, struct node* nodes, int* count, int row_count, int col_count) {

    struct node** neighbours = (struct node**)malloc(sizeof(struct node*) * 5);
    int neighbours_count = 0;

    int row = id / col_count;
    int col = id % col_count;
    
    int neighbour_ids[4] = {-1, -1, -1, -1};

    if(row > 0) neighbour_ids[0] = (row - 1) * col_count + col;
    if(row < row_count - 1) neighbour_ids[1] = (row + 1) * col_count + col;
    if(col > 0) neighbour_ids[2] = row * col_count + (col - 1);
    if(col < col_count - 1) neighbour_ids[3] = row * col_count + (col + 1);

    for (int i = 0; i < 4; i++) {
        if (neighbour_ids[i] < 0) continue;
        if (visited[neighbour_ids[i]] > -1) continue;
        neighbours[neighbours_count++] = &nodes[neighbour_ids[i]];
    } 

    *count = neighbours_count;
    return neighbours;
}   


struct node* get_random_neighbour(int id, struct node* nodes, int row_count, int col_count) {

    int neighbouring_count = 0;
    struct node** neighbouring = get_neighbours(id, nodes, &neighbouring_count, row_count, col_count);

    if (neighbouring_count == 0) return NULL;
    if (neighbouring_count == 1) {
        struct node* res = neighbouring[0];
        free(neighbouring);
        return res;
    }

    srand(time(NULL));
    struct node* res = neighbouring[rand() % (neighbouring_count - 1)];
    free(neighbouring);
    return res;
}


void visit(int stack_pointer, struct node* nodes, int row_count, int col_count) {

    if (stack_pointer < 0) return;

    struct node* n = stack[stack_pointer];

    int id = n->id;
    visited[id] = id;

    struct node* next;
    while ((next = get_random_neighbour(id, nodes, row_count, col_count)) != NULL) {
        stack[++stack_pointer] = next;
        visit(stack_pointer, nodes, row_count, col_count);
        n->edges[n->edge_count++] = next->id;
    }

    visit(--stack_pointer, nodes, row_count, col_count);
    
}


struct mesh* recursive_backtrack(int screen_w, int screen_h, float node_size, int* start, int* end) {

    int col_count = (int)(screen_w / node_size);
    int row_count = (int)(screen_h / node_size);
    int node_count = col_count * row_count;
    struct node* nodes = (struct node*)malloc(sizeof(struct node) * (node_count + 1));
    visited = (int*)malloc(sizeof(int) * (node_count + 1));
    stack = (struct node**)malloc(sizeof(struct node*) * (node_count + 1));

    int index = 0;
    for (int i = 0; i < row_count; i++) {

        float pos_y = (float)(i * node_size);

        for (int j = 0; j < col_count; j++) {
            
            struct node n;

            n.id = index;
            n.x = (float)(j * node_size);
            n.y = pos_y;
            n.edge_count = 0;
            n.edges = (int*)malloc(sizeof(int) * 5);
            visited[index] = -1;
            nodes[index++] = n;

        }
    }

    // pick at random from edge nodes
    int s = 0;
    stack[0] = &nodes[s];
    visit(0, nodes, row_count, col_count);
    // pick end opposite to start

    free(visited);
    free(stack);

    *start = s;
    *end = 3;

    struct mesh* m = (struct mesh*)malloc(sizeof(struct mesh));
    m->nodes = nodes;
    m->node_count = node_count;

    return m;
    
}