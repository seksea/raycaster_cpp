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

		Vec2 findRayIntersection(Vec2 p1, Vec2 p2)
		{
			// TODO: rewrite bc plagiarism :rofl:
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

		struct RayIntersectResult
		{
			Vec2 pos = Vec2(0, 0);
			int side = 0;// top = 0, right = 1, bottom = 2, left = 3
		};
		RayIntersectResult findRayIntersectionWithRectangle(const Rect& rect)
		{
			if (m_direction.m_y < 0)
			{
				const Vec2 topIntersection = findRayIntersection(rect.m_min, Vec2(rect.m_max.m_x, rect.m_min.m_y));
				if (topIntersection.m_x < rect.m_max.m_x && topIntersection.m_x > rect.m_min.m_x)
					return { topIntersection, 0 };
			}

			if (m_direction.m_x > 0)
			{
				const Vec2 rightIntersection = findRayIntersection(Vec2(rect.m_max.m_x, rect.m_min.m_y), rect.m_max);
				if (rightIntersection.m_y < rect.m_max.m_y && rightIntersection.m_y > rect.m_min.m_y)
					return {rightIntersection, 1 };
			}

			if (m_direction.m_y > 0)
			{
				const Vec2 bottomIntersection = findRayIntersection(rect.m_max, Vec2(rect.m_min.m_x, rect.m_max.m_y));
				if (bottomIntersection.m_x < rect.m_max.m_x && bottomIntersection.m_x > rect.m_min.m_x)
					return {bottomIntersection, 2 };
			}

			if (m_direction.m_x < 0)
			{
				const Vec2 leftIntersection = findRayIntersection(Vec2(rect.m_min.m_x, rect.m_max.m_y), rect.m_min);
				if (leftIntersection.m_y < rect.m_max.m_y && leftIntersection.m_y > rect.m_min.m_y)
					return {leftIntersection, 3 };
			}

			return { Vec2(0, 0), 0 };
		}

		struct TraceResult
		{
			Vec2 hitPos = Vec2(0, 0);
			std::shared_ptr<Textures::BaseTexture> texture = {};
			int side = 0; // top = 0, right = 1, bottom = 2, left = 3
		};

		TraceResult trace()
		{
			std::shared_ptr<Map::EmptySpace> curSpace = m_startingSpace;
			RayIntersectResult intersectionPos;
			std::shared_ptr<Map::EmptySpace> hitSpace = {};
			while (curSpace)
			{
				intersectionPos = findRayIntersectionWithRectangle(curSpace->m_position);

				hitSpace = curSpace;
				curSpace = curSpace->getConjoinedSpaceAtPoint(intersectionPos.pos + m_direction / 50);
			}

			return { intersectionPos.pos, hitSpace->m_wallTexture[intersectionPos.side], intersectionPos.side};
		}

		std::shared_ptr<Map::EmptySpace> m_startingSpace;
		Vec2 m_startPos;
		Vec2 m_direction;
	};
}
