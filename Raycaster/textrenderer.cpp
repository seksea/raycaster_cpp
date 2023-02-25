#include "textrenderer.hpp"

namespace TextRenderer
{

	constexpr int TEXT_WIDTH = 12;

	std::shared_ptr<Textures::BaseTexture> numbers[10];
	void init()
	{
		numbers[0] = std::make_shared<Textures::BaseTexture>("resources/0.png");
		numbers[1] = std::make_shared<Textures::BaseTexture>("resources/1.png");
		numbers[2] = std::make_shared<Textures::BaseTexture>("resources/2.png");
		numbers[3] = std::make_shared<Textures::BaseTexture>("resources/3.png");
		numbers[4] = std::make_shared<Textures::BaseTexture>("resources/4.png");
		numbers[5] = std::make_shared<Textures::BaseTexture>("resources/5.png");
		numbers[6] = std::make_shared<Textures::BaseTexture>("resources/6.png");
		numbers[7] = std::make_shared<Textures::BaseTexture>("resources/7.png");
		numbers[8] = std::make_shared<Textures::BaseTexture>("resources/8.png");
		numbers[9] = std::make_shared<Textures::BaseTexture>("resources/9.png");
	}

	void drawNumber(int num, Vec2 pos)
	{
		char buf[32];
		_itoa_s(num, buf, sizeof(buf), 10);
		int xOffset = 0;
		for(int i = 0; buf[i]; i++)
		{
			int curDigit = buf[i] - '0'; // take away 0 to get difference from 0
			numbers[curDigit]->drawImage(pos.m_x + xOffset, pos.m_y);
			xOffset += TEXT_WIDTH;
		}
	}
}
