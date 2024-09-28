#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <fstream>
#include <string>

#include "class.h"
using namespace std;

static struct termios old, current;

bool kbhit()
{
	int byteswaiting;
	ioctl(0, FIONREAD, &byteswaiting);
	return byteswaiting > 0;
}

string itos(int i)
{
	string s;
	do
	{
		s.insert(s.begin(), i % 10 + '0');
		i /= 10;
	} while (i);
	return s;
}

bool createRandomFood(Map *map, Snake *snake, Food *food, vector2 range)
{
	srand(clock());
	int l, c;
	int limit = (range.l - 2) * (range.c - 2);
	for (int i = 0; i < limit; i++)
	{
		l = rand() % (range.l - 2) + 1;
		c = rand() % (range.c - 2) + 1;
		if ((*food).isFood(l, c) || (*snake).isAnybody(l, c) || !(*map).isWalkable(l, c))
		{
			continue;
		}
		else
		{
			(*food).createOne(l, c);
			return true;
		}
	}
	/*一定次数随机不到空位置，就得全遍历了，不过暂时不写*/
	return false;
}

bool readConfig(string key, int *valueTo)
{
	// 打开文件
	ifstream file;
	file.open("config.ini");
	if (!file.is_open())
	{
		return false;
	}
	// 逐行读取，直到读到key或者读完文件
	string line;
	while (!file.eof())
	{
		file >> line;
		if (!line.compare(key)) // 找到键值
		{
			file >> line;
			if (!line.compare("=")) // 紧接着是等于号
			{
				file >> line;
				*valueTo = stoi(line);
				file.close();
				return true;
			}
		}
	}
	file.close();
	return false;
}

bool writeConfig(string key, int *valueTo)
{
	fstream file;
	file.open("config.ini", ios::in | ios::out);
	if (!file.is_open())
	{
		return false;
	}

	// 读指针依次后移直到连续两次一次键值一次等于号，一次值
	string readLine;
	while (!file.eof())
	{
		file >> readLine;
		if (readLine.compare(key))
		{
			continue;
		}
		file >> readLine;
		if (readLine.compare("="))
		{
			continue;
		}
		// 读写指针是共同前进的！
		// 读取数据获取其长度
		file >> readLine;
		int oldLen = readLine.length();
		// 根据原字符串长度组合新字符串
		string newValue = itos(*valueTo);
		if (newValue.length() < oldLen)
		{
			newValue.append(oldLen - newValue.length(), ' ');
		}
		// 将写指针前移等长度字符,写入数据
		file.seekp(-1 * oldLen, ios::cur);
		file << newValue << flush;
	}
	// 关闭文件
	file.close();
	return true;
}
// 传入所有对象和forbiden数组的指针，返回点数量和更新禁区点集，最蠢的方法
void UpdateForbiden(vector2 *Forbiden, int *amt, Map *map, Snake *snake, Food *food, vector2 range)
{
	*amt = 0;
	for (int l = 0; l <= range.l; l++)
		for (int c = 0; c <= range.c; c++)
		{
			if ((*food).isFood(l, c) || (*snake).isAnybody(l, c) || !(*map).isWalkable(l, c))
			{
				Forbiden[(*amt)++] = {l, c};
			}
		}
}

int main()
{
INIT:
	system("clear");
	/*------------控制台参数-----------*/
	tcgetattr(0, &old);
	current = old;
	current.c_lflag &= ~ICANON;
	current.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &current);
	/*-------------------------------*/

	/*------------测试区域-------------*/
	/*--------------------------------*/

	/*------------------主程序循环用到的参数----------------------*/
	int snake_T;   // 蛇运动周期
	int food_T;	   // 食物尝试刷新、运动周期
	int killer_T;  // 杀手食物刷新、运动周期
	int P_food;	   // 更多的食物最大生成几率(p/100)
	int Ptime;	   // 保护状态最大周期数（对于杀手周期）
	int mapselect; // 当前选择的地图
	/*---------------------------------------------------------*/

	// 读取配置
	bool flag = true;
	flag *= readConfig("snake.t", &snake_T);
	flag *= readConfig("food.t", &food_T);
	flag *= readConfig("killer.t", &killer_T);
	flag *= readConfig("food.spawnRate", &P_food);
	flag *= readConfig("proctect.t", &Ptime);
	flag *= readConfig("map.select", &mapselect);
	if (!flag)
	{
		cout << "配置读取出错" << endl;
		return -1;
	}

