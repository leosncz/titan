/* Author: leosncz
Handles everything related to shaders
*/
#pragma once
#include "../light/light.h"

#include <vector>
using namespace std;
class shader
{
public:
    //Material properties
    float metallic;
    float roughness;
    float ambient;
    //End material properties

    shader();

    void registerLightToRender(vector<light*> sceneLights, int numberOfLight, bool forwardRendering = false);
    void compileQuadRenderShader();
    void compileSSAOShader();
    void compileDefaultShader();
    void compileGShaderShader();
    void compileDepthShader();

     GLuint getShaderID();
    ~shader();
private:
    GLuint m_shaderID;
    int id;

    //Material matrix locations
    GLuint specStrenghtID;
    GLuint ambStrenghtID;
    GLuint numberOfLightID;

};
