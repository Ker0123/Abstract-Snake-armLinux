#include <iostream>

#include "vector2.h"
#include "lcd.h"
#include "touch.h"
#include "dots.h"
#include "menu.h"

using namespace std;
static LCD_Manager lcd;
static Touch_Manager touch;

/*------------------游戏设置-------------------*/
int snake_T = 600;                     // 蛇运动周期
int food_T = 200;                      // 食物尝试刷新、运动周期
int killer_T = 1200;                   // 杀手食物刷新、运动周期
int P_food = 6;                        // 更多的食物最大生成几率(p/100)
int Ptime = 5;                         // 保护状态最大周期数（对于杀手周期）
int mapselect = 1;                     // 当前选择的地图
int selection[6] = {1, 1, 1, 1, 1, 1}; // 选择的选项
/*---------------------------------------------------------*/

void change_option(int argc, char *argv[]);

bool createRandomFood(Map *map, Snake *snake, Food *food, Vector2<int> range);

void UpdateForbiden(Vector2<int> *Forbiden, int &amt, Map *map, Snake *snake, Food *food, Vector2<int> range);

void Update_scoreBoard(double scoreRate, int rawScore, int proctectTime);

int main()
{
    /**
     * 菜单
     * 蛇移速: < 龟速(1000ms) - 默认(600ms) - 较快(400ms) - 兔速(300ms) >
     * 食物移速: < 饥荒(800ms) - 默认(200ms) - 较快(150ms) - 糖浆时代(80ms) >
     * 生气食物移速: < 四体不勤(2000ms) - 默认(1200ms) - 较快(1000ms) - 全民皆兵(400ms) >
     * 食物生成几率: < 单抽出金(2%) - 默认(6%) - 较高(8%) - 生育潮(12%) >
     * 威慑回合长度: < 不息怒火(1turn) - 默认(5turn) - 和平年代(9turn) - 懦夫(12turn) >
     * 地图选择: < 四四方方x10 - 四四方方x8 - 有点障碍x12 - 拥挤x10 >
     * 开始游戏
     */

    // 加载字体
    lcd.Load_FontImage("./res/font image han.bmp", "./res/font image han index.txt", {32, 250});
    lcd.font_inversion = true;
    // 创建要用到的图层,加载图片资源,加载背景
    lcd.add_layer("background");
    lcd.add_layer("map");
    lcd.add_layer("obj");
    lcd.add_layer("UI");
    lcd.add_layer("dynamic_UI");
    lcd.load_image("./res/head 800x480.bmp", "background");
    lcd.load_image("./res/grass.bmp", "grass");
    lcd.load_image("./res/wall.bmp", "wall");
    lcd.load_image("./res/head.bmp", "head");
    lcd.load_image("./res/body.bmp", "body");
    lcd.load_image("./res/body_turn.bmp", "body_turn");
    lcd.load_image("./res/tail.bmp", "tail");
    lcd.load_image("./res/food.bmp", "food");
    lcd.load_image("./res/food_terrified.bmp", "food_terrified");
    lcd.load_image("./res/food_annoyed.bmp", "food_annoyed");
    lcd.draw_image("background", {0, 0}, "background", {0, 0}, 0x00);
    lcd.update();
    // 显示菜单
    lcd.Print_Text("UI", {450, 30}, "配置菜单", 0.8f, 0xFFFFFFFF, {0, 0});
    lcd.Print_Text("UI", {450, 100}, "蛇移速:       <", 0.5f, 0xFFFFFFFF, {-4, 0});
    lcd.Print_Text("UI", {750, 100}, ">", 0.5f, 0xFFFFFFFF, {0, 0});
    lcd.Print_Text("UI", {450, 150}, "食物移速:     <", 0.5f, 0xFFFFFFFF, {-4, 0});
    lcd.Print_Text("UI", {750, 150}, ">", 0.5f, 0xFFFFFFFF, {0, 0});
    lcd.Print_Text("UI", {450, 200}, "生气食物移速: <", 0.5f, 0xFFFFFFFF, {-4, 0});
    lcd.Print_Text("UI", {750, 200}, ">", 0.5f, 0xFFFFFFFF, {0, 0});
    lcd.Print_Text("UI", {450, 250}, "食物生成几率: <", 0.5f, 0xFFFFFFFF, {-4, 0});
    lcd.Print_Text("UI", {750, 250}, ">", 0.5f, 0xFFFFFFFF, {0, 0});
    lcd.Print_Text("UI", {450, 300}, "威慑回合长度: <", 0.5f, 0xFFFFFFFF, {-4, 0});
    lcd.Print_Text("UI", {750, 300}, ">", 0.5f, 0xFFFFFFFF, {0, 0});
    lcd.Print_Text("UI", {450, 350}, "地图选择:     <", 0.5f, 0xFFFFFFFF, {-4, 0});
    lcd.Print_Text("UI", {750, 350}, ">", 0.5f, 0xFFFFFFFF, {0, 0});
    lcd.font_inversion = false;
    lcd.Print_Text("UI", {500, 400}, "开始游戏", 0.8f, 0xFFFFFFFF, {0, 0});
    lcd.font_inversion = true;
    lcd.update();
    char **args = NULL;
    args = (char *[]){"snake_T", "nothing", NULL};
    change_option(3, args);
    args = (char *[]){"food_T", "nothing", NULL};
    change_option(3, args);
    args = (char *[]){"killer_T", "nothing", NULL};
    change_option(3, args);
    args = (char *[]){"P_food", "nothing", NULL};
    change_option(3, args);
    args = (char *[]){"Ptime", "nothing", NULL};
    change_option(3, args);
    args = (char *[]){"mapselect", "nothing", NULL};
    change_option(3, args);
    // 创建切换按钮, 连接切换显示文字和配置的函数
    AbstractButton switcher[12];
    for (int i = 0; i < 12; i += 2)
    {
        switcher[i] = AbstractButton({600, 90 + 50 * (i / 2)}, {60, 40});
        switcher[i + 1] = AbstractButton({720, 90 + 50 * (i / 2)}, {60, 40});
    }
    // 开始游戏按钮
    AbstractButton start_game = AbstractButton({500, 400}, {200, 40});
    // 进入菜单循环, 开始触控监听, 获取点击位置, 匹配按下的按钮, 调用相应的函数
    touch.Start_Listen();
    bool enter_game = false;
    bool is_pressing = false;
    bool was_pressing = false;
    Vector2<int> touchPos;
    while (!enter_game)
    {
        is_pressing = touch.Is_Pressing();
        if (!was_pressing && is_pressing) // 上升沿
        {
            was_pressing = true;
            usleep(100000);
            touchPos = touch.Get_Current_Position();
        }
        else if (was_pressing && !is_pressing) // 下降沿
        {
            was_pressing = false;
            continue;
        }
        else // 高电平或者低电平
        {
            continue;
        }
        // 只有遇到上升沿才会执行以下代码
        if (start_game.isClicked(touchPos)) // 是否点击开始按钮
        {
            enter_game = true;
            lcd.clear_layer("UI");
            lcd.clear_layer("dynamic_UI");
            lcd.update();
            break;
        }
        for (int i = 0; i < 12; i++) // 遍历查询是否点击了选项按钮
        {
            if (switcher[i].isClicked(touchPos))
            {
                // 擦除原有文字所在区域
                lcd.erase_layer("dynamic_UI", {640, 100 + 50 * (i / 2)}, {120, 40});

                // 根据按下的按钮组合命令, 调用相应的函数
                char **args = NULL;
                switch (i)
                {
                case 0: // 蛇移速-
                    args = (char *[]){"snake_T", "left", NULL};
                    break;
                case 1: // 蛇移速+
                    args = (char *[]){"snake_T", "right", NULL};
                    break;
                case 2: // 食物移速-
                    args = (char *[]){"food_T", "left", NULL};
                    break;
                case 3: // 食物移速+
                    args = (char *[]){"food_T", "right", NULL};
                    break;
                case 4: // 生气食物移速-
                    args = (char *[]){"killer_T", "left", NULL};
                    break;
                case 5: // 生气食物移速+
                    args = (char *[]){"killer_T", "right", NULL};
                    break;
                case 6: // 食物生成几率-
                    args = (char *[]){"P_food", "left", NULL};
                    break;
                case 7: // 食物生成几率+
                    args = (char *[]){"P_food", "right", NULL};
                    break;
                case 8: // 威慑回合长度-
                    args = (char *[]){"Ptime", "left", NULL};
                    break;
                case 9: // 威慑回合长度+
                    args = (char *[]){"Ptime", "right", NULL};
                    break;
                case 10: // 地图选择-
                    args = (char *[]){"mapselect", "left", NULL};
                    break;
                case 11: // 地图选择+
                    args = (char *[]){"mapselect", "right", NULL};
                    break;
                }
                change_option(3, args);
            }
        }
    }
    // 游戏运行时用到的变量
    cout << "应用配置..." << endl;
    Map *map = new Map(lcd, mapselect);
    Vector2<int> range_ = map->getRange();
    Food *food = new Food(lcd, range_);
    Snake *snake = new Snake(lcd, range_);
    int rawscore = 0;                              // 原始分数
    double scoreRate;                              // 分数倍率
    Vector2<int> Forbiden[range_.x * range_.y];    // 已经被占用的坐标
    int forbidAmount = 0;                          // 已经被占用的坐标数量
    int maxFood = (range_.x * range_.y) / (4 * 4); // 最多存在的食物数量
    clock_t timer;                                 // 当前循环所在时间点
    long snake_C = 0;                              // 蛇上一次运动时间点
    long food_C = 0;                               // 食物上一次尝试运动、刷新时间点
    long killer_C = 0;                             // 杀手食物上一次运动、刷新时间点
    int protectTime = 0;                           // 保护状态还剩多少周期（对于杀手周期）
    char input = '\0';                             // 当前周期附带的输入(无输入则为0)
    // 根据参数计算分数倍率:
    printf("scoreRate = 1.0 * (1000 - @%d) / 400 + 1.0 * (2000 - @%d) / 800 + 1.0 * (@%d - 800) / 600 + 1.0 * (12 - @%d) / 4 + 1.0 * (12 - @%d) / 4\n", snake_T, killer_T, food_T, P_food, Ptime);
    scoreRate = 1.0 * (1000 - snake_T) / 2000 + 1.0 * (2000 - killer_T) / 4000 - 1.0 * (food_T - 800) / 3000 + 1.0 * (12 - P_food) / 30 + 1.0 * (12 - Ptime) / 35;
    if (scoreRate > 1.0)
    {
        scoreRate = scoreRate / 1.637619 * 4; // 随着难度提高,倍率最多到4(已知按原式得到的最高倍率为1.637619)
    }
    // 默认: 600 1200 200 6 5
    // 最次: 1000 2000 800 12 12

    // 游戏运行时的信息显示
    lcd.Print_Text("UI", {450, 30}, "计分板", 0.8f, 0xFFFFFFFF, {-4, 0});
    lcd.Print_Text("UI", {450, 100}, "难度系数:", 0.5f, 0xFFFFFFFF, {-4, 0});
    lcd.Print_Text("UI", {450, 150}, "身体长度:", 0.5f, 0xFFFFFFFF, {-4, 0});
    lcd.Print_Text("UI", {450, 200}, "最终得分:", 0.5f, 0xFFFFFFFF, {-4, 0});
    Update_scoreBoard(scoreRate, rawscore, protectTime);

    // 正式开始
    cout << "游戏开始..." << endl;
    while (true) // 游戏循环
    {
        // 记录循环开始时间点	单位：ms
        timer = clock() / 1000;

        // 以snake_T的周期进入如下过程
        if (timer > snake_C + snake_T)
        {
            // 获取输入
            Vector2<int> inputDir = touch.Get_Direction(100, true);
            if (abs(inputDir.x) > abs(inputDir.y))
                input = inputDir.x > 0 ? 'd' : 'a';
            else if (abs(inputDir.x) < abs(inputDir.y))
                input = inputDir.y > 0 ? 's' : 'w';
            else
                input = '\0';

            snake_C = timer;
            int result = 0;

            // 尝试更改前进方向
            snake->changeHeadfor(input);
            // 记录旧头尾所在位置
            Vector2<int> oldhead = snake->getHeadLocation();
            Vector2<int> oldtail = snake->getTailLocation();
            // 计算新头应该在的位置
            Vector2<int> newhead = {oldhead.x + snake->headFor.x, oldhead.y + snake->headFor.y};
            // 判定是否能吃到食物
            result = food->eat(newhead); // eat方法吃掉了返回1,没吃掉返回0
            // 如果吃掉了食物原始分数+1，进入一段保护时间,更新计分板显示
            if (result)
            {
                rawscore++;
                protectTime = Ptime;
                Update_scoreBoard(scoreRate, rawscore, protectTime);
            }
            // 移动并判定是否撞到自身
            result = snake->move(result); // move方法生长模式传入1,不生长模式传入0,正常前进返回0,撞到自身返回-1，生长成功返回1
            // 如果保护时间结束，受到惊吓食物有概率生气
            // 惊吓新的周围的食物
            food->terrify(newhead - Vector2<int>(1, 1), newhead + Vector2<int>(1, 1));
            // 判定是否撞到墙
            if (!map->isWalkable(newhead))
            {
                Update_scoreBoard(scoreRate, rawscore, 0);
                lcd.Print_Text("dynamic_UI", {50, 50}, "蛇被撞,就会死!!", 0.5f, 0xFF606060, {-4, 0});
                lcd.Print_Text("dynamic_UI", {50, 100}, "点击任意位置结束", 0.3f, 0xFF606060, {-1, 0});
                lcd.update();
                while (!touch.Is_Pressing())
                    ;
                system("./AS_arm.o &");
                return 0;
            }

            if (result == -1)
            {
                Update_scoreBoard(scoreRate, rawscore, 0);
                lcd.Print_Text("dynamic_UI", {50, 50}, "衔尾之蛇!!", 0.8f, 0xFF606060, {-4, 0});
                lcd.Print_Text("dynamic_UI", {50, 100}, "点击任意位置结束", 0.3f, 0xFF606060, {-1, 0});
                lcd.update();
                while (!touch.Is_Pressing())
                    ;
                system("./AS_arm.o &");
                return 0;
            }
            if (protectTime >= 0)
            {
                // TODO: 显示保护时间
                Update_scoreBoard(scoreRate, rawscore, protectTime);
                protectTime--;
            }
            input = '\0';
        }

        // 以food_T的周期进入如下过程
        if (timer > food_C + food_T)
        {
            food_C = timer;
            // 检测食物数量
            int amt = food->getAmount();
            // 没有食物->生成食物
            if (amt == 0)
            {
                createRandomFood(map, snake, food, map->getRange());
            }
            else
            {
                // 食物越多->生成几率越小
                int p = (1.0 - 1.0 * amt / maxFood) * P_food;
                srand(timer);
                if (p > rand() % 100)
                {
                    createRandomFood(map, snake, food, map->getRange());
                }
            }

            // 更新禁区点集，让食物在限定范围内动起来
            UpdateForbiden(Forbiden, forbidAmount, map, snake, food, map->getRange());// TODO: 需要优化效率
            food->move(Forbiden, forbidAmount, timer);
        }

        // 以killer_T周期进入如下分支
        if (timer > killer_C + killer_T)
        {
            killer_C = timer;
            // 保护时间结束，每回合每个受惊吓食物有4%几率变成杀手
            if (protectTime <= 0)
            {
                food->transf(0x0100, 0x0200, 4, timer);

                // 食物来追蛇头了
                if (food->attack(map, snake))
                {
                    Update_scoreBoard(scoreRate, rawscore, 0);
                    lcd.Print_Text("dynamic_UI", {50, 50}, "食材打人了!!", 0.8f, 0xFF606060, {-4, 0});
                    lcd.Print_Text("dynamic_UI", {50, 100}, "点击任意位置结束", 0.3f, 0xFF606060, {-1, 0});
                    lcd.update();
                    while (!touch.Is_Pressing())
                        ;
                    system("./AS_arm.o &");
                    return 0;
                }
            }
        }
    }
}

