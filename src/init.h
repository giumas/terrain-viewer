/**
 * init.h
 */
#ifndef INIT_H
#define INIT_H
#include "terrain.h"
#include "vec.h"
void init(FILE * const file);
void init_world_data(worldData * const w);
void load_file(mapData * const mData, FILE * const fileData);
void make_vertex(vec4 * const v, int x, int z, mapData const * const mData);
void get_average_normal(vec3 * const v, unsigned int x, unsigned int z, mapData const * const mData);
void make_normal_top(vec3 * const n, int x, int z, mapData const * const mData);
void make_normal_bottom(vec3 * const n, int x, int z, mapData const * const mData);
#endif
