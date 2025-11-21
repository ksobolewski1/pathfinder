#include "astar.h"

#include "core.h"
#include "queue.h"

#include <math.h>
#include <stdlib.h>

#include <stdio.h>


struct node_data* get_node_data(int i, float hc, float gc, struct node* n, struct node_data* p) {

    struct node_data* res = (struct node_data*)malloc(sizeof(struct node_data));
    res->index = i;
    res->hcost = hc;
    res->gcost = gc;
    res->n = n;
    res->parent = p;

    return res;
}


int get_node_data_id(struct node_data* nd) {
    return nd->n->id;
}


int cost(void* d1, void* d2) {

    struct node_data* nd1 = (struct node_data*)d1;
    struct node_data* nd2 = (struct node_data*)d2;
    return nd1->hcost + nd1->gcost < nd2->hcost + nd2->gcost;

}


float get_hcost(struct node* n, struct node* end) {
    float delta_x = n->x - end->x;
    float delta_y = n->y - end->y;
    return (float)sqrt(delta_x * delta_x + delta_y * delta_y);
}


int* find_path(struct mesh* m, int start, int end, int* path_len) {

    // struct binary_heap queue;
    // queue.N = 1;
    // queue.data = (void**)malloc(sizeof(struct node_data*) * m->node_count);
    // queue.rating = cost;

    int qptr = 0;
    struct node_data** queue = (struct node_data**)malloc(sizeof(struct node_data*) * m->node_count);
    
    int* enqueued = (int*)malloc(sizeof(int) * m->node_count);
    int* visited = (int*)malloc(sizeof(int) * m->node_count);
    for (int i = 0; i < m->node_count; i++) {
        enqueued[i] = -1;
        visited[i] = -1;
    }

    struct node_data* start_data = 
    get_node_data(
        0, 
        get_hcost(&m->nodes[start], &m->nodes[end]), 
        0.0f, 
        &m->nodes[start], 
        NULL
    );

    queue[qptr++] = start_data;
    //enqueue(&queue, start_data);
    enqueued[get_node_data_id(start_data)] = get_node_data_id(start_data);

    int found = 0;
    struct node_data* current_node_data;
    int current_node_data_id;

    while(qptr > 0) {
        
        current_node_data = queue[--qptr];
        //current_node_data = (struct node_data*)dequeue(&queue);
        current_node_data_id = get_node_data_id(current_node_data);

        if (current_node_data_id == end) {
            found = 1;
            break;
        }

        visited[current_node_data_id] = current_node_data_id;

        for (int i = 0; i < current_node_data->n->edge_count; i++) {

            int neighbour_id = current_node_data->n->edges[i];

            if (visited[neighbour_id] > -1) continue;

            if (enqueued[neighbour_id] == -1) {

                struct node_data* next = 
                get_node_data(
                    current_node_data->index + 1,
                    get_hcost(&m->nodes[neighbour_id], &m->nodes[end]),
                    current_node_data->gcost + get_hcost(current_node_data->n, &m->nodes[neighbour_id]),
                    &m->nodes[neighbour_id],
                    current_node_data
                );

                queue[qptr++] = next;
                //enqueue(&queue, next);
                enqueued[neighbour_id] = neighbour_id;
            }
            else { 

                struct node_data* enqueued_neighbour;
                for (int j = 0; j < qptr; j++) {
                    if (get_node_data_id(queue[j]) == neighbour_id) {
                        enqueued_neighbour = queue[j];
                        break;
                    }
                }

                if (current_node_data->gcost + get_hcost(current_node_data->n, &m->nodes[neighbour_id]) < enqueued_neighbour->gcost) { 
                    enqueued_neighbour->gcost = current_node_data->gcost + get_hcost(current_node_data->n, enqueued_neighbour->n);
                    current_node_data->parent = enqueued_neighbour;
                }
                else continue;

            }

        }
    }

    int* res = NULL;
    if (found) {
        
        printf("PATH FOUND\n");

        *path_len = current_node_data->index;
        res = (int*)malloc(sizeof(int) * current_node_data->index);
        
        while (current_node_data->parent != NULL) {

            res[current_node_data->index - 1] = current_node_data->n->id;
            current_node_data = current_node_data->parent;
        }
    }
    else {
        *path_len = 0;
    }

    free(queue);
    //free(queue.data);
    free(enqueued);
    free(visited);

    return res;
}