#include "DxLib.h"
#include "Global.h"
#include "Vector3.h"

// 関数プロトタイプ宣言
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

void DrawAxis3D(const float length);	// x, y, z 軸の描画

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	// 画面サイズの最大サイズ、カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// 画面の背景色を設定する
	SetBackgroundColor(0, 0, 64);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// Zバッファを有効にする
	SetUseZBuffer3D(TRUE);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(TRUE);

	// クリップ面( 近, 遠 )
	SetCameraNearFar(1.0f, 1000.0f);	// カメラの有効範囲を設定
	SetCameraScreenCenter(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0);	// 画面の中心をカメラの中心に合わせる
	SetCameraPositionAndTargetAndUpVec(
		Vector3(0.0f, 0.0f, -120.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f)
	);

	// 時間計測に必要なデータ
	long long startCount = 0;
	long long nowConst = 0;
	long long elapsedCount = 0;

	// 補間で使うデータ
	// start -> end を 5 [s] で完了させる
	Vector3 start(-100.0f, 0, 0);	// スタート地点
	Vector3 end(+100.0f, 0, 0);		// エンド地点
	float maxTime = 5.0f;			// 全体時間 [s]
	float timeRate;					// 何%時間が進んだか(率)

	// 球の位置
	Vector3 position;

	// 実行前にカウンタ値を取得
	startCount = GetNowHiPerformanceCount();	// long long int型 64Bit int


	// 最新のキーボード情報用
	char keys[256] = { 0 };

	// 1ループ(フレーム)前のキーボード情報
	char oldkeys[256] = { 0 };

	// ゲームループ
	while (true) {
		// 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
		// 最新のキーボード情報を取得
		GetHitKeyStateAll(keys);

		// 画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		// 更新処理

		// [R]キーでリスタート
		if (CheckHitKey(KEY_INPUT_R))
		{
			startCount = GetNowHiPerformanceCount();
		}
		// 経過時間(elapsedTime [s]) の時間
		nowConst = GetNowHiPerformanceCount();
		elapsedCount = nowConst - startCount;
		float elapsedTime = static_cast<float> (elapsedCount) / 1'000'000.0f;

		// スタート地点 : start 
		// エンド地点　 : end
		// 経過時間　　 : elapsedTime [s]
		// 移動完了の率(経過時間/全体時間) : timeRate (%)
		timeRate = min(elapsedTime / maxTime, 1.0f);

		position = lerp(start, end, timeRate);

		// 描画処理
		ClearDrawScreen();	// 画面を消去
		DrawAxis3D(500.0f);	// xyz軸を表示

		// 球の描画
		DrawSphere3D(position, 5.0f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);

		//
		DrawFormatString(0, 0, GetColor(255, 255, 255), "position (%5.1f,%5.1f,%5.1f)", position.x, position.y, position.z);
		DrawFormatString(0, 20, GetColor(255, 255, 255), "%7.3f [s]", elapsedTime);
		DrawFormatString(0, 40, GetColor(255, 255, 255), "[R] : Restart");

		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();

		// 20ミリ秒待機(疑似60FPS)
		WaitTimer(20);

		// Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1) {
			break;
		}

		// ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
}

// オーバーロード関数
// 表示位置に ベクトル(Vector2) を指定できるようにする

//int DrawCircle(Vector2 vec, int r, unsigned int color) {
//	return DrawCircle(static_cast<int>(vec.x), static_cast<int>(vec.y), r, color);
//}

int DrawSphere3D(
	const Vector3& CenterPos, const float r, const float DivNum,
	const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag)
{
	VECTOR centerPos = { CenterPos.x,CenterPos.y,CenterPos.z };	// 構造体 初期化リスト
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

// x軸、y軸、z軸 の描画
void DrawAxis3D(const float length)
{
	DrawLine3D(Vector3(-length, 0, 0), Vector3(+length, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(Vector3(0, -length, 0), Vector3(0, +length, 0), GetColor(0, 255, 0));
	DrawLine3D(Vector3(0, 0, -length), Vector3(0, 0, +length), GetColor(0, 0, 255));
}