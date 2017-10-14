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

	if (syscall_num == SYS_HALT) {
		halt();
	}
	else if (syscall_num == SYS_EXIT) {
		//f->eax
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
		f->eax = sum_of_four_integers((int)f_esp[6], (int)f_esp[7], (int)f_esp[8], (int)f_esp[9]);
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
	// exception handling(?): ���� �����尡 ��ȿ�� ���������� üũ
	/*struct thread *exec_thread;
	void *temp;

	exec_thread = thread_current();
	temp = pagedir_get_page(exec_thread->pagedir, cmd_line);

	if (temp == NULL)
		syscall_exit(-1);*/

	return process_execute(cmd_line);
}

int wait(pid_t pid) {
	return process_wait(pid);
}

void exit(int status)
{
	struct thread *current_thread = thread_current();

	sema_down(&current_thread->parent_status); // �ڽ� thread�� ����� �θ� thread ���� ������Ʈ; �ڽ� thread�� ����
	sema_up(&(current_thread->parent->wait_flag)); // �θ� thread ������ ���� (���μ����� �θ� ����ϴ� ���)

	current_thread->parent->dead = true; // (���μ����� �θ� ����ϴ� ���)
	current_thread->parent->child_dead = status; // ���� thread�� ����

	printf("%s: exit(%d)\n", current_thread->name, status);

	thread_exit();
}

int fibonacci(int n)
{
	// ����
}

int sum_of_4_integers(int n1, int n2, int n3, int n4)
{
	return n1 + n2 + n3 + n4;
}