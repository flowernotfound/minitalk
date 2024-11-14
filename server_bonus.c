#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void receive_signal(int sig)
{
	static int count_bit = 0;
	static unsigned char current_byte = 0;

	if (sig == SIGUSR1)
		current_byte |= (1 << (7 - count_bit));
	count_bit++;

	if (count_bit == 8)
	{
		write(1, &current_byte, 1);
		count_bit = 0;
		current_byte = 0;
	}
}

int main(void)
{
	struct sigaction sa;
	pid_t pid;

	pid = getpid();
	printf("%d\n", pid);
	sa.sa_handler = receive_signal;
	sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (1);
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
		return (1);
	while (1)
		pause();
	return 0;
}
