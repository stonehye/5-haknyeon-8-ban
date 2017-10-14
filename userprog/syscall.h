#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);

/* project 1 */
void halt(void);
int read(int fd, void *buffer, unsigned size);
int write(int fd, const void *buffer, unsigned size);
pid_t exec(const char *cmd_line);
int wait(pid_t pid);
void exit(int status);
/*************/
#endif /* userprog/syscall.h */
