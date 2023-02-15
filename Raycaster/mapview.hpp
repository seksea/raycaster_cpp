#pragma once
#include "player.hpp"
#include "map.hpp"

namespace MapView
{
	inline bool mapViewOpen = true;
	void renderRecursively(std::shared_ptr<Map::EmptySpace> root)
	{
		/*printf("%.2f, %.2f, %.2f, %.2f", root->m_position.m_min.m_x, root->m_position.m_min.m_y, root->m_position.m_max.m_x, root->m_position.m_max.m_y);
		if (root)
		{
			for (const auto& child : root->m_conjoinedSpaces)
			{
				renderRecursively(child);
			}
		}*/
	}
}