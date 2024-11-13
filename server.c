#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void receive_signal(int sig)
{
	// SIGUSR1の場合
	if (sig == SIGUSR1)
	{

	}	// SIGUSR2の場合
	else if (sig ==SIGUSR2)
	{

	}
}

int main(void)
{
	struct sigaction sa;
	pid_t pid;

	// プロセスのIDを取得
	pid = getpid();

	// プロセスIDを表示
	printf("%d\n", pid);

	// シグナル処理の設定
	sa.sa_handler = receive_signal;
	sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		return (1);
	}
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		return (1);
	}

	while (1)
	{
		pause();
	}
	return 0;
}
