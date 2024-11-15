#include "minitalk.h"

void send_end_signal(int pid)
{
    static int count = 0;

    if (count <= 8)
        if (kill(pid, SIGUSR1) == -1)
            error_handler(0);
    count++;
}

void convert_and_send_signal(int pid, char *str)
{
    static int bit_position = 0;
    static char *current_str = 0;

    if (str)
        current_str = str;
    if (*current_str)
    {
        unsigned char current_char = *current_str;
        unsigned char shift = current_char >> bit_position;
        unsigned char current_bit = shift & 1;
        if (current_bit == 0)
        {
            if (kill(pid, SIGUSR1) == -1)
                error_handler(0);
        }
        else
        {
            if (kill(pid, SIGUSR2) == -1)
                error_handler(0);
        }
        bit_position++;
        if (bit_position == 8)
        {
            current_str++;
            bit_position = 0;
        }
    }

    if (!(*current_str))
        send_end_signal(pid);
}

void process_response(int sig, siginfo_t *info, void *context)
{
	static int pid_client;

    (void)context;
    if (info->si_pid != 0)
        pid_client = info->si_pid;
    // usleep(100);
    if (sig == SIGUSR1)
        convert_and_send_signal(pid_client, NULL);
    else if (sig == SIGUSR2)
    {
        write(1, "received\n", 9);
        exit(EXIT_SUCCESS);
    }
}

int main(int ac, char **av)
{
	pid_t pid_server;
    struct sigaction sa;

	if (ac != 3)
		return (1);
	// if (strlen(av[2]) == 0)
	// 	return (1);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = process_response;
    if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) == -1)
		error_handler(1);
	pid_server = ft_atoi(av[1]);
	if (pid_server <= 0)
		return (1);
	convert_and_send_signal(pid_server, av[2]);
    while (1)
        pause();
	return 0;
}
