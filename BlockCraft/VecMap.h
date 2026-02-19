#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>

// 用于存储地图信息

namespace ANSI {
	inline const std::string RESET = "\033[0m";// 重置颜色

	inline const std::string BLACK = "\033[30m";// 黑色
	inline const std::string RED = "\033[31m";// 红色
	inline const std::string GREEN = "\033[32m";// 绿色
	inline const std::string YELLOW = "\033[33m";// 黄色
	inline const std::string BLUE = "\033[34m";// 蓝色
	inline const std::string MAGENTA = "\033[35m";// 品红色
	inline const std::string CYAN = "\033[36m";// 青色
	inline const std::string WHITE = "\033[37m";// 白色

	//高亮颜色 
	inline const std::string GRAY = "\033[90m";// 灰色
	inline const std::string BRIGHT_RED = "\033[91m";// 高亮红色
	inline const std::string BRIGHT_GREEN = "\033[92m";// 高亮绿色
	inline const std::string BRIGHT_YELLOW = "\033[93m";// 高亮黄色
	inline const std::string BRIGHT_BLUE = "\033[94m";// 高亮蓝色
	inline const std::string BRIGHT_MAGENTA = "\033[95m";// 高亮品红色
	inline const std::string BRIGHT_CYAN = "\033[96m";// 高亮青色
	inline const std::string BRIGHT_WHITE = "\033[97m";// 高亮白色
	inline const std::string BG_BROWN = "\033[48;5;94m";// 棕色背景


	// 背景颜色
	inline const std::string BG_RED = "\033[41m";// 背景红色
	inline const std::string BG_GREEN = "\033[42m";// 背景绿色
	inline const std::string BG_YELLOW = "\033[43m";// 背景黄色
	inline const std::string BG_BLUE = "\033[44m";// 背景蓝色
	inline const std::string BG_MAGENTA = "\033[45m";// 背景品红色
	inline const std::string BG_CYAN = "\033[46m";// 背景青色
	inline const std::string BG_WHITE = "\033[47m";// 背景白色
	inline const std::string BG_BLACK = "\033[40m";// 背景黑色
	inline const std::string BG_GRAY = "\033[100m";// 背景灰色

	// 高亮背景颜色
	inline const std::string BG_BRIGHT_RED = "\033[101m";// 高亮背景红色
	inline const std::string BG_BRIGHT_GREEN = "\033[102m";// 高亮背景绿色
	inline const std::string BG_BRIGHT_YELLOW = "\033[103m";// 高亮背景黄色
	inline const std::string BG_BRIGHT_BLUE = "\033[104m";// 高亮背景蓝色
	inline const std::string BG_BRIGHT_MAGENTA = "\033[105m";// 高亮背景品红色
	inline const std::string BG_BRIGHT_CYAN = "\033[106m";// 高亮背景青色
	inline const std::string BG_BRIGHT_WHITE = "\033[107m";// 高亮背景白色

	//字体样式
	inline const std::string BOLD = "\033[1m";// 粗体
	inline const std::string DIM = "\033[2m";// 弱化
	inline const std::string ITALIC = "\033[3m";// 斜体
	inline const std::string UNDERLINE = "\033[4m";// 下划线
	inline const std::string REVERSED = "\033[7m";// 反转颜色
	inline const std::string HIDDEN = "\033[8m";// 隐藏文本
	inline const std::string STRIKETHROUGH = "\033[9m";// 删除线
	inline const std::string DOUBLE_UNDERLINE = "\033[21m";// 双下划线
	inline const std::string NORMAL = "\033[22m";// 正常字体
	inline const std::string NO_UNDERLINE = "\033[24m";// 取消下划线
	inline const std::string NO_REVERSED = "\033[27m";// 取消反转颜色

	//控制台ANSI
	inline const std::string CLEAR = "\033c";// 清屏
	inline const std::string CLEAR_LINE = "\033[2K";// 清除当前行
	inline const std::string CURSOR_HOME = "\033[H";// 光标移到首页
	inline const std::string CURSOR_UP = "\033[A";// 光标上移一行
	inline const std::string CURSOR_DOWN = "\033[B";// 光标下移一行
	inline const std::string CURSOR_FORWARD = "\033[C";// 光标右移一列
	inline const std::string CURSOR_BACKWARD = "\033[D";// 光标左移一列
	inline const std::string HIDE_CURSOR = "\033[?25l";// 隐藏光标
	inline const std::string SHOW_CURSOR = "\033[?25h";// 显示光标
}

int canDig = 5;  // 可挖掘方块种类数量
enum BlockType{
	EMPTY = 0,	// 占位

	GRASS,		// 草地
	STONE,		// 石头
	DIRT,		// 泥土
	WOOD,		// 木头
	LEAF,		// 树叶
	IRON,		// 铁矿
	DIAMOND,	// 钻石

