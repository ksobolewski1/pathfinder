#pragma once

struct mesh;
struct node;

struct node_data {

    int index;
    float hcost;
    float gcost;
    struct node* n;
    struct node_data* parent;

};

int* find_path(struct mesh* m, int start, int end, int* path_len);