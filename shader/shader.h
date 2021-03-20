/* Author: leosncz
Handles everything related to shaders
*/
#pragma once
#include <iostream>
#include <string>
#include <stdlib.h>
#include "../light/light.h"
#include "../renderObject/renderObject.h"
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
using namespace std;
class shader
{
public:
    //Material properties
    float specStrenght = 0.5;
    float ambStrenght = 0.1;
    //End material properties

    shader()
    {
        id = rand();
    }

    void init_light_system() // Shouldnt be called unless u know what ur doin
    {
        // Initialize all ogl light ids

        //Light 0
        lightColor0ID = glGetUniformLocation(m_shaderID,"lightColor0");
        lightPos0ID = glGetUniformLocation(m_shaderID,"lightPos0");
        lightDir0ID = glGetUniformLocation(m_shaderID,"lightDir0");
        lightStrength0ID = glGetUniformLocation(m_shaderID,"lightStrength0");
        lightType0ID = glGetUniformLocation(m_shaderID,"lightType0");

        lightConstant0ID = glGetUniformLocation(m_shaderID, "lightConstant0");
        lightLinear0ID = glGetUniformLocation(m_shaderID, "lightLinear0");
        lightQuadratic0ID = glGetUniformLocation(m_shaderID, "lightQuadratic0");

        lightCutOff0ID = glGetUniformLocation(m_shaderID, "lightCutOff0");

        //Light 1
        lightColor1ID = glGetUniformLocation(m_shaderID,"lightColor1");
        lightPos1ID = glGetUniformLocation(m_shaderID,"lightPos1");
        lightDir1ID = glGetUniformLocation(m_shaderID,"lightDir1");
        lightStrength1ID = glGetUniformLocation(m_shaderID,"lightStrength1");
        lightType1ID = glGetUniformLocation(m_shaderID,"lightType1");

        lightConstant1ID = glGetUniformLocation(m_shaderID, "lightConstant1");
        lightLinear1ID = glGetUniformLocation(m_shaderID, "lightLinear1");
        lightQuadratic1ID = glGetUniformLocation(m_shaderID, "lightQuadratic1");

        lightCutOff1ID = glGetUniformLocation(m_shaderID, "lightCutOff1");
        //Light 2
        lightColor2ID = glGetUniformLocation(m_shaderID,"lightColor2");
        lightPos2ID = glGetUniformLocation(m_shaderID,"lightPos2");
        lightDir2ID = glGetUniformLocation(m_shaderID,"lightDir2");
        lightStrength2ID = glGetUniformLocation(m_shaderID,"lightStrength2");
        lightType2ID = glGetUniformLocation(m_shaderID,"lightType2");

        lightConstant2ID = glGetUniformLocation(m_shaderID, "lightConstant2");
        lightLinear2ID = glGetUniformLocation(m_shaderID, "lightLinear2");
        lightQuadratic2ID = glGetUniformLocation(m_shaderID, "lightQuadratic2");

        lightCutOff2ID = glGetUniformLocation(m_shaderID, "lightCutOff2");
        //Light 3
        lightColor3ID = glGetUniformLocation(m_shaderID,"lightColor3");
        lightPos3ID = glGetUniformLocation(m_shaderID,"lightPos3");
        lightDir3ID = glGetUniformLocation(m_shaderID,"lightDir3");
        lightStrength3ID = glGetUniformLocation(m_shaderID,"lightStrength3");
        lightType3ID = glGetUniformLocation(m_shaderID,"lightType3");

        lightConstant3ID = glGetUniformLocation(m_shaderID, "lightConstant3");
        lightLinear3ID = glGetUniformLocation(m_shaderID, "lightLinear3");
        lightQuadratic3ID = glGetUniformLocation(m_shaderID, "lightQuadratic3");

        lightCutOff3ID = glGetUniformLocation(m_shaderID, "lightCutOff3");
        //Light 4
        lightColor4ID = glGetUniformLocation(m_shaderID,"lightColor4");
        lightPos4ID = glGetUniformLocation(m_shaderID,"lightPos4");
        lightDir4ID = glGetUniformLocation(m_shaderID,"lightDir4");
        lightStrength4ID = glGetUniformLocation(m_shaderID,"lightStrength4");
        lightType4ID = glGetUniformLocation(m_shaderID,"lightType4");

        lightConstant4ID = glGetUniformLocation(m_shaderID, "lightConstant4");
        lightLinear4ID = glGetUniformLocation(m_shaderID, "lightLinear4");
        lightQuadratic4ID = glGetUniformLocation(m_shaderID, "lightQuadratic4");

        lightCutOff4ID = glGetUniformLocation(m_shaderID, "lightCutOff4");
        //Light 5
        lightColor5ID = glGetUniformLocation(m_shaderID,"lightColor5");
        lightPos5ID = glGetUniformLocation(m_shaderID,"lightPos5");
        lightDir5ID = glGetUniformLocation(m_shaderID,"lightDir5");
        lightStrength5ID = glGetUniformLocation(m_shaderID,"lightStrength5");
        lightType5ID = glGetUniformLocation(m_shaderID,"lightType5");

        lightConstant5ID = glGetUniformLocation(m_shaderID, "lightConstant5");
        lightLinear5ID = glGetUniformLocation(m_shaderID, "lightLinear5");
        lightQuadratic5ID = glGetUniformLocation(m_shaderID, "lightQuadratic5");

        lightCutOff5ID = glGetUniformLocation(m_shaderID, "lightCutOff5");
        //Light 6
        lightColor6ID = glGetUniformLocation(m_shaderID,"lightColor6");
        lightPos6ID = glGetUniformLocation(m_shaderID,"lightPos6");
        lightDir6ID = glGetUniformLocation(m_shaderID,"lightDir6");
        lightStrength6ID = glGetUniformLocation(m_shaderID,"lightStrength6");
        lightType6ID = glGetUniformLocation(m_shaderID,"lightType6");

        lightConstant6ID = glGetUniformLocation(m_shaderID, "lightConstant6");
        lightLinear6ID = glGetUniformLocation(m_shaderID, "lightLinear6");
        lightQuadratic6ID = glGetUniformLocation(m_shaderID, "lightQuadratic6");

        lightCutOff6ID = glGetUniformLocation(m_shaderID, "lightCutOff6");

        numberOfLightID = glGetUniformLocation(m_shaderID,"numberOfLight");

        //Material properties
        specStrenghtID = glGetUniformLocation(m_shaderID,"specStrenght");
        ambStrenghtID = glGetUniformLocation(m_shaderID,"ambStrenght");
    }

