#include "wall.h"

void Wall::Draw(Graphics & gfx) const
{
	for (auto& w : walls)
	{
		gfx.DrawRect(w, Colors::Black);
	}
}
