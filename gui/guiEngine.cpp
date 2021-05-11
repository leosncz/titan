#include "guiEngine.h"
#include "../version/version.h"
#include "../renderObjectScene/renderObjectScene.h"
#include "../prefab/cube/cube.h"
#include "../prefab/plane/plane.h"

guiEngine::guiEngine(display* display_, scene* scene_) : gui(display_)
{
	m_showLightingEditor = false;
	m_showRenderingDebug = false;
	m_showSceneEditor = false;
	m_showHelloMessage = true;
	m_showExportMenu = false;
	m_showTexturePool = false;
	m_texturePathEditing = false;
	m_normalMapPathEditing = false;
	m_roughnessMapPathEditing = false;
	m_metallicMapPathEditing = false;
	m_lconstant = 1.0;
	m_llinear = 1.0;
	m_lquadratic = 1.0;
	m_lshadowResolution = 500;
	m_lcomputeShadows = true;
	m_chosenMeshInverseNormals = false;
	m_currentObjectEdit = -1;
	m_editingImportMesh = false; 
	m_lpos[0] = 0.0;
	m_lpos[1] = 0.0;
	m_lpos[2] = 0.0;

	m_ldir[0] = 0.0;
	m_ldir[1] = -0.3;
	m_ldir[2] = -1.0;

	m_lcol[0] = 10.0;
	m_lcol[1] = 10.0;
	m_lcol[2] = 10.0;

	m_textureLogo = scene_->getTexturePool()->getCacheTextureID("graphicData/logo.jpg", false);
}

