

bool 绘制开关 = false;
bool 雷达开关 = false;

bool 人物射线 = false;
bool 人物名称 = false;
bool 人物方框 = false;
bool 人物信息 = false;

bool 怪物射线 = false;
bool 怪物名称 = false;
bool 怪物方框 = false;

bool 物资射线 = false;
bool 物资名称 = false;
bool 物资防重 = false;
bool 去除无用 = true;

bool 过滤开关 = true;

char 线程提示[60] = "请启动绘制";

long 模块1 = 0x0;//u3d
long 模块2 = 0x0;//il2

int 记录数量 = 120;
float 记录间隔 = 0.3f;

//特征码获取到的地址
long 矩阵地址 = 0x0;
long 路线指针 = 0x0;

long 世界指针 = 0x0;
long 人物世界 = 0x0;
long 怪物世界 = 0x0;
long 物资世界 = 0x0;

long 自身结构 = 0x0;
long 准心指针 = 0x0;
int 人物数量 = 0;

bool 物资测试排除 = true;

ImU32 常见颜色 = IM_COL32(39, 174,  96, 255);
ImU32 稀有颜色 = IM_COL32(142, 68, 173, 255);
ImU32 罕见颜色 = IM_COL32(241, 196, 15, 255);
ImU32 珍惜颜色 = IM_COL32(211, 84,   0, 255);
ImU32 宝椁颜色 = IM_COL32(52, 152, 219, 255);


ImU32 海蓝 = IM_COL32(98, 208, 247, 230);
ImU32 淡紫 = IM_COL32(255, 103, 212, 230);
ImU32 艳青 = IM_COL32(0, 255, 255, 255);
ImU32 红色 = IM_COL32(255, 0, 0, 255);
ImU32 黄色 = IM_COL32(255, 255, 0, 255);
ImU32 蓝色 = IM_COL32(0, 0, 255, 255);
ImU32 浅蓝 = IM_COL32(56, 176, 227, 255);
ImU32 橙色 = IM_COL32(240, 165, 0, 255);
ImU32 粉红 = IM_COL32(255, 110, 124, 255);
ImU32 白色 = IM_COL32(255, 255, 255, 255);
ImU32 绿色 = IM_COL32(0, 255, 0, 255);
ImU32 紫色 = IM_COL32(252, 72, 252, 255);
ImU32 天蓝 = IM_COL32(52, 119, 226, 255);
ImU32 深天蓝 = IM_COL32(0, 191, 255, 255);
ImColor 黑色 = ImColor(0,0,0);

struct wlist {
	std::vector<std::string> 列表物品名称;
	std::vector<int> 列表物品价值;
	std::vector<int> 列表物品id;
	bool 列表物品选择[300] = {};
} 物品列表;

Vector3A 自身坐标 = { 0, 0, 0};
float 偏航角 = 0;
float 俯仰角 = 0;


struct 游戏结构 {
	Vector2A 视角坐标;
    float x;
    float y;
    float w;
	float m;
    float h;
	char 名字[100];
	int 编号 = 0;
	int 队伍 = 0;
	int 类型 = 0;
	float 血量 = 0;
	long 结构地址 = 0;
	Vector3A 坐标;
} 结构数据[2000];

struct 路线结构 {
    float x;
    float y;
    float w;
	float m;
    float h;
	Vector3A 坐标;
} 路线数据[100];


int 入口出口 = 6;
int 宝椁    = 49;
int 陷阱    = 57;
int 杂物罐  = 72;
int 蜘蛛网  = 80;
int 宝库钥匙 = 84;
int 连弩    = 81;
int 柜子    = 125;
int 金币堆  = 131;
int 土堆   = 136;
int 漩涡    = 142;

static int 过滤价值 = 0;
static int 排除数值[] = 
{
	入口出口,
	宝椁,
	陷阱,
	杂物罐,
	蜘蛛网, 
	连弩, 
	土堆,
	柜子,
	金币堆
};

static std::set<int> 排除数组(std::begin(排除数值), std::end(排除数值));
static vector<Point> points;


