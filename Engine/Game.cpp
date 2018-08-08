/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "ChiliUtil.h"
#include <algorithm>
#include <functional>

 // these are the layout strings for the scenery (background tilemaps)
const std::string layer1 =
"GBCGFDEEBGDEEGBCBDEBDEBCG"
"BCBBBBEBEGBDDBDCCDECBBBBB"
"CCEDDBBCBFDBBCCBEGCCCBEEC"
"CEEBBFEDEFBCDCCECCCEBDDCC"
"DDCCCCCGBEFEEDEECBCDEEDBD"
"GBBBBBCEBCBCBGBCBDEBDEBCG"
"CBCGFDEEBGEDBCDDEFECBEEBC"
"BDCCCBCCEEEEEBEEGDEGBEDEB"
"BCEECEBCCDEDBBEDDBDBFBDDB"
"FCBCDDFBGFDDEFEDEEDBCCBEF"
"CBBDEEEECECDECFBEDCBEBDBC"
"EEBGEEEFEDCCEECFDCBDDBDDE"
"CCEDDBBCBFDBBCCBEGCCCBEEC"
"BCBBBBEBEGBDDBDCCDECBBBBB"
"BGEDCCEDEDCCCBCEECCDCEDDB"
"EGDDEGCBBCBEEEBCFCDCEGGCE"
"EBEBFBCFBBEDCEDBDGBFBCGBE"
"DDECBCDDCCCDCDDDECECDDDBD"
"CBCGFDEEBGEDBCDDEFECBEEBC";

const std::string layer2 =
"LLLLLLLLLLAAALLLLLLLLLLLL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LLLLLLLLLLLLLLLLLLLLLLLLL";

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	rng(std::random_device()())
{
	std::uniform_int_distribution<int> xDist(0, Graphics::ScreenWidth - 1);
	std::uniform_int_distribution<int> yDist(0, Graphics::ScreenHeight - 1);
	//Color
	std::uniform_int_distribution<int> Red(0, 255);
	std::uniform_int_distribution<int> Green(0, 255);
	std::uniform_int_distribution<int> Blue(0, 255);
	//radian
	std::uniform_int_distribution<int> radian(100, 200);
	//velocity
	std::uniform_int_distribution<int> velx(-150, 150);
	std::uniform_int_distribution<int> vely(-150, 150);
	lights.emplace_back(Light({ (float)xDist(rng), (float)yDist(rng) },
		Colors::MakeRGB((unsigned char)Red(rng), (unsigned char)Green(rng), (unsigned char)Blue(rng)), false,
		radian(rng), { (float)velx(rng), (float)vely(rng) }));
	
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = ft.Mark();
	while (!wnd.mouse.IsEmpty())
	{
		const Mouse::Event e = wnd.mouse.Read();
		if (e.GetType() == Mouse::Event::Type::LPress)
		{
			//position
			std::uniform_int_distribution<int> xDist(0, Graphics::ScreenWidth - 1);
			std::uniform_int_distribution<int> yDist(0, Graphics::ScreenHeight - 1);
			//Color
			std::uniform_int_distribution<int> Red(0, 255);
			std::uniform_int_distribution<int> Green(0, 255);
			std::uniform_int_distribution<int> Blue(0, 255);
			//radian
			std::uniform_int_distribution<int> radian(100, 200);
			//velocity
			std::uniform_int_distribution<int> velx(-150, 150);
			std::uniform_int_distribution<int> vely(-150, 150);
			lights.emplace_back(Light({ (float)xDist(rng), (float)yDist(rng) }, 
				Colors::MakeRGB((unsigned char)Red(rng), (unsigned char)Green(rng), (unsigned char)Blue(rng)),
				false, radian(rng), { (float)velx(rng), (float)vely(rng) }));
		}
		if (e.GetType() == Mouse::Event::Type::RPress)
		{
			if (lights.size() > 1)
			{
				lights.pop_back();
			}
		}
	}
	
	for (auto& l : lights)
	{
		l.Update(wall, wnd.mouse.GetPos(), dt, rng);
	}
}

void Game::ComposeFrame()
{

	//bencher.Start();
	bg.Draw(gfx);
	// draw scenery underlayer
	for (auto& l : lights)
	{
		l.Draw(gfx);
	}
	//light2.Draw(gfx);
	//light3.Draw(gfx);
	//light4.Draw(gfx);
	/*if (bencher.End())
	{
		OutputDebugString((std::wstring(bencher) + L"\n").c_str());
	}*/
	wall.Draw(gfx);
}
