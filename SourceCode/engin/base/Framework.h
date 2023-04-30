#pragma once

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Audio.h"
#include "Object3d.h"
#include "Fbx_Object3d.h"
#include "Sprite.h"
#include "SceneManager.h"
<<<<<<< HEAD:Framework.h
=======
#include "PostEffect.h"

>>>>>>> ã‚¤ã‚«ã‚¿ã‚³æˆ¦äº‰:SourceCode/engin/base/Framework.h
/// <summary>
/// ƒtƒŒ[ƒ€ƒ[ƒN
/// </summary>
class Framework
{
public:
	/// <summary>
	/// Às
	/// </summary>
	void Run();
	
	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// I—¹
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// –ˆƒtƒŒ[ƒ€XV
	/// </summary>
	virtual void Update();

	/// <summary>
	/// •`‰æ
	/// </summary>
	virtual void Draw();

	virtual bool IsEndRequst() { return endRequst; }

protected:
	bool endRequst = false;
	//ƒ|ƒCƒ“ƒ^’u‚«ê
	WinApp* winApp = nullptr;
	Input* input = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Audio* audio = nullptr;
	DebugText* debugText = nullptr;
<<<<<<< HEAD:Framework.h
	SpriteCommon* spriteCommon = nullptr;
	SceneManager* sceneManager = nullptr;
=======
	SceneManager* sceneManager = nullptr;
	PostEffect* postEffect = nullptr;

>>>>>>> ã‚¤ã‚«ã‚¿ã‚³æˆ¦äº‰:SourceCode/engin/base/Framework.h
};

