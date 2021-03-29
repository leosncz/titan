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

    void registerLightToRender(vector<light*> sceneLights, int numberOfLight) // the shader chosen must support light
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
    void compileQuadRenderShader()
    {
        std::cout << "---> Compiling quadRenderShader for scene"<< std::endl;

        const char* vertex_shader =
            "#version 330 core\n"
            "layout(location = 0) in vec3 vp;"
            "layout(location = 1) in vec3 color;"
            "layout(location = 2) in vec2 inputTexCoord;"
           
            "out vec2 texCoord;"
            "out vec3 final_color;"
            
            "void main() {"
            "   gl_Position = vec4(vp, 1.0);"
            "   final_color = color;"
            "   texCoord = inputTexCoord;"
            "}";

        const char* fragment_shader =
            "#version 330 core\n"
            "out vec4 frag_colour;"
            "in vec3 final_color;"
            "in vec2 texCoord;"
            "uniform sampler2D hdrTexture;"
            "uniform float gamma;"
            "uniform float exposure;"
           
            "void main() {"
            // GAMA AND HDR CORRECTION
            "  vec3 hdrColor = texture(hdrTexture, texCoord).rgb;"
            "  vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);"
            "  mapped = pow(mapped, vec3(1.0 / gamma));"
            "  frag_colour = vec4(mapped, 1.0);"
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
        "layout(location = 4) in vec3 tangent;"
        "layout(location = 5) in vec3 bitangent;"
        "out vec3 aNormals;"
        "out vec3 fragPos;"
        "out vec2 texCoord;"
        "out mat3 TBN;"
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
        "uniform int useNormalMap;"
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
        "   if(useNormalMap == 1){vec3 T = normalize(vec3(model * vec4(tangent, 0.0))); vec3 B = normalize(vec3(model * vec4(bitangent, 0.0))); vec3 N = normalize(vec3(model * vec4(aNormals, 0.0))); TBN = mat3(T, B, N);}"
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
        "in VS_OUT{"
        "vec3 FragPos;"
        "vec3 Normal;"
        "vec2 TexCoords;"
        "vec4 FragPosLightSpace;"
        "vec4 FragPosLightSpace1;"
        "} fs_in;"
        //number of lights
        "uniform int numberOfLight;"
        // next important render things
        "uniform vec3 viewPos;"
        "out vec4 frag_colour;"
        "in vec3 final_color;"
        "in vec3 aNormals;"
        "in vec3 fragPos;"
        "in vec2 texCoord;"
        "uniform int howManyTex;"
        "in mat3 TBN;"
        // TEXTURES
        "uniform sampler2D texture1;"
        "uniform sampler2D texture2;"
        "uniform sampler2D texture3;"
        "uniform samplerCube textureDepthCubemap;"
        "uniform samplerCube textureDepthCubemap1;"
        "uniform samplerCube textureDepthCubemap2;"
        "uniform samplerCube textureDepthCubemap3;"
        "uniform samplerCube textureDepthCubemap4;"
        "uniform samplerCube textureDepthCubemap5;"
        "uniform samplerCube textureDepthCubemap6;"
        "uniform sampler2D textureDepthMap;"
        "uniform sampler2D textureDepthMap1;"
        "uniform sampler2D specularMap;"
        "uniform sampler2D normalMap;"
        "uniform int useNormalMap;"
        "uniform int useSpecularMap;"
        // METHODS
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
        "vec4 calculateLightOutput(){"
        "  vec4 lightSummary = vec4(0,0,0,0);"
        "  for(int i = 0;i<numberOfLight;i++){"
        "    vec3 norm = vec3(0,0,0);"
        "    if(useNormalMap == 0){"
        "    norm = normalize(aNormals);}"
        "    else{vec3 normal = texture(normalMap, texCoord).rgb; normal = normalize(normal * 2.0 - 1.0); normal = normalize(TBN * normal); norm = normal;}"
        "    vec3 lightDir;"
        "    float attenuation = 1.0;"
        "    float shadowCoeff = 1.0;"
        "  if(lightsType[i] == 1){" //directionnal
        "    lightDir = normalize(-lightsDir[i]);"
        "    if(i == 0){shadowCoeff = 1-ShadowCalculation(fs_in.FragPosLightSpace,textureDepthMap) * (1 - ambStrenght);}"
        "    else if(i == 1){shadowCoeff = 1-ShadowCalculation(fs_in.FragPosLightSpace1,textureDepthMap1) * (1 - ambStrenght);}"
        "  }"
        "  else if(lightsType[i] == 0){"
        "    lightDir = normalize(lightsPosition[i] - fragPos);" //point light
        "    float distance    = length(lightsPosition[i] - fragPos);"
        "    attenuation = 1.0 / (lightsConstant[i] + lightsLinear[i] * distance + lightsQuadratic[i] * (distance));"
        "    if(i == 0){shadowCoeff = (1-ShadowCalculationPL(fs_in.FragPos,lightsPosition[i],textureDepthCubemap));}"
        "    else if(i == 1){shadowCoeff = (1-ShadowCalculationPL(fs_in.FragPos,lightsPosition[i],textureDepthCubemap1));}"
        "    else if(i == 2){shadowCoeff = (1-ShadowCalculationPL(fs_in.FragPos,lightsPosition[i],textureDepthCubemap2));}"
        "    else if(i == 3){shadowCoeff = (1-ShadowCalculationPL(fs_in.FragPos,lightsPosition[i],textureDepthCubemap3));}"
        "    else if(i == 4){shadowCoeff = (1-ShadowCalculationPL(fs_in.FragPos,lightsPosition[i],textureDepthCubemap4));}"
        "    else if(i == 5){shadowCoeff = (1-ShadowCalculationPL(fs_in.FragPos,lightsPosition[i],textureDepthCubemap5));}"
        "    else if(i == 6){shadowCoeff = (1-ShadowCalculationPL(fs_in.FragPos,lightsPosition[i],textureDepthCubemap6));}"
        "    if(shadowCoeff < ambStrenght && distance > 25.0){shadowCoeff = 1.0;}"
        "    if(shadowCoeff < ambStrenght){shadowCoeff = ambStrenght;}"
        "  }"
        "  else if(lightsType[i] == 2){" // spotlight
        "    lightDir = normalize(lightsPosition[i] - fragPos);"
        "    float theta = dot(normalize(lightsPosition[i] - fragPos), normalize(-lightsDir[i]));"
        "    if(theta > lightsCutoff[i]){"
        "      float distance = length(lightsPosition[i] - fragPos);"
        "      attenuation = 1.0 / (lightsConstant[i] + lightsLinear[i] * distance + lightsQuadratic[i] * (distance * distance));"
        "    }"
        "    else{"
        "       attenuation = 0.1;"
        "    }"
        "  }"
        "  float diff = max(dot(norm, lightDir), 0.0);"
        "  vec3 diffuse = diff * lightsColor[i];"
        "  float ambiantStrength = ambStrenght;"
        "  vec3 ambiant = ambiantStrength * lightsColor[i];"
        "  float specularStrength = specStrenght;"
        "  vec3 viewDir = normalize(viewPos - fragPos);"
        "  vec3 reflectDir = reflect(-lightDir, norm);"
        "  vec3 halfwayDir = normalize(lightDir + viewDir);"
        "  float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);"
        "  vec3 specular = vec3(0,0,0);"
        "  if(useSpecularMap == 1){specular = (specularStrength * spec) * lightsColor[i] * vec3(texture(specularMap, texCoord));}"
        "  else{specular = (specularStrength * spec) * lightsColor[i];}"
        "  lightSummary += vec4(((ambiant*=attenuation) + (diffuse*=attenuation) + (specular*=attenuation)) * shadowCoeff * final_color,1.0);"
        "  }"
        "  return lightSummary;"
        "}"
        //""
        "void main() {"
        // APPLY LIGHT EFFECT
        "  vec4 lightSummary = calculateLightOutput();"
        // APPLY TEXTURE
        "  vec4 textureResult = vec4(1,1,1,1);"
        "  if(howManyTex >= 1){textureResult = texture(texture1,texCoord);}"
        "  if(howManyTex >= 2){textureResult = textureResult * texture(texture2,texCoord);}"
        "  if(howManyTex >= 3){textureResult = textureResult * texture(texture3,texCoord);}"
        "  if(lightSummary == 0){frag_colour = ambStrenght * textureResult;}else{frag_colour = lightSummary * textureResult;}"
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
