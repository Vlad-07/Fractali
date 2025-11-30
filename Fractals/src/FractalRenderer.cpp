#include "FractalRenderer.h"

#include <Eis/Rendering/Renderer/Renderer2D.h>


FractalRenderer::FractalRenderer(const Eis::OrthoCameraController& cam) : m_SceneCam(cam)
{
	m_Shader = Eis::Renderer2D::GetQuadShader();

	m_Shader->Bind();
	m_Shader->SetFloat2("u_resolution", { (float)Eis::Application::Get().GetWindow().GetWidth(),
											(float)Eis::Application::Get().GetWindow().GetHeight() });
	m_Shader->SetFloat("u_zoom", 1.1f);
}

/*
void FractalRenderer::CreateCanvas(uint32_t width, uint32_t height, float samplesPerPixel)
{
	if (m_Canvas)
	if (width == m_Canvas->GetWidth() && height == m_Canvas->GetHeight() && samplesPerPixel == m_SamplesPerPixel)
		return;

	m_Canvas.reset();
	delete[] m_Buffer;

	width *= samplesPerPixel;
	height *= samplesPerPixel;

	m_BufSize = width * height * 4;
	m_Buffer = new uint8_t[m_BufSize]();
	for (uint32_t y = 0; y < height; y++)
		for (uint32_t x = 0; x < width; x++)
			m_Buffer[y * width * 4 + x * 4 + 3] = 0xff; // set the alpha channel

	m_Canvas = Eis::Texture2D::Create(width, height);
	m_Canvas->SetData(m_Buffer, m_BufSize);

	m_Computed = false;
}//*/


void FractalRenderer::SetParams(glm::dvec2 constant, uint32_t maxIt, uint8_t spp, float zoom)
{
//	if (constant == m_Constant && maxIt == m_MaxIt)
//		m_Computed = false;

//	if (m_Computed)
//		return;

	m_Shader->Bind();

	if (constant != m_Constant)
	{
		m_Constant = constant;
		m_Shader->SetFloat2("u_c", m_Constant);
	}

	if (maxIt != m_MaxIt)
	{
		m_MaxIt = maxIt;
		m_Shader->SetInt("u_maxIter", m_MaxIt);
	}

	if (spp != m_SamplesPerPixel)
	{
		m_SamplesPerPixel = spp;
		m_Shader->SetInt("u_SPP", m_SamplesPerPixel);
	}

	if (zoom != m_Zoom)
	{
		m_Zoom = zoom;
		m_Shader->SetFloat("u_zoom", m_Zoom);
	}


//	const float scale = m_SceneCam.GetZoom() / (float)m_Canvas->GetHeight();
//	ComputeRegion(m_Buffer, 0, m_Canvas->GetWidth(), m_SceneCam.GetPosition(), m_Constant, m_MaxIt, scale, glm::vec2(m_Canvas->GetWidth(), m_Canvas->GetHeight()));

//	m_WorkGroup.Dispatch();
//	m_WorkGroup.JoinAll();

//	m_Canvas->SetData((void*)m_Buffer, m_BufSize);
}

void FractalRenderer::Render() const
{
	m_Shader->Bind();
	m_Shader->SetFloat2("u_offset", m_SceneCam.GetPosition());

	Eis::Renderer2D::DrawQuad(m_SceneCam.GetPosition(), glm::vec2(2.0f * m_SceneCam.GetAspectRatio(), 2.0f) * m_SceneCam.GetZoom(), glm::vec4(1.0f));
}

void FractalRenderer::OnEvent(Eis::Event& e)
{
	Eis::EventDispatcher d(e);
	d.Dispatch<Eis::WindowResizeEvent>([&](Eis::WindowResizeEvent& e) -> bool
	{
		m_Shader->Bind();
		m_Shader->SetFloat2("u_resolution", { (float)e.GetWidth() , (float)e.GetHeight() });
		return false;
	});
}

/*
glm::dvec2 FractalRenderer::ComputeNextIt(glm::dvec2 current, glm::dvec2 constant)
{
	// current ^ 2
	const double real = current.x * current.x - current.y * current.y;
	const double imag = 2.0 * current.x * current.y;

	return glm::dvec2(real, imag) + constant;
}

float FractalRenderer::ComputeIt(glm::dvec2 n0, glm::dvec2 constant, uint32_t maxIt)
{
	uint32_t it = 0;
	for (; glm::dot(n0, n0) < 4.0 && it < maxIt; it++)
		n0 = ComputeNextIt(n0, constant);

	const float mod = glm::sqrt(glm::dot(n0, n0));
	const float smoothIt = (float)it - glm::log2(glm::max(1.0f, glm::log2(mod)));
	return smoothIt;
}

glm::u8vec3 FractalRenderer::ColorPallette(float it, uint32_t maxIt)
{
	return glm::u8vec3(uint8_t(255.0f * it / (float)maxIt));
}

void FractalRenderer::ComputeToBuffer(uint8_t* buffer, glm::uvec2 pixelPos, glm::dvec2 pos, glm::dvec2 constant, uint32_t maxIt, double scale, glm::uvec2 size)
{
	const glm::dvec2 pixelCoords = glm::dvec2((double)pixelPos.x - size.x / 2.0, (double)pixelPos.y - size.y / 2.0) * scale + pos;

	glm::u8vec3 res = ColorPallette(ComputeIt(pixelCoords, constant, maxIt), maxIt);

	buffer[pixelPos.y * size.x * 4 + pixelPos.x * 4 + 0] = res.r;
	buffer[pixelPos.y * size.x * 4 + pixelPos.x * 4 + 1] = res.g;
	buffer[pixelPos.y * size.x * 4 + pixelPos.x * 4 + 2] = res.b;
}

void FractalRenderer::ComputeRegion(uint8_t* buffer, uint32_t startCol, uint32_t colNr, glm::dvec2 pos, glm::dvec2 constant, uint32_t maxIt, double scale, glm::uvec2 size)
{
	for (uint32_t col = startCol; col < startCol + colNr && col < size.y; col++)
		for (uint32_t line = 0; line < size.x; line++)
			ComputeToBuffer(buffer, { line, col }, pos, constant, maxIt, scale, size);
}//*/