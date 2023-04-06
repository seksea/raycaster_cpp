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
			printf("texture %s couldn't be loaded!\n", filename);
		}
		printf("texture %s loaded (%ix%i, %ichan)\n", filename, m_width, m_height, m_channels);
	}

	void BaseTexture::drawImage(int x, int y) const
	{
		glRasterPos2i(x, y);
		glDrawPixels(m_width, m_height, m_channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, m_data);
	}

	void BaseTexture::drawColumn(int x, int y, int column, int thickness, int height, float brightness) const
	{
		float ratio = ((float)m_height / ((float)height / (float)thickness));
		glPointSize(thickness);
		for (int row = 0; row < height/thickness; row++)
		{
			int index = ((int)((float)row * ratio) * m_width + column) * m_channels;

			if (y + row * thickness < 0 || y + row * thickness > 400)
				continue; // DIY fix for drawing points out of bounds and causing slow down
			
			glColor4ub(m_data[index] * brightness, m_data[index + 1] * brightness, m_data[index + 2] * brightness, m_channels == 4 ? m_data[index + 3] : 255);
			glBegin(GL_POINTS);

			glVertex2f(x, y + row*thickness);

			glEnd();
		}
	}

	void BaseTexture::drawScaledImage(int x, int y, int pixelWidth, int size) const
	{
		for (int i = 0; i < size; i+=pixelWidth)
		{
			drawColumn(x + i, y, (float)i * ((float)m_width / (float)size), pixelWidth, size);
		}
	}


	void initTextures()
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		stbi_set_flip_vertically_on_load(false);
		dev = std::make_shared<BaseTexture>("resources/dev.png");
		brick = std::make_shared<BaseTexture>("resources/brick.png");
		planks = std::make_shared<BaseTexture>("resources/planks.png");
		cobble = std::make_shared<BaseTexture>("resources/cobble.png");

		zombie = std::make_shared<BaseTexture>("resources/zombie.png");

		barrel = std::make_shared<BaseTexture>("resources/barrel.png");

		sky = std::make_shared<BaseTexture>("resources/sky.png");

		stbi_set_flip_vertically_on_load(true);
		hotbar = std::make_shared<BaseTexture>("resources/hotbar.png");
		pistol = std::make_shared<BaseTexture>("resources/pistol.png");
	}
}
