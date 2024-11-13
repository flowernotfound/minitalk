#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(int ac, char **av)
{
	pid_t pid_server;
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
	printf("%d", pid_server);

	// 渡せる形に変換する

	// シグナル送る
	// 一文字ずつ送信する
	// シグナルはSIGUSR1とSIGUSR2を使う どっちでもいい

	// ポーズする
	return 0;
}
