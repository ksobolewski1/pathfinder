#include "astar.h"

#include "core.h"
#include "queue.h"

#include <math.h>
#include <stdlib.h>


int cost(struct node* n1, struct node* n2) {

    // return get_hcost(n1, end) + n1->gcost < get_hcost(n2, end) + n2->gcost

    return 0;
}


float get_hcost(struct node* n, struct node* end) {
    float delta_x = n->x - end->x;
    float delta_y = n->y - end->y;
    return (float)sqrt(delta_x * delta_x + delta_y * delta_y);
}


int* find_path(struct mesh* m, int start, int end, int* path_len) {

    int* res = (int*)malloc(sizeof(int) * m->node_count);
    int res_count = 0;
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
    while(qsize(&queue) > 0) {

        struct node* current = dequeue(&queue);
        visited[current->id] = current->id;

        for (int i = 0; i < current->edge_count; i++) {

            int neighbour_id = current->edges[i];

            if (visited[neighbour_id] > -1) continue;

            if (enqueued[neighbour_id] == -1) {
                
                struct node* next = &m->nodes[neighbour_id];

                // add to path 
                // set h cost - distance between next and destination
                // set g cost - total cost so far - (current g cost + distance between current and next)
                // set current as next parent // 
                struct node_data nd; 
                nd.hcost = get_hcost(next, &m->nodes[end]);
                //nd.gcost = 

                enqueue(&queue, next);
                enqueued[neighbour_id] = neighbour_id;
            }
            else { 
                
                // if below, a batter path to this neighbour has been found (so we can shorten the path)

                //if (current g cost + distance < enqueued[neighbour_id] g cost) {
                // enqueued[neighbour_id] gcost = current g cost + distance between current and enqueued[neighbour_id]
                // enqueued[neighbour_id] becomes parent of current
                // }
                //else continue 

            }

        }
    }


    if (found) {

        // reconstruct the path
    }


    free(queue.data);
    free(enqueued);
    free(visited);

    *path_len = res_count;
    return res;
}