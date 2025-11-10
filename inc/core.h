
#pragma once 


struct node {

    float x;
    float y;
    int id;

    int edge_count;
    int* edges;
};


struct mesh {

    struct node* nodes;
    int node_count;
};





