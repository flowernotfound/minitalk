#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void receive_signal(int sig, siginfo_t *info, void *context)
{
	static int count_bit = 0;
	static unsigned char current_byte = 0;
	static pid_t client_pid = 0;

	(void)context;

    if (info->si_pid != client_pid) //ここちゃんと書き換えないとクライアントのプロセスIDが更新されなくて複数回の実行で不具合出る
    { // 今は問題なし
        client_pid = info->si_pid;
        count_bit = 0;
        current_byte = 0;
    }

	if (sig == SIGUSR1)
		current_byte |= (1 << (7 - count_bit));
	count_bit++;

	if (count_bit == 8)
	{
		write(1, &current_byte, 1);
		// 応答
		if (kill(client_pid, SIGUSR1) == -1)
            printf("Failed to send confirmation\n"); //debug
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
