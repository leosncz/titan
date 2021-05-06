#include "light.h"

light::light(int type_, vec3 position_, vec3 color_, vec3 direction_, bool computeShadows, int shadowResolution_, float constant_, float linear_, float quadratic_, float cutoff_)
{
    shadowResolution = shadowResolution_;
    type = type_;
    cutOff = cutoff_;
    lightPosition = position_;
    lightColor = color_;
    constant = constant_;
    linear = linear_;
    quadratic = quadratic_;
    mustBeDeleted = false;
    if (computeShadows)
    {
        lightDirection = vec4(direction_, computeShadows);
    }
    else
    {
        lightDirection = vec4(direction_, computeShadows);
    }
}

bool light::doesComputeShadows()
{
    return lightDirection[3];
}

void light::setComputeShadows(bool compute)
{
    lightDirection[3] = compute;
}

light::~light()
{
    if (glIsTexture(textureDepthMap))
    {
        glDeleteFramebuffers(1, &depthMapFBO);
        glDeleteTextures(1, &textureDepthMap);
    }

}