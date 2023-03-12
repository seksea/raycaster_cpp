#pragma once
#include "map.hpp"

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
		{
			static float viewDist = 320 / tanf(0.8f/2);

			// get position relative to player
			Vec2 relativePos = m_pos - localPlayer.m_pos;

			float distance = m_pos.distTo(localPlayer.m_pos);

			float ang = relativePos.angle() - localPlayer.m_lookDir.angle();
			// TODO: sprite render

			Vec2 screenPos;
			screenPos.m_x = tanf(ang) * viewDist;
			screenPos.m_y = 150;

			Textures::dev->drawImage(screenPos.m_x, screenPos.m_y);
			/*glColor3f(m_r, m_g, m_b);
			glPointSize(5);
			glBegin(GL_POINTS);
			glVertex2f(relativePos.m_x, relativePos.m_y);
			glEnd();*/
		}

		float m_r=1, m_g=1, m_b=1;
	};

	inline std::vector<std::shared_ptr<BaseSprite>> sprites = 
	{
		std::make_shared<PointSprite>(PointSprite(Vec2(5, 5), 0.8f, 0.25f, 1.f))
	};

	inline void renderSprites()
	{
		for (auto& sprite : sprites)
		{
			sprite->render();
		}
	}
}
