
#include "bw.h"
#include "core.h"

#include <stdlib.h>
#include <math.h>


struct circumcircle {
    float radius;
    float centre_x;
    float centre_y;
};

struct edge {
    int start;
    int end;
    int unique;
};

struct triangle {
    struct node vertices[3];
    struct edge edges[3];
    struct circumcircle circumcircle;
};


int equal_edge(struct edge one, struct edge two) {
    return (one.start == two.end || one.start == two.start) && (one.end == two.start || one.end == two.end);
}


int circumcircle_contains(struct node pt, struct circumcircle circc) {

	struct node to_centre;
	to_centre.x = pt.x - circc.centre_x;
	to_centre.y = pt.y - circc.centre_y;

	float len = (float)sqrt(to_centre.x * to_centre.x + to_centre.y * to_centre.y);

	if (len < circc.radius) return 1;
	else return 0;
}


struct circumcircle get_circumcircle(struct triangle triangle) {

	struct circumcircle res;

	struct node A = triangle.vertices[0];
	struct node B = triangle.vertices[1];
	struct node C = triangle.vertices[2];

	float det = (A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y)) * 2;

	res.centre_x = (1 / det) *
	    ((A.x * A.x + A.y * A.y) * (B.y - C.y)
	     + (B.x * B.x + B.y * B.y) * (C.y - A.y)
	     + (C.x * C.x + C.y * C.y) * (A.y - B.y));
	res.centre_y = (1 / det) *
	    ((A.x * A.x + A.y * A.y) * (C.x - B.x)
	     + (B.x * B.x + B.y * B.y) * (A.x - C.x)
	     + (C.x * C.x + C.y * C.y) * (B.x - A.x));
	
	struct node diff;
	diff.x = A.x - res.centre_x;
	diff.y = A.y - res.centre_y;
	res.radius = sqrt(diff.x * diff.x + diff.y * diff.y);

	return res;
}


struct triangle get_super_triangle(int pt_c, int sw, int sh) {

    // a circle that encompasses the area on which the mesh is to be defined 
	struct node excircle_centre = {sw / 2.0f, sh / 2.0f};
	float excircle_rad = (float)sqrt(-excircle_centre.x * -excircle_centre.x
					 + -excircle_centre.y * -excircle_centre.y);

	struct triangle triangle;
	float side_len = 2 * excircle_rad * (float)sqrt(3);

	struct node top_pt = {
	    excircle_centre.x,
	    excircle_centre.y - (excircle_rad + side_len / (float)sqrt(3))
	};
	
	top_pt.id = pt_c;
	
	triangle.vertices[0] = top_pt;

	struct node left_pt = { excircle_centre.x - side_len / 2.0f, excircle_centre.y + excircle_rad };
	left_pt.id = pt_c + 1;
	triangle.vertices[1] = left_pt;

	struct node right_pt = { excircle_centre.x + side_len / 2.0f, excircle_centre.y + excircle_rad };
	right_pt.id = pt_c + 2;
	triangle.vertices[2] = right_pt;

	struct edge one = { triangle.vertices[0].id, triangle.vertices[1].id, 0 };
	triangle.edges[0] = one;
	struct edge two = { triangle.vertices[1].id, triangle.vertices[2].id, 0 };
	triangle.edges[1] = two;
	struct edge three = { triangle.vertices[2].id, triangle.vertices[0].id, 0 };
	triangle.edges[2] = three;

	triangle.circumcircle = get_circumcircle(triangle);

	return triangle;
}


void remove_triangle(int arr_size, struct triangle* t_arr, int at_index) {
	for (int i = at_index; i < arr_size - 1; ++i) t_arr[i] = t_arr[i + 1];
}


struct triangle* resize_tr_arr(int arr_size, struct triangle* t_arr) {

	int n_size = arr_size << 1;
	struct triangle* n_arr = (struct triangle*)malloc(sizeof(struct triangle) * n_size);

	if (n_arr == NULL) {
		return NULL;
	}

	for (int i = 0; i < arr_size; ++i) n_arr[i] = t_arr[i];

	free(t_arr);

	return n_arr;
}


