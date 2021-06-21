#include "GameScene.h"

// �v���O������ android_main ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//������
	if (DxLib_Init() == -1) {
		return -1;
	}
	//��ʃT�C�Y�ݒ�
	SetGraphMode(ANDROID_WIDTH, ANDROID_HEIGHT, 32);
	//�`���𗠉�ʂɐݒ�
	SetDrawScreen(DX_SCREEN_BACK);

	//�Q�[������
	SceneManager scene(make_shared<GameScene>());
	while (ProcessMessage() == 0) {
		scene.Update();
		scene.Draw();
	}

	//�I������
	DxLib_End();
	return 0;
}