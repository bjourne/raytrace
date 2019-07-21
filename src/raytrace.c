// Copyright (C) 2017-2018 Björn Lindqvist
//
// This is a raytracer written in C. The code is verrrry much based
// on examples from www.scratchapixel.com
#include <assert.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datatypes/common.h"
#include "linalg/linalg.h"
#include "paths/paths.h"

#include "common.h"
#include "triangle_mesh.h"

static bool
v3_array_to_ppm(vec3 *arr, const char *filename,
                  int width, int height) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        return false;
    }
    if (fprintf(f, "P6\n%d %d\n255\n", width, height) <= 0) {
        return false;
    }
    for (int i = 0; i < width * height; i++) {
        int r = (int)CLAMP(255.0f * arr[i].x, 0, 255);
        int g = (int)CLAMP(255.0f * arr[i].y, 0, 255);
        int b = (int)CLAMP(255.0f * arr[i].z, 0, 255);
        if (fprintf(f, "%c%c%c", r, g, b) <= 0) {
            return false;
        }
    }
    fclose(f);
    return true;
}

typedef struct _raytrace_settings {
    vec3 position;
    mat4 view;
    float fov;
    int width;
    int height;
    float scale;
    vec3 translate;
    char *mesh_file;
    char *image_file;
    vec3 bg_col;
} raytrace_settings;

static raytrace_settings *
rt_from_args(int argc, char *argv[]) {
    if (argc != 9) {
        return NULL;
    }
    int width = atoi(argv[3]);
    int height = atoi(argv[4]);
    if (width <= 0 || width > 2048 ||
        height <= 0 || height > 2048) {
        return NULL;
    }

    raytrace_settings *me = (raytrace_settings *)
        malloc(sizeof(raytrace_settings));

    // Scaling and translation
    me->scale = (float)atof(argv[5]);
    me->translate.x = (float)atof(argv[6]);
    me->translate.y = (float)atof(argv[7]);
    me->translate.z = (float)atof(argv[8]);

    me->width = width;
    me->height = height;
    me->mesh_file = strdup(argv[1]);
    me->image_file = strdup(argv[2]);
    me->fov = 50.0393f;
    me->bg_col.x = 0.1f;
    me->bg_col.y = 0.1f;
    me->bg_col.z = 0.2f;
    me->position = (vec3){0, 0, -20.0};
    mat4 tmp = {
        {
            {0.707107f, -0.331295f, 0.624695f, 0.0f},
            {0.0f, 0.883452f, 0.468521f, 0.0f},
            {-0.707107f, -0.331295f, 0.624695f, 0.0f},
            {-1.63871f, -5.747777f, -40.400412f, 1.0f}
        }
    };
    me->view = m4_inverse(tmp);
    return me;
}

static void
rt_free(raytrace_settings *me) {
    free(me->mesh_file);
    free(me->image_file);
    free(me);
}

static vec3
rt_ray_direction(raytrace_settings *rt, int x, int y,
                 float aspect_ratio, float scale) {

    float ray_x = (2 * (x + 0.5f) / (float)rt->width - 1)
        * aspect_ratio * scale;
    float ray_y = (1 - 2 * (y + 0.5f) / (float)rt->height)
        * scale;
    vec3 dir = {ray_x, ray_y, -1};
    dir = m4_mul_v3d(rt->view, dir);
    dir = v3_normalize(dir);
    return dir;
}

static vec3
shade_intersection(vec3 orig, vec3 dir, ray_intersection *ri,
                   triangle_mesh *tm) {
    #if SHADING_STYLE == FANCY_SHADING
    vec3 normal;
    vec2 coords;
    tm_get_surface_props(tm, ri, &normal, &coords);
    float n_dot_view = MAX(0.0f, v3_dot(normal, v3_neg(dir)));
    float m = 10.0f;
    float checker = (float)((fmod(coords.x * m, 1.0f) > 0.5f) ^
                            (fmod(coords.y * m, 1.0f) < 0.5f));
    float c = 0.3f * (1 - checker) + 0.7f * checker;
    return v3_from_scalar(c * n_dot_view);
    #else
    return (vec3){1.0, 1.0, 1.0};
    #endif
}

static size_t hits = 0;

static vec3
cast_ray(vec3 orig, vec3 dir, vec3 bg_col, triangle_mesh *tm) {
    ray_intersection ri;
    if (tm_intersect(tm, orig, dir, &ri)) {
        hits++;
        return shade_intersection(orig, dir, &ri, tm);
    }
    return bg_col;
}

static void
render(raytrace_settings *rt, triangle_mesh *tm, vec3 *fbuf) {
    size_t start = nano_count();
    int w = rt->width;
    int h = rt->height;
    vec3 orig = m4_mul_v3p(rt->view, rt->position);
    float scale = tanf(to_rad(rt->fov * 0.5f));
    float aspect_ratio = (float)w / (float)h;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            vec3 dir = rt_ray_direction(rt, x, y, aspect_ratio, scale);
            *fbuf = cast_ray(orig, dir, rt->bg_col, tm);
            fbuf++;
        }
    }
    size_t end = nano_count();
    double secs = (double)(end - start) / 1000 / 1000 / 1000;
    printf("%s %s %s %d %d %.5f %zd\n",
           ISECT_NAME,
           rt->mesh_file,
           paths_basename(rt->mesh_file),
           rt->width,
           rt->height,
           secs, hits);
}

static void
usage() {
    printf("usage: raytrace mesh-file image "
           "width[0-2048] height[0-2048] scale tx ty tz\n");
    exit(1);
}

int
main(int argc, char *argv[]) {
    rand_init(0);
    raytrace_settings *rt = rt_from_args(argc, argv);
    if (!rt) {
        usage();
    }
    triangle_mesh *tm = tm_from_file(rt->mesh_file,
                                     rt->scale,
                                     rt->translate);
    if (!tm) {
        error("Failed to read mesh from file '%s'.\n",
              rt->mesh_file);
    }
    int w = rt->width;
    int h = rt->height;
    vec3 *fbuf = (vec3 *)malloc(w * h * sizeof(vec3));
    render(rt, tm, fbuf);
    if (!v3_array_to_ppm(fbuf, rt->image_file, w, h)) {
        error("Failed to save to '%s'.", rt->image_file);
    }
    free(fbuf);
    tm_free(tm);
    rt_free(rt);
    return 0;
}
