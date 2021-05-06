#pragma once
#include <vector>
#include <GL/glew.h>
#include <string>
#include <iostream>

using namespace std;
class texturePool
{
public:
	GLuint getCacheTextureID(string path, bool isDiffuseTexture = false);
	void addTextureToCache(GLuint textureID, string path);
	vector<GLuint>* getTextureIDs();
	vector<string>* getTexturePaths();
	~texturePool();
private:
	vector<GLuint> textureIDs; // textures ids
	vector<string> texturePaths; // texture paths
	//First textureID corresponds to first texturePaths
};