bool 添加物品(const wlist& 列表, std::string 名字, int 价值, int id) {
	// 检查是否已经存在
	if (std::find(物品列表.列表物品名称.begin(), 物品列表.列表物品名称.end(), 名字) != 物品列表.列表物品名称.end()) {
		return false; // 已存在，不添加
	}
	
	if (物资测试排除 && 价值 != 0) {
		return false;
	}
        
	物品列表.列表物品名称.push_back(名字);
	物品列表.列表物品价值.push_back(价值);
	物品列表.列表物品id.push_back(id);
	return true; // 添加成功
}

float 计算距离(Vector3A p1, Vector3A p2) {
    float dx = p2.X - p1.X;
    float dy = p2.Y - p1.Y;
    float dz = p2.Z - p1.Z;
    return (float)std::sqrt(dx * dx + dy * dy + dz * dz);
}

void 绘制雷达背景(float 雷达半径 = 80.0f, float 偏航 = 0.0f) {
    ImDrawList* 绘制列表 = ImGui::GetForegroundDrawList();
    // 保存中心点位置供敌人绘制使用 
    static ImVec2 全局中心点;
    全局中心点 = 雷达中心;
    
    //外圆盘与背景填充
    绘制列表->AddCircleFilled(雷达中心, 雷达半径, IM_COL32(20, 30, 40, 100), 40);
    
    //距离环
    for (int i = 1; i <= 3; ++i) {
        float 环半径 = 雷达半径 * i / 3;
        绘制列表->AddCircle(雷达中心, 环半径, 白色, 64, 1.0f);
    }
    
    //十字参考线 =====
    ImU32 参考线颜色 = 白色;
    绘制列表->AddLine(ImVec2(雷达中心.x - 雷达半径, 雷达中心.y), ImVec2(雷达中心.x + 雷达半径, 雷达中心.y), 参考线颜色, 1.2f);
    绘制列表->AddLine(ImVec2(雷达中心.x, 雷达中心.y - 雷达半径), ImVec2(雷达中心.x, 雷达中心.y + 雷达半径), 参考线颜色, 1.2f);
    
    // 移植的方向文字跟随旋转功能
    float 雷达旋转角度 = 偏航; // 使用第二个雷达系统中的偏航角
    
    const float 文字圈半径 = 雷达半径 + 20.0f;
    const ImVec4 字体颜色 = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    const float 字体大小 = 28.0f;
    ImFont* font = ImGui::GetFont();
    font->Scale = 字体大小 / 26.0f;
    
    const char* 方向文字[] = {"南", "东南", "东", "东北", "北", "西北", "西", "西南"};

    for (int i = 0; i < 8; i++)
    {
        float 文字角度 = IM_PI * 2.0f * (float)i / 8.0f - IM_PI/2.0f + 雷达旋转角度;        
        ImVec2 文字位置 = {
            雷达中心.x + cosf(文字角度) * 文字圈半径,
            雷达中心.y + sinf(文字角度) * 文字圈半径
        };        
        ImVec2 边缘位置 = {
            雷达中心.x + cosf(文字角度) * 雷达半径,
            雷达中心.y + sinf(文字角度) * 雷达半径
        };        
        ImVec2 文字尺寸 = font->CalcTextSizeA(字体大小, FLT_MAX, 0.0f, 方向文字[i]);
        文字位置.x -= 文字尺寸.x * 0.5f;
        文字位置.y -= 文字尺寸.y * 0.5f;
        
        // 绘制方向文字
        绘制列表->AddText(font, 字体大小, 文字位置, ImColor(字体颜色), 方向文字[i]);
    }
    font->Scale = 1.0f;
}

