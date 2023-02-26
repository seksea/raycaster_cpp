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
		const Vec2 oldPos = m_pos;
		if (keysPressed.w) m_pos = m_pos + m_lookDir * moveSpeed;
		if (keysPressed.s) m_pos = m_pos + m_lookDir * -moveSpeed;
		if (keysPressed.a) m_lookDir = m_lookDir.rotate(-rotationSpeed);
		if (keysPressed.d) m_lookDir = m_lookDir.rotate(rotationSpeed);

		const std::shared_ptr<Map::EmptySpace> newSpace = m_curSpace->getConjoinedSpaceAtPoint(m_pos);
		if (newSpace)
			m_curSpace = newSpace;
		else
			m_pos = oldPos;
		
		constexpr float width = 640;
		constexpr float columnWidth = 3;
		constexpr float FOVRad = 1.2f;
		
		//for (float i = m_lookDir.angle() - 0.8f; i < m_lookDir.angle() + 0.8f; i += 0.02f)
		for (float i = 0; i < width; i += columnWidth)
		{
			Vec2 ang = m_lookDir.rotate(-FOVRad/2 + ((i / width)*FOVRad));
			Renderer::Ray ray(m_pos, ang, m_curSpace);
			const Renderer::Ray::TraceResult result = ray.trace();
			float distance = result.hitPos.distTo(m_pos);

			result.texture->drawColumn(i, 150 - (1 / distance) * 3000, abs(((int)(result.hitPos.m_x * 4) + (int)(result.hitPos.m_y * 4))) % 64, columnWidth, (1 / distance) * 6000, result.side == 0 || result.side == 1 ? 0.75f : 1.f);
		}
	}

	Vec2 m_lookDir = Vec2(1, 0);
	Vec2 m_pos = Vec2(0, 0);
	std::shared_ptr<Map::EmptySpace> m_curSpace = {};

	int m_health = 100;
	int m_ammo = 60;

} inline localPlayer(Vec2(0, 0));