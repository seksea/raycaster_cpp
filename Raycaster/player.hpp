#pragma once

inline class Player
{
public:
	Player(float x, float y)
		: m_x(x), m_y(y) {}
	float m_x, m_y;
} localPlayer(0, 0);