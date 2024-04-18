#pragma once
#include<Windows.h>
#include<dxgi1_6.h>
#include<DirectXMath.h>
#include<memory>
#include "Dx12Wrapper.h"
#include "renderer.h"
#include<SpriteFont.h>//�������\������̂ɕK�v�Ȃ���
#include<ResourceUploadBatch.h>//DXTK�n��̃��\�[�X���g�p����̂ɕK�v�Ȃ���

class Dx12Wrapper;
class renderer;
class mesh;

class Application
{
private: 
	//�E�B���h�E�֘A
	WNDCLASSEX _windowClass;
	HWND _hwnd;
	void CreateGameWindow(HWND& hwnd, WNDCLASSEX& windowClass);//�E�B���h�E�����̂��߂̐ݒ�

	std::shared_ptr<Dx12Wrapper> _dx12;

	//���̓f�o�C�X(�}�E�X)
	RAWINPUTDEVICE _mouse;
	void GetMouseDevice(RAWINPUTDEVICE& mouse);

	//�E�B���h�E��̕����֘A
	DirectX::GraphicsMemory* _geometry = nullptr;
	DirectX::SpriteFont* _spriteFont = nullptr;
	DirectX::SpriteBatch* _spriteBatch = nullptr;
	
	//�V���O���g���Ȃ̂ŃR���X�g���N�^�������ɒu���Ă���
	Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

public:
	//�V���O���g���C���X�^���X��Ԃ�
	static Application& Instance();

	//���͒l�𑗂邽�߂Ɏd���Ȃ�public�ɐݒ蒆
	std::shared_ptr<renderer> _renderer;

	bool Init();
	void Run();
	void Terminate();
	SIZE GetWindowSize()const;
	~Application();
};