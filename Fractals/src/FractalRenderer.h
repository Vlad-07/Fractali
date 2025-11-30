#pragma once

#include <Eis.h>


class FractalRenderer
{
public:
	FractalRenderer(const Eis::OrthoCameraController& cam);
	~FractalRenderer() = default;
	
//	void CreateCanvas(uint32_t dislpayWidth, uint32_t displayHeight, float samplesPerPixel);


	void SetParams(glm::dvec2 constant, uint32_t maxIt, uint8_t spp, float zoom);
	void Render() const;

	void OnEvent(Eis::Event& e);

private:
//	static glm::dvec2 ComputeNextIt(glm::dvec2 current, glm::dvec2 constant);
//	static float ComputeIt(glm::dvec2 n0, glm::dvec2 constant, uint32_t maxIt);
//	static glm::u8vec3 ColorPallette(float it, uint32_t maxIt);
//	static void ComputeToBuffer(uint8_t* buffer, glm::uvec2 pixelPos, glm::dvec2 pos, glm::dvec2 constant, uint32_t maxIt, double scale, glm::uvec2 size);
//	static void ComputeRegion(uint8_t* buffer, uint32_t startCol, uint32_t colNr, glm::dvec2 pos, glm::dvec2 constant, uint32_t maxIt, double scale, glm::uvec2 size);

private:
//	Eis::WorkGroup m_WorkGroup;

//	Eis::Ref<Eis::Texture2D> m_Canvas;
//	uint8_t* m_Buffer = nullptr;
//	uint32_t m_BufSize = 0;

	Eis::Ref<Eis::Shader> m_Shader;

	glm::dvec2 m_Constant{};
	uint32_t m_MaxIt = 0;
	uint8_t m_SamplesPerPixel = 0;
	float m_Zoom = 0.0f;

//	bool m_Computed = false;

	const Eis::OrthoCameraController& m_SceneCam;
};