#pragma once
#include <map>

#include "gamestate.hpp"
#include "renderer.hpp"

struct KeysPressed
{
	bool w = false, a = false, s = false, d = false, shift = false;
} inline keysPressed;

struct DepthBufferItem
{
	float distanceToWall = 0.f;
	std::vector<std::shared_ptr<Map::Room>> roomsPassedThrough = {};
};

inline std::map<float, DepthBufferItem> depthBuffer;

class Player
{
public:
	Player(Vec2 pos)
	{
		m_pos = pos;
		m_curRoom = Map::map;
	}
	
	void renderView()
	{
		const float moveSpeed = (keysPressed.shift ? 0.07f : 0.03f) * GameState::deltaTime; // TODO: respect deltatime
		const float rotationSpeed = 0.002f * GameState::deltaTime; // TODO: respect deltatime
		const Vec2 oldPos = m_pos;
		if (keysPressed.w) m_pos = m_pos + m_lookDir * moveSpeed;
		if (keysPressed.s) m_pos = m_pos + m_lookDir * -moveSpeed;
		if (keysPressed.a) m_lookDir = m_lookDir.rotate(-rotationSpeed);
		if (keysPressed.d) m_lookDir = m_lookDir.rotate(rotationSpeed);

		const std::shared_ptr<Map::Room> newSpace = m_curRoom->getConjoinedRoomAtPoint(m_pos);
		if (newSpace)
			m_curRoom = newSpace;
		else
			m_pos = oldPos;
		
		constexpr float width = 640;
		constexpr float columnWidth = 4;
		constexpr float FOVRad = 0.8f; // ~90 degrees FOV

		depthBuffer.clear(); // empty the depth buffer

		//for (float i = m_lookDir.angle() - 0.8f; i < m_lookDir.angle() + 0.8f; i += 0.02f)
		for (float i = 0; i < width; i += columnWidth)
		{
			Vec2 ang = m_lookDir.rotate(-FOVRad/2 + ((i / width)*FOVRad));

			Renderer::Ray ray(m_pos, ang, m_curRoom);
			const Renderer::Ray::TraceResult result = ray.trace();

			// correct distance to prevent fisheye
			float distance = result.hitPos.distTo(m_pos) * cosf(m_lookDir.angle() - ang.angle());

			if (MapView::mapViewOpen) {
				constexpr Vec2 offset = Vec2(320, 300);
				constexpr float zoom = 2.f;

				glColor4f(0.6f, 0.6f, 0.6f, 0.6f);
				glBegin(GL_LINES);
				glVertex2f(offset.m_x + (m_pos.m_x * zoom), offset.m_y + (m_pos.m_y * zoom));
				glVertex2f(offset.m_x + (result.hitPos.m_x * zoom), offset.m_y + (result.hitPos.m_y * zoom));
				glEnd();
			}
			else {
				depthBuffer.insert({ ang.angle(), DepthBufferItem(distance, result.roomsPassedThrough) });
				
				result.texture->drawColumn(i, 150 - (1 / distance) * 3000, abs(((int)(result.hitPos.m_x * 4) + (int)(result.hitPos.m_y * 4))) % 64, columnWidth, (1 / distance) * 6000, result.side == 0 || result.side == 1 ? 0.75f : 1.f);
			}
		}

		/*float count = 0;
		for (auto& depthBufferItem : depthBuffer)
		{
			glColor3f(0.8f, 0.2f, 0.5f);
			glPointSize(5);
			glBegin(GL_POINTS);
			glVertex2f(count, depthBufferItem.second.distanceToWall);
			glEnd();
			count += 4;
		}*/

	}

	Vec2 m_lookDir = Vec2(1, 0);
	Vec2 m_pos = Vec2(0, 0);
	std::shared_ptr<Map::Room> m_curRoom = {};

	int m_health = 100;
	int m_ammo = 60;

} inline localPlayer(Vec2(0, 0));