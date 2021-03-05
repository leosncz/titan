#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED
#include <glm.hpp>
#define POINT_LIGHT 0
#define DIRECTIONNAL_LIGHT 1
#define SPOT_LIGHT 2
using namespace glm;
class light
{
  public:
      light() { type = DIRECTIONNAL_LIGHT; cutOff = glm::cos(glm::radians(12.5f));  lightPosition = vec3(0, 0, 0); lightColor = vec3(1, 1, 1); lightDirection = vec3(0, 0, -1); lightStrength = 100; constant = 1.0; linear = 0.19; quadratic = 0.032; }
      void setData(vec3 position, vec3 color, vec3 direction, float strength = 0.0, float cutoff = glm::cos(glm::radians(12.5))) { cutOff = cutoff; lightPosition = position; lightColor = color; lightStrength = strength; lightDirection = direction; }

     vec3 lightPosition;
     vec3 lightColor;
     vec3 lightDirection;
     float lightStrength;

     float constant, linear, quadratic;
     float cutOff;

     int type; //  0= pointlight 1 = directionnal 2 = spotlight

};
#endif // LIGHT_H_INCLUDED
