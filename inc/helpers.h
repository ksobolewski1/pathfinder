
#pragma once 

struct Point {
    float x;
    float y;
    int id;
};


struct Edge {
    int start;
    int end;
    int unique;
};


struct Circumcircle {
    float radius;
    float centre_x;
    float centre_y;
};


struct Triangle {
    struct Point vertices[3];
    struct Edge edges[3];
    struct Circumcircle circumcircle;
};


static inline int equal_edge(struct Edge one, struct Edge two) {
    if ((one.start == two.end || one.start == two.start)
	&& (one.end == two.start || one.end == two.end)) return 1;
    else return 0;
}


int circumcircle_contains(struct Point pt, struct Circumcircle circc);


struct Circumcircle get_circumcircle(struct Triangle triangle);


struct Triangle get_super_triangle(int pt_c, int sw, int sh);


void remove_triangle(int arr_size, struct Triangle* t_arr, int at_index);


struct Triangle* resize_tr_arr(int arr_size, struct Triangle* t_arr);
