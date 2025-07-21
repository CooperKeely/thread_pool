#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <time.h>

#ifndef LOG_LEVEL
#define LOG_LEVEL 3
#endif

#ifndef LOG_MODULE_NAME
#define LOG_MODULE_NAME __FILE__
#endif


// ANSI color codes
#define COLOR_RESET 	"\033[0m"
#define COLOR_RED 	"\033[31m"
#define COLOR_YELLOW 	"\033[33m"
#define COLOR_CYAN 	"\033[36m"

static inline const char* log_get_timestamp(){
	static char buf[20];
	time_t now = time(NULL);
	struct tm* tm_info = localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
	return buf;
}

// Core log Macro
#define LOG_PRINT(level_str, color, fmt, ...) \
	fprintf(stderr, color "[%s] %s [%s:%d (%s)] " fmt COLOR_RESET "\n", \
		level_str, log_get_timestamp(), __FILE__, __LINE__, LOG_MODULE_NAME, ##__VA_ARGS__)

#if LOG_LEVEL >= 3
#define LOG_DBG(fmt, ...) LOG_PRINT("DBG", COLOR_CYAN, fmt, ##__VA_ARGS__)
#else
#define LOG_DBG(fmt, ...)
#endif

#if LOG_LEVEL >= 2
#define LOG_INF(fmt, ...) LOG_PRINT("INF", COLOR_RESET, fmt, ##__VA_ARGS__)
#else
#define LOG_INF(fmt, ...)
#endif

#if LOG_LEVEL >= 1
#define LOG_WRN(fmt, ...) LOG_PRINT("WRN", COLOR_YELLOW, fmt, ##__VA_ARGS__)
#else
#define LOG_WRN(fmt, ...)
#endif

#if LOG_LEVEL >= 0
#define LOG_ERR(fmt, ...) LOG_PRINT("ERR", COLOR_RED, fmt, ##__VA_ARGS__)
#else
#define LOG_ERR(fmt, ...)
#endif

#endif // LOG_H


