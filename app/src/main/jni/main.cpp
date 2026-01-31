#include "Tool/内存读写.h"
#include "Tool/YC.h"
#include "Tool/AlguiMemTool.h"
#include "全局.h"
#include "imgui/字体.h"
#include "绘制.h"
#include "功能.h"
#include "布局.h"
int Screen_px = 0, Screen_py = 0;
int _ScreenX = 0, _ScreenY = 0;
int abs_ScreenX = 0, abs_ScreenY = 0;
bool g_Initialized = false;
ImGuiWindow *Window = nullptr;
ANativeWindow *window_sv = nullptr;
EGLDisplay g_EglDisplay = EGL_NO_DISPLAY;
EGLSurface g_EglSurface = EGL_NO_SURFACE;
EGLContext g_EglContext = EGL_NO_CONTEXT;

bool 兼容模式 = false;
int 目标帧率 = 0;

float px = 0;
float py = 0;

void init(ANativeWindow *g_EglNativeWindowType)
{
    if (g_Initialized)
        return;

    //window = windowSurface.getSurface();
    ANativeWindow_acquire(g_EglNativeWindowType);

    // Initialize EGL
    // This is mostly boilerplate code for EGL...
    {
        g_EglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (g_EglDisplay == EGL_NO_DISPLAY)
            //__android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglGetDisplay(EGL_DEFAULT_DISPLAY) returned EGL_NO_DISPLAY");
            printf("eglGetDisplay(EGL_DEFAULT_DISPLAY) returned EGL_NO_DISPLAY\n");

        if (eglInitialize(g_EglDisplay, 0, 0) != EGL_TRUE)
            //__android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglInitialize() returned with an error");
            printf("eglInitialize() returned with an error\n");

        const EGLint egl_attributes[] = {EGL_BLUE_SIZE, 8,
                                         EGL_GREEN_SIZE, 8,
                                         EGL_RED_SIZE, 8,
                                         EGL_ALPHA_SIZE, 8,
                                         EGL_DEPTH_SIZE, 24,
                                         EGL_SURFACE_TYPE,
                                         EGL_WINDOW_BIT,
                                         EGL_NONE};
        EGLint num_configs = 0;
        if (eglChooseConfig(g_EglDisplay, egl_attributes, nullptr, 0, &num_configs) != EGL_TRUE)
            //__android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglChooseConfig() returned with an error");
            printf("eglChooseConfig() returned with an error\n");
        if (num_configs == 0)
            //__android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglChooseConfig() returned 0 matching config");
            printf("eglChooseConfig() returned 0 matching config\n");
        // Get the first matching config
        EGLConfig egl_config;
        eglChooseConfig(g_EglDisplay, egl_attributes, &egl_config, 1, &num_configs);
        EGLint egl_format;
        eglGetConfigAttrib(g_EglDisplay, egl_config, EGL_NATIVE_VISUAL_ID, &egl_format);
        ANativeWindow_setBuffersGeometry(g_EglNativeWindowType, 0, 0, egl_format);

        const EGLint egl_context_attributes[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
        g_EglContext = eglCreateContext(g_EglDisplay, egl_config, EGL_NO_CONTEXT, egl_context_attributes);

        if (g_EglContext == EGL_NO_CONTEXT)
            //__android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglCreateContext() returned EGL_NO_CONTEXT");
            printf("eglCreateContext() returned EGL_NO_CONTEXT\n");

        g_EglSurface = eglCreateWindowSurface(g_EglDisplay, egl_config, g_EglNativeWindowType, NULL);
        eglMakeCurrent(g_EglDisplay, g_EglSurface, g_EglSurface, g_EglContext);

        printf("init success\n");
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = NULL;
    ImGui::StyleColorsDark();
    ImGuiStyle* style;
    
    style = &ImGui::GetStyle();	
    ImGui::SetNextWindowBgAlpha(0.95);
    style->LogSliderDeadzone = 2.0f;
    style->WindowTitleAlign = ImVec2(0.5, 0.5);
    style->FramePadding = ImVec2(7, 7);
    style->FrameBorderSize = 0;
    style->FrameRounding = 4.5f;
    style->WindowRounding = 2.0f;
    style->WindowBorderSize = 0.0f;
    style->GrabRounding = 1.0f;
    style->ScrollbarSize = 30.0f;
    style->ChildBorderSize = 0.0f;
    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);

    io.Fonts->AddFontFromMemoryTTF((void *)font_data, font_size, 26.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

    ImGui_ImplAndroid_Init(g_EglNativeWindowType);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    g_Initialized = true;
}


/************用户自定义**************/
void ImGuiMainWinStart() {
    ImGuiIO& io = ImGui::GetIO();
    
    glViewport(0.0f, 0.0f, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT
    glFlush();

    if (g_EglDisplay == EGL_NO_DISPLAY){
        return;
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();
}
void ImGuiMainWinEnded() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    eglSwapBuffers(g_EglDisplay, g_EglSurface);
}

void setFPS(int target_fps) {
    static auto last_time = std::chrono::steady_clock::now();
    
    // 计算目标帧时间（使用更高精度）
    using namespace std::chrono;
    auto target_duration = duration<double>(1.0 / target_fps);
    
    // 计算已用时间
    auto current_time = steady_clock::now();
    auto elapsed_time = current_time - last_time;
    
    // 如果需要休眠
    if (elapsed_time < target_duration) {
        // 计算剩余需要休眠的时间（纳秒精度）
        auto sleep_time = target_duration - elapsed_time;
        
        // 使用更精确的休眠方式
        auto sleep_start = steady_clock::now();
        while (steady_clock::now() - sleep_start < sleep_time) {
            // 使用更短的休眠间隔来提高精度
            std::this_thread::sleep_for(1ms);  // 或者使用 yield()
        }
    }
    
    // 更新时间为当前精确时间
    last_time = steady_clock::now();
}

void initGui() {
    if (g_Initialized) {
        return;
    }
	init(window_sv);
    ANativeWindow_release(window_sv);
    IMGUI_CHECKVERSION();
    ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();//黑色
	IM_ASSERT(io.Fonts != NULL);  
	 
	 //分辨率设置
	abs_ScreenX = (_ScreenX > _ScreenY ? _ScreenX : _ScreenY);
    abs_ScreenY = (_ScreenX < _ScreenY ? _ScreenX : _ScreenY);   
    
    px = abs_ScreenX / 2.0f;
    py = abs_ScreenY / 2.0f;
    
	ImGui::GetStyle().ScaleAllSizes(2.0f);
    g_Initialized = true;
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    while(true) {
	    ::ImGuiMainWinStart();
        ::ImGuiMainMenuWind();
        ::ImGuiMainWinEnded();
    }
}



void ImGuiMainMenuWind() {
    
    if (兼容模式 && 目标帧率 != 0) {
        setFPS(目标帧率);
    }
    
    布局();
}

