#define ERR_SUCCESS 0
#define ERR_FAILURE 1
#define ERR_BOOTRET 2 /* the boot task returned */

// Boot the system with a default task.
// Should not return.
int ansys_boot(void (*fcn)(void *), void *input);

// Create a new task.
int ansys_create_task(void (*fcn)(void *), int prio);

// Yield the current task.
void ansys_yield(void);
