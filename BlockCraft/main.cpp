#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "VecMap.h"
#include "Player.h"
#include "Game.h"
#include "UI.h"

using namespace std;

int mapR = 200;// 行数
int mapC = 1000;// 列数

int printSize = 11;//玩家周围 字符的数量(窗口大小)
bool makeTree = true;// 是否生成树木

int main() {
	UI::Start();

	unsigned int seed;
	
	cout << "请输入随机种子: ";
	cin >> seed;

	WorldMap world(mapR,mapC,seed,makeTree);
	world.makeWorld();

	Player player(world, 5, 2);

	Game game(world, player, printSize);

	game.game(SURVIVAL);

	UI::End();
	return 0;
}