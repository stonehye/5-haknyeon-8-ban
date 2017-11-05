#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <user/syscall.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "devices/input.h"
#include "userprog/pagedir.h"
#include "userprog/process.h"
#include "threads/init.h"

static void syscall_handler (struct intr_frame *);
void halt(void);
int read(int fd, void *buffer, unsigned size);
int write(int fd, const void *buffer, unsigned size);
pid_t exec(const char *file);
int wait(pid_t pid);
void exit(int status);
int fibonacci(int n);
int sum_of_4_integers(int a, int b, int c, int d);
void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
	int syscall_num = *(int*)(f->esp);
	int *f_esp = f->esp;

	int i;
	for (i = 0; i < 4; i++) {
		if (is_kernel_vaddr(f_esp[i])) {
			exit(-1);
			break;
		}
	}
	if (syscall_num == SYS_HALT) {
		halt();
	}
	else if (syscall_num == SYS_EXIT) {
		f->eax = *(int*)(f->esp+4);
		exit((int)f_esp[1]);
	}
	else if (syscall_num == SYS_EXEC) {
		f->eax = exec((const char*)f_esp[1]);
	}
	else if (syscall_num == SYS_WAIT) {
		f->eax = process_wait((pid_t)f_esp[1]);
	}
	else if (syscall_num == SYS_READ) {
		f->eax = read((int)*(f_esp+1), (void*)f_esp[2], (unsigned)f_esp[3]);
	}
	else if (syscall_num == SYS_WRITE) {
		f->eax = write((int)*(f_esp+1),(void*)*(f_esp+2), (unsigned)*(f_esp+3));
	}
	else if (syscall_num == SYS_NUM) {
		f->eax = sum_of_4_integers((int)f_esp[1], (int)f_esp[2], (int)f_esp[3], (int)f_esp[4]);
	}
	else if (syscall_num == SYS_FIBO) {
		f->eax = fibonacci((int)f_esp[1]);
	}
	else if (syscall_num == SYS_CREATE) {

	}
	else if (syscall_num == SYS_REMOVE) {

	}
	else if (syscall_num == SYS_OPEN) {

	}
	else if (syscall_num == SYS_CLOSE) {

	}
	else if (syscall_num == SYS_FILESIZE) {

	}
	else if (syscall_num == SYS_SEEK) {

	}
	else if (syscall_num == SYS_TELL) {

	}
	else {
		exit(-1);
	}
}

void halt(void)
{
	shutdown_power_off();
}

int read(int fd, void *buffer, unsigned size)
{
	int i = 0;
	uint8_t temp;
	if (buffer > PHYS_BASE) //buffer in kernel memory
	{
		exit(-1);
	}

	if (fd == 0) {
		for (i = 0; i < size; i++) {
			temp = input_getc();
			memcpy(buffer++, &temp, 1);
		}
	}
	else
		exit(-1);
}

int write(int fd, const void *buffer, unsigned size)
{
	if (fd == 1) { // write on the console
		putbuf((char*)buffer, size);
		return size;
	}
	return -1;
}

pid_t exec(const char *file)
{
	struct thread *current_thread;
	void *addr;

	current_thread = thread_current();
	addr = pagedir_get_page(current_thread->pagedir, file);

	if (addr == NULL)
		return -1;

	return process_execute(file);
}

int wait(pid_t pid) {
	return process_wait(pid);
}

void exit(int status)
{
	struct thread *current_thread = thread_current();

	sema_down(&current_thread->parent_status); 
	sema_up(&(current_thread->parent->wait_flag)); 

	current_thread->parent->dead = true;
	current_thread->parent->child_dead = status;

	printf("%s: exit(%d)\n", current_thread->name, status);

	thread_exit();
}

int fibonacci(int n)
{
	int i;
	int x0, x1, fibo;

	if (n == 0)
		return 0;
	else if (n == 1)
		return 1;
	else {
		x0 = 0;
		x1 = 1;
		for (i = 0; i < n - 1; i++) {
			fibo = x0 + x1;
			x0 = x1;
			x1 = fibo;
		}
		return fibo;
	}
}

int sum_of_4_integers(int a, int b, int c, int d)
{
	printf("USERPROG : %d %d %d %d\n",a,b,c,d);
	return a+b+c+d;
}
