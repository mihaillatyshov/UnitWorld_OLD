#pragma once

#include "Layer.h"

#include "Renderers/RealBatchRenderer.h"

class BugTestsLayer : public Layer
{
public:
	BugTestsLayer();
	~BugTestsLayer();
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;

protected:
	SimpleSubBuffer m_Buffer1;
	SimpleSubBuffer m_Buffer2;
};