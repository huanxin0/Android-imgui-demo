#include <jni.h>
#include "全局.h"

static bool g_TextInputActive = false;
static std::string g_InputTextBuffer;

JavaVM* g_JVM;
// 全局引用，避免每次FindClass
static jclass g_KeyboardViewServiceClass = nullptr;

// 初始化全局引用
void InitGlobalClasses(JNIEnv* env) {
    if (g_KeyboardViewServiceClass == nullptr) {
        jclass localClass = env->FindClass("com/NeKo/WuLi/ImGui/KeyboardViewService");
        if (localClass != nullptr) {
            g_KeyboardViewServiceClass = (jclass)env->NewGlobalRef(localClass);
            env->DeleteLocalRef(localClass);
        } else {
            __android_log_print(ANDROID_LOG_ERROR, "ImGui", "Failed to find KeyboardViewService class");
        }
    }
}

// 清理全局引用
void CleanupGlobalClasses(JNIEnv* env) {
    if (g_KeyboardViewServiceClass != nullptr) {
        env->DeleteGlobalRef(g_KeyboardViewServiceClass);
        g_KeyboardViewServiceClass = nullptr;
    }
}

void ShowAndroidKeyboard(bool show) {
    if (g_JVM == nullptr) {
        __android_log_print(ANDROID_LOG_WARN, "ImGui", "JVM is null");
        return;
    }
    
    JNIEnv* env = nullptr;
    int getEnvStat = g_JVM->GetEnv((void**)&env, JNI_VERSION_1_6);
    
    bool attached = false;
    if (getEnvStat == JNI_EDETACHED) {
        if (g_JVM->AttachCurrentThread(&env, nullptr) != 0) {
            __android_log_print(ANDROID_LOG_ERROR, "ImGui", "Failed to attach thread");
            return;
        }
        attached = true;
    } else if (getEnvStat != JNI_OK) {
        __android_log_print(ANDROID_LOG_ERROR, "ImGui", "Failed to get JNIEnv");
        return;
    }
    
    // 检查全局类引用是否已初始化
    if (g_KeyboardViewServiceClass == nullptr) {
        InitGlobalClasses(env);
        if (g_KeyboardViewServiceClass == nullptr) {
            __android_log_print(ANDROID_LOG_ERROR, "ImGui", "KeyboardViewService class not initialized");
            if (attached) {
                g_JVM->DetachCurrentThread();
            }
            return;
        }
    }
    
    // 获取方法ID
    jmethodID methodId = env->GetStaticMethodID(g_KeyboardViewServiceClass, 
                                               "addKeyboardView", 
                                               "()V");
    if (methodId != nullptr) {
        env->CallStaticVoidMethod(g_KeyboardViewServiceClass, methodId);
        __android_log_print(ANDROID_LOG_INFO, "ImGui", "Called addKeyboardView successfully");
    } else {
        __android_log_print(ANDROID_LOG_ERROR, "ImGui", "addKeyboardView method not found");
        // 尝试查找show/hide方法
        jmethodID altMethodId = nullptr;
        if (show) {
            altMethodId = env->GetStaticMethodID(g_KeyboardViewServiceClass, 
                                                "showKeyboardView", 
                                                "()V");
        } else {
            altMethodId = env->GetStaticMethodID(g_KeyboardViewServiceClass, 
                                                "hideKeyboardView", 
                                                "()V");
        }
        if (altMethodId != nullptr) {
            env->CallStaticVoidMethod(g_KeyboardViewServiceClass, altMethodId);
        }
    }
    
    if (attached) {
        g_JVM->DetachCurrentThread();
    }
}

// 开始文本输入模式
void StartTextInput() {
    if (!g_TextInputActive) {
        g_TextInputActive = true;
        g_InputTextBuffer.clear();
        ShowAndroidKeyboard(true);
    }
}

// 结束文本输入模式
void StopTextInput() {
    if (g_TextInputActive) {
        g_TextInputActive = false;
        g_InputTextBuffer.clear();
        ShowAndroidKeyboard(false);
    }
}

