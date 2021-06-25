#ifndef _CAMERA_H
#define _CAMERA_H

#include "ray.h"
#include "utils.h"

vec3 random_in_unit_disk(){

    vec3 p;

    do {
        p = 2.0*vec3(random_number(), random_number(), 0) - vec3(1,2,0);
    } while (dot(p,p) >= 1.0);

    return p;

}

class camera {
public:

    vec3 u, v, w;
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    float lens_radius;

    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist) {

        float theta = vfov * pi()/180;
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;
        lens_radius = aperture / 2;

        origin = lookfrom;

        w = unit_vector(lookfrom - lookat);     // The negatice Axis from where we're looking at
        u = unit_vector(cross(vup, w));        // The X axis from the Lookfrom
        v = cross(w, u);                       // The Y axis from the Lookfrom

        lower_left_corner = origin - half_width*focus_dist*u - half_height*focus_dist*v - focus_dist*w;

        horizontal        = 2*half_width*focus_dist*u;
        vertical          = 2*half_height*focus_dist*v;

    }

    ray get_ray(float s, float t) {

        vec3 rd = lens_radius*random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();

        return ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);

    }

};

#endif