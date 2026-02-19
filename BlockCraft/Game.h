#pragma once
#include "Player.h"
#include "VecMap.h"
#include "saveMap.h"
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <algorithm>
#include <iomanip>
#include <string>
#include <Windows.h>

// 用于处理游戏的综合

enum GameMode {
	CREATIVE,	// 创造模式
	SURVIVAL,	// 生存模式
};

class Game {
private:
	WorldMap& worldMap; // 游戏世界地图
	Player& player;     // 玩家对象
	int windowSize = 12;
	int maxDistance = 4; // 可交互的最远距离

	std::string saveFile = "save.txt"; // 存档文件名
public:
	Game(WorldMap& map, Player& p,int winSize=12,std::string FileName="save.txt") :
		worldMap(map), player(p) ,windowSize(winSize), saveFile(FileName){}

	void game(GameMode mode = CREATIVE) {
		// 创建游戏逻辑
		printf("%s", ANSI::CLEAR.c_str());// 清屏
		
		while(true){
			// 打印玩家周围的地图区域
			printf("%s", ANSI::CURSOR_HOME.c_str());// 光标移到首页
			printf("%s", ANSI::HIDE_CURSOR.c_str());// 隐藏光标
			player.printWindow(windowSize);
			std::cout << "玩家位置:  "
				<< std::setw(4) << player.GetRow() << "行 " 
				<< std::setw(4) << player.GetCol() << "列\n";

			#pragma region 打印物品栏
			for (int i = 0; i < 8; i++){
				Inventory inv = player.getItem(i);
				if (inv.type <= canDig && inv.type > EMPTY &&
					inv.count > 0) {
					//移动光标至地图后面(有Bug)
					//std::string moveCursor = "\033[" + std::to_string(windowSize * 2 + 1 + 2) + ";" + std::to_string(i + 1) + "H";
					//std::cout << moveCursor;
					std::cout << BlockChars[inv.type] << "x"
					<< std::setw(2) << inv.count << " ";
				}
				else {
					std::cout << "      ";// 空格占位
				}
			}
			std::cout << "\n\n选中 > "
				<< BlockChars[player.getSelectedSlot().type] << " x "
				<< player.getSelectedSlot().count << "  \n";
			
			char ch = _getch();
			#pragma endregion

			// 退出(ESC)
			if (ch == 27) {
				SaveMap save(player);
				if (save.loadMap(saveFile)) {
					printf("\n地图加载成功！\n");
				}
				else {
					printf("\n地图加载失败！\n\n");
				}
				break;
			}
			// 移动控制(WASD)
			else if ((ch == 'w' || ch == 'W') && mode == CREATIVE) {
				player.Move(-1, 0);
			}
			else if ((ch == 's' || ch == 'S') && mode == CREATIVE) {
				player.Move(1, 0);
			}
			else if (ch == 'a' || ch == 'A') {
				if (!player.Move(0, -1) && mode == SURVIVAL) {
					player.Move(-1, 0);
					player.Move(0, -1);
					player.AdaptPos();
				}
			}
			else if (ch == 'd' || ch == 'D') {
				if (!player.Move(0, 1) && mode == SURVIVAL) {
					player.Move(-1, 0);
					player.Move(0, 1);
					player.AdaptPos();
				}
			}
			// 挖掘方块(小写ijkl)
			else if (ch == 'i') {
				int Dis = player.GetDistance(maxDistance, UP);
				//player.DigAroundBlock(-1, 0);
				player.DigAroundBlock(-Dis, 0);
			}
			else if (ch == 'k') {
				int Dis = player.GetDistance(maxDistance, DOWN);
				//player.DigAroundBlock(1, 0);
				player.DigAroundBlock(Dis, 0);
			}
			else if (ch == 'j') {
				int Dis = player.GetDistance(maxDistance, LEFT);
				//player.DigAroundBlock(0, -1);
				player.DigAroundBlock(0, -Dis);
			}
			else if (ch == 'l') {
				int Dis = player.GetDistance(maxDistance, RIGHT);
				//printf("Debug 距离: %d\n", Dis);
				//player.DigAroundBlock(0, 1);
				player.DigAroundBlock(0, Dis);
			}
			// 放置方块(大写IJKL)
			else if (ch == 'I') {
				int Dis = player.GetDistance(maxDistance + 1, UP) - 1;
				//printf("Debug 距离: %d\n", Dis);
				//player.PlaceSelectedAroundBlock(-1, 0);
				player.PlaceSelectedAroundBlock(-Dis, 0);
			}
			else if (ch == 'K') {
				int Dis = player.GetDistance(maxDistance + 1, DOWN) - 1;
				//player.PlaceSelectedAroundBlock(1, 0);
				if (!player.PlaceSelectedAroundBlock(Dis, 0)) {
					player.Move(-1, 0);
					player.PlaceSelectedAroundBlock(1, 0);
				}
			}
			else if (ch == 'J') {
				int Dis = player.GetDistance(maxDistance + 1, LEFT) - 1;
				//player.PlaceSelectedAroundBlock(0, -1);
				player.PlaceSelectedAroundBlock(0, -Dis);
			}
			else if (ch == 'L') {
				int Dis = player.GetDistance(maxDistance + 1, RIGHT) - 1;
				//player.PlaceSelectedAroundBlock(0, 1);
				player.PlaceSelectedAroundBlock(0, Dis);
			}
			// 切换指针(1~8)
			else if (ch >= '1' && ch <= '8') {
				int slotIndex = ch - '1';
				player.selectSlot(slotIndex);
			}
			// 丢弃物品(Q) 永久丢弃单个物品
			else if (ch == 't' || ch == 'T') {
				Inventory selected = player.getSelectedSlot();
				if (selected.type != EMPTY && selected.count > 0) {
					// 减少物品栏中该物品的数量
					//player.getSelectedSlot().count--;
					player.subItem(player.getSlot());
					if (player.getSelectedSlot().count <= 0) {
						player.getSelectedSlot() = { EMPTY,0 };
					}
					player.sortInventory();
				}
			}
			// 保存(P)和加载(O)
			else if (ch == 'p' || ch == 'P') {
				// 保存地图
				SaveMap save(player);
				if (save.saveMap(saveFile)) {
					printf("地图保存成功！\n");
				}
				else {
					printf("地图保存失败！\n");
				}
			}
			else if (ch == 'o' || ch == 'O') {
				// 加载地图
				SaveMap save(player);
				if (save.loadMap(saveFile)) {
					printf("地图加载成功！\n");
				}
				else {
					printf("地图加载失败！\n");
				}
			}
			// 打开背包(E)
			else if (ch == 'e' || ch == 'E') {
				while (true) {
					printf("%s", ANSI::CLEAR.c_str());
					// 打印所有物品
					for (int i = 0; i < player.getINVSize(); i++) {
						printf("%2d:", i + 1);
						if (player.getItem(i).count > 0) {//如果物品栏不为空
							printf("%sx%2d",
								BlockChars[player.getItem(i).type].c_str(),
								player.getItem(i).count);
						}
						else {
							printf("     ");//空格占位
						}
						printf("  ");
					
						if ((i + 1) % 8 == 0) {
							printf("\n\n");
						}
					}
					printf("\n\n>>>");

					int ch_ = _getch();
					if (ch_ == 'e' || ch_ == 'E') {
						printf("%s", ANSI::CLEAR.c_str());
						printf("%s", ANSI::CURSOR_HOME.c_str());
						break;
					}
					else if (ch_ == '/') {
						std::cout << "/";
						std::string cmd;// 指令
						std::cin >> cmd;
						player.command(cmd);
					}
				}
			}
			
			printf("                \n");// 用空格覆盖
			if (mode == SURVIVAL) {
				// 生存模式：玩家自动下落
				player.AdaptPos();
			}
		}
		printf("退出中...\n");
		Sleep(500);
		printf("%s", ANSI::CLEAR.c_str());// 清屏
		printf("%s", ANSI::SHOW_CURSOR.c_str());// 显示光标
	}
};