#pragma once
#include <memory>
#include <GL/glut.h>

typedef unsigned char stbi_uc;

namespace Textures {
	class BaseTexture
	{
	public:
		BaseTexture(const char* filename);

		void drawImage(int x, int y) const;

		void drawColumn(int x, int y, int column, int thickness, int height, float brightness = 1.f) const;

		int m_width, m_height, m_channels;
		stbi_uc* m_data;
	};

	inline std::shared_ptr<BaseTexture> dev;
	inline std::shared_ptr<BaseTexture> brick;
	inline std::shared_ptr<BaseTexture> cobble;
	inline std::shared_ptr<BaseTexture> planks;

	inline std::shared_ptr<BaseTexture> sky;

	inline std::shared_ptr<BaseTexture> hotbar;
	inline std::shared_ptr<BaseTexture> pistol;

	void initTextures();
}