void guiEngine::update(scene* scene_)
{
	if (glfwGetKey(m_display->getGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		isVisible = !isVisible;
		if (isVisible == true)
		{
			camera* cam = scene_->getCamera();
			cam->pauseControls();
		}
		else
		{
			camera* cam = scene_->getCamera();
			cam->resumeControls();
		}
	}
	if (isVisible)
	{
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (m_showHelloMessage)
		{
			showWelcome();
		}
		if (m_showRenderingDebug)
		{
			showRenderingDebug(scene_->getExposure(), scene_->getGamma(), scene_->getSSAOBias(), scene_->getGPosition(), scene_->getGNormals(), scene_->getGAlbedo(), scene_->getGRoughness(), scene_->getGMetallic(), scene_->getGAmbient(), scene_->getSSAO());
		}
		if (m_showSceneEditor)
		{
			showSceneEditor(scene_->getObjectHolder(), scene_);
		}
		if (m_showLightingEditor)
		{
			showLightingEditor(scene_->getFarShadow(), scene_->getLights(), scene_);
		}
		if (m_showExportMenu)
		{
			showExportMenu();
		}
		if (m_showTexturePool)
		{
			showTexturePool(scene_);
		}

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Rendering debug"))
			{
				m_showRenderingDebug = true;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Lighting editor"))
			{
				m_showLightingEditor = true;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Scene editor"))
			{
				m_showSceneEditor = true;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View texture cache"))
			{
				m_showTexturePool = true;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Export this project"))
			{
				m_showExportMenu = true;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Quit"))
			{
				m_display->setExitStatus(true);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
void guiEngine::showLightingEditor(float* farShadow, vector<light*>* lights, scene* scene_)
{
	ImGui::Begin("Lighting editor", &m_showLightingEditor);
	ImGui::SetWindowFontScale(1.1);

	ImGui::SliderFloat("Far shadow", farShadow, 2.0, 200.0);
	ImGui::Separator();

	ImGui::TextWrapped("Create new light : ");

	showLightCreationDialog(scene_);

	ImGui::Separator();

	if (lights->size() == 0)
	{
		ImGui::TextWrapped("No light !");
	}
	else
	{
		for (int i = 0; i < lights->size(); i++)
		{
			IM_ASSERT(lights->at(i)->textureDepthMap);
			string content = "Light ";
			content.append(std::to_string(i));
			content.append(" :");
			ImGui::TextWrapped(content.c_str());

			content = "Shadow resolution: ";
			content.append(to_string((lights->at(i)->shadowResolution)));
			ImGui::TextWrapped(content.c_str());

			if (lights->at(i)->type == DIRECTIONNAL_LIGHT)
			{
				ImGui::TextWrapped("Type: Directionnal");
				content = "ShadowMap (if enabled): ";
				ImGui::TextWrapped(content.c_str());
				ImGui::Image((void*)(intptr_t)lights->at(i)->textureDepthMap, ImVec2(400, 300), ImVec2(0, 1), ImVec2(1, 0));
			}
			else if (lights->at(i)->type == POINT_LIGHT)
			{
				ImGui::TextWrapped("Type: Point light");
			}

			content = "Position ##";
			content.append(to_string(i));
			float lightPos[3] = { lights->at(i)->lightPosition.x, lights->at(i)->lightPosition.y, lights->at(i)->lightPosition.z };
			if (ImGui::InputFloat3(content.c_str(), lightPos))
			{
				lights->at(i)->lightPosition.x = lightPos[0];
				lights->at(i)->lightPosition.y = lightPos[1];
				lights->at(i)->lightPosition.z = lightPos[2];
			}

			content = "Color ##";
			content.append(to_string(i));
			float lightCol[3] = { lights->at(i)->lightColor.r, lights->at(i)->lightColor.g, lights->at(i)->lightColor.b };
			if (ImGui::InputFloat3(content.c_str(), lightCol))
			{
				lights->at(i)->lightColor.r = lightCol[0];
				lights->at(i)->lightColor.g = lightCol[1];
				lights->at(i)->lightColor.b = lightCol[2];
			}

			content = "Constant ##";
			content.append(to_string(i));
			ImGui::SliderFloat(content.c_str(), &lights->at(i)->constant, 0.0, 4.0);

			content = "Linear ##";
			content.append(to_string(i));
			ImGui::SliderFloat(content.c_str(), &lights->at(i)->linear, 0.0, 4.0);

			content = "Quadratic ##";
			content.append(to_string(i));
			ImGui::SliderFloat(content.c_str(), &lights->at(i)->quadratic, 0.0, 4.0);

			content = "Delete ";
			content.append(to_string(i));
			if (ImGui::Button(content.c_str()))
			{
				scene_->deleteLight(i);
			}

			ImGui::Separator();
		}
	}
	ImGui::End();
}

void guiEngine::showTexturePool(scene* scene_)
{
	ImGui::Begin("Show texture cache", &m_showTexturePool);
	ImGui::SetWindowFontScale(1.1);

	ImGui::TextColored(ImVec4(1, 0, 0, 1), "Texture Pool: ");

	texturePool* texPool = scene_->getTexturePool();
	vector<string>* texPoolNames = texPool->getTexturePaths();

	for (int i = 0; i < texPoolNames->size(); i++)
	{
		ImGui::TextWrapped(texPoolNames->at(i).c_str());
	}

	ImGui::End();
}

void guiEngine::showExportMenu()
{
	ImGui::Begin("Export your project", &m_showExportMenu);
	ImGui::SetWindowFontScale(1.1);

	ImGui::TextWrapped("Select target device :");

	ImGui::Button("Microsoft Windows");
	ImGui::Button("Apple Mac OS (must support OpenGL)");
	ImGui::Button("Linux");

	ImGui::Separator();

	char output[] = "C:\\Users\\...";
	ImGui::TextWrapped("Select output path: ");
	ImGui::InputText("Output path", output, 10);

	ImGui::Button("Export");
	ImGui::End();
}

void guiEngine::showRenderingDebug(float* exposure, float* gamma, float* ssaoBias, GLuint positionTexture, GLuint normalTexture, GLuint albedoSpecTexture, GLuint roughnessTexture, GLuint metallicTexture, GLuint ambientTexture, GLuint ssaoTexture)
{
	ImGui::Begin("Deferred rendering debug", &m_showRenderingDebug);
	ImGui::SetWindowFontScale(1.1);

	char* renderer = (char*)glGetString(GL_RENDERER);
	char* vendor = (char*)glGetString(GL_VENDOR);
	ImGui::TextWrapped(renderer);
	ImGui::TextWrapped(vendor);
	ImGui::Separator();

	ImGui::SliderFloat("Gamma", gamma, 0.0, 5.0);
	ImGui::SliderFloat("Exposure", exposure, 0.0, 12.0);
	ImGui::SliderFloat("SSAO Bias", ssaoBias, 0.0, 0.9);


	ImGui::Separator();

	ImGui::TextWrapped("SSAO : ");
	IM_ASSERT(ssaoTexture);
	ImGui::Image((void*)(intptr_t)ssaoTexture, ImVec2(400, 300), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::Separator();

	ImGui::TextWrapped("Position: ");
	IM_ASSERT(positionTexture);
	ImGui::Image((void*)(intptr_t)positionTexture, ImVec2(400, 300), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::Separator();

	ImGui::TextWrapped("Normals: ");
	IM_ASSERT(normalTexture);
	ImGui::Image((void*)(intptr_t)normalTexture, ImVec2(400, 300), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::Separator();

	ImGui::TextWrapped("Albedo: ");
	IM_ASSERT(albedoSpecTexture);
	ImGui::Image((void*)(intptr_t)albedoSpecTexture, ImVec2(400, 300), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::Separator();

	ImGui::TextWrapped("Roughness: ");
	IM_ASSERT(roughnessTexture);
	ImGui::Image((void*)(intptr_t)roughnessTexture, ImVec2(400, 300), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::Separator();

	ImGui::TextWrapped("Metallic: ");
	IM_ASSERT(metallicTexture);
	ImGui::Image((void*)(intptr_t)metallicTexture, ImVec2(400, 300), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::Separator();

	ImGui::TextWrapped("Ambient: ");
	IM_ASSERT(ambientTexture);
	ImGui::Image((void*)(intptr_t)ambientTexture, ImVec2(400, 300), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}

void guiEngine::showLightCreationDialog(scene* scene_)
{

	ImGui::InputFloat3("Light Position", m_lpos);
	ImGui::InputFloat3("Light Direction", m_ldir);
	ImGui::InputFloat3("Light Color", m_lcol);
	ImGui::SliderFloat("Constant", &m_lconstant, 0.0, 4.0);
	ImGui::SliderFloat("Linear", &m_llinear, 0.0, 4.0);
	ImGui::SliderFloat("Quadratic", &m_lquadratic, 0.0, 4.0);
	ImGui::SliderInt("Shadow resolution", &m_lshadowResolution, 100, 20000);
	ImGui::Checkbox("Compute shadow", &m_lcomputeShadows);

	if (ImGui::Button("Create point light"))
	{
		light* mylight = new light(POINT_LIGHT, glm::vec3(m_lpos[0], m_lpos[1], m_lpos[2]), glm::vec3(m_lcol[0], m_lcol[1], m_lcol[2]), glm::vec3(m_ldir[0], m_ldir[1], m_ldir[2]), m_lcomputeShadows, m_lshadowResolution, m_lconstant, m_llinear, m_lquadratic);
		mylight->mustBeDeleted = true;

		scene_->addLight(mylight);
	}
	ImGui::SameLine();
	if (ImGui::Button("Create directionnal light"))
	{
		light* mylight = new light(DIRECTIONNAL_LIGHT, glm::vec3(m_lpos[0], m_lpos[1], m_lpos[2]), glm::vec3(m_lcol[0], m_lcol[1], m_lcol[2]), glm::vec3(m_ldir[0], m_ldir[1], m_ldir[2]), m_lcomputeShadows, m_lshadowResolution, m_lconstant, m_llinear, m_lquadratic);
		mylight->mustBeDeleted = true;

		scene_->addLight(mylight);
	}
}

void guiEngine::showWelcome()
{
	//Welcome
	ImGui::Begin("Hello", &m_showHelloMessage);
	ImGui::SetWindowSize(ImVec2(460, 550));
	ImGui::SetWindowFontScale(1.1);
	IM_ASSERT(m_textureLogo);
	ImGui::Image((void*)(intptr_t)m_textureLogo, ImVec2(450, 370), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::TextColored(ImVec4(0, 1, 0, 1), "Welcome to TITAN Engine !");
	string textVersion = "TiTan ";
	textVersion.append(version::getTitanVersion());
	ImGui::TextWrapped(textVersion.c_str());
	ImGui::TextWrapped("Press ESCAPE key to show/hide this interface");
	ImGui::TextColored(ImVec4(1, 0, 0, 1), "From github.com/leosncz/titan");
	ImGui::End();
}

void guiEngine::showSceneEditor(vector<renderObject*>* objectHolder, scene* scene_)
{
	//Scene information
	ImGui::Begin("Scene editor", &m_showSceneEditor);
	ImGui::SetWindowFontScale(1.1);
	if (ImGui::Button("Import object (.obj)"))
	{
		m_fileDialog.SetTypeFilters({ ".obj", ".test" });
		m_fileDialog.SetTitle("Import OBJ file");
		m_fileDialog.Open();
		m_editingImportMesh = true;
	}
	if (m_editingImportMesh)
	{
		m_fileDialog.Display();
		if (m_fileDialog.HasSelected())
		{
			renderObjectScene importModel(scene_);
			importModel.loadOBJFromFile(m_fileDialog.GetSelected().string().c_str());
			m_fileDialog.Close();
			m_editingImportMesh = false;
		}
		m_fileDialog.ClearSelected();
	}
	ImGui::Separator();
	ImGui::InputText("Object name", m_chosenMeshName, 10);
	ImGui::Checkbox("Inverse normals", &m_chosenMeshInverseNormals);
	if (ImGui::Button("Create Cube"))
	{
		cube* cube_ = 0;
		if (m_chosenMeshInverseNormals)
		{
			cube_ = new cube(scene_->getTexturePool(), 0, true, m_chosenMeshName);
		}
		else
		{
			cube_ = new cube(scene_->getTexturePool(), 0, false, m_chosenMeshName);
		}
		cube_->setDeleteStatus(true);
		scene_->addDrawableObject(cube_);
	}
	else if (ImGui::Button("Create Plane"))
	{
		plane* plane_ = 0;
		if (m_chosenMeshInverseNormals)
		{
			plane_ = new plane(scene_->getTexturePool(), 0, true, m_chosenMeshName);
		}
		else
		{
			plane_ = new plane(scene_->getTexturePool(), 0, false, m_chosenMeshName);
		}
		plane_->setDeleteStatus(true);
		scene_->addDrawableObject(plane_);
	}
	ImGui::Separator();


	vector<renderObject*>* objs = objectHolder;
	for (int i = 0; i < objectHolder->size(); i++)
	{
		renderObject* object = objs->at(i);
		string name = "ID : ";
		name.append(to_string(object->getID()));
		ImGui::Text(name.c_str());

		name = "Tag : ";
		name.append(object->getTag());
		ImGui::Text(name.c_str());

		name = "Position ##";
		name.append(to_string(object->getID()));
		float* objPosition = object->getPosition();
		if (ImGui::InputFloat3(name.c_str(), objPosition))
		{
			object->moveObjectFromSceneOrigin(glm::vec3(objPosition[0], objPosition[1], objPosition[2]));
		}

		name = "Scale ##";
		name.append(to_string(object->getID()));
		float* objScale = object->getScale();
		if (ImGui::InputFloat3(name.c_str(), objScale))
		{
			object->scaleObject(glm::vec3(objScale[0], objScale[1], objScale[2]));
		}

		name = "Metallic ##";
		name.append(to_string(object->getID()));
		ImGui::SliderFloat(name.c_str(), &object->getShader()->metallic, 0.0, 1.0);

		name = "Roughness ##";
		name.append(to_string(object->getID()));
		ImGui::SliderFloat(name.c_str(), &object->getShader()->roughness, 0.0, 1.0);

		name = "Ambient ##";
		name.append(to_string(object->getID()));
		ImGui::SliderFloat(name.c_str(), &object->getShader()->ambient, 0.0, 1.0);

		if (object->getNbOfTextures() < 2)
		{
			name = "Import texture ##";
			name.append(to_string(object->getID()));
			if (ImGui::Button(name.c_str())) { m_currentObjectEdit = object->getID(); m_fileDialog.SetTitle("Choose texture file"); m_fileDialog.SetTypeFilters({ ".jpg", ".jpeg", ".png" }); m_fileDialog.Open(); m_texturePathEditing = true; }
			if (m_texturePathEditing && m_currentObjectEdit == object->getID())
			{
				m_fileDialog.Display();
				if (m_fileDialog.HasSelected())
				{
					object->addTexture(m_fileDialog.GetSelected().string().c_str());
					m_fileDialog.ClearSelected();
					m_texturePathEditing = false;
				}
			}
		}
		ImGui::Text("Texture 1:");
		if (object->getNbOfTextures() == 0) { ImGui::Text("No"); }
		else
		{
			IM_ASSERT(object->getTextures()[0]); ImGui::Image((void*)(intptr_t)object->getTextures()[0], ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
			string name_ = "Delete texture 1";
			name_.append(" ##");
			name_.append(to_string(object->getID()));
			ImGui::SameLine();
			if (ImGui::Button(name_.c_str())) { object->removeTexture1(); }
		}

		if (object->getNbOfTextures() == 2)
		{
			ImGui::Text("Texture 2:");
			if (object->getNbOfTextures() == 1) { ImGui::Text("No"); }
			else
			{
				IM_ASSERT(object->getTextures()[1]); ImGui::Image((void*)(intptr_t)object->getTextures()[1], ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
				string name_ = "Delete texture 2";
				name_.append(" ##");
				name_.append(to_string(object->getID()));
				ImGui::SameLine();
				if (ImGui::Button(name_.c_str())) { object->removeTexture2(); }
			}
		}

		ImGui::Text("Normal map:");
		if (!object->doesMeshHasNormalMap()) { ImGui::Text("No"); }
		else
		{
			IM_ASSERT(object->getNormalMap()); ImGui::Image((void*)(intptr_t)object->getNormalMap(), ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
		}

		name = "Import normal map ##";
		name.append(to_string(object->getID()));
		ImGui::SameLine();
		if (ImGui::Button(name.c_str())) { m_currentObjectEdit = object->getID(); m_fileDialog.SetTitle("Choose normal map file"); m_fileDialog.SetTypeFilters({ ".jpg", ".jpeg", ".png" }); m_fileDialog.Open(); m_normalMapPathEditing = true; }
		if (object->doesMeshHasNormalMap())
		{
			string name_ = "Delete normal map";
			name_.append(" ##");
			name_.append(to_string(object->getID()));
			ImGui::SameLine();
			if (ImGui::Button(name_.c_str())) { object->removeNormalMap(); }
		}
		if (m_normalMapPathEditing && m_currentObjectEdit == object->getID())
		{
			m_fileDialog.Display();
			if (m_fileDialog.HasSelected())
			{
				object->setNormalMap(m_fileDialog.GetSelected().string().c_str());
				m_fileDialog.ClearSelected();
				m_normalMapPathEditing = false;
			}
		}


		ImGui::Text("Roughness map:");
		if (!object->doesMeshHasRoughnessMap()) { ImGui::Text("No"); }
		else
		{
			IM_ASSERT(object->getRoughnessMap()); ImGui::Image((void*)(intptr_t)object->getRoughnessMap(), ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
		}

		name = "Import roughness map ##";
		name.append(to_string(object->getID()));
		ImGui::SameLine();
		if (ImGui::Button(name.c_str())) { m_currentObjectEdit = object->getID(); m_fileDialog.SetTitle("Choose roughness map file"); m_fileDialog.SetTypeFilters({ ".jpg", ".jpeg", ".png" }); m_fileDialog.Open();  m_roughnessMapPathEditing = true; }
		if (object->doesMeshHasRoughnessMap())
		{
			string name_ = "Delete roughness map";
			name_.append(" ##");
			name_.append(to_string(object->getID()));
			ImGui::SameLine();
			if (ImGui::Button(name_.c_str())) { object->removeRoughnessMap(); }
		}
		if (m_roughnessMapPathEditing && m_currentObjectEdit == object->getID())
		{
			m_fileDialog.Display();
			if (m_fileDialog.HasSelected())
			{
				object->setRoughnessMap(m_fileDialog.GetSelected().string().c_str());
				m_fileDialog.ClearSelected();
				m_roughnessMapPathEditing = false;
			}
		}

		ImGui::Text("Metallic map:");
		if (!object->doesMeshHasMetallicMap()) { ImGui::Text("No"); }
		else
		{
			IM_ASSERT(object->getMetallicMap()); ImGui::Image((void*)(intptr_t)object->getMetallicMap(), ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
		}

		name = "Import metallic map ##";
		name.append(to_string(object->getID()));
		ImGui::SameLine();
		if (ImGui::Button(name.c_str())) { m_currentObjectEdit = object->getID(); m_fileDialog.SetTitle("Choose metallic map file"); m_fileDialog.SetTypeFilters({ ".jpg", ".jpeg", ".png" }); m_fileDialog.Open(); m_metallicMapPathEditing = true; }
		if (object->doesMeshHasMetallicMap())
		{
			string name_ = "Delete metallic map";
			name_.append(" ##");
			name_.append(to_string(object->getID()));
			ImGui::SameLine();
			if (ImGui::Button(name_.c_str())) { object->removeMetallicMap(); }
		}
		if (m_metallicMapPathEditing && m_currentObjectEdit == object->getID())
		{
			m_fileDialog.Display();
			if (m_fileDialog.HasSelected())
			{
				object->setMetallicMap(m_fileDialog.GetSelected().string().c_str());
				m_fileDialog.ClearSelected();
				m_metallicMapPathEditing = false;
			}
		}

		string buttonName = "Delete object ##";
		buttonName.append(to_string(object->getID()));
		if (ImGui::Button(buttonName.c_str())) { scene_->deleteRenderObject(i); }

		ImGui::Separator();

	}
	ImGui::End();
}