#include "Framework.h"
#include "FbxLoader.h"
#include "Input.h"
#include "ParticleManager.h"
void Framework::Run()
{
	//ƒQ[ƒ€‚Ì‰Šú‰»
	Initialize();

	while (true)  // ƒQ[ƒ€ƒ‹[ƒv
	{
		//–ˆƒtƒŒ[ƒ€XV
		Update();
		Input* input = Input::GetInstance();
		if (input->PushKey(DIK_ESCAPE)) {
			break;
		}

		//I—¹ƒŠƒNƒGƒXƒg‚ª—ˆ‚½‚ç”²‚¯‚é
		if (IsEndRequst()) {
			break;
		}
		//•`‰æ
		Draw();
	}
	// ƒQ[ƒ€‚ÌI—¹
	Finalize();
}

void Framework::Initialize()
{
	//WindowsAPI‚Ì‰Šú‰»
	winApp = new WinApp();
	winApp->Initialize();

	//DirectX‚Ì‰Šú‰»
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);


	// ƒXƒvƒ‰ƒCƒgÃ“I‰Šú‰»
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	ParticleManager::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList());

	//ƒfƒoƒbƒNƒeƒLƒXƒg—p‚ÌƒeƒNƒXƒ`ƒƒ”Ô†‚ğw’è
	const int debugTextTexNumber = 2;
	// ƒfƒoƒbƒOƒeƒLƒXƒg—pƒeƒNƒXƒ`ƒƒ“Ç‚İ‚İ
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return;
	}
	//ƒfƒoƒbƒNƒeƒLƒXƒg
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextTexNumber);

	//“ü—Í‚Ì‰Šú‰»
	input = Input::GetInstance();
	input->Initialize(winApp);


	//ƒI[ƒfƒBƒI‚Ì‰Šú‰»
	/*audio = Audio::GetInstance();
	audio->Initialize();*/

	//3DƒIƒuƒWƒFƒbƒgÃ“I‰Šú‰»
	Object3d::StaticInitialize(dxCommon->GetDev(),dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);
<<<<<<< HEAD:Framework.cpp
=======
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());
>>>>>>> ã‚¤ã‚«ã‚¿ã‚³æˆ¦äº‰:SourceCode/engin/base/Framework.cpp
	//ƒV[ƒ“ƒ}ƒl[ƒMƒƒ[‚Ì¶¬
	this->sceneManager = new SceneManager();
}

void Framework::Finalize()
{
<<<<<<< HEAD:Framework.cpp

=======
	
>>>>>>> ã‚¤ã‚«ã‚¿ã‚³æˆ¦äº‰:SourceCode/engin/base/Framework.cpp
	//ƒV[ƒ“ƒ}ƒl[ƒWƒƒ[‰ğ•ú
	delete sceneManager;
	//ƒVƒ“ƒOƒ‹ƒgƒ“‚É‚µ‚½‚çdeleteƒ_ƒ
	//DirectX‰ğ•ú
	delete dxCommon;
	//WindowsAPI‚ÌI—¹ˆ—
	winApp->Finalize();
	//WindowsAPI‰ğ•ú
	delete winApp;
	//FBX‚Ì‰ğ•ú
	FbxLoader::GetInstance()->Finalize();

}

void Framework::Update()
{
#pragma region ƒEƒBƒ“ƒhƒEƒƒbƒZ[ƒWˆ—
	if (winApp->ProcessMessage()) {
		//ƒQ[ƒ€ƒ‹[ƒv‚ğ”²‚¯‚é
		this->endRequst = true;
		return;
	}
	//“ü—Í‚ÌXV
	input->Update();
	/// <summary>
	/// ƒV[ƒ“‚ÌXV
	/// </summary>
	this->sceneManager->Update(dxCommon);
}

void Framework::Draw()
{
<<<<<<< HEAD:Framework.cpp
	//•`‰æ‘Oˆ—
	dxCommon->PreDraw();

	//ƒV[ƒ“•`‰æ
	this->sceneManager->Draw(dxCommon);

	//ƒfƒoƒbƒNƒeƒLƒXƒg•`‰æ
	debugText->DrawAll();

	//•`‰æŒãˆ—
	dxCommon->PostDraw();
=======
	//ƒV[ƒ“•`‰æ
	this->sceneManager->Draw(dxCommon);
	//ƒfƒoƒbƒNƒeƒLƒXƒg•`‰æ
	//debugText->DrawAll();
>>>>>>> ã‚¤ã‚«ã‚¿ã‚³æˆ¦äº‰:SourceCode/engin/base/Framework.cpp
}
