#pragma once
#include <vector>
#include <GL/glew.h>
#include <string>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image/stb_image.h"
using namespace std;
class texturePool
{
public:
	GLuint getCacheTextureID(string path, bool isDiffuseTexture=false) {
		if (textureIDs.size() != 0) {
			for(int i=0;i<textureIDs.size();i++){
				if (path == texturePaths[i]) { cout << "--> Texture " << path << " has been found in tex cache" << endl; return textureIDs[i]; }
			}
		}
		//If not in cache
        stbi_set_flip_vertically_on_load(true);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

        GLuint texture = 0;

        if (data)
        {
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			if (isDiffuseTexture)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Error loading image at " << path << std::endl;
			stbi_image_free(data);
			glDeleteTextures(1, &texture);
			return 0;
        }
        stbi_image_free(data);

        addTextureToCache(texture,path);

        glBindTexture(GL_TEXTURE_2D, 0);
		return texture;
	}
	void addTextureToCache(GLuint textureID, string path)
	{
		textureIDs.push_back(textureID);
		texturePaths.push_back(path);

		cout << "--> Texture " << path << " (" << textureID << ") has been added to tex cache" << endl;
	}

	~texturePool()
	{
		cout << "--> Destroying textures" << endl;
		for (int i = 0; i < textureIDs.size(); i++)
		{
			cout << "---> Deleting texture " << textureIDs[i] << " : " << texturePaths[i] << endl;
			glDeleteTextures(1, &textureIDs[i]);
		}
	}
private:
	vector<GLuint> textureIDs; // textures ids
	vector<string> texturePaths; // texture paths
	//First textureID corresponds to first texturePaths
};