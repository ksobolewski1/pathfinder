#pragma once 


struct node; 
struct edge;
struct circumcircle;
struct triangle;


int circumcircle_contains(struct node pt, struct circumcircle circc);


struct circumcircle get_circumcircle(struct triangle triangle);


struct triangle get_super_triangle(int pt_c, int sw, int sh);


void remove_triangle(int arr_size, struct triangle* t_arr, int at_index);


struct triangle* resize_tr_arr(int arr_size, struct triangle* t_arr);


struct triangle* triangulate(int pt_count, int sw, int sh, struct node* points, int* res_count);