// 检查 ImGui 输入状态
void CheckImGuiTextInput() {
    ImGuiIO& io = ImGui::GetIO();
    
    static bool wasTextInputActive = false;
    bool isTextInputActive = io.WantTextInput;
    
    if (isTextInputActive && !wasTextInputActive) {
        StartTextInput();
    } else if (!isTextInputActive && wasTextInputActive) {
        StopTextInput();
    }
    
    wasTextInputActive = isTextInputActive;
}

// 处理 Android 输入到 ImGui
void ProcessAndroidInputToImGui() {
    ImGuiIO& io = ImGui::GetIO();
    
    if (!g_InputTextBuffer.empty()) {
        // 将输入的字符添加到 ImGui
        for (char c : g_InputTextBuffer) {
            io.AddInputCharacter(c);
        }
        g_InputTextBuffer.clear();
    }
}

extern "C" {
    //初始化opengl窗口
    void setSurface(JNIEnv *env, jclass cla, jobject surface) {
        ::window_sv = ANativeWindow_fromSurface(env, surface);
        
        // 在设置Surface时初始化全局类
        InitGlobalClasses(env);
        
        std::thread t1(initGui);
        t1.detach();
    }

    void MotionEventClick(JNIEnv * env, jobject thiz, jboolean action, jfloat pos_x, jfloat pos_y) {
        if (!ImGui::GetCurrentContext()) return;
        
        ImGuiIO& io = ImGui::GetIO();
		io.AddMousePosEvent(pos_x, pos_y);
		io.AddMouseButtonEvent(0, action);
    }

    jstring Start(JNIEnv *env, jclass clazz, jint width, jint height){
        // 程序其他的操作
        _ScreenX = width;
        _ScreenY = height;
        glViewport(0, 0, width, height);
        
        // 在Start时初始化全局类
        InitGlobalClasses(env);
        
        return env->NewStringUTF("sfg");
    }

    void Set_uuid(JNIEnv *env, jclass clazz, jstring uuid_j) {
        const char *uuid_c = env->GetStringUTFChars(uuid_j, JNI_FALSE);
		env->ReleaseStringUTFChars(uuid_j, uuid_c);
	}
		
    void Set_Key(JNIEnv *env, jclass clazz, jstring key_j) {
        char local_stringDatat_input[128];
        const char *str_C = env->GetStringUTFChars(key_j, NULL);
        if (strlen(str_C) >= sizeof(local_stringDatat_input)) {
            memmove((void *)&(local_stringDatat_input), (void *) str_C, 10);                                
            strcat(local_stringDatat_input, "...文件过长");//校验长度
        } else {
            memmove((void *)&(local_stringDatat_input), (void *) str_C, sizeof(local_stringDatat_input));                                
        }
        
    }
    
    void setPid(JNIEnv *env, jclass clazz, jint pid) {
    }
    
    void setImGuiType(JNIEnv *env, jclass clazz, jboolean type, jint fps) {
        兼容模式 = (bool)type;
        目标帧率 = (int)fps;
    }
    
    jintArray getWindowsId(JNIEnv* env, jobject thiz) {
        if (!ImGui::GetCurrentContext()) return nullptr;
        
        // 获取ImGui上下文
        ImGuiContext& g = *GImGui;

        // 收集可见窗口的ID
        std::vector<jint> windowIds;
        for (int i = 0; i < g.Windows.Size; i++) {
            ImGuiWindow* window = g.Windows[i];
            if (window && window->Active && !window->Hidden && window->WasActive) {
                // 使用窗口的指针作为ID，确保唯一性
                int windowId = (int)(intptr_t)window;
                windowIds.push_back(windowId);
            }
        }

        // 创建int数组
        jintArray result = env->NewIntArray(windowIds.size());
        if (!result) {
            __android_log_print(ANDROID_LOG_ERROR, "raincat", "Failed to create int array for window IDs");
            return nullptr;
        }

        // 复制数据到Java数组
        env->SetIntArrayRegion(result, 0, windowIds.size(), &windowIds[0]);
        return result;
    }
    
    
    jintArray getWindowsRect(JNIEnv* env, jobject thiz) {
        if (!ImGui::GetCurrentContext()) return nullptr;
    
        // 获取ImGui上下文
        ImGuiContext& g = *GImGui;

        // 收集可见窗口的矩形数据
        std::vector<jint> windowRects;
        for (int i = 0; i < g.Windows.Size; i++) {
            ImGuiWindow* window = g.Windows[i];
            if (window && window->Active && !window->Hidden && window->WasActive) {
                int left = (int)window->Pos.x;
                int top = (int)window->Pos.y;
                int right = (int)(window->Pos.x + window->Size.x);
                int bottom = (int)(window->Pos.y + window->Size.y);

                // 过滤掉无效的窗口大小
                if (right - left <= 0 || bottom - top <= 0) {
                    continue;
                }

                // 添加矩形数据：left, top, right, bottom
                windowRects.push_back(left);
                windowRects.push_back(top);
                windowRects.push_back(right);
                windowRects.push_back(bottom);
            }
        }

        // 创建int数组
        jintArray result = env->NewIntArray(windowRects.size());
        if (!result) {
            __android_log_print(ANDROID_LOG_ERROR, "raincat", "Failed to create int array for window rects");
            return nullptr;
        }

        // 复制数据到Java数组
        env->SetIntArrayRegion(result, 0, windowRects.size(), &windowRects[0]);
        return result;
    }
    
    void UpdateInputText(JNIEnv* env, jobject thiz, jstring text) {
        const char* utf8Chars = env->GetStringUTFChars(text, nullptr);
        if (!g_TextInputActive) {
            return;
        }
    
        const char* utf8Charsx = env->GetStringUTFChars(text, nullptr);
        if (utf8Chars != nullptr) {
            // 直接传递给 ImGui
            ImGuiIO& io = ImGui::GetIO();
        
            // 方法1: 使用 AddInputCharactersUTF8 (推荐用于中文)
            io.AddInputCharactersUTF8(utf8Charsx);
        
            env->ReleaseStringUTFChars(text, utf8Charsx);
        }
    }
    
    void DeleteInputText(JNIEnv* env, jobject thiz) {
        if (!g_TextInputActive) {
            return;
        }
    
        ImGuiIO& io = ImGui::GetIO();
    
        // 方法1: 发送退格键事件
        io.AddKeyEvent(ImGuiKey_Backspace, true);
        io.AddKeyEvent(ImGuiKey_Backspace, false);
    }
    
    void showKeyboard(JNIEnv* env, jobject thiz, jboolean show) {
        ShowAndroidKeyboard(show);
    }
	
}









