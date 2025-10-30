#pragma once 


struct Point; 


struct Triangle* triangulate(int pt_count, int sw, int sh, struct Point* points, int* res_count);
