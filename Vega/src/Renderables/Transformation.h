#pragma once

#include <glm.hpp>

namespace LM
{

	class Transformation
	{
	public:
		Transformation(const glm::mat4& transformation = glm::mat4(1.0f))
		{
			SetTransformation(transformation);
		}

		inline void SetTransformation(const glm::mat4& transformation)
		{
			m_Transformation = transformation;
			m_IsChanged = true;
		}

		inline void SetIsChanged(bool isChanged = false)
		{
			m_IsChanged = isChanged;
		}

		inline const glm::mat4& GetTransformation()
		{
			return m_Transformation;
		}

		inline bool GetIsChanged()
		{
			return m_IsChanged;
		}

	protected:
		glm::mat4 m_Transformation;
		bool m_IsChanged;
	};

}