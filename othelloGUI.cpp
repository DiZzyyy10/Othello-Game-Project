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
int GetPosfromMouse(int MouseX, int MouseY, int* boardPosX, int* boardPosY)
{
	//check if it's in the board area
	if (MouseX < sx || MouseX > ex || MouseY < sy || MouseY > ey)
		return -1; //盤外なら-1を返す
	else
	{
		int x_coord = 0;
		int y_coord = 0;

		//getting the x and y position from mouse
		if (MouseX < sx + d)
			x_coord = 1;
		else if (MouseX > ex - d)
			x_coord = SIDE;
		else
			x_coord = (MouseX - sx) / d + 1;//マスの番号を計算

		if (MouseY < sy + d)
			y_coord = 1;
		else if (MouseY > ey - d)
			y_coord = SIDE;
		else
			y_coord = (MouseY - sy) / d + 1;//マスの番号を計算

		//代入する
		*boardPosX = x_coord;
		*boardPosY = y_coord;
	}
	return 0;
}
// GUI用のmanPlayer関数
Move manPlayerGUI()
{
	Move available_moves[MOVENUM]; // Renamed 'moves' to avoid conflict
	int num_legal_moves = generateMoves(available_moves); // Renamed 'num'

	if (num_legal_moves == 0)
	{
		return PASSMOVE;
	}

	Move selected_move; 
	static int prevMouseInputState = 0;

	do
	{
		if (ProcessMessage() == -1 || GetWindowUserCloseFlag()) {
			return PASSMOVE;
		}

		int currentMouseInputState = GetMouseInput();
		if ((currentMouseInputState & MOUSE_INPUT_LEFT) != 0 && !(prevMouseInputState & MOUSE_INPUT_LEFT))
		{
			int mouse_dx_x, mouse_dx_y; 
			int board_click_x, board_click_y; 

			GetMousePoint(&mouse_dx_x, &mouse_dx_y);

			int click_on_board_result = GetPosfromMouse(mouse_dx_x, mouse_dx_y, &board_click_x, &board_click_y);
			if (click_on_board_result == 0) 
			{
				selected_move = getposition(board_click_x, board_click_y);
				if (isLegalMove(selected_move))
				{
					prevMouseInputState = currentMouseInputState;
					return selected_move; 
				}
				else
				{
					DrawFormatString(rx, 240, GetColor(250, 0, 0), "Illegal Move!!");
				}
			}
		}
		prevMouseInputState = currentMouseInputState;
		WaitTimer(1); 

	} while (1);
}

