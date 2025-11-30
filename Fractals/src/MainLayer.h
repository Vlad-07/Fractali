#pragma once

#include <Eis.h>
#include <imgui.h>

#include "FractalRenderer.h"


class MainLayer : public Eis::Layer
{
public:
	MainLayer();
	virtual ~MainLayer() = default;

	virtual void Attach() override;

	virtual void Update(Eis::TimeStep ts) override;
	virtual void Render() override;
	virtual void ImGuiRender() override;
	virtual void OnEvent(Eis::Event& e) override;

private:
	Eis::OrthoCameraController m_CamController;
	FractalRenderer m_FractalRenderer;

	glm::vec2 m_Constant = glm::vec2(-0.209, 0.696);
	uint32_t m_MaxIt = 500;
	int m_Sampling = 4;
	bool m_MouseInput = false;

	glm::vec2 m_MouseClickOrigin{};
	glm::vec2 m_CamClickOrigin{};

	inline static const std::array s_FractalLib {
		std::make_pair(glm::vec2(-0.209, 0.696), 500),
		std::make_pair(glm::vec2(-0.4, 0.6), 200),
		std::make_pair(glm::vec2(-0.02, 0.79), 150),
		std::make_pair(glm::vec2(-0.8, 0.156), 500),
		std::make_pair(glm::vec2(0.36, 0.358), 200),
		std::make_pair(glm::vec2(-0.21, 0.79), 400),
		std::make_pair(glm::vec2(0.407, 0.306), 100),
		std::make_pair(glm::vec2(-1.166, 0.188), 1000)
	};
};