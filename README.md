# 代码文档

------------

## 目录

1. [`main`](#主函数main)
2. [`vector2`](#二维向量vector2)
3. [`lcd`](#屏幕lcd)
4. [`touch`](#触控touch)
5. [`dots`](#点集dots)
6. [`menu`](#菜单menu)

------------

## 主函数`main`

游戏流程控制，包括游戏初始化，菜单循环，加载配置，游戏循环。

实现的游戏玩法：

- 蛇控制：滑动屏幕控制蛇的方向。  
  只会获取两次移动之间的最后一次滑动进行判断、往移动方向相反方向滑动，不会自己撞自己。  
  蛇头接近食物时会使得食物进入`惊慌状态`。  
  蛇吃掉食物后身体变长，进入一段时间的`威慑状态`。  
  蛇头撞上身体后，达成*结局————"衔尾之蛇!"*，游戏结束。  
- 食物控制：食物会发生状态转变，不同的状态行为逻辑不同。  
  正常状态下，停留在原地。  
  `惊慌状态`下，会以特定频率尝试往四个方向移动。不会碰壁、不会撞蛇。一段时间后
  `生气状态`下，会以特定频率(相较惊慌状态更慢)往蛇头方向靠近。撞到蛇的身体达成*结局————"食材打人了"*，游戏结束。  
  `威慑状态`下，食物不会进入`生气状态`。  
- 地图控制：可以在代码里面通过编写二维数组来创建地图，可以通过更改配置修改要游玩的地图。  
  地图只有两种方块：`草地`和`墙`。前者可通行，后者不可通行，蛇撞上去会死达成*结局————"蛇被撞, 就会死!"*，游戏结束

### 函数

- `void change_option(int arg, char *argv[])`  
  传入参数，修改游戏配置  
  只需要第二个参数传入字符串数组:{"设置名"，"更改方向"}就可以了。通过修改代表游戏配置的全局变量来实现。
- `bool createRandomFood(Map *map, Snake *snake, Food *food, Vector2<int> range)`  
  传入对应参数，在指定范围内可通行位置生成食物。  
- `void UpdateForbiden(Vector2<int> *Forbiden, int &amt, Map *map, Snake *snake, Food *food, Vector2<int> range)`  
  更新`禁区`，主要是告诉食物不能生成在禁区内。也不能往禁区移动。`禁区`包括蛇体节所在位置、墙。、
- `void Update_scoreBoard(double scoreRate, int rawScore, int proctectTime)`  
  更新`得分板`，根据裸分、分数倍率、保护时间长度，在屏幕上特定位置显示当前得分，是否处于威慑状态等信息。  
  威慑状态只在有剩余时间的时候才会显示，为红色，但是时间越短颜色越淡。

------------

## 二维向量`vector2`

2D向量类，封装了常用的向量操作，主要是向量运算，向量显示。用操作符重载实现。

重载的运算方式包括：向量之间相加相减、向量与数相乘、相除。

另外给出获取向量长度，向量角度，向量单位化，等函数。

代码较为简单，不多赘述。

------------

## 屏幕`lcd`

屏幕控制类。用以在屏幕上显示图像、文字、几何图形。

包含文本框系统。

### Image类

存储一张图片的必要信息和数据。

#### 字段和方法

- `string name`：图片名
- `int width`：图片宽度
- `int height`：图片高度
- `pixel *data`：图片像素数据，以数组形式存储图片中的所有像素点，按行->列->排列，像素点为32位的ARGB颜色值。
- `Image(string name, int width, int height, pixel *data)`  
  构造函数，传入相关信息直接创建图片对象。

### Layer类

存储一个图层的必要信息和数据。

#### 字段和方法

- `string name`：图层名
- `bool visible`: 图层可见性
- `pixel *pixels`: 图层像素数据
- `Layer(string name, bool visible = true)`  
  构造函数，传入相关信息直接创建图层对象。

### LCD_Manager类

屏幕管理类。作为单例使用，是惟一能够控制屏幕的类。提供图层相关功能、文本框相关功能。

基本运行逻辑是：创建好多个图层，导入需要的图像资源、字体资源。调用相关函数在图层上绘制图像、文字。最后调用`update`函数刷新屏幕，按照图层遮挡关系，在LCD屏幕上显示最终画面。

#### 字段和方法 - 基础

- `int lcd_fd`: 存储屏幕文件的文件描述符。
- `pixel *mmap_ptr`：屏幕内存映射指针。
- `LCD_Manager()`：构造函数，初始化屏幕管理器，打开屏幕设备文件，建立内存映射。
- `~LCD_Manager()`：析构函数，释放屏幕内存映射，关闭屏幕设备文件，删除加载的图片、字体资源。

#### 字段和方法 - 图层系统

- `pixel per_frame[]`: 预备帧。更新图层的时候，先把多图层计算的结果记录在此；调用`update()`的时候，再把图像信息显示到屏幕上。
- `vector<Image> images`: 用于存储加载的图片的容器。
- `vector<Layer> layers`: 用于存储图层的容器。
- `map<string, int> image_map`: 图片名到索引的映射。
- `map<string, int> layer_map`: 图层名到索引的映射。
- `void load_image_bmp(const char *Path, string name)`  
  加载bmp图片。传入图片路径，自己指定的图片名。即能导入图片到类中，导入过程中会在终端输出图片信息。
- `void load_image_jpg(const char *Path, string name)`  
  加载jpg图片。传入图片路径，自己指定的图片名。即能导入图片到类中，导入过程中会在终端输出图片信息。
- `void add_layer(const string &name)`  
  添加图层。指定图层名称即可。图层的顺序自动按照添加的顺序排列：即最先创建的图层在最底层，最后创建的图层在最上层。
- `void remove_layer(包含多个重载)`  
  删除图层。图层将不再参与计算。其上图层依次下移。可以传入图层索引或名称。
- `void clear_layer(包含多个重载)`  
  清空图层。清零图层上所有可见像素点。可以传入图层索引或名称。
- `void show_layer(包含多个重载)`  
  显示图层。控制之后的update()中，是否显示该图层。调用此函数并不会让图片马上显示。可以传入图层索引或名称。
- `void hide_layer(包含多个重载)`  
  隐藏图层。控制之后的update()中，是否显示该图层。调用此函数并不会让图片马上消失。可以传入图层索引或名称。
- `void erase_layer(int index, Vector2<int> start_pos, Vector2<int> size)`  
  擦除指定图层的指定区域。传入图层次序、起始位置、区域大小。擦除指定区域的像素。

#### 字段和方法 - 文本框系统

- `Font font`: 当前字体信息。存储了一套字体的图片尺寸、单个字的尺寸、图像深度、指向图像数据的指针(`pixel`类型)。
- `map<wchar_t, Vector2<int>> font_map`: 字符到字母位置的映射。
- `bool font_inversion`: 是否反色显示。部分二值图片用画板代表颜色，会出现黑白与01对应关系不可视的情况。可以利用反色来正常显示。
- `bool is_han(wchar_t c) const`  
  判断字符是否是汉字。
- `int get_utf8_len_from_first_byte(unsigned char first_byte) const`  
  获取utf8编码的字节数。
- `void Load_FontImage(包含多个重载)`:  
  加载字库图像。有两个重载，第一种：只传入图片路径，会仅解析ASCII码字符，第二种：传入图片路径、字库索引文件路径、切割信息，能解析索引文件中的所有字符，包括汉字。
- `void Print_Char(int layer_index, Vector2<int> scr_pos, wchar_t c, float scale, pixel color)`  
  打印字符到图层。可传入图层索引、屏幕坐标、字符、缩放比例、颜色。
- `void Print_Text(有多个重载)`  
  打印文本到图层。可传入图层索引、文本框起始位置、文本框大小、文本、缩放比例、颜色、左侧对齐偏移量。

```C++
class LCD_Manager
{
private:
    int get_layer_index(const string &name) const; // 获取图层索引
    int get_image_index(const string &name) const; // 获取图片索引
    int lcd_fd;                                    // 屏幕设备文件
    pixel *mmap_ptr;                               // 屏幕内存映射指针
    pixel per_frame[800 * 480];                    // 预备帧
    struct Font
    {
        Vector2<int> image_size;
        Vector2<int> word_size;
        int bits_per_pixel;
        pixel *data;
    } font;                                                                     // 字库图像：属性和数据指针
    vector<Image> images;                                                       // 加载图片的容器
    vector<Layer> layers;                                                       // 图层的容器
    map<wchar_t, Vector2<int>> font_map;                                        // 字符到字母位置的映射
    map<string, int> image_map;                                                 // 图片名到索引的映射
    map<string, int> layer_map;                                                 // 图层名到索引的映射
    void load_image_bmp(const char *Path, string name);                         // 加载bmp图片
    void load_image_jpg(const char *Path, string name);                         // 加载jpg图片
    void printJPEGDecompressStructMembers(const jpeg_decompress_struct &cinfo); // 打印jpeg解码器信息
    bool is_in_range(int x, int y) const;                                       // 判断坐标是否在屏幕范围内
    bool is_in_range(Vector2<int> pos) const;                                   // 判断坐标是否在屏幕范围内

    bool is_han(wchar_t c) const;                                     // 判断是否是汉字
    int get_utf8_len_from_first_byte(unsigned char first_byte) const; // 获取utf8编码的字节数

public:
    LCD_Manager();  // 初始化：建立内存映射
    ~LCD_Manager(); // 析构：释放内存映射

    bool font_inversion; // 是否反色显示

    void Load_FontImage(const char *path_to_font_image);                                        // 加载字库图像
    void Load_FontImage(const char *font_image, const char *font_index_text, Vector2<int> cut); // 加载字库图像
    void load_image(const char *Path, string name);                                             // 加载图片

    void add_layer(const string &name);    // 添加图层
    void remove_layer(int index);          // 删除图层
    void remove_layer(const string &name); // 删除图层
    void clear_layer(int index);           // 清空图层
    void clear_layer(const string &name);  // 清空图层
    void show_layer(int index);            // 显示图层
    void show_layer(const string &name);   // 显示图层
    void hide_layer(int index);            // 隐藏图层
    void hide_layer(const string &name);   // 隐藏图层

    void erase_layer(int index, Vector2<int> start_pos, Vector2<int> size);          // 擦除区域
    void erase_layer(const string &name, Vector2<int> start_pos, Vector2<int> size); // 擦除区域

    void Print_Char(int layer_index, Vector2<int> scr_pos, wchar_t c, float scale, pixel color); // 打印字符到图层
    void Print_Text(int layer_index, Vector2<int> start_pos, string text,
                    float scale = 1.0, pixel color = 0xFF000000, Vector2<int> LC_adjust = Vector2<int>(0, 0)); // 打印文本到图层
    void Print_Text(int layer_index, Vector2<int> start_pos, Vector2<int> end_pos, string text,
                    float scale = 1.0, pixel color = 0xFF000000, Vector2<int> LC_adjust = Vector2<int>(0, 0)); // 打印文本到图层
    void Print_Text(const string &layer_name, Vector2<int> start_pos, const string &text,
                    float scale = 1.0, pixel color = 0xFF000000, Vector2<int> LC_adjust = Vector2<int>(0, 0)); // 打印文本到图层
    void Print_Text(const string &layer_name, Vector2<int> start_pos, Vector2<int> end_pos, const string &text,
                    float scale = 1.0, pixel color = 0xFF000000, Vector2<int> LC_adjust = Vector2<int>(0, 0)); // 打印文本到图层

    void draw_image(int layer_index, Vector2<int> loc, int image_index, Vector2<float> pivot);                                   // 绘制图片到图层
    void draw_image(int layer_index, Vector2<int> loc, int image_index, Vector2<float> pivot, char modi_code);                   // 绘制图片到图层
    void draw_image(const string &layer_name, Vector2<int> loc, const string &image_name, Vector2<float> pivot);                 // 绘制图片到图层
    void draw_image(const string &layer_name, Vector2<int> loc, int image_index, Vector2<float> pivot);                          // 绘制图片到图层
    void draw_image(const string &layer_name, Vector2<int> loc, const string &image_name, Vector2<float> pivot, char modi_code); // 绘制图片到图层
    void draw_pixel(int layer_index, Vector2<int> loc, pixel color);                                                             // 绘制像素到图层
    void draw_line(int layer_index, Vector2<int> start, Vector2<int> end, pixel color);                                          // 绘制线条到图层
    void draw_line(int layer_index, Vector2<int> start, Vector2<int> end, int thickness, pixel color);                           // 绘制有粗度的线条到图层
    void draw_rect(int layer_index, Vector2<int> top_left, Vector2<int> bottom_right, pixel color);                              // 绘制矩形到图层
    void draw_circle(int layer_index, Vector2<int> center, int radius, pixel color);                                             // 绘制圆形到图层

    void LayersInfo(); // 打印图层信息
    void update();     // 更新显示
}
```

------------

## 触控`touch`

```C++
struct touch_event
{
    __u16 type; // 1:按下 0:松开 2:移动
    Vector2<int> pos;
    timeval time;
};

class Touch_Manager
{
public:
    Touch_Manager();
    ~Touch_Manager();

    pthread_t Monitor_Thread;   // 监听线程
    vector<touch_event> events; // 触摸事件容器

    bool should_stop; // 停止监听标志位
    static void *Monitor(void *touch_manager);
    void Start_Listen();
    void Stop_Listen();

    void print_events();

    bool Is_Pressing();
    bool Is_Moving(int threshold);
    Vector2<int> Get_Start_Position();
    Vector2<int> Get_Current_Position();
    Vector2<int> Get_Direction(int tms = 100,bool consider_last_touch = false);
    float Get_Speed(int tms = 100,bool consider_last_touch = false);
    int Get_Duration(bool without_moving = true);
};
```

------------

## 点集`dots`

```C++
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
    LCD_Manager *lcd;     // 传入LCD管理器
    vector<dot> dots;     // 以容器形式存储点集
    Vector2<int> cursor; // 虚拟屏幕指针位置
    Vector2<int> range;     // 存储操作范围

    virtual void drawSingel(const dot one) = 0; // 传入一个点，绘制一个点
    virtual void drawAll() = 0;                    // 绘制所有点

    dot *get(Vector2<int> loc);                // 传坐标x,y,返回找到的第一个点
    dot *get(Vector2<int> loc, int start);    // 传坐标x,y,从start找起来，返回第一个找到的点
    bool find(Vector2<int> loc, int start); // 传坐标x,y,从start开始，如果有该坐标的点，返回真，否则假
    void cursorTo(int x, int y);            // 将指针移动到(l,c)处
    void cursorTo(Vector2<int> loc);        // 将指针移动到loc处
};

class Map : Dots
{
private:
    void drawSingel(const dot one); // 传入点，绘制点
public:
    void drawAll();                               // 绘制全图   待优化速度
    Map(LCD_Manager &lcd, Vector2<int> range); // 初始化时要给出范围,自动生成地图
    Map(LCD_Manager &lcd, int mapselect);       // 初始化给出选择什么地图
    void resetMap(int mapselect);               // 重置地图
    Vector2<int> getRange();                   // 获取地图大小
    void drawSingel(Vector2<int> loc);           // 传入坐标，绘制点（其实就是找到点的指针调用另一个重构）
    bool isWalkable(Vector2<int> loc);           // 查找第一个该坐标的点的类型是不是"可通行"的
};

class Snake : Dots
{
private:
    void drawSingel(dot one); // 传入一个点，绘制一个点（好像只有内部用到）
public:
    Vector2<int> headFor; // 当前运动方向，二维向量

    Snake(LCD_Manager &lcd, Vector2<int> range, Vector2<int> headFor = Vector2<int>(0, -1)); // 初始要传入操作范围，初始方向向上
    void drawAll();                                                                             // 绘制所有体节，一般只在初始化时用到
    int move(bool isGrow);                                                                     // 传入移动方向以及是否生长，进行移动，撞到身体返回-1,吃到食物返回1
    void changeHeadfor(char key);                                                             // 传入键值，尝试控制方向
    Vector2<int> getHeadLocation();                                                             // 获取头位置
    Vector2<int> getTailLocation();                                                             // 获取尾位置
    bool isAnybody(Vector2<int> loc, bool icHead = true);                                     // 查询坐标下是否有体节,可选择是否包括头
};

class Food : Dots
{
private:
    void drawSingel(const dot one); // 传入容器中一个点，绘制一个食物
    void drawAll();                    // 绘制所有食物
public:
    Food(LCD_Manager &lcd, Vector2<int> range_);                   // 初始化时要给出范围
    int getAmount(char t = 0);                                       // 查询食物数量，可以查询特定种类食物数量（目前没啥用）
    bool createOne(Vector2<int> loc);                               // 在某个坐标创建一个普通食物（或许能造点不普通的）
    bool isFood(Vector2<int> loc);                                   // 判断某处有没有食物
    bool eat(Vector2<int> loc);                                       // 尝试吃某处的食物，有就吃掉返回真，无就返回假
    void terrify(Vector2<int> start, Vector2<int> end);               // 使某个范围的食物受到惊吓
    void transf(int t0, int t1, int p_in_100, int seed = 0);       // 食物类型转化(前后种类，转化成功几率)
    void move(Vector2<int> forbiden[], int arrlen, int rseed = 0); // 使可以动的食物移动一次
    void getFoodLoc(Vector2<int> locs[], int t);                   // 获取某种食物的坐标（存放多个位置的数组）
    bool attack(Map *map, Snake *snake);                           // 生气食物向蛇头行进
};
```

## 菜单`menu`
