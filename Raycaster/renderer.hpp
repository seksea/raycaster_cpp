#pragma once
#include "map.hpp"

namespace
{
	constexpr float RAY_LENGTH = 2048;
}

namespace Renderer
{
	class Ray
	{
	public:
		Ray(const Vec2& startPos, const Vec2& direction, const std::shared_ptr<Map::EmptySpace>& startingSpace)
		{
			m_startPos = startPos;
			m_direction = direction;
			m_startingSpace = startingSpace;
		}

		Vec2 doesRayIntersectLine(Vec2 p1, Vec2 p2)
		{
			// FUNCTION DOESNT WORK!! FIX!! TODO: FIX doesRayIntersectLine func

			//http://mathworld.wolfram.com/Line-LineIntersection.html

			Vec2 endPos = m_startPos + m_direction * RAY_LENGTH;

			float detL1 = p1.determinate(p2);
			float detL2 = m_startPos.determinate(endPos);
			float x1mx2 = p1.m_x - p2.m_x;
			float x3mx4 = m_startPos.m_x - endPos.m_x;
			float y1my2 = p1.m_y - p2.m_y;
			float y3my4 = m_startPos.m_y - endPos.m_y;

			float xnom = Vec2(detL1, x1mx2).determinate(Vec2(detL2, x3mx4));
			float ynom = Vec2(detL1, y1my2).determinate(Vec2(detL2, y3my4));
			float denom = Vec2(x1mx2, y1my2).determinate(Vec2(x3mx4, y3my4));
			if (denom == 0.0)//Lines don't seem to cross
			{
				return Vec2(FLT_MAX, FLT_MAX);
			}

			return Vec2(xnom / denom, ynom / denom); //All OK
		}

		Vec2 trace()
		{
			std::shared_ptr<Map::EmptySpace> curSpace = m_startingSpace;
			
			printf("Top: %.2f\n", doesRayIntersectLine(curSpace->m_position.m_min, Vec2(curSpace->m_position.m_max.m_x, curSpace->m_position.m_min.m_y)).m_x);
			printf("Right: %.2f\n", doesRayIntersectLine(Vec2(curSpace->m_position.m_max.m_x, curSpace->m_position.m_min.m_y), curSpace->m_position.m_max).m_x);
			printf("Bottom: %.2f\n", doesRayIntersectLine(curSpace->m_position.m_max, Vec2(curSpace->m_position.m_min.m_x, curSpace->m_position.m_max.m_y)).m_x);
			printf("Left: %.2f\n\n\n", doesRayIntersectLine(Vec2(curSpace->m_position.m_min.m_x, curSpace->m_position.m_max.m_y), curSpace->m_position.m_min).m_x);
			return doesRayIntersectLine(Vec2(curSpace->m_position.m_max.m_x, curSpace->m_position.m_min.m_y), curSpace->m_position.m_max);
		}

		std::shared_ptr<Map::EmptySpace> m_startingSpace;
		Vec2 m_startPos;
		Vec2 m_direction;
	};
}
