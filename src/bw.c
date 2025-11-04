
#include <bw.h>
#include <helpers.h>
#include <stdlib.h>

// Triangulation algorithm 
struct Triangle* triangulate(int pt_count, int screen_width, int screen_height, struct Point* points, int* res_count) {

	if (pt_count < 3) return NULL; 

	int tr_arr_size = pt_count * 3;
	struct Triangle* triangles = (struct Triangle*)malloc(sizeof(struct Triangle) * tr_arr_size);
	
	if (triangles == NULL) return NULL;

	triangles[0] = get_super_triangle(pt_count, screen_width, screen_height);
	for (int i = 0; i < 3; i++) points[pt_count + i] = triangles[0].vertices[i];
	int tr_count = 1; 
	
	// populates poly_edges lookup array ahead of Edge-uniqueness check 
	struct Edge null_edge = { -1, -1, 1 };
	

	for (int pt_i = 0; pt_i < pt_count; pt_i++) {

	    struct Triangle* bad_tr = (struct Triangle*)malloc(sizeof(struct Triangle) * tr_count);

		if (bad_tr == NULL) {
		    return NULL;
		}

		int bad_tr_count = 0;
		// get all the "bad triangles" to define the polygon hole
		for (int j = 0; j < tr_count; j++) {

			if (circumcircle_contains(points[pt_i], triangles[j].circumcircle)) {
				bad_tr[bad_tr_count++] = triangles[j];
				remove_triangle(tr_count, triangles, j);
				--j;
				--tr_count;
			}

		}

		// representing the polygon hole with poly_edges lookup array
		int edges_arr_size = bad_tr_count * 3; 

		struct Edge* poly_edges = (struct Edge*)malloc(sizeof(struct Edge) * edges_arr_size);

		if (poly_edges == NULL) {
			return NULL;
		}

		for (int i = 0; i < edges_arr_size; i++) poly_edges[i] = null_edge; 
		
		// checking if the edges are unique 
		for (int i = 0; i < bad_tr_count; i++) {

			for (int j = 0; j < 3; ++j) {

				// *** Unroll this loop? 

				struct Edge next = bad_tr[i].edges[j];
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

			struct Triangle next;

			next.vertices[0] = points[pt_i];
			next.vertices[1] = points[poly_edges[i].start];
			next.vertices[2] = points[poly_edges[i].end];

			struct Edge one = {next.vertices[0].id, next.vertices[1].id, 0};
			next.edges[0] = one;
			struct Edge two = {next.vertices[1].id, next.vertices[2].id, 0};
			next.edges[1] = two;
			struct Edge three = {next.vertices[2].id, next.vertices[0].id, 0};
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

	*res_count = tr_count; 
   
	return triangles; 
}
