#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static int g_received_count = 0;

void received_confirmation(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;

	if (sig == SIGUSR1)
		g_received_count++;
}

void convert_and_send_signal(int pid, char c)
{
	int bit_position;

	bit_position = 7;

	while (bit_position >= 0)
	{
		if (c & (1 << bit_position))
		{
			if (kill(pid, SIGUSR1) == -1)
			{
				printf("Failed to send signal\n"); //debug
				exit(1);
			}
		}
		else
		{
			if (kill(pid, SIGUSR2) == -1)
			{
		        printf("Failed to send signal\n");//debug
        	    exit(1);
			}
		}

		usleep(100);
		bit_position--;
	}
	usleep(100);
}

int main(int ac, char **av)
{
	struct sigaction sa;
	pid_t pid_server;
	int i;
	int len;

	if (ac != 3)
		return (1);

	pid_server = atoi(av[1]);
	if (pid_server <= 0)
		return (1);

	sa.sa_sigaction = received_confirmation;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
        return (1);
    if (sigaction(SIGUSR2, &sa, NULL) == -1)
        return (1);

	len = 0;
	while (av[2][len])
		len++;
	printf("Sent: %d\n", len);

	g_received_count = 0; //これ多分いらない

	i = 0;
	while (av[2][i])
	{
		convert_and_send_signal(pid_server, av[2][i]);
		i++;
	}

	usleep(len * 100);
	printf("Received:  %d\n", g_received_count);

	return 0;
}
