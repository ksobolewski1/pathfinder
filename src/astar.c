#include "astar.h"

#include "core.h"
#include "queue.h"

#include <math.h>
#include <stdlib.h>


int cost(struct node_data n1, struct node_data n2) {

    // return get_hcost(n1, end) + n1->gcost < get_hcost(n2, end) + n2->gcost

    return 0;
}


float get_hcost(struct node* n, struct node* end) {
    float delta_x = n->x - end->x;
    float delta_y = n->y - end->y;
    return (float)sqrt(delta_x * delta_x + delta_y * delta_y);
}


int* find_path(struct mesh* m, int start, int end, int* path_len) {

    struct binary_heap queue;
    queue.N = 0;
    queue.data = (struct node**)malloc(sizeof(struct node*) * m->node_count);
    queue.rating = cost;
    
    int* enqueued = (int*)malloc(sizeof(int) * m->node_count);
    int* visited = (int*)malloc(sizeof(int) * m->node_count);
    for (int i = 0; i < m->node_count; i++) {
        enqueued[i] = -1;
        visited[i] = -1;
    }

    int found = 0;
    float gcost = 0.0f;

    struct node_data start_data;
    start_data.id = m->nodes[start].id;
    start_data.hcost = get_hcost(&m->nodes[start], &m->nodes[end]);
    start_data.gcost = 0.0f;
    start_data.parent = -1;
    start_data.index = 0;

    enqueue(&queue, start_data);
    enqueued[start_data.id] = start_data.id;

    struct node_data current;
    while(qsize(&queue) > 0) {

        current = dequeue(&queue);
        struct node* current_node = &m->nodes[current.id];
        visited[current.id] = current.id;

        if (current.id == end) {
            found = 1;
            break;
        }

        for (int i = 0; i < current_node->edge_count; i++) {

            int neighbour_id = current_node->edges[i];

            if (visited[neighbour_id] > -1) continue;

            struct node* next = &m->nodes[neighbour_id];

            if (enqueued[neighbour_id] == -1) {

                // set current as next parent 
                struct node_data nd; 
                nd.parent = current.id;
                nd.id = neighbour_id;
                nd.hcost = get_hcost(next, &m->nodes[end]);
                nd.gcost = gcost + get_hcost(current_node, next);
                nd.index = current.index + 1;

                enqueue(&queue, nd);
                enqueued[neighbour_id] = neighbour_id;
            }
            else { // a shorter path has been found to/through an already enqueued node

                //if (current.gcost + get_hcost(current_node, next) < [the g cost of the enqueued neighbour - requires to search for it in the queue]) {
                // enqueued[neighbour_id] gcost = current g cost + distance between current and enqueued[neighbour_id]
                // enqueued[neighbour_id] becomes parent of current
                // }
                //else continue 

            }

        }
    }

    int* res = NULL;
    if (found) {
        
        *path_len = current.index;
        // count in the path
        res = (int*)malloc(sizeof(int) * current.index);
        
        for (int i = 0; i < current.index; i++) {

        }
        // reconstruct the path
    }


    free(queue.data);
    free(enqueued);
    free(visited);

    return res;
}