void 绘制敌人标记(Vector3A& 自身坐标, float 偏航角, Vector3A& 敌人坐标, float 最大探测范围, ImU32 敌颜色, float 雷达半径 = 80.0f) {
    ImDrawList* 绘制列表 = ImGui::GetForegroundDrawList();
    float dx = 敌人坐标.X - 自身坐标.X;  // 游戏世界X轴（东为正）
    float dz = 敌人坐标.Z - 自身坐标.Z;  // 游戏世界Z轴（北为正）
    
    //转换到玩家相对坐标系
    float 玩家系x = dx * cosf(偏航角) + dz * sinf(偏航角);
    float 玩家系z = -dx * sinf(偏航角) + dz * cosf(偏航角);
    
    //计算极坐标（雷达坐标系）
    float 实际距离 = sqrtf(玩家系x*玩家系x + 玩家系z*玩家系z);
    float 方位角 = atan2f(玩家系x, 玩家系z); // 注意：参数顺序为(X,Z)
    
    //计算屏幕位置（考虑屏幕Y轴向下）
    float 缩放距离 = (实际距离 > 最大探测范围) ? 雷达半径 : (实际距离 / 最大探测范围) * 雷达半径;
    ImVec2 敌位置 = ImVec2( 雷达中心.x + 缩放距离 * sinf(方位角), 雷达中心.y - 缩放距离 * cosf(方位角));
    
    //绘制敌人图标
    绘制列表->AddCircleFilled(敌位置, 7.0f, 敌颜色, 10);
    绘制列表->AddCircle(敌位置, 7.0f, IM_COL32(100, 100, 100, 160), 12, 2.0f);
}



void 绘制初始化() {
	模块1 = getbss("libunity.so:bss");
	模块2 = getbss("libil2cpp.so:bss");
    
	clearResultList();//清空之前的搜索结果
	setMemoryArea(RANGE_ANONYMOUS);//设置A内存
	MemorySearch("1080033280", TYPE_DWORD);
	ImproveOffset("1084227584", TYPE_DWORD, 0x4);
	ImproveOffset("0", TYPE_DWORD, 0x1C);
	//ImproveOffset("-1", TYPE_DWORD, 0x8C);
	
	printResultListToFile("/storage/emulated/0/人物.log");//打印内存搜索筛选结果列表到文件 【传入文件绝对路径】
	
	if (getResultList().size() != 0) {
		世界指针 = getResultList()[0];
		物资世界 = getZZ(getResultList()[0] - 0x2D0) + 0x18;//物资
		怪物世界 = getZZ(getResultList()[0] - 0x2D0) + 0x18;//怪物
		人物世界 = getZZ(getResultList()[0] - 0x300) + 0x18;//人物
	}
	clearResultList();//清空之前的搜索结果
}