// Triangulation algorithm 
struct triangle* triangulate(int pt_count, int screen_width, int screen_height, struct node* nodes, int* res_count) {

	if (pt_count < 3) return NULL; 

	int tr_arr_size = pt_count * 3;
	struct triangle* triangles = (struct triangle*)malloc(sizeof(struct triangle) * tr_arr_size);
	
	if (triangles == NULL) return NULL;

	triangles[0] = get_super_triangle(pt_count, screen_width, screen_height);
	for (int i = 0; i < 3; i++) nodes[pt_count + i] = triangles[0].vertices[i];
	int tr_count = 1; 
	
	// populates poly_edges lookup array ahead of Edge-uniqueness check 
	struct edge null_edge = { -1, -1, 1 };
	

	for (int pt_i = 0; pt_i < pt_count; pt_i++) {

	    struct triangle* bad_tr = (struct triangle*)malloc(sizeof(struct triangle) * tr_count);

		if (bad_tr == NULL) {
		    return NULL;
		}

		int bad_tr_count = 0;
		// get all the "bad triangles" to define the polygon hole
		for (int j = 0; j < tr_count; j++) {

			if (circumcircle_contains(nodes[pt_i], triangles[j].circumcircle)) {
				bad_tr[bad_tr_count++] = triangles[j];
				remove_triangle(tr_count, triangles, j);
				--j;
				--tr_count;
			}

		}

		// representing the polygon hole with poly_edges lookup array
		int edges_arr_size = bad_tr_count * 3; 

		struct edge* poly_edges = (struct edge*)malloc(sizeof(struct edge) * edges_arr_size);

		if (poly_edges == NULL) {
			return NULL;
		}

		for (int i = 0; i < edges_arr_size; i++) poly_edges[i] = null_edge; 
		
		// checking if the edges are unique 
		for (int i = 0; i < bad_tr_count; i++) {

			for (int j = 0; j < 3; ++j) {

				// *** Unroll this loop? 

				struct edge next = bad_tr[i].edges[j];
				int lookup_index = (next.start ^ next.end) * 7 % edges_arr_size;
				
				while (1) {
					if (poly_edges[lookup_index].start == -1) {
						poly_edges[lookup_index] = next;
						break;
					}
					if (equal_edge(poly_edges[lookup_index], next)) {
						++poly_edges[lookup_index].unique;
						break; 
					}
					lookup_index = (lookup_index + 1) % edges_arr_size;
				}

			}

		}

		// create new triangles out of the polygon and add them to triangles 
		for (int i = 0; i < edges_arr_size; i++) {

			if (poly_edges[i].unique > 0) continue;

			struct triangle next;

			next.vertices[0] = nodes[pt_i];
			next.vertices[1] = nodes[poly_edges[i].start];
			next.vertices[2] = nodes[poly_edges[i].end];

			struct edge one = {next.vertices[0].id, next.vertices[1].id, 0};
			next.edges[0] = one;
			struct edge two = {next.vertices[1].id, next.vertices[2].id, 0};
			next.edges[1] = two;
			struct edge three = {next.vertices[2].id, next.vertices[0].id, 0};
			next.edges[2] = three;

			next.circumcircle = get_circumcircle(next);
			triangles[tr_count++] = next;

			if (tr_count / (float)tr_arr_size >= 0.9f) { 
				triangles = resize_tr_arr(tr_arr_size, triangles);
				tr_arr_size = tr_arr_size << 1;
			}
		}

		// pre-allocate both bad_tr and poly_edges, and free alongside triangle 
		free(poly_edges);
		free(bad_tr);
	}

	// loop through all triangles and remove any that share vertices with the super triangle 
	for (int i = 0; i < tr_count; i++) {

		if (triangles[i].vertices[0].id >= pt_count || triangles[i].vertices[1].id >= pt_count
		    || triangles[i].vertices[2].id >= pt_count) {
			remove_triangle(tr_count, triangles, i);
			--tr_count; 
			--i; 
		}
	} 


	// RETURN THE NODES INSTEAD (SET THEIR MEMBERS HERE), AND NODE_COUNT

	*res_count = tr_count; 
   
	return triangles; 
}
