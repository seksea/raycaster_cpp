#include "textures.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "include/stbi.hpp"

namespace Textures
{
	BaseTexture::BaseTexture(const char* filename)
	{
		m_data = stbi_load(filename, &m_width, &m_height, &m_channels, 0);
		if (m_data == nullptr)
		{
			printf("texture %s couldn't be loaded!", filename);
		}
		printf("texture %s loaded (%ix%i, %ichan) ", filename, m_width, m_height, m_channels);
	}

	const void BaseTexture::drawImage(int x, int y)
	{
		glRasterPos2i(x, y);
		glDrawPixels(m_width, m_height, m_channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, m_data);
		glFlush();
	}

	void initTextures()
	{
		stbi_set_flip_vertically_on_load(true);
		dev = std::make_shared<BaseTexture>("resources/dev.png");
		hotbar = std::make_shared<BaseTexture>("resources/hotbar.png");
	}
}