#ifndef NELEM
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#endif
extern "C" {
    int jniRegisterNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *gMethods, int numMethods) {
        jclass clazz = env->FindClass(className);
        if (clazz == NULL) {
            return JNI_FALSE;
        }
        if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
            return JNI_FALSE;
        }
        return JNI_TRUE;
    }
}
extern "C" {
    static JNINativeMethod native_method_table[] = {
        {"start", "(II)Ljava/lang/String;", (void *) Start},
        {"setSurface", "(Landroid/view/Surface;)V", (void *) setSurface},		
        {"setPid", "(I)V", (void *) setPid},		
		{"setUUid", "(Ljava/lang/String;)V", (void *) Set_uuid},		
		{"setKey", "(Ljava/lang/String;)V", (void *) Set_Key},		
        {"MotionEventClick", "(ZFF)V",  (void*) MotionEventClick},
        {"setImGuiType", "(ZI)V", (void*) setImGuiType},
        {"getWindowsRect", "()[I", (void*) getWindowsRect},
        {"getWindowsId", "()[I", (void*) getWindowsId},
        
        {"UpdateInputText", "(Ljava/lang/String;)V", (void*) UpdateInputText},
        {"DeleteInputText", "()V", (void*) DeleteInputText},
        {"showKeyboard", "(Z)V", (void*) showKeyboard},
        
    };
    
    int register_native_api(JNIEnv *env) {
        return jniRegisterNativeMethods(env, "com/NeKo/WuLi/ImGui/ImGuiJNI", native_method_table, NELEM(native_method_table));
    }
}
extern "C" {
    jint JNI_OnLoad(JavaVM *vm, void *reserved) {
        JNIEnv *env;
        g_JVM = vm;
        if (vm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK) {
            return -1;
        }
        assert(env != NULL);
        
        // 在JNI_OnLoad中预先初始化全局类引用
        InitGlobalClasses(env);
        
        if (!register_native_api(env)) {//注册接口
            return -1;
        }
        return JNI_VERSION_1_6;
	}
}