#pragma once

#include "VecMap.h"

// 玩家类，包含玩家位置和移动方法
struct Inventory {
	BlockType type;
	int count;
};

enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Player {
private:
	WorldMap& worldMap; // 游戏地图

	int posRow;        // 玩家当前位置行号
	int posCol;        // 玩家当前位置列号

	int INVSize = 64; // 物品栏大小
	int slotSize = 64; // 每个槽最大数量
	int slot = 0;// 当前选中槽位
	std::vector<Inventory> inventory = std::vector<Inventory>(INVSize, { EMPTY,0 });

public:
	
	Player(WorldMap& map, int row = 5, int col = 0) :
		worldMap(map), posRow(row), posCol(col) {
		AdaptPos();
	}
	Player(WorldMap& map, int col = 0) :
		worldMap(map), posRow(5), posCol(col) {
		AdaptPos();
	}
	
	void AdaptPos(BlockType playerT = PLAYER) {
		// 适配角色的位置
		for (int r = posRow; r < worldMap.getRows() - 1; r++) {
			if (worldMap.mapData[r + 1][posCol] == AIR) {
				// 下方是空气，继续下落
				worldMap.mapData[posRow][posCol] = AIR; // 清除旧位置
				posRow = r + 1;
				worldMap.mapData[posRow][posCol] = playerT; // 设置新位置
			}
			else {
				// 找到支撑点，停止下落
				break;
			}
		}
	}

	int GetRow() const { return posRow; }
	int GetCol() const { return posCol; }

	int SetPos(int newRow, int newCol) {
		// 直接设置位置（用于初始化或传送等情况）
		if (newRow >= 0 && newRow < worldMap.getRows() &&
			newCol >= 0 && newCol < worldMap.getCols()) {
			worldMap.mapData[posRow][posCol] = AIR; // 清除旧位置
			posRow = newRow;
			posCol = newCol;
			worldMap.mapData[posRow][posCol] = PLAYER; // 设置新位置
			return 0;
		}
		return -1; // 位置无效
	}
	bool Move(int dRow, int dCol, BlockType playerT = PLAYER) {
		// 计算新位置
		int newRow = posRow + dRow;
		int newCol = posCol + dCol;
		// 检查新位置是否在地图范围内且是空气
		if (newRow >= 0 && newRow < worldMap.getRows() &&
			newCol >= 0 && newCol < worldMap.getCols() &&
			worldMap.mapData[newRow][newCol] == AIR &&
			worldMap.mapData[newRow][newCol] != BORDER) {
			// 更新玩家位置
			worldMap.mapData[posRow][posCol] = AIR; // 将旧位置设为空气
			posRow = newRow;
			posCol = newCol;
			worldMap.mapData[posRow][posCol] = playerT; // 将新位置设为玩家
			return true; // 移动成功
		}
		return false; // 移动失败
	}

	void printWindow(int viewSize) const {
		// 打印玩家周围的地图区域
		worldMap.printMap(posCol - viewSize, posCol + viewSize, posRow - viewSize, posRow + viewSize);
	}

	//查看玩家离最近方块的距离 (可交互的最远距离)
	int GetDistance(int maxDis, Direction dir)const {
		int distance = 1;
		int row = posRow;
		int col = posCol;

		while (distance < maxDis) {
			switch (dir) {
			case UP:    row--; break;
			case DOWN:  row++; break;
			case LEFT:  col--; break;
			case RIGHT: col++; break;
			}

			if (row < 0 || row >= worldMap.getRows() ||
				col < 0 || col >= worldMap.getCols() ||
				worldMap.getBlock(row, col) != AIR) {
				break;
			}
			distance++;
		}
		return distance;
	}

	#pragma region 方块操作
	void DigBlock(int targetRow, int targetCol) {
		if (targetRow >= 0 && targetRow < worldMap.getRows() &&
			targetCol >= 0 && targetCol < worldMap.getCols() &&
			worldMap.mapData[targetRow][targetCol] != BORDER &&
			worldMap.mapData[targetRow][targetCol] <= canDig &&
			canAddItem(static_cast<BlockType>(worldMap.getBlock(targetRow, targetCol)))) {
			// 挖掘方块+拾取物品
			BlockType dugType = static_cast<BlockType>(worldMap.getBlock(targetRow, targetCol));
			addItem(dugType);
			worldMap.setBlock(targetRow, targetCol, AIR);
		}
	}
	void DigAroundBlock(int dRow, int dCol) {// 相对于玩家位置挖掘方块
		int targetRow = posRow + dRow;
		int targetCol = posCol + dCol;
		DigBlock(targetRow, targetCol);
	}
	bool PlaceBlock(int targetRow, int targetCol, BlockType type) {
		if (targetRow >= 0 && targetRow < worldMap.getRows() &&
			targetCol >= 0 && targetCol < worldMap.getCols() &&
			worldMap.mapData[targetRow][targetCol] == AIR) {
			// 放置方块
			worldMap.mapData[targetRow][targetCol] = type;
			return true;
		}
		return false;
	}
	bool PlaceSelectedBlock(int targetRow, int targetCol) {
		Inventory selected = getSelectedSlot();
		if (selected.type != EMPTY && selected.count > 0) {
			if (PlaceBlock(targetRow, targetCol, selected.type)) {
				// 减少物品栏中该物品的数量
				//inventory[slot].count--;
				subItem(slot);
				if (inventory[slot].count <= 0) {
					inventory[slot] = { EMPTY,0 };
				}
				sortInventory();
				return true;
			}
		}
		return false;
	}
	bool PlaceSelectedAroundBlock(int dRow, int dCol) {
		int targetRow = posRow + dRow;
		int targetCol = posCol + dCol;
		return PlaceSelectedBlock(targetRow, targetCol);
	}
	bool PlaceAroundBlock(int dRow, int dCol, BlockType type) {// 相对于玩家位置放置方块
		int targetRow = posRow + dRow;
		int targetCol = posCol + dCol;
		return PlaceBlock(targetRow, targetCol, type);
	}
	#pragma endregion

	#pragma region 物品栏
	int getINVSize() const {
		return INVSize;
	}
	void selectSlot(int slotIndex) {
		if (slotIndex >= 0 && slotIndex < INVSize) {
			slot = slotIndex;
		}
	}
	Inventory getItem(int slot)const {
		if (slot >= 0 && slot < INVSize) {
			return inventory[slot];
		}
		return { EMPTY,0 }; // 无效槽位返回空
	}
	Inventory getSelectedSlot() const {
		return inventory[slot];
	}
	void sortInventory() {
		// 将非空槽位移到前面
		/*
		std::vector<Inventory> newInv;
		for (const auto& item : inventory) {
			if (item.count > 0) {
				newInv.push_back(item);
			}
		}
		while (newInv.size() < INVSize) {
			newInv.push_back({ EMPTY,0 });
		}
		inventory = newInv;
		*/
		
		// 合并相同类型的物品 O(n^2) (合并物品&清理无效项)
		for (int i = 0; i < INVSize - 1; ++i) {
			if (inventory[i].type != EMPTY) {
				for (int j = i + 1; j < INVSize; ++j) {
					if (inventory[j].type == inventory[i].type) {
						int totalCnt = inventory[i].count + inventory[j].count;
						if (totalCnt <= slotSize) {
							// 可以完全合并
							inventory[i].count = totalCnt;
							inventory[j] = { EMPTY,0 };
						}
					}
				}
			}
		}
	}
	void addItem(BlockType type, int cnt = 1) {
		// 向物品栏添加物品
		while (cnt > 0) {
			int tempCnt = cnt;
			bool added = false;
			for (auto& slot : inventory) {
				if (slot.type == type && slot.count < slotSize) {
					int space = slotSize - slot.count;
					int toAdd = std::min(space, cnt);// 计算可添加数量
					slot.count += toAdd;// 添加物品
					cnt -= toAdd;// 减少剩余数量
					if (cnt <= 0) {
						added = true;
						break;
					}
				}
			}
			if (added) {
				break;
			}
			// 尝试放入空槽位
			for (auto& slot : inventory) {
				if (slot.type == EMPTY) {// 是空槽位
					int toAdd = std::min(slotSize, cnt);// 计算可添加数量
					slot = { type, toAdd };
					cnt -= toAdd;
					if (cnt <= 0) {
						added = true;
						break;
					}
				}
			}
			if (!added) break; // 无法添加更多，退出循环
		}
		sortInventory();
	}
	bool canAddItem(BlockType type, int cnt = 1) const {
		// 检查物品栏是否有足够空间添加指定数量的物品
		int Space = 0;
		for (const auto& slot : inventory) {// 遍历每个槽位
			if (slot.type == type) {// 相同类型
				Space += (slotSize - slot.count);
			}
			else if (slot.type == EMPTY) {// 空槽位
				Space += slotSize;
			}
			if (Space >= cnt) {
				return true; // 有足够空间
			}
		}
		return false; // 空间不足
	}
	void subItem(int slot, int cnt = 1) {
		// 从指定槽位减少物品数量
		if (slot >= 0 && slot < INVSize) {
			if (inventory[slot].count >= cnt) {
				inventory[slot].count -= cnt;
				if (inventory[slot].count <= 0) {
					inventory[slot] = { EMPTY,0 };
				}
				sortInventory();
			}
		}
	}
	int getSlot() const {
		return slot;
	}
	bool setItem(int slot, BlockType type, int cnt) {
		// 设置指定槽位的物品类型和数量（用于加载存档等）
		if (slot >= 0 && slot < INVSize &&
			cnt >= 0 && cnt <= slotSize) {
			inventory[slot] = { type, cnt };
			sortInventory();
			return true;
		}
		return false; // 无效槽位
	}
	#pragma endregion

	WorldMap& getMapData() const {
		return worldMap;
	}

	bool command(std::string cmd) {
		if (cmd == "swap") {// 交换物品
			int a, b;
			std::cin >> a >> b;
			a--, b--;
			if (a >= INVSize || a < 0 ||
				b >= INVSize || b < 0 || a == b) {
				return false;
			}
			std::swap(inventory[a],inventory[b]);
		}
		else if (cmd == "clear") { // 清空物品栏
			for (auto& slot : inventory) {
				slot = { EMPTY,0 };
			}
		}
		else if (cmd == "throw") {// 扔掉物品
			int slot;
			std::cin >> slot;
			slot--;
			if (slot >= INVSize || slot < 0) {
				return false;
			}
			subItem(slot);
		}
		else if (cmd == "throw_all") {// 扔掉该槽所有物品
			int slot;
			std::cin >> slot;
			slot--;
			if (slot >= INVSize || slot < 0) {
				return false;
			}
			subItem(slot, inventory[slot].count);
		}
		else {
			return false; // 未识别
		}
		return true; // 成功
	}
};