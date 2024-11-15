#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void receive_signal(int sig, siginfo_t *info, void *context)
{
	static int count_bit = 0;
	static pid_t client_pid = 0;
	static unsigned char current_byte = 0;

	(void)context;
	if (!client_pid)
		client_pid = info->si_pid;
	if (sig == SIGUSR1)
		current_byte |= (1 << (7 - count_bit));
	count_bit++;
	if (count_bit == 8)
	{
		count_bit = 0;
		if (!current_byte)
		{
			kill(client_pid, SIGUSR1);
			client_pid = 0;
			return ;
		}
		write(1, &current_byte, 1);
		current_byte = 0;
		kill(client_pid, SIGUSR2);
	}
}

int main(void)
{
	struct sigaction sa;
	pid_t pid_server;

	pid_server = getpid();
	printf("%d\n", pid_server);
	sa.sa_sigaction = receive_signal;
	sa.sa_flags = SA_SIGINFO;

	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (1);
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
		return (1);
	while (1)
		pause();
	return 0;
}