void 获取绘制数据() {
	模块1 = getbss("libunity.so:bss");
	模块2 = getbss("libil2cpp.so:bss");
    
	strcpy(线程提示,"获取数组前停止运行");
	while(绘制开关) {
		
		int 人数 = 0;
		long 人物数组 = getZZ(人物世界)+0x30;
		long 怪物数组 = getZZ(怪物世界)+0x30;
		long 物资数组 = getZZ(物资世界)+0x30;
		矩阵地址 = 跳指针(模块1 + 0x611B0, 0x240, 0x10, 0x30C);
		
		float 矩阵[16];
		memset(矩阵, 0, 16);
		vm_readv(矩阵地址, 矩阵, 16 * 4);
		
		for (int i = 0; i < getDword(人物世界 + 0x8); i++) {
			
			long 对象结构 = getZZ(人物数组 + 0x18 * i);
			
			Vector3A 玩家坐标 = {
				getFloat(对象结构 + 0x584),
				getFloat(对象结构 + 0x588),
				getFloat(对象结构 + 0x58C)
			};
			
			char 玩家名字[100];
            
			getUTF8(玩家名字,getZZ(对象结构 + 0xB0) + 0x14);
			int 自身判断 = getDword(对象结构 + 0xA8);
			int 玩家队伍 = getDword(对象结构 + 0x28C);
			int 玩家血量 = getDword(对象结构 + 0x258);
			
			if (getDword(对象结构) == 0 || 玩家坐标.Y == 0) {
				continue;
			}
			
			if (自身判断 == 1) {
				自身坐标 = 玩家坐标;
				自身结构 = 对象结构;
				continue;
			}
			
			if (玩家血量 == 0) {
				continue;
			}
			
			float camear_z = 矩阵[3] * 玩家坐标.X + 矩阵[7] * 玩家坐标.Y + 矩阵[11] * 玩家坐标.Z + 矩阵[15];	// 相机Z
			float r_x = px + (矩阵[0] * 玩家坐标.X + 矩阵[4] * 玩家坐标.Y + 矩阵[8] * 玩家坐标.Z + 矩阵[12]) / camear_z * px;
			float r_y = py - (矩阵[1] * 玩家坐标.X + 矩阵[5] * (玩家坐标.Y + 1) + 矩阵[9] * 玩家坐标.Z + 矩阵[13]) / camear_z * py;
			float r_w = py - (矩阵[1] * 玩家坐标.X + 矩阵[5] * (玩家坐标.Y + 3) + 矩阵[9] * 玩家坐标.Z + 矩阵[13]) / camear_z * py;
			
			float m = std::fabs(camear_z / 1);
			偏航角 = std::atan2(矩阵[8], 矩阵[0]);//偏航角
			俯仰角 = std::atan2(矩阵[9], 矩阵[5]);//俯仰角
			
			float x=r_x - (r_y - r_w) / 4;
			float y=(r_y) ;
			float w=(r_y - r_w) / 2 ;
			float h=(r_y - r_w);
            
			游戏结构 *key = &结构数据[人数];
			key->视角坐标 = {r_x, r_y};
			key->x = x;
			key->y = y;
			key->w = w;
			key->h = h;
			key->m = m;
			key->类型 = 0;
			key->队伍 = 玩家队伍;
			key->血量 = (float)玩家血量;
			key->坐标 = 玩家坐标;
			key->结构地址 = 对象结构;
			sprintf(key->名字,"%s",玩家名字);
			人数++;
		}
		
		
		/*for (int i = 0; i < getDword(怪物世界 + 0x8); i++) {
			
			long 对象结构 = getZZ(怪物数组 + 0x18 * i);
			
			Vector3A 怪物坐标 = {
				getFloat(对象结构 + 0x278),
				getFloat(对象结构 + 0x27C),
				getFloat(对象结构 + 0x280)
			};
			
			char 怪物名字[100];
			getUTF8(怪物名字, getZZ(getZZ(对象结构 + 0xB0) + 0x28) + 0x14);
			int 怪物血量 = getDword(对象结构 + 0x730);
			
			if (getDword(对象结构) == 0 || 怪物坐标.Y == 0 || 怪物血量 == 0) {
				continue;
			}
			
			if (怪物血量 == 0) {
				continue;
			}
			
			float camear_z = 矩阵[3] * 怪物坐标.X + 矩阵[7] * 怪物坐标.Y + 矩阵[11] * 怪物坐标.Z + 矩阵[15];	// 相机Z
			float r_x = px + (矩阵[0] * 怪物坐标.X + 矩阵[4] * 怪物坐标.Y + 矩阵[8] * 怪物坐标.Z + 矩阵[12]) / camear_z * px;
			float r_y = py - (矩阵[1] * 怪物坐标.X + 矩阵[5] * (怪物坐标.Y + 1) + 矩阵[9] * 怪物坐标.Z + 矩阵[13]) / camear_z * py;
			float r_w = py - (矩阵[1] * 怪物坐标.X + 矩阵[5] * (怪物坐标.Y + 3) + 矩阵[9] * 怪物坐标.Z + 矩阵[13]) / camear_z * py;
			float m = std::fabs(camear_z / 1);
			偏航角 = std::atan2(矩阵[8], 矩阵[0]);//偏航角
			俯仰角 = std::atan2(矩阵[9], 矩阵[5]);//俯仰角
			float x=r_x - (r_y - r_w) / 4;
			float y=(r_y) ;
			float w=(r_y - r_w) / 2 ;
			float h=(r_y - r_w);
			
			游戏结构 *key = &结构数据[人数];
			key->视角坐标 = {r_x, r_y};
			key->x = x;
			key->y = y;
			key->w = w;
			key->h = h;
			key->m = m;
			key->类型 = 1;
			key->队伍 = 99;
			key->血量 = (float)怪物血量;
			key->坐标 = 怪物坐标;
			key->结构地址 = 对象结构;
			sprintf(key->名字,"%s",怪物名字);
			人数++;
		}*/
		
		for (int i = 0; i < getDword(物资世界 + 0x8); i++) {
			
			long 对象结构 = getZZ(物资数组 + 0x18 * i);
			
			Vector3A 物资坐标 = {
				getFloat(对象结构 + 0x290),
				getFloat(对象结构 + 0x294),
				getFloat(对象结构 + 0x298)
			};
			
			int 物资价值 = getDword(对象结构 + 0x1B4);
			int 物资id = getDword(对象结构 + 0x54);
			int 棺材打开 = getDword(对象结构 + 0x4A8);
			int 物资捡起1 = getDword(对象结构 + 0x320);
			//int 物资捡起2 = getDword(对象结构 + 0x350);
			
			char 物资名字[100];
			getUTF8(物资名字, getZZ(getZZ(对象结构 + 0xD0) + 0x28) + 0x14);
			
			if (过滤开关) {
				if (物资捡起1 == 257 || 物资捡起1 == 256 || 棺材打开 == 5) {
					continue;
				}
				
				if (getDword(对象结构) == 0 || 物资坐标.Y == 0) {
					continue;
				}
			
				if (排除数组.count(物资id) == 0 && 物资价值 == 0 && (std::string)物资名字 != "精绝古城门空物体") {
					continue;
				}
			
				if (物资价值 <= 过滤价值 && 排除数组.count(物资id) == 0) {
					continue;
				}
			}
			
			
			float camear_z = 矩阵[3] * 物资坐标.X + 矩阵[7] * 物资坐标.Y + 矩阵[11] * 物资坐标.Z + 矩阵[15];	// 相机Z
			float r_x = px + (矩阵[0] * 物资坐标.X + 矩阵[4] * 物资坐标.Y + 矩阵[8] * 物资坐标.Z + 矩阵[12]) / camear_z * px;
			float r_y = py - (矩阵[1] * 物资坐标.X + 矩阵[5] * (物资坐标.Y + 1) + 矩阵[9] * 物资坐标.Z + 矩阵[13]) / camear_z * py;
			float r_w = py - (矩阵[1] * 物资坐标.X + 矩阵[5] * (物资坐标.Y + 3) + 矩阵[9] * 物资坐标.Z + 矩阵[13]) / camear_z * py;
			float m = std::fabs(camear_z / 1);
			偏航角 = std::atan2(矩阵[8], 矩阵[0]);//偏航角
			俯仰角 = std::atan2(矩阵[9], 矩阵[5]);//俯仰角
			float x=r_x - (r_y - r_w) / 4;
			float y=(r_y) ;
			float w=(r_y - r_w) / 2 ;
			float h=(r_y - r_w);
			
			游戏结构 *key = &结构数据[人数];
			key->视角坐标 = {r_x, r_y};
			key->x = x;
			key->y = y;
			key->w = w;
			key->h = h;
			key->m = m;
			key->类型 = 2;
			key->队伍 = 物资id;
			key->血量 = 物资价值;
			key->坐标 = 物资坐标;
			key->结构地址 = 对象结构;
			sprintf(key->名字,"%s",物资名字);
			std::string 名字s = key->名字;
			添加物品(物品列表, 名字s, 物资价值, 物资id);
			
			人数++;
		}
		人物数量 = 人数;
		strcpy(线程提示,"正常运行中");
	}
	strcpy(线程提示,"没有启动");
}



