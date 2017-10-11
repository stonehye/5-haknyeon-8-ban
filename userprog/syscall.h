#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);

void halt(void);
int read(int fd, void *buffer, unsigned size);
int write(int fd, const void *buffer, unsigned size);
#endif /* userprog/syscall.h */
