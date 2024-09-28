#include <iostream>
#include <stdlib.h>

#include "dots.h"

using namespace std;

dot *Dots::get(Vector2<int> loc)
{
	int i = 0;
	for (dot one : dots)
	{
		if (one.x == loc.x && one.y == loc.y)
		{
			dot *findDot = &dots[i];
			return findDot;
		}
		i++;
	}

	return nullptr;
}

dot *Dots::get(Vector2<int> loc, int start)
{
	int i = 0;
	for (dot one : dots)
	{
		if (i < start)
		{
			i++;
			continue;
		}
		if (one.x == loc.x && one.y == loc.y)
		{
			dot *findDot = &dots[i];
			return findDot;
		}

		i++;
	}

	return nullptr;
}

bool Dots::find(Vector2<int> loc, int start = 0)
{
	int i = 0;
	for (dot one : dots)
	{
		if (i < start)
		{
			i++;
			continue;
		}
		if (one.x == loc.x && one.y == loc.y)
		{
			return true;
		}
		i++;
	}
	return false;
}

void Dots::cursorTo(int x, int y)
{
	this->cursor = Vector2<int>(30 + x * 30, 30 + y * 30);
	return;
}

void Dots::cursorTo(Vector2<int> loc)
{
	this->cursor = Vector2<int>(30 + loc.x * 30, 30 + loc.y * 30);
	return;
}

Map::Map(LCD_Manager &lcd, Vector2<int> range)
{
	this->lcd = &lcd;
	range = range;
	for (int x = 0; x < range.x; x++)
	{
		for (int y = 0; y < range.y; y++)
		{
			if (x == 0 || x == range.x - 1 || y == 0 || y == range.y - 1)
			{
				dots.push_back({x, y, 'w'});
			}
			else
			{
				dots.push_back({x, y, 'g'});
			}
		}
	}
}

Map::Map(LCD_Manager &lcd, int mapselect)
{
	this->lcd = &lcd;
	resetMap(mapselect);
}

void Map::resetMap(int mapselect)
{
	dots.clear();
	if (mapselect == 0)
	{
		range = {10, 10};
		bool map_0[10][10] = {
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		};
		for (int l = 0; l < 10; l++)
			for (int c = 0; c < 10; c++)
			{
				if (map_0[l][c])
				{
					dots.push_back({c, l, 'w'});
				}
				else
				{
					dots.push_back({c, l, 'g'});
				}
			}
	}

	if (mapselect == 1)
	{
		range = {8, 8};
		bool map_1[8][8] = {
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
		};
		for (int l = 0; l < 8; l++)
			for (int c = 0; c < 8; c++)
			{
				if (map_1[l][c])
				{
					dots.push_back({c, l, 'w'});
				}
				else
				{
					dots.push_back({c, l, 'g'});
				}
			}
	}
	if (mapselect == 2)
	{
		range = {12, 12};
		bool map_2[12][12] = {
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		};
		for (int l = 0; l < 12; l++)
			for (int c = 0; c < 12; c++)
			{
				if (map_2[l][c])
				{
					dots.push_back({c, l, 'w'});
				}
				else
				{
					dots.push_back({c, l, 'g'});
				}
			}
	}
	if (mapselect == 3)
	{
		range = {10, 10};
		bool map_3[10][10] = {
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 1, 1, 0, 0, 1, 1, 0, 1},
			{1, 0, 1, 1, 0, 0, 1, 1, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 1, 1, 0, 0, 1, 1, 0, 1},
			{1, 0, 1, 1, 0, 0, 1, 1, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		};
		for (int l = 0; l < 10; l++)
			for (int c = 0; c < 10; c++)
			{
				if (map_3[l][c])
				{
					dots.push_back({c, l, 'w'});
				}
				else
				{
					dots.push_back({c, l, 'g'});
				}
			}
	}
}

Vector2<int> Map::getRange()
{
	return range;
}

void Map::drawAll()
{
	for (dot one : dots)
	{
		drawSingel(one);
	}
	lcd->update();
}

void Map::drawSingel(const dot one)
{
	cursorTo(one.x, one.y);
	// 根据type打印不同表情，并刷新输出缓存
	switch (one.t)
	{
	case 'w':
		lcd->draw_image("map", cursor, "wall", {0.5, 0.5});
		break;
	case 'g':
		lcd->draw_image("map", cursor, "grass", {0.5, 0.5});
		break;
	default:
		break;
	}
	return;
}

void Map::drawSingel(Vector2<int> loc)
{
	dot *one = get(loc);
	drawSingel(*one);
}

