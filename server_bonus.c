#include "minitalk.h"

void receive_signal(int sig, siginfo_t *info, void *context)
{
	static int count_bit = 0;
	static int pid_client = 0;
	static unsigned char current_byte = 0;

	if (info->si_pid != 0)
		pid_client = info->si_pid;
	(void)context;

	if (sig == SIGUSR2)
		current_byte |= (1 << count_bit);

	count_bit++;

	if (count_bit == 8)
	{
		count_bit = 0;
		if (current_byte == 0)
			if (kill(pid_client, SIGUSR2) == -1)
				error_handler(0);
		if (current_byte != 0)
			write(1, &current_byte, 1);
		current_byte = 0;
	}

	if (kill(pid_client, SIGUSR1) == -1)
		error_handler(0);
}

int main(void)
{
	struct sigaction sa = { 0 };
	pid_t pid_server;

	sa.sa_sigaction = receive_signal;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGUSR1);
    sigaddset(&sa.sa_mask, SIGUSR2);
	pid_server = getpid();
	if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		error_handler(1);
		return (1);
	}
	ft_putnbr(pid_server);
	write(1, "\n", 1);
	while (1)
		pause();
	return 0;
}
