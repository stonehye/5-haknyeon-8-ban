#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

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
		f->eax = exit((int)f_esp[1]);
	}
	else if (syscall_num == SYS_EXEC) {
		f->eax = exec((const char*)f_esp[1]);
	}
	else if (syscall_num == SYS_WAIT) {
		f->eax = process_wait((pid_t)f_esp[1]);
	}
	else if (syscall_num == SYS_READ) {
		f->eax = read((int)f_esp[5], (void*)f_esp[6], (unsigned)f_esp[7]);
	}
	else if (syscall_num == SYS_WRITE) {
		f->eax = write((int)f_esp[5], (void*)f_esp[6], (unsigned)f_esp[7]);
	}
	else if (syscall_num == SYS_NUM) {
		f->eax = sum_of_4_integers((int)f_esp[6], (int)f_esp[7], (int)f_esp[8], (int)f_esp[9]);
	}
	else if (syscall_num == SYS_FIBO) {
		f->eax = fibonacci((int)f_esp[1]);
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
		exit(-1);

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
	if (buffer >= PHYS_BASE || (pagedir_get_page(thread_current()->pagedir, buffer) == NULL) || buffer == NULL)
		exit(-1);
	if (fd == 1) { // write on the console
		putbuf((char*)buffer, size);
	}
	return size;
}

pid_t exec(const char *cmd_line)
{
	struct thread *current_thread;
	void *addr;

	current_thread = thread_current();
	addr = pagedir_get_page(current_thread->pagedir, cmd_line);

	if (addr == NULL)
		exit(-1); 

	return process_execute(cmd_line);
}

int wait(pid_t pid) {
	return process_wait(pid);
}

void exit(int status)
{
	struct thread *current_thread = thread_current();

	sema_down(&current_thread->parent_status); // 자식 thread에 저장된 부모 thread 상태 업데이트; 자식 thread의 종료
	sema_up(&(current_thread->parent->wait_flag)); // 부모 thread 대기상태 해제 (프로세스의 부모가 대기하는 경우)

	current_thread->parent->dead = true; // (프로세스의 부모가 대기하는 경우)
	current_thread->parent->child_dead = status; // 현재 thread의 상태

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

int sum_of_4_integers(int n1, int n2, int n3, int n4)
{
	return n1 + n2 + n3 + n4;
}