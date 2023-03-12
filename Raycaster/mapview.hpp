#pragma once
#include <memory>


namespace Map {
	class EmptySpace;
}

namespace MapView
{
	inline bool mapViewOpen = false;
	void render(const std::shared_ptr<Map::EmptySpace>& root);
}
