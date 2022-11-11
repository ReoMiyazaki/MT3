#include "DxLib.h"
#include "Global.h"
#include "Vector3.h"

// �֐��v���g�^�C�v�錾
// int DrawCircle(Vector2 vec, int r, unsigned int color);
int DrawSphere3D(
	const Vector3& CenterPos, const float r, const float DivNum,
	const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag);

int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int Color);

int SetCameraPositionAndTargetAndUpVec(
	const Vector3& cameraPosition,
	const Vector3& cameraTarget,
	const Vector3& cameraUp
);

void DrawAxis3D(const float length);	// x, y, z ���̕`��

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	// �E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	// ��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�(���j�^�[�̉𑜓x�ɍ��킹��)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// ��ʂ̔w�i�F��ݒ肷��
	SetBackgroundColor(0, 0, 64);

	// DXlib�̏�����
	if (DxLib_Init() == -1) { return -1; }

	// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	// Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(TRUE);

	// �N���b�v��( ��, �� )
	SetCameraNearFar(1.0f, 1000.0f);	// �J�����̗L���͈͂�ݒ�
	SetCameraScreenCenter(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0);	// ��ʂ̒��S���J�����̒��S�ɍ��킹��
	SetCameraPositionAndTargetAndUpVec(
		Vector3(0.0f, 0.0f, -120.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f)
	);

	// ���Ԍv���ɕK�v�ȃf�[�^
	long long startCount = 0;
	long long nowConst = 0;
	long long elapsedCount = 0;

	// ��ԂŎg���f�[�^
	// start -> end �� 5 [s] �Ŋ���������
	Vector3 start(-100.0f, 0, 0);	// �X�^�[�g�n�_
	Vector3 end(+100.0f, 0, 0);		// �G���h�n�_
	float maxTime = 5.0f;			// �S�̎��� [s]
	float timeRate;					// ��%���Ԃ��i�񂾂�(��)

	// ���̈ʒu
	Vector3 position;

	// ���s�O�ɃJ�E���^�l���擾
	startCount = GetNowHiPerformanceCount();	// long long int�^ 64Bit int


	// �ŐV�̃L�[�{�[�h���p
	char keys[256] = { 0 };

	// 1���[�v(�t���[��)�O�̃L�[�{�[�h���
	char oldkeys[256] = { 0 };

	// �Q�[�����[�v
	while (true) {
		// �ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		// �ŐV�̃L�[�{�[�h�����擾
		GetHitKeyStateAll(keys);

		// ��ʃN���A
		ClearDrawScreen();
		//---------  ��������v���O�������L�q  ----------//

		// �X�V����

		// [R]�L�[�Ń��X�^�[�g
		if (CheckHitKey(KEY_INPUT_R))
		{
			startCount = GetNowHiPerformanceCount();
		}
		// �o�ߎ���(elapsedTime [s]) �̎���
		nowConst = GetNowHiPerformanceCount();
		elapsedCount = nowConst - startCount;
		float elapsedTime = static_cast<float> (elapsedCount) / 1'000'000.0f;

		// �X�^�[�g�n�_ : start 
		// �G���h�n�_�@ : end
		// �o�ߎ��ԁ@�@ : elapsedTime [s]
		// �ړ������̗�(�o�ߎ���/�S�̎���) : timeRate (%)
		timeRate = min(elapsedTime / maxTime, 1.0f);

		position = lerp(start, end, timeRate);

		// �`�揈��
		ClearDrawScreen();	// ��ʂ�����
		DrawAxis3D(500.0f);	// xyz����\��

		// ���̕`��
		DrawSphere3D(position, 5.0f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);

		//
		DrawFormatString(0, 0, GetColor(255, 255, 255), "position (%5.1f,%5.1f,%5.1f)", position.x, position.y, position.z);
		DrawFormatString(0, 20, GetColor(255, 255, 255), "%7.3f [s]", elapsedTime);
		DrawFormatString(0, 40, GetColor(255, 255, 255), "[R] : Restart");

		//---------  �����܂łɃv���O�������L�q  ---------//
		// (�_�u���o�b�t�@)����
		ScreenFlip();

		// 20�~���b�ҋ@(�^��60FPS)
		WaitTimer(20);

		// Windows�V�X�e�����炭�������������
		if (ProcessMessage() == -1) {
			break;
		}

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dx���C�u�����I������
	DxLib_End();

	// ����I��
	return 0;
}

// �I�[�o�[���[�h�֐�
// �\���ʒu�� �x�N�g��(Vector2) ���w��ł���悤�ɂ���

//int DrawCircle(Vector2 vec, int r, unsigned int color) {
//	return DrawCircle(static_cast<int>(vec.x), static_cast<int>(vec.y), r, color);
//}

int DrawSphere3D(
	const Vector3& CenterPos, const float r, const float DivNum,
	const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag)
{
	VECTOR centerPos = { CenterPos.x,CenterPos.y,CenterPos.z };	// �\���� ���������X�g
	return DrawSphere3D(centerPos, r, DivNum, DifColor, SpcColor, FillFlag);
}

int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int Color)
{
	VECTOR p1 = { Pos1.x, Pos1.y, Pos1.z };
	VECTOR p2 = { Pos2.x, Pos2.y, Pos2.z };
	return DrawLine3D(p1, p2, Color);
}

int SetCameraPositionAndTargetAndUpVec(
	const Vector3& cameraPosition,
	const Vector3& cameraTarget,
	const Vector3& cameraUp
) {
	VECTOR position = { cameraPosition.x , cameraPosition.y , cameraPosition.z };
	VECTOR target = { cameraTarget.x , cameraTarget.y , cameraTarget.z };
	VECTOR up = { cameraUp.x , cameraUp.y , cameraUp.z };

	return SetCameraPositionAndTargetAndUpVec(position, target, up);
}

// x���Ay���Az�� �̕`��
void DrawAxis3D(const float length)
{
	DrawLine3D(Vector3(-length, 0, 0), Vector3(+length, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(Vector3(0, -length, 0), Vector3(0, +length, 0), GetColor(0, 255, 0));
	DrawLine3D(Vector3(0, 0, -length), Vector3(0, 0, +length), GetColor(0, 0, 255));
}