	AIR,		// 空气
	PLAYER,		// 玩家
	BORDER,		// 边界
};
std::string BlockChars[] = {
	"  ",
	ANSI::BG_BRIGHT_GREEN + "  " + ANSI::RESET,
	ANSI::BG_GRAY + "  " + ANSI::RESET,
	ANSI::BG_YELLOW + "  " + ANSI::RESET,
	ANSI::YELLOW + ANSI::BG_BRIGHT_BLUE + "卌" + ANSI::RESET,
	ANSI::BRIGHT_GREEN + ANSI::BG_BRIGHT_BLUE + "##" + ANSI::RESET,
	ANSI::BG_GRAY + ANSI::BLACK + "::" + ANSI::RESET,
	ANSI::BG_GRAY + ANSI::CYAN + "::" + ANSI::RESET,//◇◇

	ANSI::BG_BRIGHT_BLUE + "  " + ANSI::RESET,
	ANSI::BRIGHT_YELLOW + ANSI::BG_BRIGHT_BLUE + "你" + ANSI::RESET,
	ANSI::BG_RED + "  " + ANSI::RESET,
};

int randNum(int a, int b) {
	return rand() % (b - a + 1) + a;
}

class WorldMap {
private:

	int MapRows = 200;// 行数
	int MapCols = 1000;// 列数

	bool makeTree = true;// 是否生成树木

	#pragma region 地形生成参数

	struct Terrain {
		unsigned int seed = 0;		// 随机种子
		int soilDepth = 3;			// 泥土层厚度
		float terrainScale = 0.012f;// 地形起伏比例(0.01~0.02)
		int groundBase;				// 地面基础高度(行号)
		int noiseAmp = 20;			// 噪声起伏放大倍数(10~20)
		int fbmLayers = 6;			// FBM叠加层数(3~5)

		Terrain(unsigned int s, int rows) :
			seed(s), groundBase(rows / 2) {}
		Terrain(int rows) :
			groundBase(rows / 2) {}
	};

	unsigned int seed = 0; // 随机种子
	const int SOIL_DEPTH = 3;    // 泥土层厚度
	const float TERRAIN_SCALE = 0.012f;// 地形起伏比例(0.01~0.02)
	const int GROUND_BASE = MapRows / 2;// 地面基础高度(行号)
	const int NOISE_AMP = 20;     // 噪声起伏放大倍数(10~20)
	const int FBM_LAYERS = 6;    // FBM叠加层数(3~5)
	#pragma endregion
	
	#pragma region 柏林噪声核心工具函数
	// 线性插值：a到b按比例t过渡(0<=t<=1)
	float lerp(float a, float b, float t) {
		return a * (1 - t) + b * t;
	}

