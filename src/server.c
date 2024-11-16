#include "../inc/minitalk.h"

static volatile sig_atomic_t g_response_info[2] = {}; // flag and pid of tha client :) Actually this global variable solved everything...

static void accumulate_string(unsigned char c) // I switched to a method of accumulating letters in a buffer
{
    static unsigned char buffer[BUFFER_SIZE];   // buffer defined in the header file
    static int i = 0;

    buffer[i] = c;
    i++;
    if (i == BUFFER_SIZE) // buffer is full
    {
        write(1, buffer, i);
        i = 0;
    }
    if (c == '\0') // end of string
    {
        write(1, buffer, i);
        write(1, "\n", 1);
        i = 0;
    }
}

static void receive_signal(int sig, siginfo_t *siginfo, void *context)
{
    static unsigned char current_byte;
    static int i;

    (void)context;
    if (sig == SIGUSR1)
        current_byte |= (1 << i);
    i++;
    if (i == 8)
    {
        accumulate_string(current_byte);
        current_byte = 0;
        i = 0;
    }
    g_response_info[0] = 1;                // set the flag
    g_response_info[1] = siginfo->si_pid;
}

int main(void)
{
	struct sigaction sa;
	pid_t pid_server;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = receive_signal;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) == -1)
		return (1);
	pid_server = getpid();
	write(1, "PID: ", 12);
	ft_putnbr(pid_server);
	write(1, "\n", 1);
	while (1)
	{
		while (g_response_info[0] != 1)
			usleep(50);
		g_response_info[0] = 0; //reset the flag
		kill(g_response_info[1], SIGUSR1);
	}
}
