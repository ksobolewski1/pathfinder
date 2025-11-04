
#include <helpers.h>
#include <stdlib.h>
#include <math.h>


int circumcircle_contains(struct Point pt, struct Circumcircle circc) {

	struct Point to_centre;
	to_centre.x = pt.x - circc.centre_x;
	to_centre.y = pt.y - circc.centre_y;

	float len = (float)sqrt(to_centre.x * to_centre.x + to_centre.y * to_centre.y);

	if (len < circc.radius) return 1;
	else return 0;
}


struct Circumcircle get_circumcircle(struct Triangle triangle) {

	struct Circumcircle res;

	struct Point A = triangle.vertices[0];
	struct Point B = triangle.vertices[1];
	struct Point C = triangle.vertices[2];

	float det = (A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y)) * 2;

	res.centre_x = (1 / det) *
	    ((A.x * A.x + A.y * A.y) * (B.y - C.y)
	     + (B.x * B.x + B.y * B.y) * (C.y - A.y)
	     + (C.x * C.x + C.y * C.y) * (A.y - B.y));
	res.centre_y = (1 / det) *
	    ((A.x * A.x + A.y * A.y) * (C.x - B.x)
	     + (B.x * B.x + B.y * B.y) * (A.x - C.x)
	     + (C.x * C.x + C.y * C.y) * (B.x - A.x));
	
	struct Point diff;
	diff.x = A.x - res.centre_x;
	diff.y = A.y - res.centre_y;
	res.radius = sqrt(diff.x * diff.x + diff.y * diff.y);

	return res;
}


struct Triangle get_super_triangle(int pt_c, int sw, int sh) {

    // a circle that encompasses the area on which the mesh is to be defined 
	struct Point excircle_centre = {sw / 2.0f, sh / 2.0f};
	float excircle_rad = (float)sqrt(-excircle_centre.x * -excircle_centre.x
					 + -excircle_centre.y * -excircle_centre.y);

	struct Triangle triangle;
	float side_len = 2 * excircle_rad * (float)sqrt(3);

	struct Point top_pt = {
	    excircle_centre.x,
	    excircle_centre.y - (excircle_rad + side_len / (float)sqrt(3))
	};
	
	top_pt.id = pt_c;
	
	triangle.vertices[0] = top_pt;

	struct Point left_pt = { excircle_centre.x - side_len / 2.0f, excircle_centre.y + excircle_rad };
	left_pt.id = pt_c + 1;
	triangle.vertices[1] = left_pt;

	struct Point right_pt = { excircle_centre.x + side_len / 2.0f, excircle_centre.y + excircle_rad };
	right_pt.id = pt_c + 2;
	triangle.vertices[2] = right_pt;

	struct Edge one = { triangle.vertices[0].id, triangle.vertices[1].id, 0 };
	triangle.edges[0] = one;
	struct Edge two = { triangle.vertices[1].id, triangle.vertices[2].id, 0 };
	triangle.edges[1] = two;
	struct Edge three = { triangle.vertices[2].id, triangle.vertices[0].id, 0 };
	triangle.edges[2] = three;

	triangle.circumcircle = get_circumcircle(triangle);

	return triangle;
}


void remove_triangle(int arr_size, struct Triangle* t_arr, int at_index) {
	for (int i = at_index; i < arr_size - 1; ++i) t_arr[i] = t_arr[i + 1];
}


struct Triangle* resize_tr_arr(int arr_size, struct Triangle* t_arr) {

	int n_size = arr_size << 1;
	struct Triangle* n_arr = (struct Triangle*)malloc(sizeof(struct Triangle) * n_size);

	if (n_arr == NULL) {
		return NULL;
	}

	for (int i = 0; i < arr_size; ++i) n_arr[i] = t_arr[i];

	free(t_arr);

	return n_arr;
}
