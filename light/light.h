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
      light() { shadowResolution = 1024;  computeShadows = true;  type = DIRECTIONNAL_LIGHT; cutOff = glm::cos(glm::radians(12.5f));  lightPosition = vec3(0, 0, 0); lightColor = vec3(1, 1, 1); lightDirection = vec3(0, 0, -1); constant = 1.0; linear = 0.19; quadratic = 0.032; }
     void setData(vec3 position, vec3 color, vec3 direction, float strength = 0.0, float cutoff_ = glm::cos(glm::radians(12.5))) { cutOff = cutoff_; lightPosition = position; lightColor = color; lightDirection = direction; }

     vec3 lightPosition;
     vec3 lightColor;
     vec3 lightDirection;

     float constant, linear, quadratic;
     float cutOff;

     int type; //  0= pointlight 1 = directionnal 2 = spotlight

     bool computeShadows;

     int shadowResolution;

     GLuint textureDepthMap, depthMapFBO;

     ~light()
     {
         glDeleteTextures(1, &textureDepthMap);
         glDeleteFramebuffers(1, &depthMapFBO);
     }

};
#endif // LIGHT_H_INCLUDED
