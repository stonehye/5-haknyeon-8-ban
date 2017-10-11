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
  printf ("system call!\n");
  thread_exit ();
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