	// 平滑函数：让插值先慢后快再慢，使柏林噪声自然
	float fade(float t) {
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	// 8个2D梯度向量
	void getGradient(int ix, int iy, float& dx, float& dy) {
		// 8个基础方向向量
		const float grads[8][2] = { {1,1},{1,-1},{-1,1},{-1,-1},{1,0},{0,1},{-1,0},{0,-1} };

		srand(seed);
		unsigned int seed2 = ix * 114514 + iy * 123456;
		srand(seed + seed2);
		int r = rand() % 8; // 随机选0~7，对应8个方向
		dx = grads[r][0];
		dy = grads[r][1];
	}

	// 2D柏林噪声核心：输入坐标(x,y)，输出-1~1的自然随机值
	float perlin2D(float x, float y) {
		// 取整数网格角和小数偏移
		int ix = int(x);
		int iy = int(y);
		float fx = x - ix;
		float fy = y - iy;

		// 计算4个网格角的点积值
		float n00, n10, n01, n11;
		float dx, dy;
		getGradient(ix, iy, dx, dy);
		n00 = dx * fx + dy * fy;
		getGradient(ix + 1, iy, dx, dy);
		n10 = dx * (fx - 1) + dy * fy;
		getGradient(ix, iy + 1, dx, dy);
		n01 = dx * fx + dy * (fy - 1);
		getGradient(ix + 1, iy + 1, dx, dy);
		n11 = dx * (fx - 1) + dy * (fy - 1);

		// 平滑偏移后两次插值
		float tx = fade(fx);
		float ty = fade(fy);
		float a = lerp(n00, n10, tx);
		float b = lerp(n01, n11, tx);
		return lerp(a, b, ty);
	}

	// 分形布朗运动FBM：叠加多层噪声，地形有大起伏+小细节
	float fbm(float x, float y) {
		float total = 0.0f;
		float weight = 0.5f; // 每层权重，逐行减半
		float freq = TERRAIN_SCALE; // 每层频率，逐行加倍
		for (int i = 0; i < FBM_LAYERS; ++i) {
			total += perlin2D(x * freq, y * freq) * weight;
			freq *= 2;
			weight *= 0.5;
		}
		return total;
	}

	// 计算指定列的地表高度（核心：用柏林噪声生成每列草皮的行号）
	int getHeight(int col) {
		// 横截面地形：y固定为0，仅让列（x）方向有起伏
		float noise_val = fbm(float(col), 0.0f) + 1.0f; // 噪声值映射到0~2
		// 计算实际地表高度，防越界（避免地表顶到顶部/沉到底部）
		int ground_h = GROUND_BASE + int(noise_val * NOISE_AMP);
		// 高度限制：最小行号20，最大行号=总行列-泥土层-5（防止泥土/石头越界）
		ground_h = std::max(20, std::min(MapRows - SOIL_DEPTH - 5, ground_h));
		return ground_h;
	}
	#pragma endregion

public:
	std::vector<std::vector<int>> mapData;

	WorldMap(int rows, int cols, unsigned int Seed, bool makeTree) : 
		MapRows(rows), 
		MapCols(cols),
		seed(Seed),
		mapData(rows, std::vector<int>(cols)),
		makeTree(makeTree) {}
	WorldMap() : mapData(200, std::vector<int>(1000)) {}
	~WorldMap() {};

	int getRows() const { return MapRows; }
	int getCols() const { return MapCols; }

	int getMapRows() const { return MapRows; }
	int getMapCols() const { return MapCols; }

	void setBlock(int row, int col, BlockType type) {
		if (row >= 0 && row < MapRows && col >= 0 && col < MapCols) {
			mapData[row][col] = type;
		}
	}
	int getBlock(int row, int col) const {
		if (row >= 0 && row < MapRows && col >= 0 && col < MapCols) {
			return mapData[row][col];
		}
		return BlockType::AIR; // 默认返回空气块
	}

	void makeWorld();

	void printMap(int c1,int c2,int r1,int r2) const {
		for (int row = r1; row < r2; ++row) {
			for (int col = c1; col < c2; ++col) {
				//判断为有效范围输出  
				if (row >= 0 && row < MapRows &&
					col >= 0 && col < MapCols) {
					printf("%s",BlockChars[getBlock(row, col)].c_str());
				}
				else {// 超出范围输出空气
					//std::cout << BlockChars[AIR];
					printf("%s", BlockChars[AIR].c_str());
				}
			}
			printf("\n");
		}
	}
};

void WorldMap::makeWorld() {
	srand(seed); // 设置种子

	// 遍历每一列(从左到右)
	for (int col = 0; col < MapCols; ++col) {
		// 1. 获取当前列的地表高度(草皮所在行号)
		int ground_row = getHeight(col);

		// 2. 遍历每一行（从上到下）
		for (int row = 0; row < MapRows; ++row) {
			if (row == ground_row) {
				setBlock(row, col, BlockType::GRASS);
			}
			else if (row > ground_row && row <= ground_row + SOIL_DEPTH) {
				setBlock(row, col, BlockType::DIRT);
			}
			else if (row > ground_row + SOIL_DEPTH) {
				setBlock(row, col, BlockType::STONE);
			}
			else {
				setBlock(row, col, BlockType::AIR);
			}
		}
	}

	// 在草地上随机生成树木
	if (makeTree) {
		for (int col = 0; col < MapCols; ++col) {
			// 以10%的概率在草地上生成树木
			if (randNum(1, 16) == 5) {
				int ground_row = getHeight(col);
				int tree_height = randNum(5, 8); // 树高4~7行
				// 树干（木头）
				for (int h = 1; h <= tree_height; ++h) {
						if (ground_row - h >= 0) {
							setBlock(ground_row - h, col, BlockType::WOOD);
						}
					}
					// 树叶（形成一个菱形层）
				for (int dx = -3; dx <= 3; ++dx) {
						for (int dy = -3; dy <= 3; ++dy) {
							if (abs(dx) + abs(dy) <= 3) { // 菱形范围
								int leaf_row = ground_row - tree_height + dy;
								int leaf_col = col + dx;
								if (leaf_row >= 0 && leaf_row < MapRows &&
									leaf_col >= 0 && leaf_col < MapCols &&
									getBlock(leaf_row, leaf_col) != BORDER) {
									setBlock(leaf_row, leaf_col, BlockType::LEAF);
								}
							}
						}
					}
			}
		}
	}
	
	// 设置边界
	for (int r = 0; r < MapRows; ++r) {
		setBlock(r, 0, BlockType::BORDER);          // 左边界
		setBlock(r, MapCols - 1, BlockType::BORDER);// 右边界
	}
	for (int c = 0; c < MapCols; c++) {
		setBlock(0, c, BlockType::BORDER);			// 上边界
		setBlock(MapRows - 1, c, BlockType::BORDER);// 下边界
	}
}