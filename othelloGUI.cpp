/*******************************************************************

	オセロ プログラム　GUI関係ファイル　　　GUIothello.cpp

	松江工業高等専門学校　情報工学科　准教授　橋本　剛
	ゲームプログラミング　授業用

*******************************************************************/

#include "DxLib.h"
#include "othello.h"

int d = 50;//マスの間隔
int r = d / 2;//石の半径
int sx = 20, sy = 20;//盤左上の座標
int ex = sx + d * SIDE;//盤右下のx座標
int ey = sy + d * SIDE;//盤右下のy座標
int rx = ex + 10;//画面右　結果など表示するための場所ｘ座標
int STRCOLOR = GetColor(0, 0, 200);//文字色
int STRFONTSIZE = 32;//文字フォントサイズ

//マウスの座標からオセロの座標に変換する関数
int GetPosfromMouse(int MouseX, int MouseY, int *boardPosX, int *boardPosY)
{
	//check if it's in the board area
	if (MouseX < sx || MouseX > ex || MouseY < sy || MouseY > ey)
		return -1; //盤外なら-1を返す
	else
	{
		int x = 0;
		int y = 0;

		//getting the x and y position from mouse
		if(MouseX < sx + d)
			x = 1;
		else if (MouseX > ex - d)
			x = SIDE;
		else
			x = (MouseX - sx) / d + 1;//マスの番号を計算

		if (MouseY < sy + d)
			y = 1;
		else if (MouseY > ey - d)
			y = SIDE;
		else
			y = (MouseY - sy) / d + 1;//マスの番号を計算

		//代入する
		*boardPosX = x;
		*boardPosY = y;
	}
	return 0;
}
// GUI用のmanPlayer関数
Move manPlayerGUI()
{
	//入力をさせ、合法手かチェック
	char line[256];
	int x, y, num, move;
	Move moves[MOVENUM];
	num = generateMoves(moves);
	// 合法手が無い場合
	if (num == 0)
	{
		printf("PASS!\n");
		printf("Press Enter!\n");
		fgets(line, 256, stdin);
		return PASSMOVE;
	}
	do
	{

		// マウスの座標を取得
		int MouseX, MouseY;
		int x = 0;
		int y = 0;
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
		{
			// 押されている
			
			GetMousePoint(&MouseX, &MouseY);//マウスの座標を取得

			int pos = GetPosfromMouse(MouseX, MouseY, &x, &y);
			if (pos < 0)
			{
				printf("Out of Board!\n");
				continue; //盤外ならやり直し
			}

			// Check legal move
			move = getposition(x, y);
			if (isLegalMove(move))
			{
				break;
			}
			else if (!isLegalMove(move))
				DrawFormatString(rx, 240, GetColor(250, 0, 0), "Ilegal Move!!");
		}
		else
		{
			// 押されていない
		}


		//old check legal move place here
	} while (1);
	return (Move)move;
}

// 画面描画関数　** 要変更！！**
void ShowBoard()
{
	int color;
	int x, y, stone;
	int ry = 40, rdy = STRFONTSIZE + 5;
	static int GrHandle = LoadGraph("./game_assets/back.bmp");//背景画像登録 640x480
	DrawGraph(0, 0, GrHandle, FALSE);//背景を描く
	SetFontSize(STRFONTSIZE);
	DrawBox(sx, sy, ex, ey, GetColor(1, 154, 48), TRUE);//緑の盤を描画

	for (y = 1; y <= SIDE; y++)
	{
		for (x = 1; x <= SIDE; x++)
		{
			//ここから枠の描画
			//ここでは小さな四角形をたくさん書いているが、別のループで長い線を引いてももちろん良い
			DrawBox(sx + ((x-1)*d), sy+((y-1) *d), sx + (x * d), sy + (y * d), GetColor(0, 0, 0), FALSE);
			//ここから石の描画
			stone = board[getposition(x, y)];//石の色を調べる
			if (stone == 0)
				continue;
			if (stone == B)
				color = GetColor(0, 0, 0);//黒
			else
			{
				color = GetColor(230, 230, 230);//白
			}
			DrawCircle((sx + (x * d) + sx + ((x - 1) * d))/2, (sy + (y * d) + sy + ((y - 1) * d))/2, (d-6)/2, color, TRUE);//円で石を描画

		}
	}
	DrawFormatString(rx, ry, GetColor(0, 0, 0), "●: %d", stonenum[BLACK_TURN]);
	ry += rdy;
	DrawFormatString(rx, ry, GetColor(255, 255, 255), "○: %d", stonenum[WHITE_TURN]);
	ry += rdy * 2;
	DrawFormatString(rx, ry, STRCOLOR, "%d 手", ply);
}


// WinMain関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	int manturn = BLACK_TURN;
	// タイトルを 変更
	SetMainWindowText("オセロ見本 by 橋本");
	// ウインドウモードに変更
	ChangeWindowMode(TRUE);
	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
		return -1;			// エラーが起きたら直ちに終了
	// マウスを表示状態にする
	SetMouseDispFlag(TRUE);

	init();
	ShowBoard();
	SetFontSize(64);
	DrawString(80, ey / 2 - 20, "Push Space Key!!", STRCOLOR);

	// スペースボタンが押されるまで待機
	while (!CheckHitKey(KEY_INPUT_SPACE))
	{
		// メッセージループに代わる処理をする
		if (ProcessMessage() == -1)
			break;		// エラーが起きたらループを抜ける
	}

	ShowBoard();
	SetFontSize(32);
	DrawString(rx, ey / 2 - 20, "GAME START!!", STRCOLOR);
	WaitTimer(2000);

	ShowBoard();

	// 対局メインループ
	while (!CheckHitKey(KEY_INPUT_ESCAPE))//ESCでゲーム終了
	{

		int num, result;
		Move moves[MOVENUM];
		// メッセージループに代わる処理をする
		if (ProcessMessage() == -1)
			break;		// エラーが起きたらループを抜ける
		num = generateMoves(moves);
		if (isTerminalNode(FALSE))//終局かチェック
		{
			SetFontSize(32);
			//石の数で勝ち負け判定し表示
			result = stonenum[BLACK_TURN] - stonenum[WHITE_TURN];//こんな感じで
			// result により表示を変える
			if (result == 0)
				DrawString(rx, ey / 2 - 20, "DRAW!!", STRCOLOR);
			else
				DrawFormatString(rx, ey / 2 - 20, STRCOLOR, "%s WIN!!", (result > 0 ? "BLACK" : "WHITE"));
			break;
		}
		if (turn == manturn)
		{
			nextmove = manPlayerGUI();
			//nextmove = randplayer();
			PlaySoundFile("./game_assets/putMAN.wav", DX_PLAYTYPE_NORMAL);
		}
		else
		{
			comPlayer();
			PlaySoundFile("./game_assets/putCOM.wav", DX_PLAYTYPE_NORMAL);
		}
		makeMove(nextmove, 0);
		if (nextmove == PASSMOVE)
			DrawString(rx, ey / 2 - 20, "PASS!!", STRCOLOR);
		else
			ply++;
		ShowBoard();
		WaitTimer(100);
	}
	DrawString(rx, ey / 2 + 50, "Push ANY Key!!", STRCOLOR);
	while (!CheckHitKeyAll())
	{
		// メッセージループに代わる処理をする
		if (ProcessMessage() == -1)
			break;		// エラーが起きたらループを抜ける
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 
}