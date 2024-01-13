#include "RayCasting.h"

Raycasting::Raycasting() : player()
{

}

Raycasting::Raycasting(const Player& _player)
{
	player = _player;
}

void Raycasting::Raycast()
{
	auto start = std::chrono::steady_clock::now();
	auto finish = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsedTime = finish - start;
	while (true) {
		finish = std::chrono::steady_clock::now();
		elapsedTime = finish - start;
		start = finish;
		if (GetAsyncKeyState('A') & 0x8000)
			player.viewAngle -= 2.0f * player.FOV * elapsedTime.count();

		if (GetAsyncKeyState('D') & 0x8000)
			player.viewAngle += 2.0f * player.FOV * elapsedTime.count();

		if (GetAsyncKeyState('W') & 0x8000) {
			if (MapController::buf[(int)(player.y) * MapController::nMapWidth + (int)(player.x + sinf(player.viewAngle) * 5.0f * elapsedTime.count())] != '#'
				&& MapController::buf[(int)(player.y + cosf(player.viewAngle) * 5.0f * elapsedTime.count()) * MapController::nMapWidth + (int)(player.x)] != '#') 
			{
				player.x += sinf(player.viewAngle) * 5.0f * elapsedTime.count();
				player.y += cosf(player.viewAngle) * 5.0f * elapsedTime.count();
			}		
		}

		if (GetAsyncKeyState('S') & 0x8000) {
			if (MapController::buf[(int)(player.y) * MapController::nMapWidth + (int)(player.x - sinf(player.viewAngle) * 5.0f * elapsedTime.count())] != '#'
				&& MapController::buf[(int)(player.y - cosf(player.viewAngle) * 5.0f * elapsedTime.count()) * MapController::nMapWidth + (int)(player.x)] != '#')
			{
				player.x -= sinf(player.viewAngle) * 5.0f * elapsedTime.count();
				player.y -= cosf(player.viewAngle) * 5.0f * elapsedTime.count();
			}
		}


		//�������� �� ����� ������
		for (int x = 0; x < ConsoleController::nScreenWidth; x++)
		{
			//������������ ���� �������� ������������ ����������� ����. �� -FOV/2 �� FOV/2
			double rayAngle = (player.viewAngle - player.FOV / 2.0) 
							  + ((double)x / (double)ConsoleController::nScreenWidth) 
							  * player.FOV;

			//���������� �� ������ �� ����� ��� ����� ������������ ����
			double distanceToWall = 0.0;

			bool isHitWall = false;

			//������ �����������, ���� ������� �����
			double eyeX = sinf(rayAngle);
			double eyeY = cosf(rayAngle);

			//���������� ������������ ����������� ���������� � ���������, �� ����������� �� �� �� ������
			while (!isHitWall && distanceToWall < player.viewRadius)
			{
				distanceToWall += 0.1;

				//�������� ������� ��� ���������� �������. 
				int nTestX = (int)(player.x + eyeX * distanceToWall);
				int nTestY = (int)(player.y + eyeY * distanceToWall);

				//���� �� ���� �� ������� �����, ������ �� ����� ��������� � �� �������
				if (nTestX < 0 || nTestX >= MapController::nMapWidth || nTestY < 0 || nTestY >= MapController::nMapHeight)
				{
					isHitWall = true;
					distanceToWall = player.viewRadius;
				}
				else //���� �� ������ �����
				{
					if (MapController::buf[nTestY * MapController::nMapWidth + nTestX] == '#')
					{
						isHitWall = true;
					}
				}
			}

			int nCeiling = (double)(ConsoleController::nScreenHeight / 2.0) - (ConsoleController::nScreenHeight / distanceToWall);
			int nFloor = ConsoleController::nScreenHeight - nCeiling;

			for (int y = 0; y < ConsoleController::nScreenHeight; y++)
			{
				if (x < MapController::nMapWidth && y < MapController::nMapHeight) {
					ConsoleController::screen[y * ConsoleController::nScreenWidth + x] = MapController::buf[y * MapController::nMapWidth + x];
					continue;
				}
				if (y < nCeiling)
				{
					ConsoleController::screen[y * ConsoleController::nScreenWidth + x] = ' ';
				}
				else if (y >= nCeiling && y <= nFloor)
				{
					ConsoleController::screen[y * ConsoleController::nScreenWidth + x] = 0x2588;
				}
				else
				{
					ConsoleController::screen[y * ConsoleController::nScreenWidth + x] = ' ';
				}
			}
			ConsoleController::screen[(int)player.y * ConsoleController::nScreenWidth + (int)player.x] = 'P';
		}
		ConsoleController::UpdateFrame();
	}
}
