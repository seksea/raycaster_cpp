#pragma once
#include <memory>
#include <vector>

#include "textures.hpp"

// Maps are layed out in a tree of conjoined "rooms" which should greatly speed up the raycasting as it
// would only have to check if a ray collides with the 4 walls of the current room

class Vec2
{
public:
	constexpr Vec2(const float x = 0, const float y = 0)
		: m_x(x), m_y(y) {}

	Vec2 operator+(Vec2 rhs) const
	{
		return Vec2(m_x + rhs.m_x, m_y + rhs.m_y);
	}

	Vec2 operator-(Vec2 rhs) const
	{
		return Vec2(m_x - rhs.m_x, m_y - rhs.m_y);
	}

	Vec2 operator*(float rhs) const
	{
		return Vec2(m_x * rhs, m_y * rhs);
	}

	Vec2 operator/(float rhs) const
	{
		return Vec2(m_x / rhs, m_y / rhs);
	}

	[[nodiscard]] float dot(const Vec2& other) const
	{
		return m_x * other.m_x + m_y * other.m_y;
	}

	[[nodiscard]] float determinate(const Vec2& other) const
	{
		return m_x * other.m_y - m_y * other.m_x;
	}

	[[nodiscard]] float length() const
	{
		return sqrtf(m_x * m_x + m_y * m_y);
	}

	Vec2 rotate(float rad)
	{
		const Vec2 oldDir = Vec2(m_x, m_y);
		return { oldDir.m_x * cosf(rad) - oldDir.m_y * sinf(rad), oldDir.m_x * sinf(rad) + oldDir.m_y * cosf(rad) };
	}

	float angle()
	{
		return atan2f(m_y, m_x);
	}

	[[nodiscard]] Vec2 normalized() const
	{
		const float len = length();
		if (len == 0) return Vec2(0, 0);
		return Vec2(m_x / len, m_y / len);
	}

	[[nodiscard]] Vec2 normal() const
	{
		return Vec2(-m_y, m_x);
	}

	[[nodiscard]] float distTo(Vec2 other) const
	{
		float a = m_x - other.m_x;
		float b = m_y - other.m_y;
		return sqrt(powf(a, 2) + powf(b, 2));
	}


	float m_x, m_y;
};

class Rect
{
public:
	Rect(const float minX, const float minY, const float maxX, const float maxY)
	{
		m_min = Vec2(minX, minY);
		m_max = Vec2(maxX, maxY);
	}

	// returns the side that the rect is conjoined to the other rect (-1 = no conjoined side)
	[[nodiscard]] int getConjoinedSide(const Rect& other) const
	{
		if (m_min.m_y == other.m_max.m_y)
			return 0; // top
		if (m_max.m_y == other.m_min.m_y)
			return 2; // bottom
		if (m_min.m_x == other.m_max.m_x)
			return 3; // left
		if (m_max.m_x == other.m_min.m_x)
			return 1; // right

		return -1;
	}
	Vec2 m_min{}, m_max{};
};

namespace Map {
	// World is laid out using conjoined areas of hollow space (rooms)
	class Room : public std::enable_shared_from_this<Room>
	{
	public:
		Room(const Rect& position, const std::shared_ptr<Textures::BaseTexture> wallTexture)
		{
			m_position = Rect(position.m_min.m_x, position.m_min.m_y, position.m_max.m_x, position.m_max.m_y);
			m_wallTexture[0] = wallTexture;
			m_wallTexture[1] = wallTexture;
			m_wallTexture[2] = wallTexture;
			m_wallTexture[3] = wallTexture;
		}

		std::shared_ptr<Room> createConjoinedRoom(Rect position, const std::shared_ptr<Textures::BaseTexture> wallTexture)
		{
			std::shared_ptr<Room> newRoom = std::make_shared<Room>(position, wallTexture);

			printf("Faces are conjoined %i.\n parent rect = min(%f %f), max(%f %f)\n child rect = min(%f %f), max(%f %f)\n",
				m_position.getConjoinedSide(position), m_position.m_min.m_x, m_position.m_min.m_y, m_position.m_max.m_x, m_position.m_max.m_y,
				position.m_min.m_x, position.m_min.m_y, position.m_max.m_x, position.m_max.m_y);

			m_conjoinedRooms.push_back(newRoom);
			newRoom->parent = shared_from_this();
			return newRoom;
		}

		std::shared_ptr<Room> getConjoinedRoomAtPoint(const Vec2& pos)
		{
			if (parent && pos.m_x >= parent->m_position.m_min.m_x && pos.m_x <= parent->m_position.m_max.m_x &&
				pos.m_y >= parent->m_position.m_min.m_y && pos.m_y <= parent->m_position.m_max.m_y)
				return parent;

			if (pos.m_x >= m_position.m_min.m_x && pos.m_x <= m_position.m_max.m_x &&
				pos.m_y >= m_position.m_min.m_y && pos.m_y <= m_position.m_max.m_y)
				return shared_from_this();

			for (auto& space : m_conjoinedRooms)
			{
				if (pos.m_x >= space->m_position.m_min.m_x && pos.m_x <= space->m_position.m_max.m_x &&
					pos.m_y >= space->m_position.m_min.m_y && pos.m_y <= space->m_position.m_max.m_y)
					return space;
			}

			return nullptr;
		}

		Rect m_position = { 0, 0, 0, 0 };

		// [0] = Top
		// [1] = Right
		// [2] = Bottom
		// [3] = Left
		std::shared_ptr<Textures::BaseTexture> m_wallTexture[4];
		std::shared_ptr<Room> parent = {};
		std::vector<std::shared_ptr<Room>> m_conjoinedRooms = {};
	};

	inline std::shared_ptr<Room> map = {};
	inline void init()
	{
		// TODO: make a map editor
		map = std::make_shared<Room>(Rect(-32, -32, 32, 32), Textures::brick);
		map->createConjoinedRoom(Rect(-64, 0, -32, 32), Textures::brick);
		auto room2 = map->createConjoinedRoom(Rect(-4, 32, 5, 33), Textures::planks)
			->createConjoinedRoom(Rect(-4, 33, 14, 52), Textures::cobble)
			->createConjoinedRoom(Rect(5.5, 32, 14, 33), Textures::planks) // non-euclidian test
			->createConjoinedRoom(Rect(-10, -10, 64, 32), Textures::brick);
		room2->createConjoinedRoom(Rect(64, 0, 128, 100), Textures::brick);

		auto hallway = room2->createConjoinedRoom(Rect(-11, -10, -10, 0), Textures::planks)
			->createConjoinedRoom(Rect(-19, -128, -11, 64), Textures::brick);

		hallway->createConjoinedRoom(Rect(-20, -40, -19, -32), Textures::planks)
			->createConjoinedRoom(Rect(-100, -60, -20, -20), Textures::cobble);

		auto magicHallway = hallway->createConjoinedRoom(Rect(-11, -42, -10, -34), Textures::planks)
			->createConjoinedRoom(Rect(-10, -42, 10, -34), Textures::cobble)
			->createConjoinedRoom(Rect(2, -34, 10, -32), Textures::planks);
		map->m_conjoinedRooms.push_back(magicHallway);
		magicHallway->m_conjoinedRooms.push_back(map);

		hallway->createConjoinedRoom(Rect(-11, 32, -10, 40), Textures::planks)
			->createConjoinedRoom(Rect(-10, 20, 60, 200), Textures::cobble);
	}
}