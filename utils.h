#ifndef _UTILS_H_
#define _UTILS_H_

#include <cstdlib>

float random_number(){
  return rand() / (RAND_MAX + 1.0);
}

float pi(){
    return 3.14159265;
}

#endif