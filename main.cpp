#include"DxLib.h"
#include <math.h>
// カメラの回転速度
#define CAMERA_ANGLE_SPEED		3.0f

// カメラの注視点の高さ
#define CAMERA_LOOK_AT_HEIGHT		400.0f
// カメラと注視点の距離
#define CAMERA_LOOK_AT_DISTANCE		2150.0f
// 移動速度
#define MOVESPEED			10.0f
// ラインを描く範囲
#define LINE_AREA_SIZE			10000.0f

// ラインの数
#define LINE_NUM			50
// カメラと注視点の距離
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    VECTOR Position;
    int    MoveFlag;
    float  Angle;
    float  CameraHAngle;
    float  CameraVAngle;
    VECTOR MoveVector;
    float  SinParam;
    float  CosParam;

    //window mode
    ChangeWindowMode(TRUE);

    // ＤＸライブラリの初期化
    DxLib_Init();

    SetMainWindowText("□チーム制作");


    //ウィンドウサイズ
    SetGraphMode(1920,1080, 32);
    // カメラの向きを初期化
    CameraHAngle = 0.0f;
    CameraVAngle = 0.0f;


    // ３Ｄモデルの座標を初期化
    Position = VGet(0.0f, 0.0f, 0.0f);

    //背景変更
    SetDrawScreen(DX_SCREEN_BACK);

    // カメラのクリッピング距離を設定
    SetCameraNearFar(100.0f, 50000.0f);

    // ＥＳＣキーが押されるか、ウインドウが閉じられるまでループ
    while (ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE))
    {

        // 画面をクリア
        ClearDrawScreen();
        // ZCSXキーでカメラの操作
        if (CheckHitKey(KEY_INPUT_D) == 1)
        {
            CameraHAngle += CAMERA_ANGLE_SPEED;
            if (CameraHAngle >= 180.0f)
            {
                CameraHAngle -= 360.0f;
            }
        }

        if (CheckHitKey(KEY_INPUT_A) == 1)
        {
            CameraHAngle -= CAMERA_ANGLE_SPEED;
            if (CameraHAngle <= -180.0f)
            {
                CameraHAngle += 360.0f;
            }
        }

        if (CheckHitKey(KEY_INPUT_W) == 1)
        {
            CameraVAngle += CAMERA_ANGLE_SPEED;
            if (CameraVAngle >= 80.0f)
            {
                CameraVAngle = 80.0f;
            }
        }

        if (CheckHitKey(KEY_INPUT_S) == 1)
        {
            CameraVAngle -= CAMERA_ANGLE_SPEED;
            if (CameraVAngle <= 0.0f)
            {
                CameraVAngle = 0.0f;
            }
        }
        // カメラの位置と向きを設定
        {
            VECTOR TempPosition1;
            VECTOR TempPosition2;
            VECTOR CameraPosition;
            VECTOR CameraLookAtPosition;

            // 注視点はキャラクターモデルの座標から CAMERA_LOOK_AT_HEIGHT 分だけ高い位置
            CameraLookAtPosition = Position;
            CameraLookAtPosition.y += CAMERA_LOOK_AT_HEIGHT;

            // カメラの位置はカメラの水平角度と垂直角度から算出

            // 最初に垂直角度を反映した位置を算出
            SinParam = sin(CameraVAngle / 180.0f * DX_PI_F);
            CosParam = cos(CameraVAngle / 180.0f * DX_PI_F);
            TempPosition1.x = 0.0f;
            TempPosition1.y = SinParam * CAMERA_LOOK_AT_DISTANCE;
            TempPosition1.z = -CosParam * CAMERA_LOOK_AT_DISTANCE;

            // 次に水平角度を反映した位置を算出
            SinParam = sin(CameraHAngle / 180.0f * DX_PI_F);
            CosParam = cos(CameraHAngle / 180.0f * DX_PI_F);
            TempPosition2.x = CosParam * TempPosition1.x - SinParam * TempPosition1.z;
            TempPosition2.y = TempPosition1.y;
            TempPosition2.z = SinParam * TempPosition1.x + CosParam * TempPosition1.z;

            // 算出した座標に注視点の位置を加算したものがカメラの位置
            CameraPosition = VAdd(TempPosition2, CameraLookAtPosition);

            // カメラの設定に反映する
            SetCameraPositionAndTarget_UpVecY(CameraPosition, CameraLookAtPosition);
        }
        // 位置関係が分かるように地面にラインを描画する
        {
            int i;
            VECTOR Pos1;
            VECTOR Pos2;

            SetUseZBufferFlag(TRUE);

            Pos1 = VGet(-LINE_AREA_SIZE / 2.0f, 0.0f, -LINE_AREA_SIZE / 2.0f);
            Pos2 = VGet(-LINE_AREA_SIZE / 2.0f, 0.0f, LINE_AREA_SIZE / 2.0f);
            for (i = 0; i <= LINE_NUM; i++)
            {
                DrawLine3D(Pos1, Pos2, GetColor(0,128, 0));
                Pos1.x += LINE_AREA_SIZE / LINE_NUM;
                Pos2.x += LINE_AREA_SIZE / LINE_NUM;
            }

            Pos1 = VGet(-LINE_AREA_SIZE / 2.0f, 0.0f, -LINE_AREA_SIZE / 2.0f);
            Pos2 = VGet(LINE_AREA_SIZE / 2.0f, 0.0f, -LINE_AREA_SIZE / 2.0f);
            for (i = 0; i < LINE_NUM; i++)
            {
                DrawLine3D(Pos1, Pos2, GetColor(0, 128, 0));
                Pos1.z += LINE_AREA_SIZE / LINE_NUM;
                Pos2.z += LINE_AREA_SIZE / LINE_NUM;
            }

            SetUseZBufferFlag(FALSE);
        }

                // 裏画面の内容を表画面に反映
                ScreenFlip();
    }

    // ライブラリの後始末
    DxLib_End();

    // ソフトの終了
    return 0;
}
