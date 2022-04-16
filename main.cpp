#include "MyGame.h"

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int)
{
	MyGame game;

	//ゲームの初期化
	game.Initialize();


	while (true)  // ゲームループ
	{
		//毎フレーム更新
		game.Update();
		//終了リクエストが来たら抜ける
		if (game.IsEndRequst()) {
			break;
		}
		//描画
		game.Draw();
	}
	// ゲームの終了
	game.Finalize();

	return 0;
}