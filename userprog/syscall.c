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
bool create(const char* file, unsigned initial_size);
int open(const char* file);
void close(int fd);
int filesize(int fd);
void seek(int fd, unsigned position);
bool remove(const char* file);
unsigned tell(int fd);

struct lock f_lock;

void
syscall_init (void) 
{
	lock_init(&f_lock);
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
		f->eax = create((const char*)f_esp[1], (unsigned)f_esp[2]);
	}
	else if (syscall_num == SYS_REMOVE) {
		f->eax = remove((const char*)f_esp[1]);
	}
	else if (syscall_num == SYS_OPEN) {
		f->eax = open((const char*)f_esp[1]);
	}
	else if (syscall_num == SYS_CLOSE) {
		close((int)f_esp[1])
	}
	else if (syscall_num == SYS_FILESIZE) {
		f->eax = filesize((int)f_esp[1]);
	}
	else if (syscall_num == SYS_SEEK) {
		seek((int)f_esp[1], (unsigned)f_esp[2]);
	}
	else if (syscall_num == SYS_TELL) {
		f->eax = tell((int)f_esp[1]);
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
	struct thread *t;
	struct list_elem *elem_p;

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
	else {
		int read_size;
		struct file_jungbo jungbo_temp;
		t = thread_current();
		for (elem_p = list_begin(&t->file_list); elem_p != list_end(&t->file_list); elem_p = list_next(elem_p)) {
			if (list_entry(elem_p, struct file_jungbo, file_elem)->fd == fd) {
				break;
			}
		}
		if (list_entry(elem_p, struct file_jungbo, file_elem)->fp == NULL)
			return -1;
		jungbo_temp = list_entry(elem_p, struct file_jungbo, file_elem);
		lock_acquire(&f_lock);
		read_size = file_read(jungbo_temp->fp, buffer, size);
		lock_release(&f_lock);
		return read_size;
	}
}

int write(int fd, const void *buffer, unsigned size)
{
	struct thread *t;
	struct list_elem *elem_p;

	if (fd == 1) { // write on the console
		putbuf((char*)buffer, size);
		return size;
	}
	else {
		int write_size;
		struct file_jungbo jungbo_temp;
		t = thread_current();
		for (elem_p = list_begin(&t->file_list); elem_p != list_end(&t->file_list); elem_p = list_next(elem_p)) {
			if (list_entry(elem_p, struct file_jungbo, file_elem)->fd == fd) {
				break;
			}
		}
		if (list_entry(elem_p, struct file_jungbo, file_elem)->fp == NULL)
			return -1;
		jungbo_temp = list_entry(elem_p, struct file_jungbo, file_elem);
		lock_acquire(&f_lock);
		write_size = file_write(jungbo_temp->fp, buffer, size);
		lock_release(&f_lock);
		return write_size;
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

bool create(const char* file, unsigned initial_size)
{
	if (!file || initial_size < 0)
		exit(-1);

	return filesys_create(file, initial_size);
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

int open(const char* file)
{
	struct thread *t = thread_current();
	struct file *f;
	struct file_jungbo f_info, f_prev;
	int i;

	if (!file) {
		exit(-1);
	}

	lock_acquire(&f_lock);
	f = filesys_open(file);
	lock_release(f_lock);

	if (f) {
		f_info = malloc(sizeof(*f_info));
		f_info->fp = f;

		if (list_empty(&t->file_list))
			f_info->fd = 2;
		else {
			f_prev = list_entry(list_back(&t->file_list), struct file_jungbo, file_elem);
			f_info->fd = f_prev->fd + 1;
		}
		list_push_back(&t->file_list, &f_info->file_elem);

		return f_info->fd;
	}
	
	return -1;
}

void close(int fd)
{
	struct thread *t = thread_current();
	struct list_elem *temp;

	if (fd == 0 || fd == 1 || fd == 2)
		return;

	for (temp = list_begin(&(t->flie_list)); temp != list_end(&(t->file_list)); temp = list_next(temp)) {
		if (list_entry(temp, struct file_jungbo, file_elem)->fd == fd)
			break;
	}

	if (temp == list_end(&(t->file_list)))
		return;
	else {
		//lock_acquire(&f_lock);
		file_close(list_entry(temp, struct file_jungbo, fild_elem)->fp);
		//lock_release(&f_lock);
		list_remove(temp);
		free(list_entry(temp, struct file_jungbo, file_elem));
	}
}

int filesize(int fd)
{
	struct thread *t = thread_current();
	struct list_elem *temp;

	for (temp = list_begin(&(t->flie_list)); temp != list_end(&(t->file_list)); temp = list_next(temp)) {
		if (list_entry(temp, struct file_jungbo, file_elem)->fd == fd)
			break;
	}

	if (temp == list_end(&(t->file_list)))
		return -1;
	else
		return file_length(list_entry(temp, struct file_list, file_elem)->fp);
}

void seek(int fd, unsigned position)
{
	struct thread *t = thread_current();
	struct list_elem *temp;

	for (temp = list_begin(&(t->flie_list)); temp != list_end(&(t->file_list)); temp = list_next(temp)) {
		if (list_entry(temp, struct file_jungbo, file_elem)->fd == fd)
			break;
	}

	if (temp == list_end(&(t->file_list)))
		return;
	else
		file_seek(list_entry(temp, struct file_list, file_elem)->fp, position);
}

bool remove(const char* file)
{
	if (file[0] == '\0')
		return false;
	if (!file)
		exit(-1);
	return filesys_remove(file);
}

unsigned tell(int fd)
{
	struct thread *t;
	struct list_elem *elem_p;
	struct file_jungbo jungbo_temp;
	unsigned tell_temp;
	t = current_thread();
	for (elem_p = list_begin(&t->file_list); elem_p != list_end(&t->file_list); elem_p = list_next(elem_p)) {
		if (list_entry(elem_p, struct file_jungbo, file_elem)->fd == fd) {
			break;
		}
	}
	if (list_entry(elem_p, struct file_jungbo, file_elem)->fp == NULL)
		return -1;
	jungbo_temp = list_entry(elem_p, struct file_jungbo, file_elem);
	lock_acquire(&f_lock);
	tell_temp = file_tell(jungbo_temp->fp);
	lock_release(&f_lock);
	return tell_temp;
}