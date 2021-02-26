#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED
#include <glm.hpp>
#define DIRECTIONNAL_LIGHT 1
#define POINT_LIGHT 0
#define SPOT_LIGHT 2
using namespace glm;
class light
{
  public:
      light(){type=DIRECTIONNAL_LIGHT; lightPosition = vec3(0,0,0); lightColor = vec3(1,1,1); lightDirection = vec3(0,0,-1); lightStrength = 100; constant = 1.0; linear = 0.19; quadratic = 0.032;}
     void setData(vec3 position, vec3 color, vec3 direction, float strength){lightPosition = position; lightColor = color; lightStrength = strength; lightDirection = direction;}

     vec3 lightPosition;
     vec3 lightColor;
     vec3 lightDirection;
     float lightStrength;

     float constant, linear, quadratic;

     int type; //  0= pointlight 1 = directionnal 2 = spotlight

};
#endif // LIGHT_H_INCLUDED
