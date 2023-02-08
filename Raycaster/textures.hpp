#pragma once
#include <memory>
#include <GL/glut.h>

#include "include/lodepng.hpp"

namespace Textures {
	class BaseTexture
	{
	public:
		BaseTexture(const char* filename)
		{
			std::vector<unsigned char> buffer;
			unsigned int error = lodepng::decode(m_image, m_width, m_height, filename);
			if (error)
			{
				printf("texture %s couldn't be loaded, this is because:\n%s\n", filename, lodepng_error_text(error));
			}
		}

		const void drawImage()
		{
			glRasterPos2i(0, 0);
			glDrawPixels(m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, &m_image[0]);
			glFlush();
		}

		unsigned int m_width, m_height;
		std::vector<unsigned char> m_image;
	};

	inline std::shared_ptr<BaseTexture> dev;
	inline std::shared_ptr<BaseTexture> hotbar;

	inline void initTextures()
	{
		dev = std::make_shared<BaseTexture>("dev.png");
		hotbar = std::make_shared<BaseTexture>("hotbar.png");

	}
}