void change_option(int argc, char *argv[])
{
    // 参数列表: 设置选项名 左移还是右移
    string option_name = argv[0];
    string left_or_right = argv[1];
    if (option_name == "snake_T")
    {
        if (left_or_right == "left")
            selection[0] = (selection[0] + 4 - 1) % 4;
        if (left_or_right == "right")
            selection[0] = (selection[0] + 1) % 4;
        switch (selection[0])
        {
        case 0:
            snake_T = 1000;
            lcd.Print_Text("dynamic_UI", {640, 100}, "龟速(1000ms)", 0.4f, 0xFF20A020, {-4, 0});
            break;
        case 1:
            snake_T = 600;
            lcd.Print_Text("dynamic_UI", {640, 100}, "默认(600ms)", 0.4f, 0xFF508020, {-4, 0});
            break;
        case 2:
            snake_T = 400;
            lcd.Print_Text("dynamic_UI", {640, 100}, "较快(400ms)", 0.4f, 0xFF805020, {-4, 0});
            break;
        case 3:
            snake_T = 300;
            lcd.Print_Text("dynamic_UI", {640, 100}, "兔速(300ms)", 0.4f, 0xFFA02020, {-4, 0});
            break;
        }
    }
    if (option_name == "food_T")
    {
        if (left_or_right == "left")
            selection[1] = (selection[1] + 4 - 1) % 4;
        if (left_or_right == "right")
            selection[1] = (selection[1] + 1) % 4;
        switch (selection[1])
        {
        case 0:
            food_T = 800;
            lcd.Print_Text("dynamic_UI", {640, 150}, "饥荒(800ms)", 0.4f, 0xFF20A020, {-4, 0});
            break;
        case 1:
            food_T = 200;
            lcd.Print_Text("dynamic_UI", {640, 150}, "默认(200ms)", 0.4f, 0xFF508020, {-4, 0});
            break;
        case 2:
            food_T = 150;
            lcd.Print_Text("dynamic_UI", {640, 150}, "较快(150ms)", 0.4f, 0xFF805020, {-4, 0});
            break;
        case 3:
            food_T = 80;
            lcd.Print_Text("dynamic_UI", {640, 150}, "糖浆时代(80ms)", 0.4f, 0xFFA02020, {-4, 0});
            break;
        }
    }
    if (option_name == "killer_T")
    {
        if (left_or_right == "left")
            selection[2] = (selection[2] + 4 - 1) % 4;
        if (left_or_right == "right")
            selection[2] = (selection[2] + 1) % 4;
        switch (selection[2])
        {
        case 0:
            killer_T = 2000;
            lcd.Print_Text("dynamic_UI", {640, 200}, "四体不勤(2000ms)", 0.4f, 0xFF20A020, {-4, 0});
            break;
        case 1:
            killer_T = 1200;
            lcd.Print_Text("dynamic_UI", {640, 200}, "默认(1200ms)", 0.4f, 0xFF508020, {-4, 0});
            break;
        case 2:
            killer_T = 1000;
            lcd.Print_Text("dynamic_UI", {640, 200}, "较快(1000ms)", 0.4f, 0xFF805020, {-4, 0});
            break;
        case 3:
            killer_T = 400;
            lcd.Print_Text("dynamic_UI", {640, 200}, "兔速(400ms)", 0.4f, 0xFFA02020, {-4, 0});
            break;
        }
    }
    if (option_name == "P_food")
    {
        if (left_or_right == "right")
            selection[3] = (selection[3] + 4 - 1) % 4;
        if (left_or_right == "left")
            selection[3] = (selection[3] + 1) % 4;
        switch (selection[3])
        {
        case 0:
            P_food = 2;
            lcd.Print_Text("dynamic_UI", {640, 250}, "单抽出金(2%)", 0.4f, 0xFFA02020, {-4, 0});
            break;
        case 1:
            P_food = 6;
            lcd.Print_Text("dynamic_UI", {640, 250}, "默认(6%)", 0.4f, 0xFF805020, {-4, 0});
            break;
        case 2:
            P_food = 8;
            lcd.Print_Text("dynamic_UI", {640, 250}, "较高(8%)", 0.4f, 0xFF508020, {-4, 0});
            break;
        case 3:
            P_food = 12;
            lcd.Print_Text("dynamic_UI", {640, 250}, "生育潮(12%)", 0.4f, 0xFF20A020, {-4, 0});
            break;
        }
    }

    if (option_name == "Ptime")
    {
        if (left_or_right == "right")
            selection[4] = (selection[4] + 4 - 1) % 4;
        if (left_or_right == "left")
            selection[4] = (selection[4] + 1) % 4;
        switch (selection[4])
        {
        case 0:
            Ptime = 1;
            lcd.Print_Text("dynamic_UI", {640, 300}, "不息怒火(1turn)", 0.4f, 0xFFA02020, {-4, 0});
            break;
        case 1:
            Ptime = 5;
            lcd.Print_Text("dynamic_UI", {640, 300}, "默认(5turn)", 0.4f, 0xFF805020, {-4, 0});
            break;
        case 2:
            Ptime = 9;
            lcd.Print_Text("dynamic_UI", {640, 300}, "和平年代(9turn)", 0.4f, 0xFF508020, {-4, 0});
            break;
        case 3:
            Ptime = 12;
            lcd.Print_Text("dynamic_UI", {640, 300}, "懦夫(12turn)", 0.4f, 0xFF20A020, {-4, 0});
            break;
        }
    }

    if (option_name == "mapselect")
    {
        if (left_or_right == "left")
            selection[5] = (selection[5] + 4 - 1) % 4;
        if (left_or_right == "right")
            selection[5] = (selection[5] + 1) % 4;
        switch (selection[5])
        {
        case 0:
            mapselect = 0; // 四四方方x10
            lcd.Print_Text("dynamic_UI", {640, 350}, "四四方方x10", 0.4f, 0xFF20A020, {-2, 0});
            break;
        case 1:
            mapselect = 1; // 四四方方x8
            lcd.Print_Text("dynamic_UI", {640, 350}, "四四方方x8", 0.4f, 0xFF508020, {-2, 0});
            break;
        case 2:
            mapselect = 2; // 有点障碍x12
            lcd.Print_Text("dynamic_UI", {640, 350}, "有点障碍x12", 0.4f, 0xFF805020, {-2, 0});
            break;
        case 3:
            mapselect = 3; // 拥挤x10
            lcd.Print_Text("dynamic_UI", {640, 350}, "拥挤x10", 0.4f, 0xFFA02020, {-2, 0});
            break;
        }

        // 创建临时对象, 显示临时对象, 删除临时对象
        Map t_map(lcd, mapselect);
        Snake t_snake(lcd, t_map.getRange());
        lcd.clear_layer("map");
        lcd.clear_layer("obj");
        t_map.drawAll();
        t_snake.drawAll();
        lcd.update();
    }

    lcd.update();
}