    void registerLightToRender(light* sceneLights[], int numberOfLight, float nearPlane=1.0f,float farPlane=50.0f) // the shader chosen must support light
    {
        //Check if there is light
            if(numberOfLight > 0) // Assuming we only got one light possible (for now), so we only send first light data
            {
                //Send material properties
                glUniform1f(specStrenghtID,specStrenght);
                glUniform1f(ambStrenghtID,ambStrenght);

                //send number of light to shader
                glUniform1i(numberOfLightID,numberOfLight);
                //send light 0
                glUniform3f(lightColor0ID,sceneLights[0]->lightColor.x,sceneLights[0]->lightColor.y,sceneLights[0]->lightColor.z);
                glUniform3f(lightPos0ID,sceneLights[0]->lightPosition.x,sceneLights[0]->lightPosition.y,sceneLights[0]->lightPosition.z);
                glUniform3f(lightDir0ID,sceneLights[0]->lightDirection.x,sceneLights[0]->lightDirection.y,sceneLights[0]->lightDirection.z);
                glUniform1f(lightStrength0ID,sceneLights[0]->lightStrength);
                glUniform1f(lightConstant0ID,sceneLights[0]->constant);
                glUniform1f(lightLinear0ID,sceneLights[0]->linear);
                glUniform1f(lightQuadratic0ID,sceneLights[0]->quadratic);
                glUniform1i(lightType0ID,sceneLights[0]->type);
                glUniform1f(lightCutOff0ID, sceneLights[0]->cutOff);

                if(numberOfLight >= 2)
                {
                    glUniform3f(lightColor1ID,sceneLights[1]->lightColor.x,sceneLights[1]->lightColor.y,sceneLights[1]->lightColor.z);
                    glUniform3f(lightPos1ID,sceneLights[1]->lightPosition.x,sceneLights[1]->lightPosition.y,sceneLights[1]->lightPosition.z);
                    glUniform3f(lightDir1ID,sceneLights[1]->lightDirection.x,sceneLights[1]->lightDirection.y,sceneLights[1]->lightDirection.z);
                    glUniform1f(lightStrength1ID,sceneLights[1]->lightStrength);
                    glUniform1i(lightType1ID,sceneLights[1]->type);
                    glUniform1f(lightConstant1ID,sceneLights[1]->constant);
                    glUniform1f(lightLinear1ID,sceneLights[1]->linear);
                    glUniform1f(lightQuadratic1ID,sceneLights[1]->quadratic);
                    glUniform1f(lightCutOff1ID, sceneLights[1]->cutOff);
                }
                if(numberOfLight >= 3)
                {
                    glUniform3f(lightColor2ID,sceneLights[2]->lightColor.x,sceneLights[2]->lightColor.y,sceneLights[2]->lightColor.z);
                    glUniform3f(lightPos2ID,sceneLights[2]->lightPosition.x,sceneLights[2]->lightPosition.y,sceneLights[2]->lightPosition.z);
                    glUniform3f(lightDir2ID,sceneLights[2]->lightDirection.x,sceneLights[2]->lightDirection.y,sceneLights[2]->lightDirection.z);
                    glUniform1f(lightStrength2ID,sceneLights[2]->lightStrength);
                    glUniform1i(lightType2ID,sceneLights[2]->type);
                    glUniform1f(lightConstant2ID,sceneLights[2]->constant);
                    glUniform1f(lightLinear2ID,sceneLights[2]->linear);
                    glUniform1f(lightQuadratic2ID,sceneLights[2]->quadratic);
                    glUniform1f(lightCutOff2ID, sceneLights[2]->cutOff);
                }
                if(numberOfLight >= 4)
                {
                    glUniform3f(lightColor3ID,sceneLights[3]->lightColor.x,sceneLights[3]->lightColor.y,sceneLights[3]->lightColor.z);
                    glUniform3f(lightPos3ID,sceneLights[3]->lightPosition.x,sceneLights[3]->lightPosition.y,sceneLights[3]->lightPosition.z);
                    glUniform3f(lightDir3ID,sceneLights[3]->lightDirection.x,sceneLights[3]->lightDirection.y,sceneLights[3]->lightDirection.z);
                    glUniform1f(lightStrength3ID,sceneLights[3]->lightStrength);
                    glUniform1i(lightType3ID,sceneLights[3]->type);
                    glUniform1f(lightConstant3ID,sceneLights[3]->constant);
                    glUniform1f(lightLinear3ID,sceneLights[3]->linear);
                    glUniform1f(lightQuadratic3ID,sceneLights[3]->quadratic);
                    glUniform1f(lightCutOff3ID, sceneLights[3]->cutOff);
                }
                if(numberOfLight >= 5)
                {
                    glUniform3f(lightColor4ID,sceneLights[4]->lightColor.x,sceneLights[4]->lightColor.y,sceneLights[4]->lightColor.z);
                    glUniform3f(lightPos4ID,sceneLights[4]->lightPosition.x,sceneLights[4]->lightPosition.y,sceneLights[4]->lightPosition.z);
                    glUniform3f(lightDir4ID,sceneLights[4]->lightDirection.x,sceneLights[4]->lightDirection.y,sceneLights[4]->lightDirection.z);
                    glUniform1f(lightStrength4ID,sceneLights[4]->lightStrength);
                    glUniform1i(lightType4ID,sceneLights[4]->type);
                    glUniform1f(lightConstant4ID,sceneLights[4]->constant);
                    glUniform1f(lightLinear4ID,sceneLights[4]->linear);
                    glUniform1f(lightQuadratic4ID,sceneLights[4]->quadratic);
                    glUniform1f(lightCutOff4ID, sceneLights[4]->cutOff);
                }
                if(numberOfLight >= 6)
                {
                    glUniform3f(lightColor5ID,sceneLights[5]->lightColor.x,sceneLights[5]->lightColor.y,sceneLights[5]->lightColor.z);
                    glUniform3f(lightPos5ID,sceneLights[5]->lightPosition.x,sceneLights[5]->lightPosition.y,sceneLights[5]->lightPosition.z);
                    glUniform3f(lightDir5ID,sceneLights[5]->lightDirection.x,sceneLights[5]->lightDirection.y,sceneLights[5]->lightDirection.z);
                    glUniform1f(lightStrength5ID,sceneLights[5]->lightStrength);
                    glUniform1i(lightType5ID,sceneLights[5]->type);
                    glUniform1f(lightConstant5ID,sceneLights[5]->constant);
                    glUniform1f(lightLinear5ID,sceneLights[5]->linear);
                    glUniform1f(lightQuadratic5ID,sceneLights[5]->quadratic);
                    glUniform1f(lightCutOff5ID, sceneLights[5]->cutOff);
                }
                if(numberOfLight >= 7)
                {
                    glUniform3f(lightColor6ID,sceneLights[6]->lightColor.x,sceneLights[6]->lightColor.y,sceneLights[6]->lightColor.z);
                    glUniform3f(lightPos6ID,sceneLights[6]->lightPosition.x,sceneLights[6]->lightPosition.y,sceneLights[6]->lightPosition.z);
                    glUniform3f(lightDir6ID,sceneLights[6]->lightDirection.x,sceneLights[6]->lightDirection.y,sceneLights[6]->lightDirection.z);
                    glUniform1f(lightStrength6ID,sceneLights[6]->lightStrength);
                    glUniform1i(lightType6ID,sceneLights[6]->type);
                    glUniform1f(lightConstant6ID,sceneLights[6]->constant);
                    glUniform1f(lightLinear6ID,sceneLights[6]->linear);
                    glUniform1f(lightQuadratic6ID,sceneLights[6]->quadratic);
                    glUniform1f(lightCutOff6ID, sceneLights[6]->cutOff);
                }
            }
    }

