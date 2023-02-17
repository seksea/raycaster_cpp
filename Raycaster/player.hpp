#pragma once
#include "renderer.hpp"

struct KeysPressed
{
	bool w = false, a = false, s = false, d = false;
} inline keysPressed;

class Player
{
public:
	Player(Vec2 pos)
	{
		m_pos = pos;
		m_curSpace = Map::map;
	}

	void renderView()
	{
		const float moveSpeed = 0.5f; // TODO: respect deltatime
		const float rotationSpeed = 0.04f; // TODO: respect deltatime
		if (keysPressed.w) m_pos = m_pos + m_lookDir * moveSpeed;
		if (keysPressed.s) m_pos = m_pos + m_lookDir * -moveSpeed;
		if (keysPressed.a) m_lookDir = m_lookDir.rotate(-rotationSpeed);
		if (keysPressed.d) m_lookDir = m_lookDir.rotate(rotationSpeed);

		if (const std::shared_ptr<Map::EmptySpace> newSpace = m_curSpace->getConjoinedSpaceAtPoint(m_pos))
			m_curSpace = newSpace;
	}

	Vec2 m_lookDir = Vec2(1, 0);
	Vec2 m_pos = Vec2(0, 0);
	std::shared_ptr<Map::EmptySpace> m_curSpace = {};

} inline localPlayer(Vec2(0, 0));