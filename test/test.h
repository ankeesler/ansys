#include <time.h>

#define TIMEOUT_S 2

#define run(test, debug) __run((test), #test, (debug))
void __run(void (*test)(void), const char *name, int debug);
#define skip(test, debug) __skip((test), #test, (debug))
void __skip(void (*test)(void), const char *name, int debug);

#define equal(a, b) __equal((a), #a, (b), #b, __FILE__, __LINE__)
void __equal(int a, const char *a_name, int b, const char *b_name,
        const char *file, int line);

#define equal_eventually(a, b) \
    do { \
        time_t now = time(NULL); \
        do { \
            sleep(1); \
            if ((time(NULL) - now) > TIMEOUT_S) { \
                __print_status((a), #a, (b), #b, "FAIL", "==", __FILE__, __LINE__); \
                exit(1); \
            } \
        } while((a) != (b)); \
        __print_status((a), #a, (b), #b, "PASS", "==", __FILE__, __LINE__); \
    } while (0);

#define note(...) fprintf(stderr, __VA_ARGS__), fflush(stderr);

void __print_status(int a, const char *a_name, int b, const char *b_name,
        const char *status, const char *op, const char *file, int line);