bool Map::isWalkable(Vector2<int> loc)
{
	for (dot one : dots)
	{
		if (one.x == loc.x && one.y == loc.y)
		{
			if (one.t == 'g')
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

Snake::Snake(LCD_Manager &lcd, Vector2<int> range, Vector2<int> headFor)
{
	this->lcd = &lcd;
	this->range = range;
	this->headFor = headFor;

	Vector2<int> spawn = {range.x / 2, range.y / 2 - 1};
	dots.push_back({spawn.x, spawn.y, 0x01});
	dots.push_back({spawn.x, spawn.y + 1, 0x11});
	dots.push_back({spawn.x, spawn.y + 2, 0x21});
}

void Snake::drawAll()
{
	for (dot one : dots)
	{
		drawSingel(one);
	}
}

void Snake::drawSingel(dot one)
{
	cursorTo(one.x, one.y);
	switch (one.t)
	{
	case 0x00:
		lcd->draw_image("obj", cursor, "grass", {0.5, 0.5});
		break;
	case 0x01:
		lcd->draw_image("obj", cursor, "head", {0.5, 0.5}, 0x00);
		break;
	case 0x02:
		lcd->draw_image("obj", cursor, "head", {0.5, 0.5}, 0x02);
		break;
	case 0x03:
		lcd->draw_image("obj", cursor, "head", {0.5, 0.5}, 0x03);
		break;
	case 0x04:
		lcd->draw_image("obj", cursor, "head", {0.5, 0.5}, 0x01);
		break;
	case 0x11:
		lcd->draw_image("obj", cursor, "body", {0.5, 0.5}, 0x00);
		break;
	case 0x12:
		lcd->draw_image("obj", cursor, "body", {0.5, 0.5}, 0x02);
		break;
	case 0x13:
		lcd->draw_image("obj", cursor, "body", {0.5, 0.5}, 0x03);
		break;
	case 0x14:
		lcd->draw_image("obj", cursor, "body", {0.5, 0.5}, 0x01);
		break;
    case 0x1A:
		lcd->draw_image("obj", cursor, "body_turn", {0.5, 0.5}, 0x00);
		break;
	case 0x1B:
		lcd->draw_image("obj", cursor, "body_turn", {0.5, 0.5}, 0x02);
		break;
	case 0x1C:
		lcd->draw_image("obj", cursor, "body_turn", {0.5, 0.5}, 0x03);
		break;
	case 0x1D:
		lcd->draw_image("obj", cursor, "body_turn", {0.5, 0.5}, 0x01);
		break;
	case 0x21:
		lcd->draw_image("obj", cursor, "tail", {0.5, 0.5}, 0x00);
		break;
	case 0x22:
		lcd->draw_image("obj", cursor, "tail", {0.5, 0.5}, 0x02);
		break;
	case 0x23:
		lcd->draw_image("obj", cursor, "tail", {0.5, 0.5}, 0x03);
		break;
	case 0x24:
		lcd->draw_image("obj", cursor, "tail", {0.5, 0.5}, 0x01);
		break;
	default:
		break;
	}

	lcd->update();
	return;
}

int Snake::move(bool isGrow) // 传入的headfor_有5种情况：4种方向与无方向
{
	// 根据朝向, 把旧头变成身体,再创建新头
	// 低位由旧头方向与新头方向共同决定
	/**
	 * 1 2 3 4 : ⬆️      ⬇️       ⬅️       ➡️
	 * A B C D : 左下角  右上角  右下角  左上角
	 */
    int old_dir = 0x00;
    int new_dir = 0x00;
    old_dir = dots[0].t & 0x0F;
    if(headFor.y < 0)
        new_dir = 1;
    if(headFor.y > 0)
        new_dir = 2;
    if(headFor.x < 0)
        new_dir = 3;
    if(headFor.x > 0)
        new_dir = 4;
    switch ((old_dir << 4) | new_dir)
    {
    case 0x11:
    case 0x12:
        dots[0].t = 0x11;
        break;
    case 0x21:
    case 0x22:
        dots[0].t = 0x12;
        break;
    case 0x33:
    case 0x34:
        dots[0].t = 0x13;
        break;
    case 0x43:
    case 0x44:
        dots[0].t = 0x14;
        break;
    case 0x13:
    case 0x42:
        dots[0].t = 0x1A;
        break;
    case 0x24:
    case 0x31:
        dots[0].t = 0x1B;
        break;
    case 0x14:
    case 0x32:
        dots[0].t = 0x1C;
        break;
    case 0x23:
    case 0x41:
        dots[0].t = 0x1D;
        break;
    default:
        break;
    }

	dot newhead = {dots[0].x + headFor.x, dots[0].y + headFor.y, 0x00};
    newhead.t = new_dir;
	dots.insert(dots.begin(), newhead);
	drawSingel(dots[0]);
	drawSingel(dots[1]);

	// 如果撞倒其他体节，返回-1
	if (find(Vector2<int>(newhead.x, newhead.y), 1))
	{
		return -1;
	}
	// 如果没有生长，删除尾巴，最后一个体节变成尾巴（方向保留）
	if (!isGrow)
	{
		// 最后一节属性改称空气并绘制
		(dots.end() - 1)->t = 0x00;
		drawSingel(*(dots.end() - 1));
		// 删除最后一节的数据
		dots.pop_back();
		// 新的最后一节改属性并绘制(根据与倒数第二节的相对位置确定方向)
		auto newtail = dots.end() - 1;
        auto last_2 = dots.end() - 2;
        if(last_2->y < newtail->y)
            newtail->t = 0x21;
        if(last_2->y > newtail->y)
            newtail->t = 0x22;
        if(last_2->x < newtail->x)
            newtail->t = 0x23;
        if(last_2->x > newtail->x)
            newtail->t = 0x24;
		drawSingel(*newtail);
		return 0;
	}
	// 剩下的情况就是生长了
	return 1;
}

void Snake::changeHeadfor(char key)
{
	// 将输入变为方向向量
	Vector2<int> tryFor;
	switch (key)
	{
	case 'w':
		tryFor = {0, -1};
		break;
	case 's':
		tryFor = {0, 1};
		break;
	case 'd':
		tryFor = {1, 0};
		break;
	case 'a':
		tryFor = {-1, 0};
		break;
	case 65:
		tryFor = {0, -1};
		break;
	case 66:
		tryFor = {0, 1};
		break;
	case 67:
		tryFor = {1, 0};
		break;
	case 68:
		tryFor = {-1, 0};
		break;
	default:
		tryFor = {0, 0};
		break;
	}
	// 新方向与原方向垂直，则录入新方向
	if (tryFor.x * headFor.y || tryFor.y * headFor.x)
	{
		headFor = tryFor;
	}
}

Vector2<int> Snake::getHeadLocation()
{
	return {dots[0].x, dots[0].y};
}

Vector2<int> Snake::getTailLocation()
{
	return {(dots.end() - 1)->x, (dots.end() - 1)->y};
}

bool Snake::isAnybody(Vector2<int> loc, bool icHead)
{
	for (dot one : dots)
	{
		if (one.x == loc.x && one.y == loc.y)
		{
			return true;
		}
	}
	return false;
}

Food::Food(LCD_Manager &lcd, Vector2<int> range)
{
	this->lcd = &lcd;
	this->range = range;
}

int Food::getAmount(char t)
{
	if (t == 0)
	{
		return dots.size();
	}
	int amount = 0;
	for (dot one : dots)
	{
		if (one.t == t)
		{
			amount++;
		}
	}
	return amount;
}

bool Food::createOne(Vector2<int> loc)
{
	dots.push_back({loc.x, loc.y, 0x0000});
	drawSingel(*(dots.end() - 1));
	return true;
}

void Food::drawSingel(const dot one)
{
	cursorTo(one.x, one.y);
	switch (one.t & 0xFF00) // 取高4位判断样式
	{
	case 0x0000:
		lcd->draw_image("obj", cursor, "food", {0.5, 0.5});
		break;
	case 0x0100:
		lcd->draw_image("obj", cursor, "food_terrified", {0.5, 0.5});
		break;
	case 0x0200:
		lcd->draw_image("obj", cursor, "food_annoyed", {0.5, 0.5});
		break;
	case 0x0F00:
		lcd->draw_image("obj", cursor, "grass", {0.5, 0.5});
		break;
	default:
		break;
	}

	lcd->update();
	return;
}

void Food::drawAll()
{
	return;
}

bool Food::isFood(Vector2<int> loc)
{
	for (dot one : dots)
	{
		if (one.x == loc.x && one.x == loc.y)
		{
			return true;
		}
	}
	return false;
}

bool Food::eat(Vector2<int> loc)
{
	for (auto ptr = dots.begin(); ptr < dots.end(); ptr++)
	{
		if (ptr->x == loc.x && ptr->y == loc.y)
		{
			dots.erase(ptr);
			return true;
		}
	}
	return false;
}

void Food::terrify(Vector2<int> start, Vector2<int> end)
{
	for (auto ptr = dots.begin(); ptr < dots.end(); ptr++)
	{
		if (start.y <= ptr->y && start.x <= ptr->x && ptr->y <= end.y && ptr->x <= end.x)
		{
			if (ptr->t == 0x0000)
			{
				ptr->t = 0x0103; // 普通食物变惊恐，给3次移动机会
			}
			drawSingel(*ptr);
		}
	}
}

void Food::transf(int t0, int t1, int p_in_100, int seed)
{
	// 不给随机数种子就自己创建
	if (seed == 0)
	{
		seed = time(0);
	}
	srand(seed);

	// 遍历所有食物，满足条件则以给定几率执行转化，以及绘制
	for (auto one = dots.begin(); one < dots.end(); one++)
	{
		if (p_in_100 > rand() % 100 && (one->t & 0xFF00) == (t0 & 0xFF00))
		{
			one->t &= 0x00FF;
			one->t |= (t1 & 0xFF00);
			drawSingel(*one);
		}
	}
}

void Food::move(Vector2<int> forbiden[], int arrlen, int rseed)
{
	// 不给随机数种子就自己创建
	if (rseed == 0)
	{
		rseed = time(0);
	}
	srand(rseed);

	// 遍历所有食物
	int p = 0;
	for (auto one = dots.begin(); one < dots.end(); one++)
	{
		// 移动次数大于0的食物移动几率为100%
		if ((one->t & 0x00FF) > 0)
		{
			p = 100;
			one->t -= 1;
		}
		// 没有移动次数的食物，按类型分配几率
		else if ((one->t & 0xFF00) == 0x0100) // 惊恐状态下的食物
		{
			p = 30;
		}
		else
		{
			p = 0;
		}

		// 根据随机数决定是否移动
		if (p > rand() % 100)
		{
			Vector2<int> newloc = {0, 0};
			// 算出随机的新地址
			int rnum = rand() % 5 - 2;						 // rnum = -2/-1/0/1/2
			newloc = {one->x + rnum % 2, one->y + rnum / 2}; //	+(-1,0)(0,-1)(0,1)(1,0)
			// 遍历禁区与目标区比对，对上了这个点就不能动
			bool canMove = true;
			for (int i = 0; i < arrlen; i++)
			{
				if (forbiden[i].x == newloc.x && forbiden[i].y == newloc.y)
				{
					canMove = false;
					break;
				}
			}
			// 能动：更新数据，绘制
			if (canMove)
			{
				int oldType = one->t;
				one->t = 0x0F00;
				drawSingel(*one);
				one->y = newloc.y;
				one->x = newloc.x;
				one->t = oldType;
				drawSingel(*one);
			}
		}
	}
}

void Food::getFoodLoc(Vector2<int> locs[], int t)
{
	int amt = 0;
	for (int i = 0; i < dots.size(); i++)
	{
		if ((dots[i].t & 0xFF00) == (t & 0xFF00))
		{
			locs[++amt] = {dots[i].x, dots[i].y};
		}
	}
	locs[0] = {amt, amt}; // 0号元素存坐标数量
	return;
}

bool Food::attack(Map *map, Snake *snake)
{
	Vector2<int> snakeHead;
	Vector2<int> distance;
	Vector2<int> newloc;
	Vector2<int> temp;
	int oldType;
	// 遍历所有食物
	for (auto one = dots.begin(); one < dots.end(); one++)
	{
		// 不是生气的食物，跳过，遍历到下一个
		if ((one->t & 0xFF00) != 0x0200)
		{
			continue;
		}
		// 更新蛇头坐标、坐标差
		snakeHead = snake->getHeadLocation();
		distance = {snakeHead.x - one->x, snakeHead.y - one->y};
		temp = {abs(distance.x), abs(distance.y)};
		// 把距离向量转化为(-1,0)(0,-1)(0,1)(1,0)其中之一
		if (temp.y == temp.x) // 强行使两边不相等
		{
			temp.y++;
		}
		if (temp.y > temp.x)
		{
			distance = {distance.x / temp.y, distance.y / temp.y};
		}
		else
		{
			distance = {distance.x / temp.x, distance.y / temp.x};
		}
		// 准备移动的是否看看地图是否可通行
		newloc = {one->x + distance.x, one->y + distance.y};
		if (!map->isWalkable(newloc))
		{
			return false;
		}
		// 看看是否有其他食物
		if (isFood(newloc))
		{
			return false;
		}
		// 移动！
		oldType = one->t;
		one->t = 0x0F00;
		drawSingel(*one);
		one->x = newloc.x;
		one->y = newloc.y;
		one->t = oldType;
		drawSingel(*one);
		// 移动结束，看看当前位置是不是蛇的体节，是的话攻击成功，返回true。否则返回false
		if (snake->isAnybody(newloc))
		{
			return true;
		}
	}
	return false;
}
