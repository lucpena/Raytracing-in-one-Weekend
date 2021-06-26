#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "utils.h"

#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"


vec3 color(const ray& r, hitable *world, int depth){

    hit_record rec;

    if(world->hit(r, 0.001, sizeof(float), rec)) {
        ray scattered;
        vec3 attenuation;

        if(depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation*color(scattered, world, depth+1);
        }
        else {
            return vec3(0, 0, 0);
        }

    }

    else {

        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0); 

    }
}

hitable* random_scene() {

    int32_t n = 500;
    hitable** list = new hitable*[n+1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));

    int32_t i = 1;

    for (int32_t a = -11; a < 11; a++) {
        for (int32_t b = -11; b < 11; b++) {

            float choose_mat = random_number();
            vec3 center(a+0.9*random_number(), 0.2, b+0.9*random_number());

            if((center-vec3(4, 0.2, 0)).length() > 0.9){

                if(choose_mat < 0.8) {  // Diffuse

                    list[i++] = new sphere(center, 0.2, new lambertian(vec3(random_number()*random_number(), random_number()*random_number(), random_number()*random_number())));

                } 
                
                else if(choose_mat < 0.95) {  // Metal

                    list[i++] = new sphere(
                                        center, 
                                        0.2,
                                        new metal(vec3(
                                                        0.5*( 1+random_number() ),  // Vec3 x
                                                        0.5*( 1+random_number() ),  // Vec3 y
                                                        0.5*( 1+random_number() )), // Vec3 z
                                                        0.5*random_number()));      // Fuzziness

                } 
                
                else {  // Glass

                    list[i++] = new sphere(center, 0.2, new dieletric(1.5));

                }
            }

        }
    }

    list[i++] = new sphere(vec3( 0, 1, 0), 1.0, new dieletric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)) );
    list[i++] = new sphere(vec3( 4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0) );
    
    return new hitable_list(list, i);
}

int main(){

    std::cout << "\nRaytracing In One Weekend. \n\n" << std::endl;
    std::cout << "Generating ppm output... \n" << std::endl;

    // Creating the PPM file
    std::ifstream fin("_settings.txt");

    // Dimensions 
    uint32_t nx = 0;
    uint32_t ny = 0;
    uint32_t ns = 0;

    // Error opening the Settings file
    if(!fin) {

        std::cerr << "Error opening the settings file...\n\n" << std::endl;
        std::cin.ignore();
        return -1;

    }

    // Get the dimensions from the Settings file
    fin >> nx >> ny >> ns;

    // Creates the output file
    std::ofstream fout("result.ppm");

    // Defining the PPM standard
    fout << "P3" << std::endl;               // Type
    fout << nx << " " << ny << std::endl;    // Dimensions
    fout << "255" << std::endl;              // Range of RGB

    // Creating the camera
    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    //float dist_to_focus = (lookfrom-lookat).length();
    float dist_to_focus = 10.0;
    float aperture = 0.1;

    camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx)/float(ny), aperture, dist_to_focus);

    // List of hittable objects    
    hitable *world = random_scene();

    // Tracing the rays 
    for(uint32_t j = ny - 1; j < ny; j--){
        for(uint32_t i = 0; i < nx; i++){

            vec3 col(0, 0, 0);

            for(uint32_t s = 0; s < ns; s++) {

                // Random numbers for the Anti Aliasing
                float u = float( i + random_number()) / float(nx);
                float v = float( j + random_number()) / float(ny);

                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world, 0);

            }

            col /= float(ns);
            col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );

            uint32_t ir = uint32_t(255.99 * col[0]);
            uint32_t ig = uint32_t(255.99 * col[1]);
            uint32_t ib = uint32_t(255.99 * col[2]);

            fout << ir << " " << ig << " " << ib << std::endl;

        }

        fout << std::endl;
    }
    
    // Closes the file
    fout.close();
    std::cout << "Done!! 'result.ppm' created.\n\n" << std::endl;
    return 0;
}
