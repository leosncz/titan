#include "texturePool.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image/stb_image.h"

GLuint texturePool::getCacheTextureID(string path, bool isDiffuseTexture) {

	//First identify filename
	int lastSlashPosition = -1;
	string fileName = path;
	for (int i = 0; i < path.length(); i++)
	{
		if (path[i] == '/' || path[i] == '\\')
		{
			lastSlashPosition = i;
		}
	}
	fileName.erase(0, lastSlashPosition + 1);

	if (textureIDs.size() != 0) {
		//Check if the texture exists
		for (int i = 0; i < textureIDs.size(); i++) {
			if (fileName == texturePaths[i]) { cout << "--> Texture " << fileName << " has been found in tex cache" << endl; return textureIDs[i]; }
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
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &texture);
		return 0;
	}
	stbi_image_free(data);

	addTextureToCache(texture, fileName);

	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

void texturePool::addTextureToCache(GLuint textureID, string path)
{
	textureIDs.push_back(textureID);
	texturePaths.push_back(path);

	cout << "--> Texture " << path << " (texID " << textureID << ") has been added to tex cache" << endl;
}

vector<GLuint>* texturePool::getTextureIDs()
{
	return &textureIDs;
}

vector<string>* texturePool::getTexturePaths()
{
	return &texturePaths;
}

texturePool::~texturePool()
{
	cout << "--> Destroying textures" << endl;
	for (int i = 0; i < textureIDs.size(); i++)
	{
		cout << "---> Deleting texture " << textureIDs[i] << " : " << texturePaths[i] << endl;
		glDeleteTextures(1, &textureIDs[i]);
	}
}