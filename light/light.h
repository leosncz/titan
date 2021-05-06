#pragma once
#include <glm.hpp>
#include <GL/glew.h>
#define POINT_LIGHT 0
#define DIRECTIONNAL_LIGHT 1
#define SPOT_LIGHT 2
using namespace glm;
class light
{
  public:
     light(int type_ = DIRECTIONNAL_LIGHT, vec3 position_ = vec3(0, 0, 0), vec3 color_ = vec3(1, 1, 1), vec3 direction_ = vec3(0, -0.3, -1), bool computeShadows = true, int shadowResolution_ = 500, float constant_ = 1.0, float linear_ = 1.0, float quadratic_ = 1.0, float cutoff_ = glm::cos(glm::radians(12.5)));
     void setComputeShadows(bool compute);
     bool doesComputeShadows();
     ~light();

     vec3 lightPosition;
     vec3 lightColor;
     vec4 lightDirection;

     float constant, linear, quadratic;
     float cutOff;

     int type; //  0= pointlight 1 = directionnal 2 = spotlight

     int shadowResolution;

     GLuint textureDepthMap, depthMapFBO;

     bool mustBeDeleted; // Indicates if this light has been dynamically created on the heap, if yes the scene will delete it
};