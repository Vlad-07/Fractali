#include "MainLayer.h"
#include <filesystem>

MainLayer::MainLayer() : Layer("Main"), m_FractalRenderer(m_CamController)
{
	Eis::Application::Get().GetWindow().SetTitle("Fractali");
	m_CamController.SetMinZoom(0.0f);
	m_CamController.SetZoom(1.1f);

	EIS_INFO("Asstets folder exists: {0}", std::filesystem::exists("./assets"));
}


void MainLayer::Attach()
{
	Eis::Renderer2D::SetClearColor(glm::vec3(1, 0, 1));
	m_CamController.SetMinZoom(0.0f);
	m_CamController.SetZoom(1.1f);
}


void MainLayer::Update(Eis::TimeStep ts)
{
	m_CamController.Update(ts);

	// Mouse panning
	if (Eis::Input::IsMouseButtonPressed(EIS_MOUSE_BUTTON_0) && !ImGui::GetIO().WantCaptureMouse)
	{
		m_CamController.SetPosition(m_CamClickOrigin); // HACK: next CalculateMouseWorldPos must use initial vp matrix
		m_CamController.SetPosition(m_CamClickOrigin + m_MouseClickOrigin - m_CamController.CalculateMouseWorldPos());
	}

	if (m_MouseInput)
		m_Constant = m_CamController.CalculateMouseWorldPos() / 2.0f;

	m_FractalRenderer.SetParams(m_Constant, m_MaxIt, (uint8_t)m_Sampling, m_CamController.GetZoom());
}

void MainLayer::Render()
{
	Eis::Renderer2D::Clear();
	Eis::Renderer2D::ResetStats();
	Eis::Renderer2D::BeginScene(m_CamController.GetCamera());

	m_FractalRenderer.Render();

	Eis::Renderer2D::EndScene();
}

void MainLayer::ImGuiRender()
{
	ImGui::Begin("Julia Set");

	ImGui::DragFloat2("Constant", (float*)&m_Constant, 0.0005f, -5.0f, 5.0f);
	ImGui::SliderInt("Detail", (int*)&m_MaxIt, 50, 1000);
	ImGui::SliderInt("Sampling", &m_Sampling, 1, 9);
	ImGui::Checkbox("Mouse Input", &m_MouseInput);
	ImGui::SameLine();
	if (ImGui::Button("Reset Camera"))
	{
		m_CamController.SetPosition(glm::vec2());
		m_CamController.SetZoom(1.1f);
	}

	static uint8_t toLoad = 0;
	for (uint8_t i = 1; i < s_FractalLib.size() + 1; i++)
	{
		if (ImGui::Button(std::to_string(i).c_str())) toLoad = i;
		ImGui::SameLine();
	}

	if (toLoad > 0 && toLoad < s_FractalLib.size() + 1)
	{
		m_Constant = s_FractalLib[toLoad - 1].first;
		m_MaxIt = s_FractalLib[toLoad - 1].second;
		toLoad = 0;

		m_CamController.SetPosition(glm::vec2());
		m_CamController.SetZoom(1.1f);
	}

	ImGui::End();
}

void MainLayer::OnEvent(Eis::Event& e)
{
	m_CamController.OnEvent(e);
	m_FractalRenderer.OnEvent(e);

	Eis::EventDispatcher d(e);
	d.Dispatch<Eis::MouseButtonPressedEvent>([&](Eis::MouseButtonPressedEvent& e) -> bool
	{
		if (e.GetMouseButton() == EIS_MOUSE_BUTTON_0)
		{
			m_MouseClickOrigin = m_CamController.CalculateMouseWorldPos();
			m_CamClickOrigin = m_CamController.GetPosition();
		}
		else if (e.GetMouseButton() == EIS_MOUSE_BUTTON_1)
			m_MouseInput = !m_MouseInput;
		return false;
	});
}