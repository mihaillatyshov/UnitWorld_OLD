#pragma once

#include "Layer.h"
#include "Player/Player.h"
#include "Renderers/Renderer.h"

namespace LM
{

	class SharedLayer : public Layer
	{
	public:
		SharedLayer();
		~SharedLayer();
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;

		void OnImGuiRender() override;
	protected:
		bool m_EditCamera = false;
		Lights* m_Lights;
		Player* m_Player;
	};

}