#pragma once
#include "map.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Sprites
{
	struct BaseSprite
	{
		virtual ~BaseSprite() = default;
		virtual void render() {};

		Vec2 m_pos;
	};

	struct PointSprite : BaseSprite
	{
		PointSprite(Vec2 pos, float r, float g, float b)
			: m_r(r), m_g(g), m_b(b)
		{
			m_pos = pos;
		}

		void render() override
		{	//		 vv good explanation on getting sprite world to screen coordinates vv
			// https://wynnliam.github.io/raycaster/news/tutorial/2019/04/03/raycaster-part-02.html

			Vec2 deltaPlayer = m_pos - localPlayer.m_pos;

			const float angleToPlayer = deltaPlayer.angle();

			constexpr float angPerPixel = 640/0.8f;

			const float angleFromLeftOfScreen = localPlayer.m_lookDir.angle() + (0.4f) - angleToPlayer;
			
			Vec2 screenPos;

			screenPos.m_x = 640 - (angleFromLeftOfScreen * angPerPixel);
			screenPos.m_y = 150;
			
			glColor3f(m_r, m_g, m_b);
			glPointSize(5);
			glBegin(GL_POINTS);
			glVertex2f(screenPos.m_x, screenPos.m_y);
			glEnd();
		}

		float m_r=1, m_g=1, m_b=1;
	};

	struct TexturedSprite : BaseSprite
	{
		TexturedSprite(Vec2 pos, std::shared_ptr<Textures::BaseTexture> texture, std::shared_ptr<Map::Room> room)
		{
			m_pos = pos;
			m_texture = texture;
			m_room = room;
		}

		void render() override
		{	//		 vv good explanation on getting sprite world to screen coordinates vv
			// https://wynnliam.github.io/raycaster/news/tutorial/2019/04/03/raycaster-part-02.html

			Vec2 deltaPlayer = m_pos - localPlayer.m_pos;

			const float angleToPlayer = deltaPlayer.angle();

			constexpr float angPerPixel = 640 / 0.8f;

			const float angleFromLeftOfScreen = localPlayer.m_lookDir.angle() + (0.4f) - angleToPlayer;

			Vec2 screenPos;

			screenPos.m_x = 640 - (angleFromLeftOfScreen * angPerPixel);
			screenPos.m_y = 150;

			int spriteSize = (1.f / m_pos.distTo(localPlayer.m_pos)) * 6000.f;

			if (!m_texture) return;

			const float leftSideOfSprite = screenPos.m_x - (spriteSize / 2);
			const float topOfSprite = screenPos.m_y - (spriteSize / 2);
			const float distance = deltaPlayer.length();
			// Draw columns of sprite if visible
			for (int i = 0; i < spriteSize; i += 4 /* column width */)
			{
				const int columnOnScreen = (leftSideOfSprite + i) / 4 /* column width */;

				if (columnOnScreen > depthBuffer.size() - 1 || columnOnScreen < 0) continue;

				bool passesThroughRoom = false;
				for (const auto& room : depthBuffer[columnOnScreen].roomsPassedThrough)
				{
					if (m_room.get() == room.get()) passesThroughRoom = true;
				}

				// If the item doesn't pass through the room the sprite is in, then the sprite is not rendered
				if (!passesThroughRoom) continue;

				if (distance > depthBuffer[columnOnScreen].distanceToWall) continue;
				
				// draw column
				m_texture->drawColumn(leftSideOfSprite + i, topOfSprite, (float)i * ((float)m_texture->m_width / (float)spriteSize), 4 /* column width */, spriteSize);
			}
		}

		std::shared_ptr<Textures::BaseTexture> m_texture = {};
		std::shared_ptr<Map::Room> m_room = {};
	};

	inline std::vector<std::shared_ptr<BaseSprite>> sprites = {};

	inline void addSprites()
	{
		//sprites.push_back(std::make_shared<TexturedSprite>(TexturedSprite(Vec2(5, 5), Textures::barrel)));
		//sprites.push_back(std::make_shared<TexturedSprite>(TexturedSprite(Vec2(10, 5), Textures::barrel)));

		//sprites.push_back(std::make_shared<TexturedSprite>(TexturedSprite(Vec2(15, 5), Textures::zombie)));
		sprites.push_back(std::make_shared<TexturedSprite>(TexturedSprite(Vec2(5, 5), Textures::dev, Map::map)));
	}

	inline void renderSprites()
	{
		for (auto& sprite : sprites)
		{
			sprite->render();
		}
	}
}
