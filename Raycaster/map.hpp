#pragma once
#include <memory>
#include <vector>

class BaseTexture
{
	
};


class Vec2
{
public:
	Vec2(const float x, const float y)
		: m_x(x), m_y(y) {}
	float m_x, m_y;
};

class Rect
{
public:
	Rect(const float minX, const float minY, const float maxX, const float maxY)
	{
		m_min = { minX, minY };
		m_max = { maxX, maxY };
	}
	Vec2 m_min, m_max;
};

// World is laid out using conjoined areas of hollow space
class EmptySpace
{
public:
	std::shared_ptr<BaseTexture> wallTexture;
	std::vector<std::shared_ptr<EmptySpace>> m_conjoinedSpaces;
};

