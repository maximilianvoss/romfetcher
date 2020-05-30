#ifndef __JSON2MAP_DEBUGGING_H__
#define __JSON2MAP_DEBUGGING_H__

// LOGGING LEVEL
// ERROR    0 
// WARN     1
// INFO     2
// DEBUG    3
// TRACE    4

#ifndef LOGGING_LEVEL
#define LOGGING_LEVEL 0
#endif

#define LOGGING_ERROR(fmt, ...) { fprintf(stderr, "ERROR: %s:%d - %s: ", __FILE__, __LINE__, __FUNCTION__); fprintf(stderr, fmt, ##__VA_ARGS__); fprintf(stderr,"\n"); }
#define LOGGING_WARN(fmt, ...) if ( LOGGING_LEVEL > 0 ) { fprintf(stderr, "WARN: %s:%d - %s: ", __FILE__, __LINE__, __FUNCTION__); fprintf(stderr, fmt, ##__VA_ARGS__); fprintf(stderr,"\n"); }
#define LOGGING_INFO(fmt, ...) if ( LOGGING_LEVEL > 1 ) { fprintf(stderr, "INFO: %s:%d - %s: ", __FILE__, __LINE__, __FUNCTION__); fprintf(stderr, fmt, ##__VA_ARGS__); fprintf(stderr,"\n"); }
#define LOGGING_DEBUG(fmt, ...) if ( LOGGING_LEVEL > 2 ) { fprintf(stderr, "DEBUG: %s:%d - %s: ", __FILE__, __LINE__, __FUNCTION__); fprintf(stderr, fmt, ##__VA_ARGS__); fprintf(stderr, "\n"); }
#define LOGGING_TRACE(fmt, ...) if ( LOGGING_LEVEL > 3 ) { fprintf(stderr, "TRACE: %s:%d - %s: ", __FILE__, __LINE__, __FUNCTION__); fprintf(stderr, fmt, ##__VA_ARGS__); fprintf(stderr, "\n"); }

#endif
