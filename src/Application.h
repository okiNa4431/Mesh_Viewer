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
	//ウィンドウ関連
	WNDCLASSEX _windowClass;
	HWND _hwnd;
	void CreateGameWindow(HWND& hwnd, WNDCLASSEX& windowClass);//ウィンドウ生成のための設定

	std::shared_ptr<Dx12Wrapper> _dx12;

	//シングルトンなのでコンストラクタをここに置いておく
	Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

public:
	//シングルトンインスタンスを返す
	static Application& Instance();

	bool Init();
	void Run();
	void Terminate();
	SIZE GetWindowSize()const;
	~Application();
};