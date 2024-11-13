#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static int g_received = 0;

void convert_and_send_signal(int pid, char c)
{
	int bit_position;

	bit_position = 7;
	while (bit_position >= 0)
	{
		if (c & (1 << bit_position))
		{
			if (kill(pid, SIGUSR1) == -1)
				exit(1);
		}
		else
		{
			if (kill(pid, SIGUSR2) == -1)
				exit(1);
		}
		usleep(100);
		bit_position--;
	}

	pause();
}

void response(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (sig == SIGUSR1)
		g_received++;
	else if (sig == SIGUSR2)
	{
		printf("Received: %d\n", g_received);
		exit(0);
	}
}

int main(int ac, char **av)
{
	pid_t pid_server;
	struct sigaction sa;
	int i;

	if (ac != 3)
		return (1);
	pid_server = atoi(av[1]);
	if (pid_server <= 0)
		return (1);

	sa.sa_sigaction = response;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) == -1 ||
        sigaction(SIGUSR2, &sa, NULL) == -1)
        return (1);

	i = 0;
	while (av[2][i])
	{
		convert_and_send_signal(pid_server, av[2][i]);
		i++;
	}
	convert_and_send_signal(pid_server, '\0');
	while (1)
		pause();
	return 0;
}
