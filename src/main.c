#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"
#include "bw.h"

int main(int argc, char *argv[])
{

    int tst_count = 101; 
    struct Point* pts = (struct Point*)malloc(sizeof(struct Point) * (tst_count + 2));

    FILE *file = fopen("bw-test", "r");
    if (file == NULL) {
        return 1;
    }

    double x, y;  // Use double for general floating-point precision

    // Read until EOF (end of file)
    int pt_count = 0;
    while (fscanf(file, "%lf %lf", &x, &y) == 2) {
        struct Point pt = {x, y, pt_count};
        pts[pt_count] = pt;
        pt_count++;
    }

    fclose(file);

    int triangle_count = 0;
    struct Triangle* triangles = triangulate(pt_count, 900.0f, 900.0f, pts, &triangle_count);

    FILE *outf = fopen("out", "w");
    if (outf == NULL) {
        return 1;
    }

    for (int i = 0; i < triangle_count; i++) {
        fprintf(outf, "%i-%i-%i\n", triangles[i].vertices[0].id, triangles[i].vertices[1].id, triangles[i].vertices[2].id);
    }

    fclose(outf);

    return 0;
}