    void compileDefaultShader() //For now handles directionnal, spot and point lights
    {
        std::cout << "---> Compiling lightBasicShader for shader ID=" << id << std::endl;

        const char* vertex_shader =
        "#version 330\n"
        "layout(location = 0) in vec3 vp;"
        "layout(location = 1) in vec3 color;"
        "layout(location = 2) in vec2 inputTexCoord;"
        "layout(location = 3) in vec3 normals;"
        "out vec3 aNormals;"
        "out vec3 fragPos;"
        "out vec2 texCoord;"
        "uniform mat4 model;"
        "uniform mat4 view;"
        "uniform mat4 projection;"
        "out vec3 final_color;"
        "out VS_OUT{"
        "vec3 FragPos;"
        "vec3 Normal;"
        "vec2 TexCoords;"
        "vec4 FragPosLightSpace;"
        "vec4 FragPosLightSpace1;"
        " } vs_out;"
        "uniform mat4 lightSpaceMatrix;"
        "uniform mat4 lightSpaceMatrix1;"
        "void main() {"
        "   gl_Position = projection * view * model * vec4(vp, 1.0);"
        "   aNormals = mat3(transpose(inverse(model))) * normals;"
        "   final_color = color;"
        "   fragPos = vec3(model * vec4(vp,1.0));"
        "   texCoord = inputTexCoord;"
        "   vs_out.FragPos = fragPos;"
        "   vs_out.Normal = aNormals;"
        "   vs_out.TexCoords = inputTexCoord;"
        "   vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);"
        "   vs_out.FragPosLightSpace1 = lightSpaceMatrix1 * vec4(vs_out.FragPos, 1.0);"
        "}";

        const char* fragment_shader =
        "#version 330\n"
        //MATERIAL PROPERTIES-----------------
        "uniform float specStrenght;"
        "uniform float ambStrenght;"
        //END MATERIAL PROPERTIES-------------
        //Light 0
        "uniform vec3 lightColor0;"
        "uniform vec3 lightPos0;"
        "uniform vec3 lightDir0;"
        "uniform int lightType0;"
        "uniform float lightConstant0;"
        "uniform float lightLinear0;"
        "uniform float lightQuadratic0;"
        "uniform float lightCutOff0;"
        //Light 1
        "uniform vec3 lightColor1;"
        "uniform vec3 lightPos1;"
        "uniform vec3 lightDir1;"
        "uniform int lightType1;"
        "uniform float lightConstant1;"
        "uniform float lightLinear1;"
        "uniform float lightQuadratic1;"
        "uniform float lightCutOff1;"
        //light 2
        "uniform vec3 lightColor2;"
        "uniform vec3 lightPos2;"
        "uniform vec3 lightDir2;"
        "uniform int lightType2;"
        "uniform float lightConstant2;"
        "uniform float lightLinear2;"
        "uniform float lightQuadratic2;"
        "uniform float lightCutOff2;"
        //light 3
        "uniform vec3 lightColor3;"
        "uniform vec3 lightPos3;"
        "uniform vec3 lightDir3;"
        "uniform int lightType3;"
        "uniform float lightConstant3;"
        "uniform float lightLinear3;"
        "uniform float lightQuadratic3;"
        "uniform float lightCutOff3;"
        //light 4
        "uniform vec3 lightColor4;"
        "uniform vec3 lightPos4;"
        "uniform vec3 lightDir4;"
        "uniform int lightType4;"
        "uniform float lightConstant4;"
        "uniform float lightLinear4;"
        "uniform float lightQuadratic4;"
        "uniform float lightCutOff4;"
        //light 5
        "uniform vec3 lightColor5;"
        "uniform vec3 lightPos5;"
        "uniform vec3 lightDir5;"
        "uniform int lightType5;"
        "uniform float lightConstant5;"
        "uniform float lightLinear5;"
        "uniform float lightQuadratic5;"
        "uniform float lightCutOff5;"
        //light 6
        "uniform vec3 lightColor6;"
        "uniform vec3 lightPos6;"
        "uniform vec3 lightDir6;"
        "uniform int lightType6;"
        "uniform float lightConstant6;"
        "uniform float lightLinear6;"
        "uniform float lightQuadratic6;"
        "uniform float lightCutOff6;"
        //number of lights
        "uniform int numberOfLight;"
        // next important render matrixes
        "uniform vec3 viewPos;"
        "out vec4 frag_colour;"
        "in vec3 final_color;"
        "in vec3 aNormals;"
        "in vec3 fragPos;"
        "in vec2 texCoord;"
        "uniform int howManyTex;"
        // TEXTURES
        "uniform sampler2D texture1;"
        "uniform sampler2D texture2;"
        "uniform sampler2D texture3;"
        "uniform sampler2D textureDepthMap;"
        "uniform sampler2D textureDepthMap1;"
        "uniform sampler2D specularMap;"
        "uniform int useSpecularMap;"
        "in VS_OUT{"
        "vec3 FragPos;"
        "vec3 Normal;"
        "vec2 TexCoords;"
        "vec4 FragPosLightSpace;"
        "vec4 FragPosLightSpace1;"
        "} fs_in;"
        "uniform float gamma;"
        "uniform samplerCube textureDepthCubemap;"
        "uniform samplerCube textureDepthCubemap1;"
        "uniform samplerCube textureDepthCubemap2;"
        "uniform samplerCube textureDepthCubemap3;"
        "uniform samplerCube textureDepthCubemap4;"
        "uniform samplerCube textureDepthCubemap5;"
        "uniform samplerCube textureDepthCubemap6;"
        "float ShadowCalculationPL(vec3 fragPos, vec3 lightPos, samplerCube tex)"
        "{"
        // get vector between fragment position and light position
        "vec3 fragToLight = fragPos - lightPos;"
        // now get current linear depth as the length between the fragment and light position
        "float currentDepth = length(fragToLight);"
        // now test for shadows
        "float shadow = 0.0;"
        "float bias = 0.05;"
        "float samples = 4.0;"
        "float offset = 0.1;"
        "for (float x = -offset; x < offset; x += offset / (samples * 0.5))"
        "{"
        "    for (float y = -offset; y < offset; y += offset / (samples * 0.5))"
        "    {"
        "        for (float z = -offset; z < offset; z += offset / (samples * 0.5))"
        "        {"
        "            float closestDepth = texture(tex, fragToLight + vec3(x, y, z)).r;"
        "            closestDepth *= 25.0;"   // undo mapping [0;1]
        "            if (currentDepth - bias > closestDepth){"
        "                shadow += 1.0;}"
        "        }"
        "    }"
        "}"
        "shadow /= (samples * samples * samples);"
        "return shadow;"
        "}"
        "float ShadowCalculation(vec4 fragPosLightSpace, sampler2D texture_)"
        "{"
        // perform perspective divide
        "vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;"
        // transform to [0,1] range
        "projCoords = projCoords * 0.5 + 0.5;"
        "float shadow = 0.0;"
        "float bias = 0.005;"
        "vec2 texelSize = 1.0 / textureSize(texture_, 0);"
        "float currentDepth = projCoords.z;"
        "for (int x = -1; x <= 1; ++x)"
        "{"
        "    for (int y = -1; y <= 1; ++y)"
        "    {"
        "        float pcfDepth = texture(texture_, projCoords.xy + vec2(x, y) * texelSize).r;"
        "        shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;"
        "    }"
        "}"
        "shadow /= 9.0;"
        "if (projCoords.z > 1.0){"
        "     shadow = 0.0;}"
        "return shadow;"
        "}"
        "vec4 calculateLightOutput0()"
        "{"
        "  vec3 norm = normalize(aNormals);"
        "  vec3 lightDir;"
        "  float attenuation = 1.0;"
        "  float shadowCoeff = 1.0;"
        "  if(lightType0 == 1){" //directionnal
        "    lightDir = normalize(-lightDir0);"
        "    shadowCoeff = 1-ShadowCalculation(fs_in.FragPosLightSpace,textureDepthMap) * (1 - ambStrenght);"
        "  }"
        "  else if(lightType0 == 0){"
        "    lightDir = normalize(lightPos0 - fragPos);" //point light
        "    float distance    = length(lightPos0 - fragPos);"
        "    attenuation = 1.0 / (lightConstant0 + lightLinear0 * distance + lightQuadratic0 * (distance));"
        "    shadowCoeff = 1-ShadowCalculationPL(fs_in.FragPos,lightPos0,textureDepthCubemap);"
        "  }"
        "  else if(lightType0 == 2){" // spotlight
        "    lightDir = normalize(lightPos0 - fragPos);"
        "    float theta = dot(normalize(lightPos0 - fragPos), normalize(-lightDir0));"
        "    if(theta > lightCutOff0) {"
        "      float distance = length(lightPos0 - fragPos);"
        "      attenuation = 1.0 / (lightConstant0 + lightLinear0 * distance + lightQuadratic0 * (distance * distance));"
        "    }else{"
        "       attenuation = 0.1;"
        "    }"
        "  }"
        "  float diff = max(dot(norm, lightDir), 0.0);"
        "  vec3 diffuse = diff * lightColor0;"
        "  float ambiantStrength = ambStrenght;"
        "  vec3 ambiant = ambiantStrength * lightColor0;"
        "  float specularStrength = specStrenght;"
        "  vec3 viewDir = normalize(viewPos - fragPos);"
        "  vec3 reflectDir = reflect(-lightDir, norm);"
        "  vec3 halfwayDir = normalize(lightDir + viewDir);"
        "  float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);"
        "  vec3 specular = vec3(0,0,0);"
        "  if(useSpecularMap == 1){specular = (specularStrength * spec) * lightColor0 * vec3(texture(specularMap, texCoord));}"
        "  else{specular = (specularStrength * spec) * lightColor0;}"
        "  vec3 result = ((ambiant*=attenuation) + (diffuse*=attenuation) + (specular*=attenuation)) * shadowCoeff * final_color;"
        "  return(vec4(result,1.0));"
        "}"
        //""
        "vec4 calculateLightOutput1()"
        "{"
        "  vec3 norm = normalize(aNormals);"
        "  vec3 lightDir;"
        "  float attenuation = 1.0;"
        "  float shadowCoeff = 1.0;"
        "  if(lightType1 == 1){" //directionnal
        "    lightDir = normalize(-lightDir1);"
        "    shadowCoeff = (1 - ShadowCalculation(fs_in.FragPosLightSpace1,textureDepthMap1) * (1 - ambStrenght));"
        "   "
        "  }"
        "  else if(lightType1 == 0){lightDir = normalize(lightPos1 - fragPos);" //point light
        "    float distance    = length(lightPos1 - fragPos);"
        "    attenuation = 1.0 / (lightConstant1 + lightLinear1 * distance + lightQuadratic1 * (distance));"
        "    shadowCoeff = (1 - ShadowCalculationPL(fs_in.FragPos, lightPos1, textureDepthCubemap1));"
        "  }"
        "  else if(lightType1 == 2){" // spotlight
        "    lightDir = normalize(lightPos1 - fragPos);"
        "    float theta = dot(normalize(lightPos1 - fragPos), normalize(-lightDir1));"
        "    if(theta > lightCutOff1) {"
        "      float distance = length(lightPos1 - fragPos);"
        "      attenuation = 1.0 / (lightConstant1 + lightLinear1 * distance + lightQuadratic1 * (distance * distance));"
        "    }else{"
        "       attenuation = 0.1;"
        "    }"
        "  }"
        "  float diff = max(dot(norm, lightDir), 0.0);"
        "  vec3 diffuse = diff * lightColor1;"
        "  float ambiantStrength = ambStrenght;"
        "  vec3 ambiant = ambiantStrength * lightColor1;"
        "  float specularStrength = specStrenght;"
        "  vec3 viewDir = normalize(viewPos - fragPos);"
        "  vec3 reflectDir = reflect(-lightDir, norm);"
        "  vec3 halfwayDir = normalize(lightDir + viewDir);"
        "  float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);"
        "  vec3 specular = vec3(0,0,0);"
        "  if(useSpecularMap == 1){specular = (specularStrength * spec) * lightColor1 * vec3(texture(specularMap, texCoord));}"
        "  else{specular = (specularStrength * spec) * lightColor1;}"
        "  vec3 result = ((ambiant*=attenuation) + (diffuse*=attenuation) + (specular*=attenuation)) * shadowCoeff * final_color;"
        "  return(vec4(result,1.0));"
        "}"
        //""
        "vec4 calculateLightOutput2()"
        "{"
        "  vec3 norm = normalize(aNormals);"
        "  vec3 lightDir;"
        "  float attenuation = 1.0;"
        "  float shadowCoeff = 1.0;"
        "  if(lightType2 == 1){" //directionnal
        "    lightDir = normalize(-lightDir2);"
        "    "
        "   }"
        "  else if(lightType2 == 0){lightDir = normalize(lightPos2 - fragPos);" //point light
        "    float distance    = length(lightPos2 - fragPos);"
        "    attenuation = 1.0 / (lightConstant2 + lightLinear2 * distance + lightQuadratic2 * (distance));"
        "    shadowCoeff = (1-ShadowCalculationPL(fs_in.FragPos,lightPos2,textureDepthCubemap2));"  
        "  }"
        "  else if(lightType2 == 2){" // spotlight
        "    lightDir = normalize(lightPos2 - fragPos);"
        "    float theta = dot(normalize(lightPos2 - fragPos), normalize(-lightDir2));"
        "    if(theta > lightCutOff0) {"
        "      float distance = length(lightPos2 - fragPos);"
        "      attenuation = 1.0 / (lightConstant2 + lightLinear2 * distance + lightQuadratic2 * (distance * distance));"
        "    }else{"
        "       attenuation = 0.1;"
        "    }"
        "  }"
        "  float diff = max(dot(norm, lightDir), 0.0);"
        "  vec3 diffuse = diff * lightColor2;"
        "  float ambiantStrength = ambStrenght;"
        "  vec3 ambiant = ambiantStrength * lightColor2;"
        "  float specularStrength = specStrenght;"
        "  vec3 viewDir = normalize(viewPos - fragPos);"
        "  vec3 reflectDir = reflect(-lightDir, norm);"
        "  vec3 halfwayDir = normalize(lightDir + viewDir);"
        "  float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);"
        "  vec3 specular = vec3(0,0,0);"
        "  if(useSpecularMap == 1){specular = (specularStrength * spec) * lightColor2 * vec3(texture(specularMap, texCoord));}"
        "  else{specular = (specularStrength * spec) * lightColor2;}"
        "  vec3 result = ((ambiant*=attenuation) + (diffuse*=attenuation) + (specular*=attenuation)) * shadowCoeff * final_color;"
        "  return(vec4(result,1.0));"
        "}"
        //""
        "vec4 calculateLightOutput3()"
        "{"
        "  vec3 norm = normalize(aNormals);"
        "  vec3 lightDir;"
        "  float attenuation = 1.0;"
        "  float shadowCoeff = 1.0;"
        "  if(lightType3 == 1){" //directionnal
        "    lightDir = normalize(-lightDir3);"
        "  "
        "   }"
        "  else if(lightType3 == 0){lightDir = normalize(lightPos3 - fragPos);" //point light
        "    float distance    = length(lightPos3 - fragPos);"
        "    attenuation = 1.0 / (lightConstant3 + lightLinear3 * distance + lightQuadratic3 * (distance));"
        "    shadowCoeff = (1-ShadowCalculationPL(fs_in.FragPos,lightPos3,textureDepthCubemap3));"
        "  }"
        "  else if(lightType3 == 2){" // spotlight
        "    lightDir = normalize(lightPos3 - fragPos);"
        "    float theta = dot(normalize(lightPos3 - fragPos), normalize(-lightDir3));"
        "    if(theta > lightCutOff3) {"
        "      float distance = length(lightPos3 - fragPos);"
        "      attenuation = 1.0 / (lightConstant3 + lightLinear3 * distance + lightQuadratic3 * (distance * distance));"
        "    }else{"
        "       attenuation = 0.1;"
        "    }"
        "  }"
        "  float diff = max(dot(norm, lightDir), 0.0);"
        "  vec3 diffuse = diff * lightColor3;"
        "  float ambiantStrength = ambStrenght;"
        "  vec3 ambiant = ambiantStrength * lightColor3;"
        "  float specularStrength = specStrenght;"
        "  vec3 viewDir = normalize(viewPos - fragPos);"
        "  vec3 reflectDir = reflect(-lightDir, norm);"
        "  vec3 halfwayDir = normalize(lightDir + viewDir);"
        "  float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);"
        "  vec3 specular = vec3(0,0,0);"
        "  if(useSpecularMap == 1){specular = (specularStrength * spec) * lightColor3 * vec3(texture(specularMap, texCoord));}"
        "  else{specular = (specularStrength * spec) * lightColor3;}"
        "  vec3 result = ((ambiant*=attenuation) + (diffuse*=attenuation) + (specular*=attenuation)) * shadowCoeff * final_color;"
        "  return(vec4(result,1.0));"
        "}"
        //""
        "vec4 calculateLightOutput4()"
        "{"
        "  vec3 norm = normalize(aNormals);"
        "  vec3 lightDir;"
        "  float attenuation = 1.0;"
        "  float shadowCoeff = 1.0;"
        "  if(lightType4 == 1){" //directionnal
        "    lightDir = normalize(-lightDir4);"
        "  "
        "  }"
        "  else if(lightType4 == 0){lightDir = normalize(lightPos4 - fragPos);" //point light
        "    float distance    = length(lightPos4 - fragPos);"
        "    attenuation = 1.0 / (lightConstant4 + lightLinear4 * distance + lightQuadratic4 * (distance));"
        "    shadowCoeff = (1-ShadowCalculationPL(fs_in.FragPos,lightPos4,textureDepthCubemap4));"
        "  }"
        "  else if(lightType4 == 2){" // spotlight
        "    lightDir = normalize(lightPos4 - fragPos);"
        "    float theta = dot(normalize(lightPos4 - fragPos), normalize(-lightDir4));"
        "    if(theta > lightCutOff4) {"
        "      float distance = length(lightPos4 - fragPos);"
        "      attenuation = 1.0 / (lightConstant4 + lightLinear4 * distance + lightQuadratic4 * (distance * distance));"
        "    }else{"
        "       attenuation = 0.1;"
        "    }"
        "  }"
        "  float diff = max(dot(norm, lightDir), 0.0);"
        "  vec3 diffuse = diff * lightColor4;"
        "  float ambiantStrength = ambStrenght;"
        "  vec3 ambiant = ambiantStrength * lightColor4;"
        "  float specularStrength = specStrenght;"
        "  vec3 viewDir = normalize(viewPos - fragPos);"
        "  vec3 reflectDir = reflect(-lightDir, norm);"
        "  vec3 halfwayDir = normalize(lightDir + viewDir);"
        "  float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);"
        "  vec3 specular = vec3(0,0,0);"
        "  if(useSpecularMap == 1){specular = (specularStrength * spec) * lightColor4 * vec3(texture(specularMap, texCoord));}"
        "  else{specular = (specularStrength * spec) * lightColor4;}"
        "  vec3 result = ((ambiant*=attenuation) + (diffuse*=attenuation) + (specular*=attenuation)) * shadowCoeff * final_color;"
        "  return(vec4(result,1.0));"
        "}"
        //""
        "vec4 calculateLightOutput5()"
        "{"
        "  vec3 norm = normalize(aNormals);"
        "  vec3 lightDir;"
        "  float attenuation = 1.0;"
        "  float shadowCoeff = 1.0;"
        "  if(lightType5 == 1){" //directionnal
        "    lightDir = normalize(-lightDir5);"
        "   "
        "  }"
        "  else if(lightType5 == 0){lightDir = normalize(lightPos5 - fragPos);" //point light
        "    float distance    = length(lightPos5 - fragPos);"
        "    attenuation = 1.0 / (lightConstant5 + lightLinear5 * distance + lightQuadratic5 * (distance));"
        "    shadowCoeff = (1-ShadowCalculationPL(fs_in.FragPos,lightPos5,textureDepthCubemap5));"
        "  }"
        "  else if(lightType5 == 2){" // spotlight
        "    lightDir = normalize(lightPos5 - fragPos);"
        "    float theta = dot(normalize(lightPos5 - fragPos), normalize(-lightDir5));"
        "    if(theta > lightCutOff5) {"
        "      float distance = length(lightPos5 - fragPos);"
        "      attenuation = 1.0 / (lightConstant5 + lightLinear5 * distance + lightQuadratic5 * (distance * distance));"
        "    }else{"
        "       attenuation = 0.1;"
        "    }"
        "  }"
        "  float diff = max(dot(norm, lightDir), 0.0);"
        "  vec3 diffuse = diff * lightColor5;"
        "  float ambiantStrength = ambStrenght;"
        "  vec3 ambiant = ambiantStrength * lightColor5;"
        "  float specularStrength = specStrenght;"
        "  vec3 viewDir = normalize(viewPos - fragPos);"
        "  vec3 reflectDir = reflect(-lightDir, norm);"
        "  vec3 halfwayDir = normalize(lightDir + viewDir);"
        "  float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);"
        "  vec3 specular = vec3(0,0,0);"
        "  if(useSpecularMap == 1){specular = (specularStrength * spec) * lightColor5 * vec3(texture(specularMap, texCoord));}"
        "  else{specular = (specularStrength * spec) * lightColor5;}"
        "  vec3 result = ((ambiant*=attenuation) + (diffuse*=attenuation) + (specular*=attenuation)) * shadowCoeff * final_color;"
        "  return(vec4(result,1.0));"
        "}"
        //""
         "vec4 calculateLightOutput6()"
        "{"
        "  vec3 norm = normalize(aNormals);"
        "  vec3 lightDir;"
        "  float attenuation = 1.0;"
        "  float shadowCoeff = 1.0;"
        "  if(lightType6 == 1){" //directionnal
        "    lightDir = normalize(-lightDir6);"
        "   "
        "  }"
        "  else if(lightType6 == 0){lightDir = normalize(lightPos6 - fragPos);" //point light
        "    float distance    = length(lightPos6 - fragPos);"
        "    attenuation = 1.0 / (lightConstant6 + lightLinear6 * distance + lightQuadratic6 * (distance));"
        "    shadowCoeff = (1-ShadowCalculationPL(fs_in.FragPos,lightPos6,textureDepthCubemap6));"
        "  }"
        "  else if(lightType6 == 2){" // spotlight
        "    lightDir = normalize(lightPos6 - fragPos);"
        "    float theta = dot(normalize(lightPos6 - fragPos), normalize(-lightDir6));"
        "    if(theta > lightCutOff6) {"
        "      float distance = length(lightPos6 - fragPos);"
        "      attenuation = 1.0 / (lightConstant6 + lightLinear6 * distance + lightQuadratic6 * (distance * distance));"
        "    }else{"
        "       attenuation = 0.1;"
        "    }"
        "  }"
        "  float diff = max(dot(norm, lightDir), 0.0);"
        "  vec3 diffuse = diff * lightColor6;"
        "  float ambiantStrength = ambStrenght;"
        "  vec3 ambiant = ambiantStrength * lightColor6;"
        "  float specularStrength = specStrenght;"
        "  vec3 viewDir = normalize(viewPos - fragPos);"
        "  vec3 reflectDir = reflect(-lightDir, norm);"
        "  vec3 halfwayDir = normalize(lightDir + viewDir);"
        "  float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);"
        "  vec3 specular = vec3(0,0,0);"
        "  if(useSpecularMap == 1){specular = (specularStrength * spec) * lightColor6 * vec3(texture(specularMap, texCoord));}"
        "  else{specular = (specularStrength * spec) * lightColor6;}"
        "  vec3 result = ((ambiant*=attenuation) + (diffuse*=attenuation) + (specular*=attenuation)) * shadowCoeff * final_color;"
        "  return(vec4(result,1.0));"
        "}"
        //""
        "void main() {"
        "  vec4 lightSummary = vec4(0.0,0.0,0.0,0.0);"
        "  if(numberOfLight >= 1){lightSummary += calculateLightOutput0();}"
        "  if(numberOfLight >= 2){lightSummary += calculateLightOutput1();}"
        "  if(numberOfLight >= 3){lightSummary += calculateLightOutput2();}"
        "  if(numberOfLight >= 4){lightSummary += calculateLightOutput3();}"
        "  if(numberOfLight >= 5){lightSummary += calculateLightOutput4();}"
        "  if(numberOfLight >= 6){lightSummary += calculateLightOutput5();}"
        "  if(numberOfLight == 7){lightSummary += calculateLightOutput6();}"
        // ""
        "  vec4 textureResult = vec4(1,1,1,1);"
        "  if(howManyTex >= 1){textureResult = texture(texture1,texCoord);}"
        "  if(howManyTex >= 2){textureResult = textureResult * texture(texture2,texCoord);}"
        "  if(howManyTex >= 3){textureResult = textureResult * texture(texture3,texCoord);}"
        "  "
        "  frag_colour = lightSummary * textureResult;"
        // GAMA CORRECTION
        "  frag_colour.rgb = pow(frag_colour.rgb, vec3(1.0 / gamma));"
        "}";

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertex_shader, NULL);
        glCompileShader(vs);
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragment_shader, NULL);
        glCompileShader(fs);

        m_shaderID = glCreateProgram();
        glAttachShader(m_shaderID, fs);
        glAttachShader(m_shaderID, vs);
        glLinkProgram(m_shaderID);

        glDeleteShader(vs);
        glDeleteShader(fs);

        init_light_system(); // MUST be called in order for the shader to work with light
    }

     void compileDepthShader()
    {
        std::cout << "---> Compiling depthShader for shader ID=" << id << std::endl;
        const char* geometry_shader =
            "#version 330 core\n"
            "layout(triangles) in;"
            "layout(triangle_strip, max_vertices = 18) out;"

            "uniform mat4 shadowMatrices[6];"
            "uniform int isPoint;"

            "out vec4 FragPos;" // FragPos from GS (output per emitvertex)

            "void main()"
            "{"
            " if(isPoint == 1){"
            "    for (int face = 0; face < 6; ++face)"
            "    {"
            "        gl_Layer = face;" // built-in variable that specifies to which face we render.
            "        for (int i = 0; i < 3; ++i)" // for each triangle vertex
            "        {"
            "            FragPos = gl_in[i].gl_Position;"
            "            gl_Position = shadowMatrices[face] * FragPos;"
            "            EmitVertex();"
            "        }"
            "        EndPrimitive();"
            "    }"
            "}"
            "else{"
            "    gl_Position = gl_in[0].gl_Position;"
            "    EmitVertex();"
            "    gl_Position = gl_in[1].gl_Position;"
            "    EmitVertex();"
            "    gl_Position = gl_in[2].gl_Position;"
            "    EmitVertex();"
            "    EndPrimitive();"
            "    FragPos = vec4(0,0,0,0);"
            "}"
            "}";

         const char* vertex_shader =
        "#version 330 core\n"
        "layout(location = 0) in vec3 vp;"
        "uniform mat4 model;"
        "uniform int isPoint;"
        "uniform mat4 lightSpaceMatrix;"
        "void main() {"
        "   if(isPoint == 1){"
        "   gl_Position = model * vec4(vp, 1.0);}else{"
        "   gl_Position = lightSpaceMatrix * model * vec4(vp, 1.0);}"
        "}";

        const char* fragment_shader =
        "#version 330 core\n"
        "in vec4 FragPos;"
        "uniform vec3 lightPos;"
        "uniform int isPoint;"
        //""
        "void main() {"
        "   if(isPoint == 1){"
        "   float lightDistance = length(FragPos.xyz - lightPos);"
        "   lightDistance = lightDistance / 25.0;"
        "   gl_FragDepth = lightDistance;}else{gl_FragDepth = gl_FragCoord.z;}"
        "}";

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertex_shader, NULL);
        glCompileShader(vs);
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragment_shader, NULL);
        glCompileShader(fs);
        GLuint gs = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gs, 1, &geometry_shader, NULL);
        glCompileShader(gs);

        m_shaderID = glCreateProgram();
        glAttachShader(m_shaderID, fs);
        glAttachShader(m_shaderID, vs);
        glAttachShader(m_shaderID, gs);
        glLinkProgram(m_shaderID);

        glDeleteShader(vs);
        glDeleteShader(fs);
        glDeleteShader(gs);
    }

    GLuint getShaderID(){return m_shaderID;}
    ~shader()
    {
        if (glIsProgram(m_shaderID))
        {
            glDeleteProgram(m_shaderID);
        }
    }
