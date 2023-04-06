#pragma once
#include <memory>


namespace Map {
	class Room;
}

namespace MapView
{
	inline bool mapViewOpen = false;
	void render(const std::shared_ptr<Map::Room>& root);
}
