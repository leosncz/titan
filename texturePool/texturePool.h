#pragma once
#include <vector>
#include <GL/glew.h>
#include <string>
#include <iostream>
using namespace std;
class texturePool
{
public:
	GLuint getCacheTextureID(string path) {
		if (textureIDs.size() != 0) {
			for(int i=0;i<textureIDs.size()-1;i++){
				if (path == texturePaths[i]) { cout << "--> Texture " << path << " has been found in tex cache" << endl; return textureIDs[i]; }
			}
		}
		return -1;
	}
	void addTextureToCache(GLuint textureID, string path)
	{
		textureIDs.push_back(textureID);
		texturePaths.push_back(path);

		cout << "--> Texture " << path << " has been added to tex cache" << endl;
	}
private:
	vector<GLuint> textureIDs; // textures ids
	vector<string> texturePaths; // texture paths
	//First textureID corresponds to first texturePaths
};