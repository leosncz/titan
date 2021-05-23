#pragma once
#include "gui.h"
#include "imfilebrowser.h"
#include "../texturePool/texturePool.h"
class guiEngine : public gui
{
public:
	guiEngine(string tag, display* display_, scene* scene_);
	void update(scene* scene_);
private:
	bool m_showRenderingDebug, m_showSceneEditor, m_showLightingEditor, m_showHelloMessage, m_showExportMenu, m_showTexturePool;

	// These variables are used to save data in dialogs
	bool m_texturePathEditing, m_normalMapPathEditing, m_roughnessMapPathEditing, m_metallicMapPathEditing;
	float m_lpos[3];
	float m_ldir[3];
	float m_lcol[3];
	float m_lconstant, m_llinear, m_lquadratic;
	int m_lshadowResolution;
	bool m_lcomputeShadows;
	char m_chosenMeshName[10];
	bool m_chosenMeshInverseNormals;

	ImGui::FileBrowser m_fileDialog;
	string m_currentObjectEdit; // Tell which object we are currently editinh in the filebrowsing window
	bool m_editingImportMesh; // Tell if we are currently looking for importing an obj file

	GLuint m_textureLogo;

	void showLightingEditor(float* farShadow, vector<light*>* lights, scene* scene_);
	void showTexturePool(scene* scene_);
	void showExportMenu();
	void showRenderingDebug(float* exposure, float* gamma, float* ssaoBias, GLuint positionTexture, GLuint normalTexture, GLuint albedoSpecTexture, GLuint roughnessTexture, GLuint metallicTexture, GLuint ambientTexture, GLuint ssaoTexture);
	void showLightCreationDialog(scene* scene_);
	void showWelcome();
	void showSceneEditor(vector<renderObject*>* objectHolder, scene* scene_);
};
