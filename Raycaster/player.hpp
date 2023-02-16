#pragma once

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
	}

	void renderView()
	{
		const float moveSpeed = 0.5f; // TODO: respect deltatime
		if (keysPressed.w) m_pos = m_pos + m_lookDir * moveSpeed;
		if (keysPressed.s) m_pos = m_pos + m_lookDir * -moveSpeed;

		const float rotationSpeed = 0.04f; // TODO: respect deltatime
		if (keysPressed.a)
		{
			Vec2 oldDir = m_lookDir;
			m_lookDir.m_y = oldDir.m_x * sinf(-rotationSpeed) + oldDir.m_y * cosf(-rotationSpeed);
			m_lookDir.m_x = oldDir.m_x * cosf(-rotationSpeed) - oldDir.m_y * sinf(-rotationSpeed);
		}

		if (keysPressed.d)
		{
			Vec2 oldDir = m_lookDir;
			m_lookDir.m_x = oldDir.m_x * cosf(rotationSpeed) - oldDir.m_y * sinf(rotationSpeed);
			m_lookDir.m_y = oldDir.m_x * sinf(rotationSpeed) + oldDir.m_y * cosf(rotationSpeed);
		}
	}

	Vec2 m_lookDir = Vec2(1, 0);
	Vec2 m_pos = Vec2(0, 0);
} inline localPlayer(Vec2(0, 0));