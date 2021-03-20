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
#include <vector>
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
        numberOfLightID = glGetUniformLocation(m_shaderID,"numberOfLight");

        //Material properties
        specStrenghtID = glGetUniformLocation(m_shaderID,"specStrenght");
        ambStrenghtID = glGetUniformLocation(m_shaderID,"ambStrenght");
    }

    void registerLightToRender(vector<light*> sceneLights, int numberOfLight, float nearPlane=1.0f,float farPlane=50.0f) // the shader chosen must support light
    {
        //Send material properties
        glUniform1f(specStrenghtID, specStrenght);
        glUniform1f(ambStrenghtID, ambStrenght);
        glUniform1i(numberOfLightID, numberOfLight);

        for (int i = 0; i < sceneLights.size(); i++)
        {
            string IDcolors = "lightsColor[";
            string IDPos = "lightsPosition[";
            string IDDir = "lightsDir[";
            string IDconstant = "lightsConstant[";
            string IDLin = "lightsLinear[";
            string IDQuad = "lightsQuadratic[";
            string IDType = "lightsType[";
            string IDCutoff = "lightsCutoff[";

            glUniform3f(glGetUniformLocation(m_shaderID, (IDcolors.append(to_string(i).append("]")).c_str())), sceneLights[i]->lightColor.x, sceneLights[i]->lightColor.y, sceneLights[i]->lightColor.z);
            glUniform3f(glGetUniformLocation(m_shaderID, (IDPos.append(to_string(i).append("]")).c_str())), sceneLights[i]->lightPosition.x, sceneLights[i]->lightPosition.y, sceneLights[i]->lightPosition.z);
            glUniform3f(glGetUniformLocation(m_shaderID, (IDDir.append(to_string(i).append("]")).c_str())), sceneLights[i]->lightDirection.x, sceneLights[i]->lightDirection.y, sceneLights[i]->lightDirection.z);
            glUniform1i(glGetUniformLocation(m_shaderID, (IDType.append(to_string(i).append("]")).c_str())), sceneLights[i]->type);
            glUniform1f(glGetUniformLocation(m_shaderID, (IDconstant.append(to_string(i).append("]")).c_str())), sceneLights[i]->constant);
            glUniform1f(glGetUniformLocation(m_shaderID, (IDLin.append(to_string(i).append("]")).c_str())), sceneLights[i]->linear);
            glUniform1f(glGetUniformLocation(m_shaderID, (IDQuad.append(to_string(i).append("]")).c_str())), sceneLights[i]->quadratic);
            glUniform1f(glGetUniformLocation(m_shaderID, (IDCutoff.append(to_string(i).append("]")).c_str())), sceneLights[i]->cutOff);
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
        "uniform vec3 lightsColor[100];"
        "uniform vec3 lightsPosition[100];"
        "uniform vec3 lightsDir[100];"
        "uniform int lightsType[100];"
        "uniform float lightsConstant[100];"
        "uniform float lightsLinear[100];"
        "uniform float lightsQuadratic[100];"
        "uniform float lightsCutoff[100];"
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
        "float bias = 0.001;"
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
        "  if(lightsType[0] == 1){" //directionnal
        "    lightDir = normalize(-lightsDir[0]);"
        "    shadowCoeff = 1-ShadowCalculation(fs_in.FragPosLightSpace,textureDepthMap) * (1 - ambStrenght);"
        "  }"
        "  else if(lightsType[0] == 0){"
        "    lightDir = normalize(lightsPosition[0] - fragPos);" //point light
        "    float distance    = length(lightsPosition[0] - fragPos);"
        "    attenuation = 1.0 / (lightsConstant[0] + lightsLinear[0] * distance + lightsQuadratic[0] * (distance));"
        "    shadowCoeff = 1-ShadowCalculationPL(fs_in.FragPos,lightsPosition[0],textureDepthCubemap);"
        "  }"
        "  else if(lightsType[0] == 2){" // spotlight
        "    lightDir = normalize(lightsPosition[0] - fragPos);"
        "    float theta = dot(normalize(lightsPosition[0] - fragPos), normalize(-lightsDir[0]));"
        "    if(theta > lightsCutoff[0]){"
        "      float distance = length(lightsPosition[0] - fragPos);"
        "      attenuation = 1.0 / (lightsConstant[0] + lightsLinear[0] * distance + lightsQuadratic[0] * (distance * distance));"
        "    }"
        "    else{"
        "       attenuation = 0.1;"
        "    }"
        "  }"
        "  float diff = max(dot(norm, lightDir), 0.0);"
        "  vec3 diffuse = diff * lightsColor[0];"
        "  float ambiantStrength = ambStrenght;"
        "  vec3 ambiant = ambiantStrength * lightsColor[0];"
        "  float specularStrength = specStrenght;"
        "  vec3 viewDir = normalize(viewPos - fragPos);"
        "  vec3 reflectDir = reflect(-lightDir, norm);"
        "  vec3 halfwayDir = normalize(lightDir + viewDir);"
        "  float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);"
        "  vec3 specular = vec3(0,0,0);"
        "  if(useSpecularMap == 1){specular = (specularStrength * spec) * lightsColor[0] * vec3(texture(specularMap, texCoord));}"
        "  else{specular = (specularStrength * spec) * lightsColor[0];}"
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
        "  if(lightsType[1] == 1){" //directionnal
        "    lightDir = normalize(-lightsDir[1]);"
        "    shadowCoeff = (1 - ShadowCalculation(fs_in.FragPosLightSpace1,textureDepthMap1) * (1 - ambStrenght));"
        "   "
        "  }"
        "  else if(lightsType[1] == 0){lightDir = normalize(lightsPosition[1] - fragPos);" //point light
        "    float distance    = length(lightsPosition[1] - fragPos);"
        "    attenuation = 1.0 / (lightsConstant[1] + lightsLinear[1] * distance + lightsQuadratic[1] * (distance));"
        "    shadowCoeff = (1 - ShadowCalculationPL(fs_in.FragPos, lightsPosition[1], textureDepthCubemap1));"
        "  }"
        "  else if(lightsType[1] == 2){" // spotlight
        "    lightDir = normalize(lightsPosition[1] - fragPos);"
        "    float theta = dot(normalize(lightsPosition[1] - fragPos), normalize(-lightsDir[0]));"
        "    if(theta > lightsCutoff[1]) {"
        "      float distance = length(lightsPosition[1] - fragPos);"
        "      attenuation = 1.0 / (lightsConstant[1] + lightsLinear[1] * distance + lightsQuadratic[1] * (distance * distance));"
        "    }else{"
        "       attenuation = 0.1;"
        "    }"
        "  }"
        "  float diff = max(dot(norm, lightDir), 0.0);"
        "  vec3 diffuse = diff * lightsColor[1];"
        "  float ambiantStrength = ambStrenght;"
        "  vec3 ambiant = ambiantStrength * lightsColor[1];"
        "  float specularStrength = specStrenght;"
        "  vec3 viewDir = normalize(viewPos - fragPos);"
        "  vec3 reflectDir = reflect(-lightDir, norm);"
        "  vec3 halfwayDir = normalize(lightDir + viewDir);"
        "  float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);"
        "  vec3 specular = vec3(0,0,0);"
        "  if(useSpecularMap == 1){specular = (specularStrength * spec) * lightsColor[1] * vec3(texture(specularMap, texCoord));}"
        "  else{specular = (specularStrength * spec) * lightsColor[1];}"
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
        "  if(lightsType[2] == 1){" //directionnal
        "    lightDir = normalize(-lightsDir[2]);"
        "    "
        "   }"
        "  else if(lightsType[2] == 0){lightDir = normalize(lightsPosition[2] - fragPos);" //point light
        "    float distance    = length(lightsPosition[2] - fragPos);"
        "    attenuation = 1.0 / (lightsConstant[2] + lightsLinear[2] * distance + lightsQuadratic[2] * (distance));"
        "    shadowCoeff = (1-ShadowCalculationPL(fs_in.FragPos,lightsPosition[2],textureDepthCubemap2));"  
        "  }"
        "  else if(lightsType[2] == 2){" // spotlight
        "    lightDir = normalize(lightsPosition[2] - fragPos);"
        "    float theta = dot(normalize(lightsPosition[2] - fragPos), normalize(-lightsDir[2]));"
        "    if(theta > lightsCutoff[0]) {"
        "      float distance = length(lightsPosition[2] - fragPos);"
        "      attenuation = 1.0 / (lightsConstant[2] + lightsLinear[2] * distance + lightsQuadratic[2] * (distance * distance));"
        "    }else{"
        "       attenuation = 0.1;"
        "    }"
        "  }"
        "  float diff = max(dot(norm, lightDir), 0.0);"
        "  vec3 diffuse = diff * lightsColor[2];"
        "  float ambiantStrength = ambStrenght;"
        "  vec3 ambiant = ambiantStrength * lightsColor[2];"
        "  float specularStrength = specStrenght;"
        "  vec3 viewDir = normalize(viewPos - fragPos);"
        "  vec3 reflectDir = reflect(-lightDir, norm);"
        "  vec3 halfwayDir = normalize(lightDir + viewDir);"
        "  float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);"
        "  vec3 specular = vec3(0,0,0);"
        "  if(useSpecularMap == 1){specular = (specularStrength * spec) * lightsColor[2] * vec3(texture(specularMap, texCoord));}"
        "  else{specular = (specularStrength * spec) * lightsColor[2];}"
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
        "  if(lightsType[3] == 1){" //directionnal
        "    lightDir = normalize(-lightsDir[3]);"
        "  "
        "   }"
        "  else if(lightsType[3] == 0){lightDir = normalize(lightsPosition[3] - fragPos);" //point light
        "    float distance    = length(lightsPosition[3] - fragPos);"
        "    attenuation = 1.0 / (lightsConstant[3] + lightsLinear[3] * distance + lightsQuadratic[3] * (distance));"
        "    shadowCoeff = (1-ShadowCalculationPL(fs_in.FragPos,lightsPosition[3],textureDepthCubemap3));"
        "  }"
        "  else if(lightsType[3] == 2){" // spotlight
        "    lightDir = normalize(lightsPosition[3] - fragPos);"
        "    float theta = dot(normalize(lightsPosition[3] - fragPos), normalize(-lightsDir[3]));"
        "    if(theta > lightsCutoff[3]) {"
        "      float distance = length(lightsPosition[3] - fragPos);"
        "      attenuation = 1.0 / (lightsConstant[3] + lightsLinear[3] * distance + lightsQuadratic[3] * (distance * distance));"
        "    }else{"
        "       attenuation = 0.1;"
        "    }"
        "  }"
        "  float diff = max(dot(norm, lightDir), 0.0);"
        "  vec3 diffuse = diff * lightsColor[3];"
        "  float ambiantStrength = ambStrenght;"
        "  vec3 ambiant = ambiantStrength * lightsColor[3];"
        "  float specularStrength = specStrenght;"
        "  vec3 viewDir = normalize(viewPos - fragPos);"
        "  vec3 reflectDir = reflect(-lightDir, norm);"
        "  vec3 halfwayDir = normalize(lightDir + viewDir);"
        "  float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);"
        "  vec3 specular = vec3(0,0,0);"
        "  if(useSpecularMap == 1){specular = (specularStrength * spec) * lightsColor[3] * vec3(texture(specularMap, texCoord));}"
        "  else{specular = (specularStrength * spec) * lightsColor[3];}"
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
        "  if(lightsType[4] == 1){" //directionnal
        "    lightDir = normalize(-lightsDir[4]);"
        "  "
        "  }"
        "  else if(lightsType[4] == 0){lightDir = normalize(lightsPosition[4] - fragPos);" //point light
        "    float distance    = length(lightsPosition[4] - fragPos);"
        "    attenuation = 1.0 / (lightsConstant[4] + lightsLinear[4] * distance + lightsQuadratic[4] * (distance));"
        "    shadowCoeff = (1-ShadowCalculationPL(fs_in.FragPos,lightsPosition[4],textureDepthCubemap4));"
        "  }"
        "  else if(lightsType[4] == 2){" // spotlight
        "    lightDir = normalize(lightsPosition[4] - fragPos);"
        "    float theta = dot(normalize(lightsPosition[4] - fragPos), normalize(-lightsDir[4]));"
        "    if(theta > lightsCutoff[4]) {"
        "      float distance = length(lightsPosition[4] - fragPos);"
        "      attenuation = 1.0 / (lightsConstant[4] + lightsLinear[4] * distance + lightsQuadratic[4] * (distance * distance));"
        "    }else{"
        "       attenuation = 0.1;"
        "    }"
        "  }"
        "  float diff = max(dot(norm, lightDir), 0.0);"
        "  vec3 diffuse = diff * lightsColor[4];"
        "  float ambiantStrength = ambStrenght;"
        "  vec3 ambiant = ambiantStrength * lightsColor[4];"
        "  float specularStrength = specStrenght;"
        "  vec3 viewDir = normalize(viewPos - fragPos);"
        "  vec3 reflectDir = reflect(-lightDir, norm);"
        "  vec3 halfwayDir = normalize(lightDir + viewDir);"
        "  float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);"
        "  vec3 specular = vec3(0,0,0);"
        "  if(useSpecularMap == 1){specular = (specularStrength * spec) * lightsColor[4] * vec3(texture(specularMap, texCoord));}"
        "  else{specular = (specularStrength * spec) * lightsColor[4];}"
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
        "  if(lightsType[5] == 1){" //directionnal
        "    lightDir = normalize(-lightsDir[5]);"
        "   "
        "  }"
        "  else if(lightsType[5] == 0){lightDir = normalize(lightsPosition[5] - fragPos);" //point light
        "    float distance    = length(lightsPosition[5] - fragPos);"
        "    attenuation = 1.0 / (lightsConstant[5] + lightsLinear[5] * distance + lightsQuadratic[5] * (distance));"
        "    shadowCoeff = (1-ShadowCalculationPL(fs_in.FragPos,lightsPosition[5],textureDepthCubemap5));"
        "  }"
        "  else if(lightsType[5] == 2){" // spotlight
        "    lightDir = normalize(lightsPosition[5] - fragPos);"
        "    float theta = dot(normalize(lightsPosition[5] - fragPos), normalize(-lightsDir[5]));"
        "    if(theta > lightsCutoff[5]) {"
        "      float distance = length(lightsPosition[5] - fragPos);"
        "      attenuation = 1.0 / (lightsConstant[5] + lightsLinear[5] * distance + lightsQuadratic[5] * (distance * distance));"
        "    }else{"
        "       attenuation = 0.1;"
        "    }"
        "  }"
        "  float diff = max(dot(norm, lightDir), 0.0);"
        "  vec3 diffuse = diff * lightsColor[5];"
        "  float ambiantStrength = ambStrenght;"
        "  vec3 ambiant = ambiantStrength * lightsColor[5];"
        "  float specularStrength = specStrenght;"
        "  vec3 viewDir = normalize(viewPos - fragPos);"
        "  vec3 reflectDir = reflect(-lightDir, norm);"
        "  vec3 halfwayDir = normalize(lightDir + viewDir);"
        "  float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);"
        "  vec3 specular = vec3(0,0,0);"
        "  if(useSpecularMap == 1){specular = (specularStrength * spec) * lightsColor[5] * vec3(texture(specularMap, texCoord));}"
        "  else{specular = (specularStrength * spec) * lightsColor[5];}"
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
        "  if(lightsType[6] == 1){" //directionnal
        "    lightDir = normalize(-lightsDir[6]);"
        "   "
        "  }"
        "  else if(lightsType[6] == 0){lightDir = normalize(lightsPosition[6] - fragPos);" //point light
        "    float distance    = length(lightsPosition[6] - fragPos);"
        "    attenuation = 1.0 / (lightsConstant[6] + lightsLinear[6] * distance + lightsQuadratic[6] * (distance));"
        "    shadowCoeff = (1-ShadowCalculationPL(fs_in.FragPos,lightsPosition[6],textureDepthCubemap6));"
        "  }"
        "  else if(lightsType[6] == 2){" // spotlight
        "    lightDir = normalize(lightsPosition[6] - fragPos);"
        "    float theta = dot(normalize(lightsPosition[6] - fragPos), normalize(-lightsDir[6]));"
        "    if(theta > lightsCutoff[6]) {"
        "      float distance = length(lightsPosition[6] - fragPos);"
        "      attenuation = 1.0 / (lightsConstant[6] + lightsLinear[6] * distance + lightsQuadratic[6] * (distance * distance));"
        "    }else{"
        "       attenuation = 0.1;"
        "    }"
        "  }"
        "  float diff = max(dot(norm, lightDir), 0.0);"
        "  vec3 diffuse = diff * lightsColor[6];"
        "  float ambiantStrength = ambStrenght;"
        "  vec3 ambiant = ambiantStrength * lightsColor[6];"
        "  float specularStrength = specStrenght;"
        "  vec3 viewDir = normalize(viewPos - fragPos);"
        "  vec3 reflectDir = reflect(-lightDir, norm);"
        "  vec3 halfwayDir = normalize(lightDir + viewDir);"
        "  float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);"
        "  vec3 specular = vec3(0,0,0);"
        "  if(useSpecularMap == 1){specular = (specularStrength * spec) * lightsColor[6] * vec3(texture(specularMap, texCoord));}"
        "  else{specular = (specularStrength * spec) * lightsColor[6];}"
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
    GLuint numberOfLightID;

};
