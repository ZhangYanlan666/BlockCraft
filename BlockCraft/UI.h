#pragma once
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include "VecMap.h"

// 用于显示游戏界面(开始界面,结束界面等)

namespace UI {
	std::string start[] = {
	"########    ##          ######      ######    ##      ##",
	"##      ##  ##        ##      ##  ##      ##  ##    ##  ",
	"##      ##  ##        ##      ##  ##      ##  ##  ##    ",
	"##      ##  ##        ##      ##  ##          ##  ##    ",
	"########    ##        ##      ##  ##          ####      ",
	"##      ##  ##        ##      ##  ##          ##  ##    ",
	"##      ##  ##        ##      ##  ##      ##  ##  ##    ",
	"##      ##  ##        ##      ##  ##      ##  ##    ##  ",
	"########    ########    ######      ######    ##      ##",
	};
	void Start() {
		printf("%s", ANSI::CLEAR.c_str());// 清屏

		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < start[i].size(); j++) {
				if (start[i][j] == '#') {
					printf("%s ", ANSI::BG_BRIGHT_CYAN.c_str());
				}
				else {
					//printf("  ");
					printf("%s ", ANSI::RESET.c_str());
				}
				printf("%s", ANSI::RESET.c_str());
			}
			printf("%s\n", ANSI::RESET.c_str());
		}

		printf("\n\n欢迎来到BlockCraft！\n");
		printf("按任意键开始游戏...\n");
		(void)_getch();
	}
	void End() {
		Sleep(1000);
		printf("%s", ANSI::CLEAR.c_str());// 清屏
		printf("感谢游玩！再见！\n\n");
		Sleep(1000);
	}
}
