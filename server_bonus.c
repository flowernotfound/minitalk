#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static pid_t g_client_pid = 0;

void receive_signal(int sig, siginfo_t *info, void *context)
{
	static int count_bit = 0;
	static int bytes_needed = 0;
	static int bytes_received = 0;
	static unsigned char current_byte = 0;
	static unsigned char utf8_buffer[4];

	(void)context;

	if (!g_client_pid)
		g_client_pid = info->si_pid;
	if (sig == SIGUSR1)
		current_byte |= (1 << (7 - count_bit));
	count_bit++;
	if (count_bit == 8)
	{
		if (bytes_needed == 0)
		{
			if ((current_byte & 0x80) == 0)
				bytes_needed = 1;
			else if ((current_byte & 0xE0) == 0xC0)
				bytes_needed = 2;
			else if ((current_byte & 0xF0) == 0xE0)
				bytes_needed = 3;
			else if ((current_byte & 0xF8) == 0xF0)
				bytes_needed = 4;
		}
		if (current_byte == '\0')
		{
			//おわり
			if (kill(g_client_pid, SIGUSR2) == -1)
				exit(1);
			g_client_pid = 0;
			bytes_needed = 0;
			bytes_received = 0;
		}
		else
		{
			utf8_buffer[bytes_received] = current_byte;
			bytes_received++;
			if (bytes_received == bytes_needed)
			{
				//応答
				write(1, utf8_buffer, bytes_needed);
				if (kill(g_client_pid, SIGUSR1) == -1)
					exit(1);
				bytes_needed = 0;
				bytes_received = 0;
			}

		}
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
	sa.sa_sigaction = receive_signal;
	sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (1);
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
		return (1);
	while (1)
		pause();
	return 0;
}
