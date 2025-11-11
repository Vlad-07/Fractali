#pragma once

#include <Eis.h>
#include <imgui.h>

#include "FractalRenderer.h"


class MainLayer : public Eis::Layer
{
public:
	MainLayer();
	virtual ~MainLayer() = default;

	virtual void OnAttach() override;

	virtual void OnUpdate(Eis::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Eis::Event& e) override;

private:
	Eis::OrthoCameraController m_CamController;
	FractalRenderer m_FractalRenderer;

	glm::dvec2 m_Constant = glm::dvec2(-0.209, 0.696);
	uint32_t m_MaxIt = 500;
	float m_Sampling = 0.5f;

	inline static const std::array s_FractalLib{
		std::make_pair(glm::dvec2(-0.209f, 0.696f), 200),
		std::make_pair(glm::dvec2(-0.4f, 0.6f), 200),
		std::make_pair(glm::dvec2(-0.02f, 0.79f), 150),
		std::make_pair(glm::dvec2(-0.8f, 0.156f), 500),
		std::make_pair(glm::dvec2(0.35f, 0.358f), 500),
		std::make_pair(glm::dvec2(-0.21f, 0.79f), 400),
		std::make_pair(glm::dvec2(0.407f, 0.306f), 100),
		std::make_pair(glm::dvec2(-1.166f, -0.259f), 100)
	};
};