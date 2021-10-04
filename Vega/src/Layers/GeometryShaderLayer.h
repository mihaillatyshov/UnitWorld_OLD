#pragma once

#include <memory>

#include "Layers/Layer.h"
#include "Core/Shader.h"
#include "Buffer/VertexArray.h"
#include "Player/Camera.h"

namespace LM
{

	class GeometryShaderLayer : public Layer
	{
	public:
		GeometryShaderLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;


		virtual void OnImGuiRender() override;

	public:
		struct Point
		{
			Point(const glm::vec3 &pos, int mid, int dp) 
				: Position(pos), MaterialID(mid), DisplayedFaces(dp) { }

			glm::vec3 Position;
			int MaterialID;
			int DisplayedFaces; 
		};

	protected:
		void ImGuiDrawMask(Point* point);
	protected:
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Camera> m_Camera;

		std::shared_ptr<VertexArray> m_VAO;
		std::shared_ptr<VertexBuffer> m_VBO;
		std::shared_ptr<IndexBuffer> m_IBO;

		std::vector<Point> m_Points;
		std::vector<uint32_t> m_Indices;

		bool m_IsPerspective = true;
	};

}