private:
    GLuint m_shaderID;
    int id;

    //Material matrix locations
    GLuint specStrenghtID;
    GLuint ambStrenghtID;

    //light matrix locations
        //light 0
        GLuint lightType0ID;
        GLuint lightColor0ID;
        GLuint lightPos0ID;
        GLuint lightDir0ID;
        GLuint lightStrength0ID;

        GLuint lightConstant0ID;
        GLuint lightLinear0ID;
        GLuint lightQuadratic0ID;

        GLuint lightCutOff0ID;

        //light 1
        GLuint lightType1ID;
        GLuint lightColor1ID;
        GLuint lightPos1ID;
        GLuint lightDir1ID;
        GLuint lightStrength1ID;

        GLuint lightConstant1ID;
        GLuint lightLinear1ID;
        GLuint lightQuadratic1ID;

        GLuint lightCutOff1ID;

        //light 2
        GLuint lightType2ID;
        GLuint lightColor2ID;
        GLuint lightPos2ID;
        GLuint lightDir2ID;
        GLuint lightStrength2ID;

        GLuint lightConstant2ID;
        GLuint lightLinear2ID;
        GLuint lightQuadratic2ID;

        GLuint lightCutOff2ID;

        //light 3
        GLuint lightType3ID;
        GLuint lightColor3ID;
        GLuint lightPos3ID;
        GLuint lightDir3ID;
        GLuint lightStrength3ID;

        GLuint lightConstant3ID;
        GLuint lightLinear3ID;
        GLuint lightQuadratic3ID;

        GLuint lightCutOff3ID;

        //light 4
        GLuint lightType4ID;
        GLuint lightColor4ID;
        GLuint lightPos4ID;
        GLuint lightDir4ID;
        GLuint lightStrength4ID;

        GLuint lightConstant4ID;
        GLuint lightLinear4ID;
        GLuint lightQuadratic4ID;

        GLuint lightCutOff4ID;

        //light 5
        GLuint lightType5ID;
        GLuint lightColor5ID;
        GLuint lightPos5ID;
        GLuint lightDir5ID;
        GLuint lightStrength5ID;

        GLuint lightConstant5ID;
        GLuint lightLinear5ID;
        GLuint lightQuadratic5ID;

        GLuint lightCutOff5ID;

        //light 6
        GLuint lightType6ID;
        GLuint lightColor6ID;
        GLuint lightPos6ID;
        GLuint lightDir6ID;
        GLuint lightStrength6ID;

        GLuint lightConstant6ID;
        GLuint lightLinear6ID;
        GLuint lightQuadratic6ID;

        GLuint lightCutOff6ID;

    GLuint numberOfLightID;

};