MENU:
	// 进入菜单界面，调配参数
	bool start = false;
	TextMenu menu({0, 0}, {12, 20});
	int menuInput = '\0';
	int select = 0;

	// 主菜单循环
	while (true)
	{
		menu.clearMenu();
		menu.resetMessages();
		menu.addMessage(1, {2, 5}, 0, "抽象贪食蛇 by kerol");
		menu.addMessage(2, {4, 7}, 1, "开始游戏");
		menu.addMessage(3, {5, 7}, 1, "修改参数");
		menu.addMessage(4, {7, 8}, 1, "退出");
		menu.addMessage(5, {10, 12}, 0, "🤑🫱 🫱 👏");
		menu.addMessage(6, {10, 2}, 0, "💚");
		menu.pointOn = 2;
		menu.printMenu(1);

		// 主菜单输入时的一个彩蛋
		// 直到按下回车，不能退出主界面
		int temp = 20;
		do
		{
			if (temp > 10)
			{
				temp--;
			}
			else if (temp > 2)
			{
				menu.delMessage(5);
				menu.addMessage(5, {10, temp}, 0, "🤑🫱 🫱 👏");
				menu.printMessage(5);
				temp--;
			}
			else if (temp >= 0)
			{
				menu.delMessage(5);
				menu.addMessage(5, {10, temp}, 0, "🤩🫱 🫱 🫱 👏");
				menu.printMessage(5);
				temp--;
			}
			else if (temp >= -3)
			{
				menu.delMessage(5);
				menu.addMessage(5, {10, 0}, 0, "🥴🫱🫱🫱 👏");
				menu.printMessage(5);
				temp--;
			}
			else if (temp >= -6)
			{
				menu.delMessage(5);
				menu.addMessage(5, {10, 0}, 0, "😵🫱🫱🫱 👏");
				menu.printMessage(5);
				temp--;
			}
			else if (temp >= -7)
			{
				menu.delMessage(5);
				menu.addMessage(5, {10, 0}, 0, "😇👣👣👣👣");
				menu.printMessage(5);
				temp--;
			}
			else if (temp >= -8)
			{
				menu.delMessage(5);
				menu.addMessage(5, {10, 0}, 0, ";        ");
				menu.printMessage(5);
				temp--;
			}

			menuInput = getchar();
			select = menu.controlMenu(menuInput);
		} while (menuInput != '\n');

		// 在主菜单选择了开始游戏->开始游戏
		if (select == 2)
		{
			start = true;
			break;
		}

		// 在主菜单选择了修改参数->修改参数菜单
		else if (select == 3)
		{
			bool quit = false;
			while (!quit)
			{
				menu.clearMenu();
				menu.resetMessages();
				menu.addMessage(1, {1, 1}, 0, "修改参数");
				menu.addMessage(2, {3, 1}, 1, "蛇周期   " + itos(snake_T));
				menu.addMessage(3, {4, 1}, 1, "食物周期 " + itos(food_T));
				menu.addMessage(4, {5, 1}, 1, "杀手周期 " + itos(killer_T));
				menu.addMessage(5, {7, 1}, 1, "返回");
				menu.addMessage(6, {3, 10}, 1, "食物生成 " + itos(P_food));
				menu.addMessage(7, {4, 10}, 1, "保护时间 " + itos(Ptime));
				menu.addMessage(8, {5, 10}, 1, "地图     " + itos(mapselect));
				menu.pointOn = 2;
				menu.printMenu(1);

				select = menu.select();
				if (select == 2) // -> 按下2 -> 蛇周期
				{
					string msg = "当前蛇周期为: ";
					string app = itos(snake_T);
					msg.append(app);

					menu.clearMenu();
					menu.resetMessages();
					menu.addMessage(1, {1, 1}, 0, "蛇周期");
					menu.addMessage(2, {2, 1}, 0, "- 蛇多少毫秒移动一次");
					menu.addMessage(3, {3, 1}, 0, "- 如果与食物同时移动,视作蛇先移动");
					menu.addMessage(4, {5, 1}, 1, "默认(600ms)");
					menu.addMessage(5, {6, 1}, 1, "龟速(1000ms)");
					menu.addMessage(6, {7, 1}, 1, "较快(400ms)");
					menu.addMessage(7, {8, 1}, 1, "兔速(300ms)");
					menu.addMessage(8, {10, 1}, 1, "返回");
					menu.addMessage(9, {10, 10}, 0, msg);
					menu.pointOn = 4;
					menu.printMenu(1);

					select = menu.select();
					switch (select)
					{
					case 4:
						snake_T = 600;
						break;
					case 5:
						snake_T = 1000;
						break;
					case 6:
						snake_T = 400;
						break;
					case 7:
						snake_T = 300;
						break;
					case 8:
						break;
					default:
						break;
					}
				}

				else if (select == 3) // 按下3 -> 食物周期
				{
					string msg = "当前食物周期为: ";
					string app = itos(food_T);
					msg.append(app);

					menu.clearMenu();
					menu.resetMessages();
					menu.addMessage(1, {1, 1}, 0, "食物周期");
					menu.addMessage(2, {2, 1}, 0, "- 食物多少毫秒进行一次判定");
					menu.addMessage(3, {3, 1}, 0, "- 包含移动判定和生成新食物(有上限)判定");
					menu.addMessage(4, {5, 1}, 1, "默认(200ms)");
					menu.addMessage(5, {6, 1}, 1, "饥荒(800ms)");
					menu.addMessage(6, {7, 1}, 1, "较快(150ms)");
					menu.addMessage(7, {8, 1}, 1, "糖浆时代(80ms)");
					menu.addMessage(8, {10, 1}, 1, "返回");
					menu.addMessage(9, {10, 10}, 0, msg);
					menu.pointOn = 4;
					menu.printMenu(1);

					select = menu.select();
					switch (select)
					{
					case 4:
						food_T = 200;
						break;
					case 5:
						food_T = 800;
						break;
					case 6:
						food_T = 150;
						break;
					case 7:
						food_T = 80;
						break;
					case 8:
						break;
					default:
						break;
					}
				}

				else if (select == 4) // 按下4 -> 杀手周期
				{
					string msg = "当前杀手周期为: ";
					string app = itos(killer_T);
					msg.append(app);

					menu.clearMenu();
					menu.resetMessages();
					menu.addMessage(1, {1, 1}, 0, "杀手周期");
					menu.addMessage(2, {2, 1}, 0, "- 生气的食物多少毫秒进行一次判定");
					menu.addMessage(3, {3, 1}, 0, "- 只影响移动速度，生成速度被蛇周期、保护时间限制");
					menu.addMessage(4, {5, 1}, 1, "默认(1200ms)");
					menu.addMessage(5, {6, 1}, 1, "四体不勤(2000ms)");
					menu.addMessage(6, {7, 1}, 1, "较快(1000ms)");
					menu.addMessage(7, {8, 1}, 1, "全民皆兵(400ms)");
					menu.addMessage(8, {10, 1}, 1, "返回");
					menu.addMessage(9, {10, 10}, 0, msg);
					menu.pointOn = 4;
					menu.printMenu(1);

					select = menu.select();
					switch (select)
					{
					case 4:
						killer_T = 1200;
						break;
					case 5:
						killer_T = 2000;
						break;
					case 6:
						killer_T = 1000;
						break;
					case 7:
						killer_T = 400;
						break;
					case 8:
						break;
					default:
						break;
					}
				}

				else if (select == 6) // 按下6 -> 食物生成
				{
					string msg = "当前每周期生成食物概率: ";
					string app = itos(P_food);
					msg.append(app);

					menu.clearMenu();
					menu.resetMessages();
					menu.addMessage(1, {1, 1}, 0, "食物生成");
					menu.addMessage(2, {2, 1}, 0, "- 每个食物周期会以此概率生成第二个食物");
					menu.addMessage(3, {3, 1}, 0, "- 单位为百分比。第三个、第四个概率会更低");
					menu.addMessage(4, {5, 1}, 1, "默认(6%)");
					menu.addMessage(5, {6, 1}, 1, "单抽出金(2%))");
					menu.addMessage(6, {7, 1}, 1, "较快(8%)");
					menu.addMessage(7, {8, 1}, 1, "生育潮(12%)");
					menu.addMessage(8, {10, 1}, 1, "返回");
					menu.addMessage(9, {10, 6}, 0, msg);
					menu.pointOn = 4;
					menu.printMenu(1);

					select = menu.select();
					switch (select)
					{
					case 4:
						P_food = 6;
						break;
					case 5:
						P_food = 2;
						break;
					case 6:
						P_food = 8;
						break;
					case 7:
						P_food = 12;
						break;
					case 8:
						break;
					default:
						break;
					}
				}

				else if (select == 7) // 按下7 -> 保护时间
				{
					string msg = "当前保护时间: ";
					string app = itos(Ptime);
					msg.append(app);

					menu.clearMenu();
					menu.resetMessages();
					menu.addMessage(1, {1, 1}, 0, "保护时间");
					menu.addMessage(2, {2, 1}, 0, "- 吃掉食物造成威慑,其他食物不敢生气");
					menu.addMessage(3, {3, 1}, 0, "- 已经愤怒的食物也会暂时停止攻击");
					menu.addMessage(4, {5, 1}, 1, "默认(5)");
					menu.addMessage(5, {6, 1}, 1, "不息怒火(1))");
					menu.addMessage(6, {7, 1}, 1, "和平年代(9)");
					menu.addMessage(7, {8, 1}, 1, "懦夫(12)");
					menu.addMessage(8, {10, 1}, 1, "返回");
					menu.addMessage(9, {10, 6}, 0, msg);
					menu.pointOn = 4;
					menu.printMenu(1);

					select = menu.select();
					switch (select)
					{
					case 4:
						Ptime = 5;
						break;
					case 5:
						Ptime = 1;
						break;
					case 6:
						Ptime = 9;
						break;
					case 7:
						Ptime = 12;
						break;
					case 8:
						break;
					default:
						break;
					}
				}

				else if (select == 8) // 按下8 -> 地图
				{
					string msg = "当前地图: ";
					string app = itos(mapselect);
					msg.append(app);

					menu.clearMenu();
					menu.resetMessages();
					menu.addMessage(1, {1, 1}, 0, "地图");
					menu.addMessage(2, {2, 1}, 0, "- 太麻烦了,所以没有做特殊地图");
					menu.addMessage(3, {3, 1}, 0, "- 就是说所有地图都是有边界且只有边界的");
					menu.addMessage(4, {5, 1}, 1, "0 - 四四方方10*10");
					menu.addMessage(5, {6, 1}, 1, "1 - 四四方方8*8");
					menu.addMessage(6, {7, 1}, 1, "2 - 有点障碍12*12");
					menu.addMessage(7, {8, 1}, 1, "3 - 拥挤10*10");
					menu.addMessage(8, {10, 1}, 1, "返回");
					menu.addMessage(9, {10, 6}, 0, msg);
					menu.pointOn = 4;
					menu.printMenu(1);

					select = menu.select();
					switch (select)
					{
					case 4:
						mapselect = 0;
						break;
					case 5:
						mapselect = 1;
						break;
					case 6:
						mapselect = 2;
						break;
					case 7:
						mapselect = 3;
						break;
					case 8:
						break;
					default:
						break;
					}
				}

				else if (select == 5) // 按下5 -> 退出配置菜单
				{
					quit = true;
				}
			}
		}

		// 在主菜单选择了退出 -> 保存配置并退出
		else if (select == 4)
		{
			bool flag = true;
			flag *= writeConfig("snake.t", &snake_T);
			flag *= writeConfig("food.t", &food_T);
			flag *= writeConfig("killer.t", &killer_T);
			flag *= writeConfig("food.spawnRate", &P_food);
			flag *= writeConfig("proctect.t", &Ptime);
			flag *= writeConfig("map.select", &mapselect);
			if (!flag)
			{
				cout << "配置保存过程中出错" << endl;
				return -1;
			}
			return 0;
		}
	}
	menu.clearMenu();

	/*------------------初始化对象，以及一些它们要用的数据------------------*/
	Map map(mapselect);
	vector2 Range = map.getRange(); // 游戏界面范围大小
	Food food(Range);
	Snake snake(Range);
	TextMenu scoreboard({0, Range.c + 1}, {8, 10});
	int rawscore = 0;							 // 原始分数
	double scoreRate;							 // 分数倍率
	vector2 Forbiden[Range.l * Range.c];		 // 已经被占用的坐标
	int forbidAmount = 0;						 // 已经被占用的坐标数量
	int maxFood = (Range.l * Range.c) / (4 * 4); // 最多存在的食物数量
	clock_t timer;								 // 当前循环所在时间点
	long snake_C = 0;							 // 蛇上一次运动时间点
	long food_C = 0;							 // 食物上一次尝试运动、刷新时间点
	long killer_C = 0;							 // 杀手食物上一次运动、刷新时间点
	int protectTime = 0;						 // 保护状态还剩多少周期（对于杀手周期）
	char input = '\0';							 // 当前周期附带的输入(无输入则为0)

	/*----------------------------------------------------------------*/

	// 根据参数计算分数倍率:
	// 与蛇运动周期成反比			300 600 1000	r = 1+(600-x)/400
	// 与杀手食物判定周期成反比		 400 1200 2000	r = 1+(1200-x)/800
	// 与食物刷新周期成正比			80 200 800		r = 1+(x-200)/600
	// 与食物生成几率成反比			2 6 12			r = 1+(6-x)/4
	// 与保护时间成反比				1 5 12			r = 1+(3-x)/3
	scoreRate = 1 + 1.0 * (600 - snake_T) / 400 + 1.0 * (1200 - killer_T) / 800 + 1.0 * (food_T - 200) / 600 + 1.0 * (6 - P_food) / 4 + 1.0 * (5 - Ptime) / 4;
	// 初始打印计分板
	scoreboard.addMessage(0, {1, 0}, 0, "计分板");
	scoreboard.addMessage(2, {3, 0}, 0, "难度倍率: " + to_string(scoreRate));
	scoreboard.addMessage(1, {5, 0}, 0, "吃掉食物: " + itos(rawscore));
	scoreboard.addMessage(3, {6, 0}, 0, "最终分数: " + to_string((int)(rawscore * scoreRate)));
	scoreboard.addMessage(4, {8, 0}, 0, "威慑状态: " + itos(protectTime));
	scoreboard.printMenu(false);
	while (true) // 游戏周期
	{
		// 记录循环开始时间点	单位：ms
		timer = clock() / 1000;
		// 循环开始获取输入,对方向键前后缀进行过滤
		if (kbhit())
		{
			while (true)
			{
				input = getchar();
				if (input == 27 || input == 91)
				{
					continue;
				}
				break;
			}
		}

		// 以snake_T的周期进入如下过程
		if (timer > snake_C + snake_T)
		{
			snake_C = timer;
			int result = 0;

			// 尝试更改前进方向
			snake.changeHeadfor(input);
			// 记录旧头尾所在位置
			vector2 oldhead = snake.getHeadLocation();
			vector2 oldtail = snake.getTailLocation();
			// 计算新头应该在的位置
			vector2 newhead = {oldhead.l + snake.headFor.l, oldhead.c + snake.headFor.c};
			// 判定是否能吃到食物
			result = food.eat(newhead.l, newhead.c); // eat方法吃掉了返回1,没吃掉返回0
			// 如果吃掉了食物原始分数+1，进入一段保护时间,更新计分板显示
			if (result)
			{
				rawscore++;
				protectTime = Ptime;
				scoreboard.addMessage(1, {3, 1}, 0, "吃掉食物: " + itos(rawscore));
				scoreboard.addMessage(3, {5, 1}, 0, "最终分数: " + to_string((int)(rawscore * scoreRate)));
				scoreboard.printMessage(1);
				scoreboard.printMessage(3);
				scoreboard.printMessage(4);
			}
			// 移动并判定是否撞到自身
			result = snake.move(result); // move方法生长模式传入1,不生长模式传入0,正常前进返回0,撞到自身返回-1，生长成功返回1
			// 如果保护时间结束，受到惊吓食物有概率生气
			// 惊吓新的周围的食物
			food.terrify({newhead.l - 1, newhead.c - 1}, {newhead.l + 1, newhead.c + 1});
			// 判定是否撞到墙
			if (!map.isWalkable(newhead.l, newhead.c))
			{
				cout << "蛇被撞，就会死！！\n\n按任意键回主菜单..." << endl;
				getchar();
				system("clear");
				goto MENU;
			}

			if (result == -1)
			{
				cout << "衔尾之蛇,象征着无穷无尽！！\n\n按任意键回主菜单..." << endl;
				getchar();
				system("clear");
				goto MENU;
			}
			if (protectTime >= 0)
			{
				scoreboard.addMessage(4, {6, 1}, 0, "威慑状态: " + itos(protectTime));
				scoreboard.printMessage(4);
				protectTime--;
			}
			input = '\0';
		}

		// 以food_T的周期进入如下过程
		if (timer > food_C + food_T)
		{
			food_C = timer;
			// 检测食物数量
			int amt = food.getAmount();
			// 没有食物->生成食物
			if (amt == 0)
			{
				createRandomFood(&map, &snake, &food, Range);
			}
			else
			{
				// 食物越多->生成几率越小
				int p = (1.0 - 1.0 * amt / maxFood) * P_food;
				srand(timer);
				if (p > rand() % 100)
				{
					createRandomFood(&map, &snake, &food, Range);
				}
			}

			// 更新禁区点集，让食物在限定范围内动起来
			UpdateForbiden(Forbiden, &forbidAmount, &map, &snake, &food, Range);
			food.move(Forbiden, forbidAmount, timer);
		}

		// 以killer_T周期进入如下分支
		if (timer > killer_C + killer_T)
		{
			killer_C = timer;
			// 保护时间结束，每回合每个受惊吓食物有4%几率变成杀手
			if (protectTime <= 0)
			{
				food.transf(0x0100, 0x0200, 4, timer);

				// 食物来追蛇头了
				if (food.attack(&map, &snake))
				{
					cout << "食材打人了！！\n\n按任意键回主菜单..." << endl;
					getchar();
					system("clear");
					goto MENU;
				}
			}
		}
	}
}
