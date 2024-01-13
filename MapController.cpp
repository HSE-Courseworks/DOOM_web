#include "MapController.h"

int MapController::nMapWidth;
int MapController::nMapHeight;
std::string MapController::buf;

MapController::MapController()
{
	std::ifstream fMap("Map.txt", std::ios::in);
	int height = 0;
	while (fMap.good()) {
		++height;
		std::string line;
		fMap >> line;
		buf += line;
	}
	nMapHeight = height;
	nMapWidth = (height != 0) ? (buf.size() / height) : (0);
	fMap.close();
}

MapController::MapController(std::string fileName) 
{
	std::fstream fMap(fileName, std::ios::in);
	int height = 0;
	while (fMap.good()) {
		++height;
		std::string line;
		fMap >> line;
		buf += line;
	}
	nMapHeight = height;
	nMapWidth = (height != 0) ? (buf.size() / height) : (0);
	fMap.close();
}

MapController::MapController(MapController& exemp)
{
	buf = exemp.buf;
}
