#ifndef LOGGER_H_
#define LOGGER_H_
#include <cstdio>

#ifndef NDEBUG
#define LOG_MSG(logType, fmt, ...)                                                                                    \
  do {                                                                                                                \
    const char* format = "[%s] [%s] [%s] [%s] [%s] [%d] -> " fmt "\n";                                                \
    (void)std::fprintf(stderr, format, __DATE__, __TIME__, logType, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
  } while (0)

#define LOG_DEB(fmt, ...) LOG_MSG("DEB", fmt, ##__VA_ARGS__)
#define LOG_INF(fmt, ...) LOG_MSG("INF", fmt, ##__VA_ARGS__)
#define LOG_WAR(fmt, ...) LOG_MSG("WAR", fmt, ##__VA_ARGS__)
#define LOG_ERR(fmt, ...) LOG_MSG("ERR", fmt, ##__VA_ARGS__)
#define LOG_FAT(fmt, ...) LOG_MSG("FAT", fmt, ##__VA_ARGS__)
#else
#define LOG(logType, fmt, ...)
#define LOG_DEB(fmt, ...)
#define LOG_INF(fmt, ...)
#define LOG_WAR(fmt, ...)
#define LOG_ERR(fmt, ...)
#define LOG_FAT(fmt, ...)
#endif
#endif