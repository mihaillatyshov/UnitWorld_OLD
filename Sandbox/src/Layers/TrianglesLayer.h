#pragma once

#include "Layer.h"

#include <GL/glew.h>
#include "Core/Shader.h"
#include "Core/Texture.h"
#include "Renderers/RealBatchRenderer.h"
#include "Renderables/Sprite.h"
#include "Renderables/Mesh.h"
#include <UnitWorld/Chunk.h>

#include "Player/Player.h"
#include "Renderables/Model.h"

class TrianglesLayer : public Layer
{
public:
	TrianglesLayer();
	~TrianglesLayer();
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;

	void OnImGuiRender() override;
protected:
	GLuint m_VAO;
	GLuint m_VBO_Vertex;
	GLuint m_VBO_TexCoord;
	GLuint m_VBO_MaterialId;
	GLuint m_VBO_TransformId;
	GLuint m_IBO;

	GLuint m_MatrixBuffer;
	GLuint m_TextureBuffer;

	GLuint m_TextureID;
	Texture2D* m_TestTexture;


	Shader *m_Shader;
	
	glm::vec4 m_TriangleColors[2] { glm::vec4(1.0f), glm::vec4(1.0f) };
	glm::vec3 m_TriangleTransforms[2] = { };

#if 0
	RealBatchRenderer m_Renderer;
	Sprite *m_Sprite;
	Sprite *m_Sprite2;
	std::vector<Sprite*> m_Sprites;
	std::vector<Texture2D*> m_Textures;
	std::vector<Material*> m_Materials;

	std::vector<Sprite*> m_BlendSprites;
	std::vector<Material*> m_BlendMaterials;

	Player* m_Player;
	Model* m_Model;

	bool m_EditCamera = false;
	//Camera m_Camera;
	Projection m_Projection;
	Chunk m_Chunk;
	Lights m_Lights;
#endif
};