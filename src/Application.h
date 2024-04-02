#pragma once
#include<Windows.h>
#include<dxgi1_6.h>
#include<DirectXMath.h>
#include<DirectXTex.h>
#include<memory>
#include "Dx12Wrapper.h"

class Dx12Wrapper;

class Application
{
private: 
	//�E�B���h�E�֘A
	WNDCLASSEX _windowClass;
	HWND _hwnd;
	void CreateGameWindow(HWND& hwnd, WNDCLASSEX& windowClass);//�E�B���h�E�����̂��߂̐ݒ�

	std::shared_ptr<Dx12Wrapper> _dx12;

	//�V���O���g���Ȃ̂ŃR���X�g���N�^�������ɒu���Ă���
	Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

public:
	//�V���O���g���C���X�^���X��Ԃ�
	static Application& Instance();

	bool Init();
	void Run();
	void Terminate();
	SIZE GetWindowSize()const;
	~Application();
};