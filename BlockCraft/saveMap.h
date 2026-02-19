#pragma once
#include "Player.h"
#include <fstream>
//用于保存和加载地图

class SaveMap {
private:
	Player& player;
	//WorldMap tempMap;     // 临时地图
public:
	SaveMap(Player& p) : player(p) {}

	bool saveMap(const std::string& filename) {
		std::ofstream outFile(filename);
		if (!outFile) {
			return false; // 文件打开失败
		}

		int mapRows = player.getMapData().getRows();
		int mapCols = player.getMapData().getCols();

		int rows = player.GetRow();
		int cols = player.GetCol();
		outFile << rows << " " << cols << "\n"; // 行列数

		// 物品栏数据
		outFile << player.getSlot() << "\n";

		for (int i = 0; i < player.getINVSize(); i++) {
			Inventory inv = player.getItem(i);
			outFile << inv.type << " " << inv.count << "    ";
		}

		for (int r = 0; r < mapRows; ++r) {
			for (int c = 0; c < mapCols; ++c) {
				outFile << player.getMapData().getBlock(r, c) << " ";
			}
			outFile << "\n";
		}
		outFile.close();
		return true;
	}

	bool loadMap(const std::string& filename) {
		std::ifstream inFile(filename);
		if (!inFile) {
			return false; // 文件打开失败
		}

		int mapRows = player.getMapData().getRows();
		int mapCols = player.getMapData().getCols();

		int rows, cols;
		inFile >> rows >> cols; // 行列数
		player.SetPos(rows, cols); // 恢复玩家位置

		int slotIndex;
		inFile >> slotIndex; // 选中槽位

		for (int i = 0; i < player.getINVSize(); i++) {
			int type, cnt;
			inFile >> type >> cnt; // 物品栏数据
			//player.getItem(i) = {
			//	static_cast<BlockType>(type),
			//	cnt};
			player.setItem(i, static_cast<BlockType>(type), cnt);
		}
		for (int r = 0; r < mapRows; ++r) {
			for (int c = 0; c < mapCols; ++c) {
				int blockType;
				inFile >> blockType; // 地图数据
				player.getMapData().setBlock(r, c, static_cast<BlockType>(blockType));
			}
		}
		player.selectSlot(slotIndex); // 恢复选中槽位
		inFile.close();
		return true;
	}
};
