#include "mapview.hpp"
#include "map.hpp"
#include "player.hpp"

#include <set>

void drawRect(Vec2 min, Vec2 max)
{
	glBegin(GL_LINE_LOOP);
	glVertex2f(min.m_x, min.m_y);
	glVertex2f(min.m_x, max.m_y);
	glVertex2f(max.m_x, max.m_y);
	glVertex2f(max.m_x, min.m_y);
	glEnd();
}

namespace MapView
{

	std::set<std::shared_ptr<Map::Room>> alreadyRendered;
	// recursively render walls
	void renderWalls(const std::shared_ptr<Map::Room>& root, Vec2 offset, float zoom)
	{
		if (root.get() == localPlayer.m_curRoom.get())
			glColor3f(1.f, 0.f, 1.f);
		else
			glColor3f(1.f, 0.f, 0.f);

		drawRect(offset + (root->m_position.m_min * zoom), offset + (root->m_position.m_max * zoom));

		alreadyRendered.insert(root);

		if (root)
		{
			for (const auto& child : root->m_conjoinedRooms)
			{
				if (!alreadyRendered.contains(child))
					renderWalls(child, offset, zoom);
			}
		}
	}

	void renderPlayer(const Player& p, Vec2 offset, float zoom)
	{
		glPointSize(3);
		glBegin(GL_POINTS);
		glVertex2f(offset.m_x + (p.m_pos.m_x * zoom), offset.m_y + (p.m_pos.m_y * zoom));
		//printf("%.2f %.2f\n", offset.m_x + (p.m_x * zoom), offset.m_y + (p.m_y * zoom));
		glEnd();

		glColor3f(0.7f, 0.7f, 0.7f);
		glBegin(GL_LINES);
		glVertex2f(offset.m_x + (p.m_pos.m_x * zoom), offset.m_y + (p.m_pos.m_y * zoom));
		glVertex2f(offset.m_x + ((p.m_pos.m_x + p.m_lookDir.m_x*4) * zoom), offset.m_y + ((p.m_pos.m_y + p.m_lookDir.m_y * 4) * zoom));
		glEnd();
	}

	void render(const std::shared_ptr<Map::Room>& root)
	{
		renderWalls(Map::map, Vec2(320, 300), 2.f);
		alreadyRendered.clear();
		glColor3f(0, 1.f, 0.f);
		renderPlayer(localPlayer, Vec2(320, 300), 2.f);
	}

}