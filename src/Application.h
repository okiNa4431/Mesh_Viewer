#pragma once
#include<Windows.h>
#include<dxgi1_6.h>
#include<DirectXMath.h>
#include<memory>
#include "Dx12Wrapper.h"
#include "renderer.h"
#include<SpriteFont.h>//文字列を表示するのに必要なもの
#include<ResourceUploadBatch.h>//DXTK系列のリソースを使用するのに必要なもの

class Dx12Wrapper;
class renderer;
class mesh;

class Application
{
private: 
	//ウィンドウ関連
	WNDCLASSEX _windowClass;
	HWND _hwnd;
	void CreateGameWindow(HWND& hwnd, WNDCLASSEX& windowClass);//ウィンドウ生成のための設定

	std::shared_ptr<Dx12Wrapper> _dx12;

	//入力デバイス(マウス)
	RAWINPUTDEVICE _mouse;
	void GetMouseDevice(RAWINPUTDEVICE& mouse);

	//ウィンドウ上の文字関連
	DirectX::GraphicsMemory* _geometry = nullptr;
	DirectX::SpriteFont* _spriteFont = nullptr;
	DirectX::SpriteBatch* _spriteBatch = nullptr;
	
	//シングルトンなのでコンストラクタをここに置いておく
	Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

public:
	//シングルトンインスタンスを返す
	static Application& Instance();

	//入力値を送るために仕方なくpublicに設定中
	std::shared_ptr<renderer> _renderer;

	bool Init();
	void Run();
	void Terminate();
	SIZE GetWindowSize()const;
	~Application();
};