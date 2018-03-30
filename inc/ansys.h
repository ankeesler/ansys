#define ERR_SUCCESS 0
#define ERR_FAILURE 1

// Boot the system with a default task.
// Should not return.
int ansys_boot(void (*fcn)(void *), void *input);

// Create a new task.
int ansys_create_task(void (*fcn)(void *));

// Yield the current task.
void ansys_yield(void);
