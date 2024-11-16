#include "../inc/minitalk_bonus.h"

volatile sig_atomic_t g_response = 0;

static void	set_flag(int sig, siginfo_t *siginfo, void *context)
{
	(void)sig;
	(void)siginfo;
	(void)context;
	g_response = 1;
}

static void send_char(int pid_server, int c)
{
    int bit_position;
    int current_bit;

    bit_position = 0;
    while (bit_position <= 7)
    {
        current_bit = c >> bit_position;
        current_bit = current_bit & 1;
        if (current_bit == 1)
        {
            if(kill(pid_server, SIGUSR1) == -1)
				exit(EXIT_FAILURE);
        }
        else
        {
            if(kill(pid_server, SIGUSR2) == -1)
				exit(EXIT_FAILURE);
        }
        bit_position++;
        while (g_response != 1) // wait for response
            usleep(100);
        g_response = 0;  // reset the flag
    }
}


int main (int ac, char **av)
{
	struct sigaction sa;
	// siginfo_t siginfo;
	pid_t pid_server;
	int i;
	char current_char;

	if (ac != 3)
		return (0);
	pid_server = ft_atoi(av[1]);
	if (pid_server <= 0)
		return (0);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = set_flag;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (1);
	// siginfo.si_pid = pid_server;
	i = 0;
	while (1)
	{
		current_char = av[2][i];
		send_char(pid_server, current_char);
		// send_char(siginfo.si_pid, current_char);
		if (current_char == '\0')
			break;
		i++;
	}
	write(1, "Received!\n", 10);
	return (0);
}
