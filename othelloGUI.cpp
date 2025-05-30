/*******************************************************************

	�I�Z�� �v���O�����@GUI�֌W�t�@�C���@�@�@GUIothello.cpp

	���]�H�ƍ������w�Z�@���H�w�ȁ@�y�����@���{�@��
	�Q�[���v���O���~���O�@���Ɨp

*******************************************************************/

#include "DxLib.h"
#include "othello.h"

int d = 50;//�}�X�̊Ԋu
int r = d / 2;//�΂̔��a
int sx = 20, sy = 20;//�Ս���̍��W
int ex = sx + d * SIDE;//�ՉE����x���W
int ey = sy + d * SIDE;//�ՉE����y���W
int rx = ex + 10;//��ʉE�@���ʂȂǕ\�����邽�߂̏ꏊ�����W
int STRCOLOR = GetColor(0, 0, 200);//�����F
int STRFONTSIZE = 32;//�����t�H���g�T�C�Y

//�}�E�X�̍��W����I�Z���̍��W�ɕϊ�����֐�
int GetPosfromMouse(int MouseX, int MouseY, int *boardPosX, int *boardPosY)
{
	//check if it's in the board area
	if (MouseX < sx || MouseX > ex || MouseY < sy || MouseY > ey)
		return -1; //�ՊO�Ȃ�-1��Ԃ�
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
			x = (MouseX - sx) / d + 1;//�}�X�̔ԍ����v�Z

		if (MouseY < sy + d)
			y = 1;
		else if (MouseY > ey - d)
			y = SIDE;
		else
			y = (MouseY - sy) / d + 1;//�}�X�̔ԍ����v�Z

		//�������
		*boardPosX = x;
		*boardPosY = y;
	}
	return 0;
}
// GUI�p��manPlayer�֐�
Move manPlayerGUI()
{
	//���͂������A���@�肩�`�F�b�N
	char line[256];
	int x, y, num, move;
	Move moves[MOVENUM];
	num = generateMoves(moves);
	// ���@�肪�����ꍇ
	if (num == 0)
	{
		printf("PASS!\n");
		printf("Press Enter!\n");
		fgets(line, 256, stdin);
		return PASSMOVE;
	}
	do
	{

		// �}�E�X�̍��W���擾
		int MouseX, MouseY;
		int x = 0;
		int y = 0;
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
		{
			// ������Ă���
			
			GetMousePoint(&MouseX, &MouseY);//�}�E�X�̍��W���擾

			int pos = GetPosfromMouse(MouseX, MouseY, &x, &y);
			if (pos < 0)
			{
				printf("Out of Board!\n");
				continue; //�ՊO�Ȃ��蒼��
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
			// ������Ă��Ȃ�
		}


		//old check legal move place here
	} while (1);
	return (Move)move;
}

// ��ʕ`��֐��@** �v�ύX�I�I**
void ShowBoard()
{
	int color;
	int x, y, stone;
	int ry = 40, rdy = STRFONTSIZE + 5;
	static int GrHandle = LoadGraph("./game_assets/back.bmp");//�w�i�摜�o�^ 640x480
	DrawGraph(0, 0, GrHandle, FALSE);//�w�i��`��
	SetFontSize(STRFONTSIZE);
	DrawBox(sx, sy, ex, ey, GetColor(1, 154, 48), TRUE);//�΂̔Ղ�`��

	for (y = 1; y <= SIDE; y++)
	{
		for (x = 1; x <= SIDE; x++)
		{
			//��������g�̕`��
			//�����ł͏����Ȏl�p�`���������񏑂��Ă��邪�A�ʂ̃��[�v�Œ������������Ă��������ǂ�
			DrawBox(sx + ((x-1)*d), sy+((y-1) *d), sx + (x * d), sy + (y * d), GetColor(0, 0, 0), FALSE);
			//��������΂̕`��
			stone = board[getposition(x, y)];//�΂̐F�𒲂ׂ�
			if (stone == 0)
				continue;
			if (stone == B)
				color = GetColor(0, 0, 0);//��
			else
			{
				color = GetColor(230, 230, 230);//��
			}
			DrawCircle((sx + (x * d) + sx + ((x - 1) * d))/2, (sy + (y * d) + sy + ((y - 1) * d))/2, (d-6)/2, color, TRUE);//�~�Ő΂�`��

		}
	}
	DrawFormatString(rx, ry, GetColor(0, 0, 0), "��: %d", stonenum[BLACK_TURN]);
	ry += rdy;
	DrawFormatString(rx, ry, GetColor(255, 255, 255), "��: %d", stonenum[WHITE_TURN]);
	ry += rdy * 2;
	DrawFormatString(rx, ry, STRCOLOR, "%d ��", ply);
}


// WinMain�֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	int manturn = BLACK_TURN;
	// �^�C�g���� �ύX
	SetMainWindowText("�I�Z�����{ by ���{");
	// �E�C���h�E���[�h�ɕύX
	ChangeWindowMode(TRUE);
	// �c�w���C�u��������������
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
		return -1;			// �G���[���N�����璼���ɏI��
	// �}�E�X��\����Ԃɂ���
	SetMouseDispFlag(TRUE);

	init();
	ShowBoard();
	SetFontSize(64);
	DrawString(80, ey / 2 - 20, "Push Space Key!!", STRCOLOR);

	// �X�y�[�X�{�^�����������܂őҋ@
	while (!CheckHitKey(KEY_INPUT_SPACE))
	{
		// ���b�Z�[�W���[�v�ɑ��鏈��������
		if (ProcessMessage() == -1)
			break;		// �G���[���N�����烋�[�v�𔲂���
	}

	ShowBoard();
	SetFontSize(32);
	DrawString(rx, ey / 2 - 20, "GAME START!!", STRCOLOR);
	WaitTimer(2000);

	ShowBoard();

	// �΋ǃ��C�����[�v
	while (!CheckHitKey(KEY_INPUT_ESCAPE))//ESC�ŃQ�[���I��
	{

		int num, result;
		Move moves[MOVENUM];
		// ���b�Z�[�W���[�v�ɑ��鏈��������
		if (ProcessMessage() == -1)
			break;		// �G���[���N�����烋�[�v�𔲂���
		num = generateMoves(moves);
		if (isTerminalNode(FALSE))//�I�ǂ��`�F�b�N
		{
			SetFontSize(32);
			//�΂̐��ŏ����������肵�\��
			result = stonenum[BLACK_TURN] - stonenum[WHITE_TURN];//����Ȋ�����
			// result �ɂ��\����ς���
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
		// ���b�Z�[�W���[�v�ɑ��鏈��������
		if (ProcessMessage() == -1)
			break;		// �G���[���N�����烋�[�v�𔲂���
	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������
	return 0;				// �\�t�g�̏I�� 
}