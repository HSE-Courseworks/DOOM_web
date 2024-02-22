#include "Map.hpp"
#include <iostream>
#include <fstream>

Map::Map(const std::string& filename) : 
	objects(), frame(), color(swamp), scheme(), wallTexture()
{
	wallTexture.loadFromFile("resources/wall.png");
	frame.setSize(Vector2f(SIZE_PIXEL_MAP, SIZE_PIXEL_MAP));
	frame.setFillColor(color);
	frame.setOutlineThickness(THICKNESS_MAP);
	frame.setOutlineColor(Color::Black);
	frame.setPosition(Vector2f(THICKNESS_MAP * 2, THICKNESS_MAP * 2));

	std::ifstream file(filename); std::string mazeLine;
	while (getline(file, mazeLine)) 
	{
		std::vector<bool> schemeLine(mazeLine.size());
		for (size_t i = 0; i < mazeLine.size(); ++i) schemeLine[i] = mazeLine[i] == '#';
		scheme.push_back(schemeLine);
	}
	file.close();
	mazeSize.x = scheme.front().size();
	mazeSize.y = scheme.size();
	wallSize.x = SIZE_PIXEL_MAP / mazeSize.x;
	wallSize.y = SIZE_PIXEL_MAP / mazeSize.y;
}

void Map::findObjects()
{
	RectangleShape wall;
	wall.setSize(Vector2f(wallSize.x, wallSize.y));
	wall.setFillColor(darkSwamp);
	wall.setOutlineThickness(1);
	wall.setOutlineColor(Color::Black);
	
	int N = scheme.size(), M = scheme.front().size();
	for (size_t i = 0; i < N; ++i)
	{
		for (size_t j = 0; j < M; ++j)
		{
			if (scheme[i][j])
			{
				float posX = frame.getPosition().x + j * wallSize.x;
				float posY = frame.getPosition().y + i * wallSize.y;
				wall.setPosition(Vector2f(posX, posY));
				objects.push_back(wall);
			}
		}
	}
}

void Map::showObjectsInWindow(RenderWindow& window) const
{
	for (auto& object : objects) window.draw(object);
}

const Color& Map::getFillColor() const
{
	return color;
}

const Vector2f& Map::getMazeSize() const
{
	return mazeSize;
}

const Vector2f& Map::getWallSize() const
{
	return wallSize;
}

const RectangleShape& Map::getFrame() const
{
	return frame;
}

const Texture* Map::getTexture() const
{
	return &wallTexture;
}

//#include <stack>
//#include <random>
//#include <chrono>
//void Map::createMap()
//{
//	using namespace std::chrono;
//
//	std::mt19937 gener(system_clock::now().time_since_epoch().count());
//
//	std::stack<int> stack;
//	std::vector<bool> visited(mazeSize.first * mazeSize.second);
//	stack.push(0); visited[0] = true;
//	while (!stack.empty())
//	{
//		int curVert = stack.top(); stack.pop();
//		std::vector<int> unvisited;
//		for (int i = 0; i < vert[curVert].size(); ++i)
//		{
//			if (!visited[vert[curVert][i]]) 
//				unvisited.push_back(vert[curVert][i]);
//		}
//
//		if (!unvisited.empty())
//		{
//			stack.push(curVert);
//			std::uniform_int_distribution<int> dist(0, unvisited.size() - 1);
//			int chosen = unvisited[dist(gener)];
//			removeElem(vert[curVert], chosen); removeElem(vert[chosen], curVert);
//			visited[chosen] = true;
//			stack.push(chosen);
//		}
//	}
//}
//
//bool find(std::vector<int> arr, int t0)
//{
//	for (int i = 0; i < arr.size(); ++i)
//		if (arr[i] == t0) return true;
//	return false;
//}
//
//void removeElem(std::vector<int>& arr, int t0)
//{
//	for (auto iter = arr.begin(); iter != arr.end();)
//	{
//		if (*iter != t0) iter++;
//		else { iter = arr.erase(iter); break; }
//	}
//}
