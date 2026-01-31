
#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <malloc.h>
#include <math.h>
#include <thread>
#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <locale>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <random>
#include <vector>
#include <set>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_android.h"

extern ImGuiWindow *Window;
extern bool g_Initialized;
extern ANativeWindow *window_sv;
extern EGLDisplay g_EglDisplay;
extern EGLSurface g_EglSurface;
extern EGLContext g_EglContext;
extern void initGui();
extern void initializeEGLContext();

extern void ImGuiMainWinStart(); //Start
extern void ImGuiMainMenuWind(); //UI
extern void ImGuiMainWinEnded();//end

extern int _ScreenX, _ScreenY;
extern int abs_ScreenX, abs_ScreenY;

//imgui模式
extern bool 兼容模式;
extern int 目标帧率;

extern ImColor 随机颜色[100];
extern float 雷达大小, 距离缩放;
extern ImVec2 雷达中心;

extern char 线程提示[60];
extern int 人物数量;
extern long 模块1, 模块2;
extern long 世界指针, 物资世界, 怪物世界, 人物世界;
extern long 自身结构;

extern JavaVM* g_JVM;
extern void CheckImGuiTextInput();
extern void ProcessAndroidInputToImGui();

extern float px, py;




