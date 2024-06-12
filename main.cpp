#include"DxLib.h"
#include <math.h>
// �J�����̉�]���x
#define CAMERA_ANGLE_SPEED		3.0f

// �J�����̒����_�̍���
#define CAMERA_LOOK_AT_HEIGHT		400.0f
// �J�����ƒ����_�̋���
#define CAMERA_LOOK_AT_DISTANCE		2150.0f
// �ړ����x
#define MOVESPEED			10.0f
// ���C����`���͈�
#define LINE_AREA_SIZE			10000.0f

// ���C���̐�
#define LINE_NUM			50
// �J�����ƒ����_�̋���
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

    // �c�w���C�u�����̏�����
    DxLib_Init();

    SetMainWindowText("���`�[������");


    //�E�B���h�E�T�C�Y
    SetGraphMode(1920,1080, 32);
    // �J�����̌�����������
    CameraHAngle = 0.0f;
    CameraVAngle = 0.0f;


    // �R�c���f���̍��W��������
    Position = VGet(0.0f, 0.0f, 0.0f);

    //�w�i�ύX
    SetDrawScreen(DX_SCREEN_BACK);

    // �J�����̃N���b�s���O������ݒ�
    SetCameraNearFar(100.0f, 50000.0f);

    // �d�r�b�L�[��������邩�A�E�C���h�E��������܂Ń��[�v
    while (ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE))
    {

        // ��ʂ��N���A
        ClearDrawScreen();
        // ZCSX�L�[�ŃJ�����̑���
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
        // �J�����̈ʒu�ƌ�����ݒ�
        {
            VECTOR TempPosition1;
            VECTOR TempPosition2;
            VECTOR CameraPosition;
            VECTOR CameraLookAtPosition;

            // �����_�̓L�����N�^�[���f���̍��W���� CAMERA_LOOK_AT_HEIGHT �����������ʒu
            CameraLookAtPosition = Position;
            CameraLookAtPosition.y += CAMERA_LOOK_AT_HEIGHT;

            // �J�����̈ʒu�̓J�����̐����p�x�Ɛ����p�x����Z�o

            // �ŏ��ɐ����p�x�𔽉f�����ʒu���Z�o
            SinParam = sin(CameraVAngle / 180.0f * DX_PI_F);
            CosParam = cos(CameraVAngle / 180.0f * DX_PI_F);
            TempPosition1.x = 0.0f;
            TempPosition1.y = SinParam * CAMERA_LOOK_AT_DISTANCE;
            TempPosition1.z = -CosParam * CAMERA_LOOK_AT_DISTANCE;

            // ���ɐ����p�x�𔽉f�����ʒu���Z�o
            SinParam = sin(CameraHAngle / 180.0f * DX_PI_F);
            CosParam = cos(CameraHAngle / 180.0f * DX_PI_F);
            TempPosition2.x = CosParam * TempPosition1.x - SinParam * TempPosition1.z;
            TempPosition2.y = TempPosition1.y;
            TempPosition2.z = SinParam * TempPosition1.x + CosParam * TempPosition1.z;

            // �Z�o�������W�ɒ����_�̈ʒu�����Z�������̂��J�����̈ʒu
            CameraPosition = VAdd(TempPosition2, CameraLookAtPosition);

            // �J�����̐ݒ�ɔ��f����
            SetCameraPositionAndTarget_UpVecY(CameraPosition, CameraLookAtPosition);
        }
        // �ʒu�֌W��������悤�ɒn�ʂɃ��C����`�悷��
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

                // ����ʂ̓��e��\��ʂɔ��f
                ScreenFlip();
    }

    // ���C�u�����̌�n��
    DxLib_End();

    // �\�t�g�̏I��
    return 0;
}
