#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void convert_and_send_signal(int pid, char c)
{
	// 渡せる形に変換する
	// シグナル送る
	// 一文字ずつ送信する
	// シグナルはSIGUSR1とSIGUSR2を使う SIGUSR1がビット1
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

}

int main(int ac, char **av)
{
	pid_t pid_server;
	int i;
	// printf("arg1: %s\n", av[1]);
	// printf("arg2: %s\n", av[2]);
	// 不正な引数
	if (ac != 3)
	{
		printf("error");
		return (0);
	}

	// サーバーのPID取得
	pid_server = atoi(av[1]);
	if (pid_server <= 0)
		return (1);
	printf("%d", pid_server);

	i = 0;
	while (av[2][i])
	{
		convert_and_send_signal(pid_server, av[2][i]);
		i++;
	}

	// ポーズする
	return 0;
}
