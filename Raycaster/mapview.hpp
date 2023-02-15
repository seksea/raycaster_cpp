#pragma once
#include "player.hpp"
#include "map.hpp"

namespace
{
	inline void drawRect(Vec2 min, Vec2 max)
	{
		glBegin(GL_LINE_LOOP);
		glVertex2f(min.m_x, min.m_y);
		glVertex2f(min.m_x, max.m_y);
		glVertex2f(max.m_x, max.m_y);
		glVertex2f(max.m_x, min.m_y);
		glEnd();
	}
}

namespace MapView
{
	inline bool mapViewOpen = true;
	inline void renderRecursively(std::shared_ptr<Map::EmptySpace> root)
	{
		//printf("%.2f, %.2f, %.2f, %.2f\n", root->m_position.m_min.m_x, root->m_position.m_min.m_y, root->m_position.m_max.m_x, root->m_position.m_max.m_y);

		glColor3f(1.f, 0.f, 0.f);
		drawRect(Vec2(320, 100) + (root->m_position.m_min * 2), Vec2(320, 100) + (root->m_position.m_max * 2));

		if (root)
		{
			for (const auto& child : root->m_conjoinedSpaces)
			{
				renderRecursively(child);
			}
		}
	}
}