
static bool 绘制 = true;
static bool 功能 = false;
static bool 设置 = false;
static bool 调试 = false;

static int 游戏x = 0;

static bool show_demo_window = false;

ImColor 随机颜色[100] = {};

float 雷达大小 = 180.0f;//雷达大小
float 距离缩放 = 150.0f;
ImVec2 雷达中心 = {340, 340};//雷达的显示位置


void 布局() {
    ImGui::Begin("Sky");
    ImGui::SetWindowSize({700, 600}, ImGuiCond_Once);
    Window = ImGui::GetCurrentWindow();
    float fps = ImGui::GetIO().Framerate;
    
    ProcessAndroidInputToImGui();
    CheckImGuiTextInput();
    
    ImGui::BeginChild("左面板", ImVec2(170, 0), true);
    if (ImGui::Button("绘制",ImVec2(140,80))) {
        绘制 = true;
        功能 = false;
        设置 = false;
        调试 = false;
    }
    if (ImGui::Button("功能",ImVec2(140,80))) {
        绘制 = false;
        功能 = true;
        设置 = false;
        调试 = false;
    }
    if (ImGui::Button("设置",ImVec2(140,80))) {
        绘制 = false;
        功能 = false;
        设置 = true;
        调试 = false;
    }
    if (ImGui::Button("调试",ImVec2(140,80))) {
        绘制 = false;
        功能 = false;
        设置 = false;
        调试 = true;
    }
    ImGui::EndChild();
    
    ImGui::SameLine();
    ImGui::BeginChild("右面板", ImVec2(0, 0), true);
    if (绘制) {
        ImGui::Text("Pid：%d", Pid);
        ImGui::Text("绘制状态：%s", 线程提示);
        ImGui::Text("平均帧率：%.2f ms/FPS", 1000.0f / fps);
	    ImGui::Text("当前帧率：%.1f FPS",fps);
        
        if (ImGui::Button("初始化",ImVec2(-1,80))) {
        
            if (Pid == -1 || 游戏x == 0) {
                Pid = getPID("com.pi.czrxdfirst:a.b");
                setPackageName("com.pi.czrxdfirst:a.b");
			    std::thread t1(绘制初始化);
                t1.detach();
                if (Pid != -1) {
                    游戏x = 0;
                }
            }
            
            if (Pid == -1 || 游戏x == 1) {
                Pid = getPID("com.pi.czrxdfirst:false");
                setPackageName("com.pi.czrxdfirst:false");
			    std::thread t1(绘制初始化);
                t1.detach();
                if (Pid != -1) {
                    游戏x = 1;
                }
            }
            
            if (Pid == -1 || 游戏x == 2) {
                Pid = getPID("com.pi.czrxdfirst");
                setPackageName("com.pi.czrxdfirst");
			    std::thread t1(绘制初始化);
                t1.detach();
                if (Pid != -1) {
                    游戏x = 2;
                }
            }
            
            for (auto & i : 随机颜色) {
                i = ImColor(ImVec4((rand() % 205 + 50) / 255.f, rand() % 255 / 255.f, rand() % 225 / 225.f, 225 / 225.f));
            }
        }
        
        ImGui::NewLine();
        if (ImGui::Checkbox("绘制开关",&绘制开关)) {
            std::thread draw(获取绘制数据);
            draw.detach();
        }
        
        ImGui::SameLine();
        if(ImGui::Button("全部开启",ImVec2(120,60))){
            人物射线 = true;
            人物名称 = true;
            人物方框 = true;
            人物信息 = true;
			怪物射线 = true;
			怪物名称 = true;
			怪物方框 = true;
			物资射线 = true;
			物资名称 = true;
			物资防重 = true;
			雷达开关 = true;
        }
        
        ImGui::SameLine();
        if(ImGui::Button("全部关闭",ImVec2(120,60))){
            人物射线 = false;
            人物名称 = false;
            人物方框 = false;
            人物信息 = false;
			怪物射线 = false;
			怪物名称 = false;
			怪物方框 = false;
			物资射线 = false;
			物资名称 = false;
			物资防重 = false;
			雷达开关 = false;
        }
                
        ImGui::SeparatorText("人物绘制设置");
        ImGui::Checkbox("人物射线",&人物射线);
        ImGui::SameLine();
        ImGui::Checkbox("人物名称",&人物名称);
        ImGui::SameLine();
        ImGui::Checkbox("人物方框",&人物方框);
        ImGui::SameLine();
        ImGui::Checkbox("人物信息",&人物信息);
					
		ImGui::SeparatorText("怪物绘制设置");
		ImGui::Checkbox("怪物射线",&怪物射线);
        ImGui::SameLine();
        ImGui::Checkbox("怪物名称",&怪物名称);
        ImGui::SameLine();
        ImGui::Checkbox("怪物方框",&怪物方框);
					
		ImGui::SeparatorText("物资绘制设置");
		ImGui::Checkbox("物资射线",&物资射线);
        ImGui::SameLine();
        ImGui::Checkbox("物资名称",&物资名称);
		ImGui::SameLine();
        ImGui::Checkbox("物资防重",&物资防重);
		ImGui::SameLine();
        ImGui::Checkbox("去除无用",&去除无用);
		ImGui::SliderInt("过滤价值", &过滤价值, 0, 10000);
                    
		ImGui::SeparatorText("其它设置");
        ImGui::Checkbox("雷达开关",&雷达开关);
        
    }
    
    if (功能) {
        static bool 消磁开关 = false;
        ImGui::NewLine();
        ImGui::SeparatorText("功能");
        
        if (ImGui::Checkbox("消磁",&消磁开关)) {
            std::thread xc(消磁);
            xc.detach();
        }
    }
    
    if (设置) {
        
    }
    
    if (调试) {
        ImGui::Checkbox("Show Demo Window",&show_demo_window);
        if (show_demo_window){
            ImGui::ShowDemoWindow(&show_demo_window);
        }
        ImGui::Text("unity:bss：0x%lX", 模块1);
        ImGui::Text("绘制数量：%d", 人物数量);
        ImGui::Text("矩阵地址：0x%lX", 矩阵地址);
        ImGui::Text("准心指针：0x%lX", 准心指针);
        ImGui::Text("世界指针：0x%lX", 世界指针);
        ImGui::Text("人物世界：0x%lX", 人物世界);
        ImGui::Text("怪物世界：0x%lX", 怪物世界);
        ImGui::Text("物资世界：0x%lX", 物资世界);
        ImGui::Text("人物偏航角：%.2f", 偏航角);
        ImGui::Text("人物俯仰角：%.2f", 俯仰角);
        ImGui::Text("当前坐标：X:%.2f Y:%.2f Z:%.2f", 自身坐标.X, 自身坐标.Y, 自身坐标.Z);
        
        if (ImGui::CollapsingHeader("物资调试")) {
            ImGui::Text("物品数量：%d", 物品列表.列表物品名称.size());
            ImGui::Checkbox("物资过滤",&过滤开关);
            ImGui::Checkbox("过滤出无价值物品",&物资测试排除);
            ImGui::BeginTable("物资列表", 3);
            ImGui::TableSetupColumn("名称");
            ImGui::TableSetupColumn("价值");
            ImGui::TableSetupColumn("id");
            ImGui::TableHeadersRow();
            for (int i = 0; i < 物品列表.列表物品名称.size(); i++) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Checkbox(物品列表.列表物品名称[i].c_str(),&物品列表.列表物品选择[i]);
                ImGui::TableNextColumn();
                ImGui::Text("%d", 物品列表.列表物品价值[i]);
                ImGui::TableNextColumn();
                ImGui::Text("%d", 物品列表.列表物品id[i]);
            }
            ImGui::EndTable();
        }
    }
    
    ImGui::EndChild();
    ImGui::End();
    
    if (绘制开关) {
        开始绘制();
    }
}