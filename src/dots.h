#pragma once
#include <vector>

#include "vector2.h"
#include "lcd.h"

using namespace std;

struct dot
{
public:
	int x;
	int y;
	int t;
};

class Dots
{
private:
public:
	LCD_Manager *lcd;	 // 传入LCD管理器
	vector<dot> dots;	 // 以容器形式存储点集
	Vector2<int> cursor; // 虚拟屏幕指针位置
	Vector2<int> range;	 // 存储操作范围

	virtual void drawSingel(const dot one) = 0; // 传入一个点，绘制一个点
	virtual void drawAll() = 0;					// 绘制所有点

	dot *get(Vector2<int> loc);				// 传坐标x,y,返回找到的第一个点
	dot *get(Vector2<int> loc, int start);	// 传坐标x,y,从start找起来，返回第一个找到的点
	bool find(Vector2<int> loc, int start); // 传坐标x,y,从start开始，如果有该坐标的点，返回真，否则假
	void cursorTo(int x, int y);			// 将指针移动到(l,c)处
	void cursorTo(Vector2<int> loc);		// 将指针移动到loc处
};

class Map : Dots
{
private:
	void drawSingel(const dot one); // 传入点，绘制点
public:
	void drawAll();							   // 绘制全图   待优化速度
	Map(LCD_Manager &lcd, Vector2<int> range); // 初始化时要给出范围,自动生成地图
	Map(LCD_Manager &lcd, int mapselect);	   // 初始化给出选择什么地图
	void resetMap(int mapselect);			   // 重置地图
	Vector2<int> getRange();				   // 获取地图大小
	void drawSingel(Vector2<int> loc);		   // 传入坐标，绘制点（其实就是找到点的指针调用另一个重构）
	bool isWalkable(Vector2<int> loc);		   // 查找第一个该坐标的点的类型是不是"可通行"的
};

class Snake : Dots
{
private:
	void drawSingel(dot one); // 传入一个点，绘制一个点（好像只有内部用到）
public:
	Vector2<int> headFor; // 当前运动方向，二维向量

	Snake(LCD_Manager &lcd, Vector2<int> range, Vector2<int> headFor = Vector2<int>(0, -1)); // 初始要传入操作范围，初始方向向上
	void drawAll();																			 // 绘制所有体节，一般只在初始化时用到
	int move(bool isGrow);																	 // 传入移动方向以及是否生长，进行移动，撞到身体返回-1,吃到食物返回1
	void changeHeadfor(char key);															 // 传入键值，尝试控制方向
	Vector2<int> getHeadLocation();															 // 获取头位置
	Vector2<int> getTailLocation();															 // 获取尾位置
	bool isAnybody(Vector2<int> loc, bool icHead = true);									 // 查询坐标下是否有体节,可选择是否包括头
};

class Food : Dots
{
private:
	void drawSingel(const dot one); // 传入容器中一个点，绘制一个食物
	void drawAll();					// 绘制所有食物
public:
	Food(LCD_Manager &lcd, Vector2<int> range_);				   // 初始化时要给出范围
	int getAmount(char t = 0);									   // 查询食物数量，可以查询特定种类食物数量（目前没啥用）
	bool createOne(Vector2<int> loc);							   // 在某个坐标创建一个普通食物（或许能造点不普通的）
	bool isFood(Vector2<int> loc);								   // 判断某处有没有食物
	bool eat(Vector2<int> loc);									   // 尝试吃某处的食物，有就吃掉返回真，无就返回假
	void terrify(Vector2<int> start, Vector2<int> end);			   // 使某个范围的食物受到惊吓
	void transf(int t0, int t1, int p_in_100, int seed = 0);	   // 食物类型转化(前后种类，转化成功几率)
	void move(Vector2<int> forbiden[], int arrlen, int rseed = 0); // 使可以动的食物移动一次
	void getFoodLoc(Vector2<int> locs[], int t);				   // 获取某种食物的坐标（存放多个位置的数组）
	bool attack(Map *map, Snake *snake);						   // 生气食物向蛇头行进
};
