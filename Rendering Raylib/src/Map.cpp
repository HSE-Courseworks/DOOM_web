#include "Map.hpp"
#include <iostream>
#include <fstream>

Map::Map(const char* filename) : 
	scheme(), objects()
{
	wallTexture = LoadTexture("resources/wall.png");
	frame.width = SIZE_PIXEL_MAP; frame.height = SIZE_PIXEL_MAP;
	frame.x = THICKNESS_MAP * 2; frame.y = THICKNESS_MAP * 2;

	std::ifstream file(filename); std::string mazeLine;
	while (std::getline(file, mazeLine)) 
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
	Rectangle wall = {0, 0, wallSize.x, wallSize.y};	
	int N = scheme.size(), M = scheme.front().size();
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			if (scheme[i][j])
			{
				wall.x = frame.x + j * wallSize.x;
				wall.y = frame.y + i * wallSize.y;
				objects.push_back(wall);
			}
		}
	}
}

void Map::showFrame() const {
	DrawRectangleRec(frame, swamp);
	DrawRectangleLinesEx(frame, THICKNESS_MAP, BLACK);
}

void Map::showObjectsInWindow() const
{
	for (auto& object : objects) {
		DrawRectangleRec(object, darkSwamp);
		DrawRectangleLinesEx(object, 1, BLACK);
	}
}

const Vector2& Map::getMazeSize() const
{
	return mazeSize;
}

const Vector2& Map::getWallSize() const
{
	return wallSize;
}

const Rectangle& Map::getFrame() const
{
	return frame;
}

const Texture2D* Map::getTexture() const
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
