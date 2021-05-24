#include "shader.h"
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <string>
#include <stdlib.h>
#include <iostream>
using namespace std;

shader::shader()
{
    metallic = 0.5;
    roughness = 0.5f;
    ambient = 0.1f;
    id = rand();
}

void shader::registerLightToRender(vector<light*> sceneLights, int numberOfLight, bool forwardRendering)
{
    //Send material properties
    glUniform1i(glGetUniformLocation(m_shaderID, "numberOfLight"), numberOfLight);

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
        glUniform4f(glGetUniformLocation(m_shaderID, (IDDir.append(to_string(i).append("]")).c_str())), sceneLights[i]->lightDirection.x, sceneLights[i]->lightDirection.y, sceneLights[i]->lightDirection.z, sceneLights[i]->lightDirection.w);
        glUniform1i(glGetUniformLocation(m_shaderID, (IDType.append(to_string(i).append("]")).c_str())), sceneLights[i]->type);
        glUniform1f(glGetUniformLocation(m_shaderID, (IDconstant.append(to_string(i).append("]")).c_str())), sceneLights[i]->constant);
        glUniform1f(glGetUniformLocation(m_shaderID, (IDLin.append(to_string(i).append("]")).c_str())), sceneLights[i]->linear);
        glUniform1f(glGetUniformLocation(m_shaderID, (IDQuad.append(to_string(i).append("]")).c_str())), sceneLights[i]->quadratic);
        glUniform1f(glGetUniformLocation(m_shaderID, (IDCutoff.append(to_string(i).append("]")).c_str())), sceneLights[i]->cutOff);
    }
}
void shader::compileQuadRenderShader()
{
    std::cout << "---> Compiling quadRender Shader" << std::endl;

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
void shader::compileSSAOShader()
{
    std::cout << "---> Compiling SSAO Shader" << std::endl;

    const char* vertex_shader =
        "#version 330 core\n"
        "layout(location = 0) in vec3 vp;"
        "layout(location = 1) in vec3 color;"
        "layout(location = 2) in vec2 inputTexCoord;"

        "out vec2 texCoord;"

        "void main() {"
        "   gl_Position = vec4(vp, 1.0);"
        "   texCoord = inputTexCoord;"
        "}";

    const char* fragment_shader =
        "#version 330 core\n"
        "out vec4 frag_colour;"
        "in vec2 texCoord;"
        "uniform sampler2D gPosition;"
        "uniform sampler2D gNormals;"
        "uniform sampler2D noiseTexture;"

        "uniform vec3 samples[64];"

        "int kernelSize = 64;"
        "float radius = 1.5;"
        "uniform float bias;"
        "const vec2 noiseScale = vec2(1620 / 4.0, 880 / 4.0);"

        "uniform mat4 projection;"
        "uniform mat4 view;"

        "void main() {"
        "vec3 fragPos = (view * vec4(texture(gPosition, texCoord).rgb,1.0)).xyz;"
        "vec3 normal = texture(gNormals, texCoord).rgb;"
        "vec3 randomVec = texture(noiseTexture, texCoord * noiseScale).rgb;"
        // create TBN change-of-basis matrix: from tangent-space to view-space
        "vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));"
        "vec3 bitangent = cross(normal, tangent);"
        "mat3 TBN = mat3(tangent, bitangent, normal);"
        "float occlusion = 0.0;"
        "for (int i = 0; i < kernelSize; ++i)"
        "{"
        // get sample position
        "vec3 samplePos = TBN * samples[i];" // from tangent to view-space
        "samplePos = fragPos + samplePos * radius;"

        // project sample position (to sample texture) (to get position on screen/texture)
        "vec4 offset = vec4(samplePos, 1.0);"
        "offset = projection * offset;" // from view to clip-space
        "offset.xyz /= offset.w;" // perspective divide
        "offset.xyz = offset.xyz * 0.5 + 0.5;" // transform to range 0.0 - 1.0

        // get sample depth
        "vec4 depthval = vec4(texture(gPosition, offset.xy).xyz,1.0);"
        "depthval = view * depthval;"
        "float sampleDepth = depthval.z;" // get depth value of kernel sample


        // range check & accumulate
        "float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));"
        "occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;"
        "}"
        "occlusion = 1.0 - (occlusion / kernelSize);"
        "frag_colour = vec4(occlusion,occlusion,occlusion,1.0);"
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
void shader::compileDefaultShader() //For now handles directionnal, spot and point lights
{
    std::cout << "---> Compiling PBR Shader" << std::endl;

    const char* vertex_shader =
        "#version 330 core\n"
        "layout(location = 0) in vec3 vp;"
        "layout(location = 1) in vec3 color;"
        "layout(location = 2) in vec2 inputTexCoord;"
        "out vec3 aNormals;"
        "out vec3 fragPos;"
        "out vec2 texCoord;"
        "out vec3 WorldPos;"
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
        "uniform sampler2D gPosition;"
        "uniform sampler2D gNormals;"
        "void main() {"
        "   gl_Position = vec4(vp, 1.0);"
        "   WorldPos = texture(gPosition, inputTexCoord).rgb;"
        "   aNormals = texture(gNormals, inputTexCoord).rgb;"
        "   final_color = color;"
        "   fragPos = texture(gPosition, inputTexCoord).rgb;"
        "   texCoord = inputTexCoord;"
        "   vs_out.FragPos = fragPos;"
        "   vs_out.Normal = aNormals;"
        "   vs_out.TexCoords = inputTexCoord;"
        "   vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);"
        "   vs_out.FragPosLightSpace1 = lightSpaceMatrix1 * vec4(fragPos, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 330 core\n"
        "uniform int numberOfLight;"
        "uniform vec3 lightsColor[100];"
        "uniform vec3 lightsPosition[100];"
        "uniform vec4 lightsDir[100];"
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
        // next important render things
        "uniform vec3 viewPos;"
        "in vec3 WorldPos;"
        "out vec4 frag_colour;"
        "in vec3 final_color;"
        "in vec3 fragPos;"
        "in vec2 texCoord;"
        "uniform mat4 lightSpaceMatrix[20];"
        "uniform sampler2D gAlbedoSpec;"
        "uniform sampler2D gNormals;"
        "uniform sampler2D gPosition;"
        "uniform sampler2D gMetallic;"
        "uniform sampler2D gRoughness;"
        "uniform sampler2D gAmbient;"
        "uniform sampler2D ssao;"
        "uniform samplerCube textureDepthCubemap[20];"
        "uniform sampler2D textureDepthMap[20];"
        "uniform float farShadow;"

        "const float PI = 3.14159265359;"
        //
        "float ShadowCalculationPL(vec3 fragPos_, vec3 lightPos, samplerCube tex)"
        "{"
        // get vector between fragment position and light position
        "vec3 fragToLight = fragPos_ - lightPos;"
        // now get current linear depth as the length between the fragment and light position
        "float currentDepth = length(fragToLight);"
        // now test for shadows
        "float shadow = 0.0;"
        "float bias = 0.0001;"
        "float samples = 3.0;"
        "float offset = 0.1;"
        "for (float x = -offset; x < offset; x += offset / (samples * 0.5))"
        "{"
        "    for (float y = -offset; y < offset; y += offset / (samples * 0.5))"
        "    {"
        "        for (float z = -offset; z < offset; z += offset / (samples * 0.5))"
        "        {"
        "            float closestDepth = texture(tex, fragToLight + vec3(x, y, z)).r;"
        "            closestDepth *= farShadow;"   // undo mapping [0;1]
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
        "float bias = 0.0001;"
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
        "vec3 fresnelSchlick(float cosTheta, vec3 F0)"
        "{"
        "   return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);"
        "}"
        "float DistributionGGX(vec3 N, vec3 H, float roughness)"
        "{"
        "    float a = roughness * roughness;"
        "    float a2 = a * a;"
        "    float NdotH = max(dot(N, H), 0.0);"
        "    float NdotH2 = NdotH * NdotH;"

        "    float num = a2;"
        "    float denom = (NdotH2 * (a2 - 1.0) + 1.0);"
        "    denom = PI * denom * denom;"

        "    return num / denom;"
        "}"

        "float GeometrySchlickGGX(float NdotV, float roughness)"
        "{"
        "    float r = (roughness + 1.0);"
        "    float k = (r * r) / 8.0;"

        "    float num = NdotV;"
        "    float denom = NdotV * (1.0 - k) + k;"

        "    return num / denom;"
        "}"
        "float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)"
        "{"
        "    float NdotV = max(dot(N, V), 0.0);"
        "    float NdotL = max(dot(N, L), 0.0);"
        "    float ggx2 = GeometrySchlickGGX(NdotV, roughness);"
        "    float ggx1 = GeometrySchlickGGX(NdotL, roughness);"

        "    return ggx1 * ggx2;"
        "}"
        //""
        "void main() {"
        "vec3 FragPos = texture(gPosition, texCoord).rgb;"
        "vec3 Normal = texture(gNormals, texCoord).rgb;"
        "vec3 Diffuse = texture(gAlbedoSpec, texCoord).rgb;"
        "float metallic = texture(gMetallic, texCoord).r;"
        "float roughness = texture(gRoughness, texCoord).r;"
        "float ambient = texture(gAmbient, texCoord).r;"

        " vec3 N = Normal;"
        " vec3 WorldPos = FragPos;"
        " vec3 V = normalize(viewPos - WorldPos);"
        " vec3 albedo = Diffuse;"

        " vec3 F0 = vec3(0.04);"
        " float metallic_ = metallic;"
        " float roughness_ = roughness;"

        " F0 = mix(F0, albedo, metallic_);"
        // reflectance equation
        "vec3 Lo = vec3(0.0);"
        "for(int i = 0;i<numberOfLight;i++)"
        "{"
        // We assume its a point light by default
        // calculate per-light radiance
        "    vec3 L = vec3(0.0);"
        "    if(lightsType[i] == 1){L = -vec3(lightsDir[i].x,lightsDir[i].y,lightsDir[i].z);}" // directionnal light
        "    else{L = normalize(lightsPosition[i] - WorldPos);}" // point light
        "    vec3 H = normalize(V + L);"
        "    float distance = length(lightsPosition[i] - WorldPos);"
        "    float attenuation = 1.0 / (lightsConstant[i] + lightsLinear[i] * distance + lightsQuadratic[i] * (distance));"
        "    vec3 radiance = vec3(0.0);"
        "    if(lightsType[i] == 1){radiance = lightsColor[i];}" // directionnal light
        "    else{radiance = lightsColor[i] * attenuation;}" // point light

            // cook-torrance brdf
        "    float NDF = DistributionGGX(N, H, roughness_);"
        "    float G = GeometrySmith(N, V, L, roughness_);"
        "    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);"

        "    vec3 kS = F;"
        "    vec3 kD = vec3(1.0) - kS;"
        "    kD *= 1.0 - metallic_;"

        "    vec3 numerator = NDF * G * F;"
        "    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);"
        "    vec3 specular = numerator / max(denominator, 0.001);"

        // add to outgoing radiance Lo
        "    float NdotL = max(dot(N, L), 0.0);"
        "    vec3 final = (kD * albedo / PI + specular) * radiance * NdotL;"
        // SHADOWS
        "    if(lightsDir[i].w == 1){"
        "    if(lightsType[i] == 1){float shadowval = ShadowCalculation(lightSpaceMatrix[i]*vec4(FragPos,1.0), textureDepthMap[i]); final *= (1-shadowval);}"
        "    else if(lightsType[i] == 0){float shadowval = ShadowCalculationPL(FragPos, lightsPosition[i], textureDepthCubemap[i]); final *= (1-shadowval);}}"
        "    Lo += final;"
        "}"
        "vec3 ambient_ = vec3(ambient) * albedo * texture(ssao,texCoord).r;"
        "vec3 color = ambient_ + Lo;"
        "if(ambient == 1.0){color -= Lo;}"
        "frag_colour = vec4(color, 1.0);"
        "}";

    ;

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

void shader::compileGShaderShader()
{
    std::cout << "---> Compiling GShader" << std::endl;

    const char* vertex_shader =
        "#version 330 core\n"
        "layout(location = 0) in vec3 vp;"
        "layout(location = 1) in vec3 color;"
        "layout(location = 2) in vec2 inputTexCoord;"
        "layout(location = 3) in vec3 normals;"
        "layout(location = 4) in vec3 tangent;"
        "layout(location = 5) in vec3 bitangent;"
        "out vec3 aNormals;"
        "out vec3 fragPos;"
        "out vec2 texCoord;"
        "out vec3 finalColor;"
        "out mat3 TBN;"
        "uniform mat4 model;"
        "uniform mat4 view;"
        "uniform mat4 projection;"
        "uniform int useNormalMap;"
        "void main() {"
        "   finalColor = color;"
        "   gl_Position = projection * view * model * vec4(vp, 1.0);"
        "   aNormals = normals;"
        "   vec4 worldPos = model * vec4(vp, 1.0);"
        "   fragPos = worldPos.xyz;"
        "   texCoord = inputTexCoord;"
        "   if(useNormalMap == 1){vec3 T = normalize(vec3(model * vec4(tangent, 0.0))); vec3 N = normalize(vec3(model * vec4(aNormals, 0.0))); T = normalize(T - dot(T, N) * N); vec3 B = cross(N, T); TBN = mat3(T, B, N);}"
        "}";

    const char* fragment_shader =
        "#version 330 core\n"
        "layout(location = 0) out vec4 gPosition;"
        "layout(location = 1) out vec4 gNormal;"
        "layout(location = 2) out vec4 gAlbedoSpec;"
        "layout(location = 3) out vec4 gRoughness;"
        "layout(location = 4) out vec4 gMetallic;"
        "layout(location = 5) out vec4 gAmbient;"
        "in vec3 aNormals;"
        "in vec3 fragPos;"
        "in vec2 texCoord;"
        "in vec3 finalColor;"
        "uniform int howManyTex;"
        "uniform mat4 model;"
        "uniform mat4 view;"
        "uniform mat4 projection;"
        "uniform mat4 inversedModel;"
        "in mat3 TBN;"
        "uniform float metallic;"
        "uniform float roughness;"
        "uniform float ambient;"
        // TEXTURES
        "uniform sampler2D texture1;"
        "uniform sampler2D texture2;"
        "uniform sampler2D texture3;"
        "uniform sampler2D normalMap;"
        "uniform int useNormalMap;"
        "uniform sampler2D roughnessMap;"
        "uniform int useRoughnessMap;"
        "uniform sampler2D metallicMap;"
        "uniform int useMetallicMap;"
        //""
        "void main() {"
        " if(useNormalMap == 0){"
        " gNormal = vec4(normalize(mat3(transpose(inversedModel)) * aNormals),1.0);}"
        " else{vec3 normal = texture(normalMap, texCoord).rgb; normal = normalize(normal * 2.0 - 1.0); normal = normalize(TBN * normal); gNormal = vec4(normal,1.0);}"
        " gAlbedoSpec = vec4(finalColor,1.0);"
        " if(howManyTex == 1){gAlbedoSpec = gAlbedoSpec * vec4(vec3(texture(texture1, texCoord)),1.0);}"
        " else if(howManyTex == 2){gAlbedoSpec = gAlbedoSpec * vec4(vec3(texture(texture1, texCoord)),1.0) * vec4(vec3(texture(texture2, texCoord)),1.0);}"
        " gPosition = vec4(fragPos,1.0);"
        " if(useMetallicMap == 0){gMetallic = vec4(vec3(metallic,metallic,metallic),1.0);}else{gMetallic = vec4(vec3(texture(metallicMap,texCoord)),1.0);}"
        " if(useRoughnessMap == 0){gRoughness = vec4(vec3(roughness,roughness,roughness),1.0);}else{gRoughness = vec4(vec3(texture(roughnessMap,texCoord)),1.0);}"
        " gAmbient = vec4(vec3(ambient,ambient,ambient),1.0);"
        " "
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

void shader::compileDepthShader()
{
    std::cout << "---> Compiling depthShader" << std::endl;
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
        "uniform float farShadow;"
        //""
        "void main() {"
        "   if(isPoint == 1){"
        "   float lightDistance = length(FragPos.xyz - lightPos);"
        "   lightDistance = lightDistance / farShadow;"
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

GLuint shader::getShaderID() { return m_shaderID; }
shader::~shader()
{
    if (glIsProgram(m_shaderID))
    {
        glDeleteProgram(m_shaderID);
    }
}