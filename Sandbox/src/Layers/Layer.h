#pragma once

class Layer 
{
protected:
public:
	virtual void OnAttach() = 0;
	virtual void OnDetach() = 0;
	virtual void OnUpdate() = 0;

	virtual void OnImGuiRender() = 0;

	Layer() = default;
	virtual ~Layer() = default;
protected:
};