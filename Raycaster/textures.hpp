#pragma once
#include <memory>
#include <GL/glut.h>

typedef unsigned char stbi_uc;

namespace Textures {
	class BaseTexture
	{
	public:
		BaseTexture(const char* filename);

		const void drawImage(int x, int y);

		int m_width, m_height, m_channels;
		stbi_uc* m_data;
	};

	inline std::shared_ptr<BaseTexture> dev;
	inline std::shared_ptr<BaseTexture> hotbar;

	void initTextures();
}