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


class RealBatchRendererLayer : public Layer
{
public:
	RealBatchRendererLayer();
	~RealBatchRendererLayer();
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;

	void OnImGuiRender() override;

protected:
	//RealBatchRenderer m_Renderer;
	Sprite *m_Sprite;
	Sprite *m_Sprite2;
	std::vector<Sprite*> m_Sprites;
	std::vector<std::shared_ptr<Texture2D>> m_Textures;
	std::vector<Material*> m_Materials;

	std::vector<Sprite*> m_BlendSprites;
	std::vector<Material*> m_BlendMaterials;

	Model* m_Model;

	//Camera m_Camera;
	Projection m_Projection;
	Chunk m_Chunk;

};