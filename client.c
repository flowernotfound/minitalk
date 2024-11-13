#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(int ac, char **av)
{
	printf("arg: %s\n", av[1]);

	// 引数を受け取る

	// 渡せる形に変換する

	// シグナル送る
	// 一文字ずつ送信する
	// シグナルはSIGUSR1とSIGUSR2を使う どっちでもいい

	// ポーズする
	return 0;
}