void 开始绘制() {
	if (雷达开关) {
		绘制雷达背景(雷达大小, 偏航角);
	}
	ImDrawList *draw_list = ImGui::GetForegroundDrawList();
	
	
	points.clear();
	
	static float j[16];
	vm_readv(矩阵地址, j, 16 * 4);
	
	for (int i = 0 ; i < 人物数量 ; i++) {
		
		char 名字[100];
		float x = 结构数据[i].x;
		float y = 结构数据[i].y;
		float w = 结构数据[i].w;
		float h = 结构数据[i].h;
		float m = 结构数据[i].m;
		int 类型 = 结构数据[i].类型;
		
		if (雷达开关) {
			
			if (类型 == 0) {
				绘制敌人标记(自身坐标, 偏航角, 结构数据[i].坐标, 距离缩放, 白色, 雷达大小);//300最好别改我也不知道是啥
			} else if (类型 == 1) {
				绘制敌人标记(自身坐标, 偏航角, 结构数据[i].坐标, 距离缩放, 红色, 雷达大小);//300最好别改我也不知道是啥
			}
		}
		
		x = x + (w/2);//屏幕矫正半个身位
		if (w<0) { continue; }
		
		if (人物射线 && 类型 == 0) {
			draw_list->AddLine({px, 30},{x,y-w-30},白色,1.0f);
		}
		if (怪物射线 && 类型 == 1) {
			draw_list->AddLine({px, 30},{x,y-w-30},红色,1.0f);
		}
		
		
		if (人物名称 && 类型 == 0) {
			sprintf(名字,"[%d]%s[%.2fM]",结构数据[i].队伍, 结构数据[i].名字, m);
			draw_list->AddText(NULL, 25.f,{x-w, y-w-30}, 白色, 名字);
		}
		if (怪物名称 && 类型 == 1) {
			sprintf(名字,"[HP:%.1f]%s[%.2fM]",结构数据[i].血量,结构数据[i].名字, m);
			draw_list->AddText(NULL, 25.f,{x-w, y-w-30}, 白色, 名字);
		}
		if (类型 == 2) {
			int 价值 = 结构数据[i].血量;
			int id = 结构数据[i].队伍;
			sprintf(名字,"[%d]%s[%.2fM]", 价值, 结构数据[i].名字, m);
			
			bool 射线 = false;
			ImU32 物资颜色 = 白色;
			
			if (价值 >= 100000) {
				射线 = true;
				物资颜色 = 珍惜颜色;
				
			} else if (价值 >= 40000) {
				射线 = true;
				物资颜色 = 罕见颜色;
				
			} else if (价值 >= 10000) {
				射线 = false;
				物资颜色 = 稀有颜色;
				
			} else if (id == 宝库钥匙 || id == 金币堆) {
				射线 = true;
				物资颜色 = IM_COL32( 255, 140, 217, 255);
				
			} else if (id == 宝椁) {
				射线 = true;
				物资颜色 = 宝椁颜色;
				
			} else if (id == 入口出口) {
				射线 = false;
				物资颜色 = IM_COL32( 0, 255, 255, 150);
				
			} else if (1000 < 价值) {
				射线 = false;
				物资颜色 = 常见颜色;
				
			} else {
				
				if (去除无用) continue;
				射线 = false;
				物资颜色 = IM_COL32( 255, 255, 255, 100);
			}
			
			if (物资名称) {
				draw_list->AddText(NULL, 25.f,{x-w, y-w-30}, 物资颜色, 名字);
			}
			
			if (射线 && 物资射线) {
				draw_list->AddLine({px, 30},{x,y-w-30},物资颜色,1.0f);
			}
			
		}
		
		if (物资防重) {
            ImVec2 ann = ImGui::CalcTextSize(名字);
			addToEnd(points,x,y,m,ann.x,ann.y,false);
			readLastPoint(points,x,y);
		}
		
		if (人物方框 && 类型 == 0) {
			draw_list->AddRect({x - (w / 2),y - w},{x + (w / 2),y + w},白色, {1}, 0, {1.0});
		}
		if (怪物方框 && 类型 == 1) {
			draw_list->AddRect({x - (w / 2),y - w},{x + (w / 2),y + w},红色, {1}, 0, {1.0});
		}
		
		if (人物信息) {
			char 信息1[50];
			char 信息2[50];
			/*sprintf(信息1,"血量：%d",人物数据[i].人物血量, 人物数据[i].人物体力);
			sprintf(信息2,"负重：%.1f", 人物数据[i].人物负重);
			draw_list->AddText(NULL, 25.f,{x-w, y-w-50}, 白色, 信息1);
			draw_list->AddText(NULL, 25.f,{x-w, y-w-70}, 白色, 信息2);*/
		}
	}
	
	if (雷达开关) {
		// ===== 雷达自身绘制（中心点） ===== 
    	draw_list->AddCircleFilled(雷达中心, 6, 白色, 8);
	}
}