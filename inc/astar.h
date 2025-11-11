#pragma once

struct mesh;

struct node_data {

    int id; 
    float hcost;
    float gcost;

};

int* find_path(struct mesh* m, int start, int end, int* path_len);