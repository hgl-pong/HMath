#ifndef LOGUTILS_H
#define LOGUTILS_H

#ifndef MATHLOG
#include <mutex>

#define MATH_LOG_LEVEL_VERBOSE 1
#define MATH_LOG_LEVEL_INFO 2
#define MATH_LOG_LEVEL_WARNING 3
#define MATH_LOG_LEVEL_ERROR 4
          
#ifndef DISABLE_MATHLOG
#if _DEBUG
#define MATH_CURRENT_LOG_LEVEL MATH_LOG_LEVEL_VERBOSE
#else
#define MATH_CURRENT_LOG_LEVEL MATH_LOG_LEVEL_ERROR
#endif //!_DEBUG
#else
#define MATH_CURRENT_LOG_LEVEL (MATH_LOG_LEVEL_ERROR + 1)
#endif // !DISABLE_MATHLOG

#define MATH_LOG_TIME_FORMAT "%Y-%m-%d %H:%M:%S"
#define MATH_GET_CURRENT_TIME_STR() ([]() -> std::string { \
    auto now = std::chrono::system_clock::now(); \
    auto now_c = std::chrono::system_clock::to_time_t(now); \
    char buffer[100]; \
    std::strftime(buffer, sizeof(buffer), MATH_LOG_TIME_FORMAT, std::localtime(&now_c)); \
    return buffer; })()


static std::mutex logMutex;

inline std::string GetLogColor(const int& level)
{
    if(level == MATH_LOG_LEVEL_VERBOSE)
        return "\033[36m";
    else if(level == MATH_LOG_LEVEL_INFO)
        return "\033[32m";
    else if(level == MATH_LOG_LEVEL_WARNING)
        return "\033[33m";
    else if(level == MATH_LOG_LEVEL_ERROR)
        return "\033[31m";
    else
        return "\033[0m";
}

inline std::string GetLogLevelStr(const int& level)
{
    if (level == MATH_LOG_LEVEL_VERBOSE)
        return "VERBOSE";
    else if (level == MATH_LOG_LEVEL_INFO)
        return "INFO";
    else if (level == MATH_LOG_LEVEL_WARNING)
        return "WARNING";
    else if (level == MATH_LOG_LEVEL_ERROR)
        return "ERROR";
    else
        return "UNKNOWN";
}

template <typename... Args>
inline void MathLogMessage(const int& level, const std::string& msg, Args &&...args)
{
    if (level < MATH_CURRENT_LOG_LEVEL)
        return;
    std::lock_guard<std::mutex> lock(logMutex);
    std::string timeStr = MATH_GET_CURRENT_TIME_STR();
    std::string printLogStr = timeStr + GetLogColor(level) + " [" + GetLogLevelStr(level) + "] " + "\033[0m" + msg;
    printf(printLogStr.c_str(), std::forward<Args>(args)...);
}

#define MATHLOG(level, msg, ...) do{ MathLogMessage(level, msg, ##__VA_ARGS__); } while (false)

#define MATHLOG_VERBOSE(msg, ...) MATHLOG(MATH_LOG_LEVEL_VERBOSE, msg, ##__VA_ARGS__)
#define MATHLOG_INFO(msg, ...) MATHLOG(MATH_LOG_LEVEL_INFO, msg, ##__VA_ARGS__)
#define MATHLOG_WARNING(msg, ...) MATHLOG(MATH_LOG_LEVEL_WARNING, msg, ##__VA_ARGS__)
#define MATHLOG_ERROR(msg, ...) MATHLOG(MATH_LOG_LEVEL_ERROR, msg, ##__VA_ARGS__)

#endif // !MATHLOG

#endif // !LOGUTILS_H
