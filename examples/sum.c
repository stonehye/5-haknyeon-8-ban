#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

int main(int argc, char *argv[])
{
	int r_sum, r_fibonacci;
	int arg1, arg2, arg3, arg4;

	arg1 = atoi(argv[1]);
	arg2 = atoi(argv[2]);
	arg3 = atoi(argv[3]);
	arg4 = atoi(argv[4]);

	r_fibonacci = fibonacci(arg1);
	r_sum = sum_of_4_integers(arg1, arg2, arg3, arg4);

	printf("Input:./%s %d %d %d %d\n", argv[0], arg1, arg2, arg3, arg4);
	printf("Output: %d %d\n", r_fibonacci, r_sum);

}