// 画面描画関数
void ShowBoard()
{
	// ClearDrawScreen() will be called in WinMain before this
	int color;
	int x, y, stone;
	int ry = 40, rdy = STRFONTSIZE + 5;
	static int GrHandle = LoadGraph("./game_assets/back.bmp");
	if (GrHandle == -1) {
		DrawBox(0, 0, 640, 480, GetColor(100, 100, 100), TRUE); // Fallback background
	}
	else {
		DrawGraph(0, 0, GrHandle, FALSE);
	}
	SetFontSize(STRFONTSIZE);
	DrawBox(sx, sy, ex, ey, GetColor(0, 160, 0), TRUE); // Darker Green

	for (y = 1; y <= SIDE; y++)
	{
		for (x = 1; x <= SIDE; x++)
		{
			DrawBox(sx + ((x - 1) * d), sy + ((y - 1) * d), sx + (x * d), sy + (y * d), GetColor(0, 0, 0), FALSE);
			stone = board[getposition(x, y)];
			if (stone == 0)
				continue;
			if (stone == B)
				color = GetColor(0, 0, 0);
			else
			{
				color = GetColor(230, 230, 230); // Slightly off-white
			}
			DrawCircle((sx + ((x - 1) * d) + d / 2), (sy + ((y - 1) * d) + d / 2), (d / 2) - 3, color, TRUE); // Stones slightly smaller
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
	int quit_flag = FALSE;

	SetMainWindowText("オセロ見本 by 橋本");
	ChangeWindowMode(TRUE);
	// SetGraphMode(640, 480, 32); // Optional: explicitly set window size

	if (DxLib_Init() == -1)
		return -1;

	SetDrawScreen(DX_SCREEN_BACK); // Draw to back buffer

	SetMouseDispFlag(TRUE);

	init();

	ClearDrawScreen();
	ShowBoard();
	SetFontSize(64);
	DrawString(80, ey / 2 - 20, "Push Space Key!!", STRCOLOR);
	ScreenFlip();

	while (!CheckHitKey(KEY_INPUT_SPACE))
	{
		if (ProcessMessage() == -1 || GetWindowUserCloseFlag()) {
			quit_flag = TRUE;
			break;
		}
		WaitTimer(16); 
	}
	if (quit_flag) { DxLib_End(); return 0; }

	ClearDrawScreen();
	ShowBoard();
	SetFontSize(32);
	DrawString(rx, ey / 2 - 20, "GAME START!!", STRCOLOR);
	ScreenFlip();

	DWORD startTime = GetNowCount();
	while (GetNowCount() - startTime < 2000) { 
		if (ProcessMessage() == -1 || GetWindowUserCloseFlag()) {
			quit_flag = TRUE;
			break;
		}
		WaitTimer(16); 
	}
	if (quit_flag) { DxLib_End(); return 0; }

	ClearDrawScreen();
	ShowBoard();
	ScreenFlip();

	while (!CheckHitKey(KEY_INPUT_ESCAPE))
	{
		if (ProcessMessage() == -1 || GetWindowUserCloseFlag()) {
			quit_flag = TRUE;
			break;
		}

		int num_moves, result; 
		Move current_moves[MOVENUM]; // Renamed 'moves'

		num_moves = generateMoves(current_moves);
		if (isTerminalNode(FALSE))
		{
			ClearDrawScreen();
			ShowBoard(); // Show final board state
			SetFontSize(32);
			result = stonenum[BLACK_TURN] - stonenum[WHITE_TURN];
			if (result == 0)
				DrawString(rx, ey / 2 - 20, "DRAW!!", STRCOLOR);
			else
				DrawFormatString(rx, ey / 2 - 20, STRCOLOR, "%s WIN!!", (result > 0 ? "BLACK" : "WHITE"));
			ScreenFlip();
			quit_flag = TRUE; // Set flag to proceed to "Push ANY Key" then exit
			break;
		}

		if (turn == manturn)
		{
			nextmove = manPlayerGUI();
			if (GetWindowUserCloseFlag()) { quit_flag = TRUE; break; } // Check if manPlayerGUI signaled quit
			if (nextmove != PASSMOVE) { // Only play sound if a move was actually made
				PlaySoundFile("./game_assets/putMAN.wav", DX_PLAYTYPE_BACK);
			}
		}
		else
		{
			comPlayer(); // comPlayer sets global 'nextmove'
			if (nextmove != PASSMOVE) {
				PlaySoundFile("./game_assets/putCOM.wav", DX_PLAYTYPE_BACK);
			}
		}
		if (GetWindowUserCloseFlag()) { quit_flag = TRUE; break; } // Check after AI turn too

		makeMove(nextmove, 0);
		if (nextmove == PASSMOVE) {
			// This DrawString will be part of the next ShowBoard cycle
		}
		else {
			ply++;
		}

		ClearDrawScreen();
		ShowBoard();
		if (nextmove == PASSMOVE) { // Draw PASS message if applicable
			DrawString(rx, ey / 2 - 40, "PASS!!", STRCOLOR); // Adjusted position
		}
		ScreenFlip();
		WaitTimer(100); // Original wait
	}
	// If quit_flag was set by game end, this check is fine. If by ESC/X, also fine.
	if (quit_flag && !(isTerminalNode(TRUE))) { // If quit by ESC/X before game naturally ended
		DxLib_End();
		return 0;
	}


	// Only show "Push ANY Key" if game ended naturally or was aborted by ESC but not X
	if (!GetWindowUserCloseFlag()) {
		ClearDrawScreen();
		ShowBoard(); // Show the very final board state
		if (isTerminalNode(TRUE) && !CheckHitKey(KEY_INPUT_ESCAPE)) { // If game ended and not by ESC key during main loop
			SetFontSize(32);
			int final_result = stonenum[BLACK_TURN] - stonenum[WHITE_TURN];
			if (final_result == 0)
				DrawString(rx, ey / 2 - 20, "DRAW!!", STRCOLOR);
			else
				DrawFormatString(rx, ey / 2 - 20, STRCOLOR, "%s WIN!!", (final_result > 0 ? "BLACK" : "WHITE"));
		}
		DrawString(rx, ey / 2 + 50, "Push ANY Key!!", STRCOLOR);
		ScreenFlip();

		while (!CheckHitKeyAll())
		{
			if (ProcessMessage() == -1 || GetWindowUserCloseFlag()) {
				quit_flag = TRUE; // Set flag, will be handled by DxLib_End()
				break;
			}
			WaitTimer(16); // Keep responsive
		}
	}

	DxLib_End();
	return 0;
}