bool createRandomFood(Map *map, Snake *snake, Food *food, Vector2<int> range)
{
    srand(clock());
    Vector2<int> loc;
    int limit = (range.x - 2) * (range.y - 2);
    for (int i = 0; i < limit; i++)
    {
        loc.x = rand() % (range.x - 2) + 1;
        loc.y = rand() % (range.y - 2) + 1;
        if (food->isFood(loc) || snake->isAnybody(loc) || !map->isWalkable(loc))
        {
            continue;
        }
        else
        {
            food->createOne(loc);
            return true;
        }
    }
    /*一定次数随机不到空位置，就得全遍历了，不过暂时不写*/
    return false;
}

void UpdateForbiden(Vector2<int> *Forbiden, int &amt, Map *map, Snake *snake, Food *food, Vector2<int> range)
{
    amt = 0;
    for (int x = 0; x <= range.x; x++)
        for (int y = 0; y <= range.y; y++)
        {
            if (food->isFood({x, y}) || snake->isAnybody({x, y}) || !map->isWalkable({x, y}))
            {
                Forbiden[amt++] = {x, y};
            }
        }
}

void Update_scoreBoard(double scoreRate, int rawScore, int proctectTime)
{
    lcd.clear_layer("dynamic_UI");
    lcd.Print_Text("dynamic_UI", {560, 100}, to_string(scoreRate), 0.6f, 0xFFFFFFFF, {-4, 0});
    lcd.Print_Text("dynamic_UI", {560, 150}, to_string(rawScore + 3), 0.6f, 0xFFFFFFFF, {-4, 0});
    lcd.Print_Text("dynamic_UI", {560, 200}, to_string((int)(scoreRate * rawScore)), 0.6f, 0xFFFFFFFF, {-4, 0});
    if (proctectTime > 0)
    {
        int color = (0x60 + 0x20 * proctectTime) > 0xFF ? 0xFF : (0x60 + 0x20 * proctectTime);
        color = 0xFF006060 | color << 16;
        lcd.Print_Text("dynamic_UI", {50, 50}, "威慑状态", 0.8f, color, {-2, 0});
        lcd.Print_Text("dynamic_UI", {50, 100}, "(食物不敢反抗)", 0.3f, color, {-1, 0});
    }
    lcd.update();
}

/**
 *  int rawscore = 0;                              // 原始分数
    double scoreRate;                              // 分数倍率
    Vector2<int> Forbiden[range_.x * range_.y];    // 已经被占用的坐标
    int forbidAmount = 0;                          // 已经被占用的坐标数量
    int maxFood = (range_.x * range_.y) / (4 * 4); // 最多存在的食物数量
    clock_t timer;                                 // 当前循环所在时间点
    long snake_C = 0;                              // 蛇上一次运动时间点
    long food_C = 0;                               // 食物上一次尝试运动、刷新时间点
    long killer_C = 0;                             // 杀手食物上一次运动、刷新时间点
    int protectTime = 0;                           // 保护状态还剩多少周期（对于杀手周期）

 */
