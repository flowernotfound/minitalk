#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(void)
{
	pid_t pid;

	pid =getpid();
	printf("%d\n", pid);
	// プロセスのIDを取得
	// プロセスIDを表示

	//　クライアントからメッセージを受け取る

	// シグナル処理の設定
	// シグナルからデコードする
	// 	複数のクライアントからメッセージを受け取れるようにする　非同期で処理
	return 0;
}
