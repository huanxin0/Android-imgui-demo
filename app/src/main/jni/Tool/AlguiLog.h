#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <android/log.h>
//Algui日志相关
//作者：ByteCat *_*    作者QQ：3353484607   游戏逆向交流QQ群：931212209

//安卓日志
#define ALOGTAG "ALGUI"
#define ALOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOGTAG, __VA_ARGS__))//信息
#define ALOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOGTAG, __VA_ARGS__))//警告
#define ALOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOGTAG, __VA_ARGS__))//调试
#define ALOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOGTAG, __VA_ARGS__))//错误

//Algui日志
//日志优先级
#define ALGUI_LOG_UNKNOWN 0  /** 仅供内部使用。 */
#define ALGUI_LOG_DEFAULT 1  /** 默认优先级，仅供内部使用。 */
#define ALGUI_LOG_VERBOSE 2  /** 详细日志。通常在发布版 apk 中应禁用。 */
#define ALGUI_LOG_DEBUG 3  /** 调试日志。通常在发布版 apk 中应禁用。 */
#define ALGUI_LOG_INFO 4  /** 信息日志。通常在发布版 apk 中应禁用。 */
#define ALGUI_LOG_WARN 5  /** 警告日志。用于可恢复的故障。 */
#define ALGUI_LOG_ERROR 6  /** 错误日志。用于不可恢复的故障。 */
#define ALGUI_LOG_FATAL 7  /** 致命日志。用于中止时。 */
#define ALGUI_LOG_SILENT 8  /** 仅供内部使用。 */

#define ALGUILOGFILE "/data/user/0/com.bytecat.algui/cache/AlguiByteCat.log"//Algui日志输出文件路径

#define LOG(priority, tag, ...) \
    ((void)__algui_log_print__(priority, tag, __VA_ARGS__))// 自定义日志宏        
#define LOGV(tag, ...) LOG(ALGUI_LOG_VERBOSE, tag, __VA_ARGS__)// 详细级别日志宏   /** 通常在发布版 apk 中应禁用。 */
#define LOGD(tag, ...) LOG(ALGUI_LOG_DEBUG, tag, __VA_ARGS__)// 调试级别日志宏     /** 通常在发布版 apk 中应禁用。 */
#define LOGI(tag, ...) LOG(ALGUI_LOG_INFO, tag, __VA_ARGS__)// 信息级别日志宏      /** 通常在发布版 apk 中应禁用。 */
#define LOGW(tag, ...) LOG(ALGUI_LOG_WARN, tag, __VA_ARGS__)// 警告级别日志宏      /** 用于可恢复的故障。 */
#define LOGE(tag, ...) LOG(ALGUI_LOG_ERROR, tag, __VA_ARGS__)// 错误级别日志宏     /** 用于不可恢复的故障。 */
#define LOGF(tag, ...) LOG(ALGUI_LOG_FATAL, tag, __VA_ARGS__)// 致命级别日志宏     /** 用于中止时。 */


const char* __priorityToString__(int priority);//将优先级映射到字符串
int __algui_log_print__(int priority, const char* tag, const char* format, ...);//在Algui日志文件中写入一行信息



/// <summary>
/// 将优先级映射到字符串
/// </summary>
/// <param name="priority">优先级id</param>
/// <returns>优先级字符串</returns>
const char* __priorityToString__(int priority) {
    switch (priority) {
    case ALGUI_LOG_VERBOSE: return "VERBOSE";
    case ALGUI_LOG_DEBUG: return "DEBUG";
    case ALGUI_LOG_INFO: return "INFO";
    case ALGUI_LOG_WARN: return "WARN";
    case ALGUI_LOG_ERROR: return "ERROR";
    case ALGUI_LOG_FATAL: return "FATAL";
    case ALGUI_LOG_SILENT: return "SILENT";
    case ALGUI_LOG_UNKNOWN:
    default: return "UNKNOWN";
    }
}
/// <summary>
/// 在Algui日志文件中写入一行信息
/// </summary>
/// <param name="priority">优先级</param>
/// <param name="tag">日志标签</param>
/// <param name="format">日志信息</param>
/// <param name="">日志信息可变参数</param>
/// <returns>成功与否</returns>
int __algui_log_print__(int priority, const char* tag, const char* format, ...) {
    //以追加模式打开文件
    FILE* file = fopen(ALGUILOGFILE, "a");
    if (file == nullptr) {
        return -1;
    }

    //获取当前时间
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    //将时间转换为本地时间
    time_t now = ts.tv_sec;
    struct tm* timeinfo = localtime(&now);

    //打印时间戳
    fprintf(file, "[%04d-%02d-%02d %02d:%02d:%02d.%03ld] ",
        timeinfo->tm_year + 1900,
        timeinfo->tm_mon + 1,
        timeinfo->tm_mday,
        timeinfo->tm_hour,
        timeinfo->tm_min,
        timeinfo->tm_sec,
        ts.tv_nsec / 1000000L); //纳秒转为毫秒

    //打印日志优先级
    fprintf(file, "[%s] ", __priorityToString__(priority));

    //打印日志标签
    fprintf(file, "[%s] ", tag);

    //打印信息内容
    va_list args;
    va_start(args, format); //初始化 va_list，用于访问可变参数
    vfprintf(file, format, args); //将格式化的日志消息写入文件
    va_end(args); //结束 va_list 的使用

    fprintf(file, "\n"); //添加换行

    fclose(file); //关闭文件
    return 0;
}
