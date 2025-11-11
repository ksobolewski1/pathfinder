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

    struct node* res = neighbouring[(int)((double)rand() / (RAND_MAX + 1.0) * 2)];
    free(neighbouring);
    return res;
}


void visit(int stack_pointer, struct node* nodes, int row_count, int col_count) {

    struct node* n = stack[stack_pointer];
    int id = n->id;
    visited[id] = id;

    while (1) {

        struct node* next = get_random_neighbour(id, nodes, row_count, col_count);
        if (!next) break;
        n->edges[n->edge_count++] = next->id;
        stack[stack_pointer + 1] = next;
        visit(stack_pointer + 1, nodes, row_count, col_count);
        next->edges[next->edge_count++] = n->id;
    }
    
}


struct mesh* recursive_backtrack(int screen_w, int screen_h, float node_size, int start) {

    srand(time(NULL));

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
            n.edges = (int*)malloc(sizeof(int) * 4);
            visited[index] = -1;
            nodes[index++] = n;

        }
    }

    stack[0] = &nodes[start];
    visit(0, nodes, row_count, col_count);

    free(visited);
    free(stack);

    struct mesh* m = (struct mesh*)malloc(sizeof(struct mesh));
    m->nodes = nodes;
    m->node_count = node_count;

    return m;
    
}