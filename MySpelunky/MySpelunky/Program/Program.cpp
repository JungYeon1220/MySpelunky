#include "framework.h"
#include "Program.h"

#include "../Scene/BasicScene/TutorialScene.h"
#include "../Scene/BasicScene/TileMapScene.h"
#include "../Scene/BasicScene/TileTestScene.h"

Program::Program()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	FONT->Add("D2Coding", L"D2Coding");

	SOUND->Add("BGM", "Resource/Sound/BGM_Basecamp_B.ogg", true);
	SOUND->Add("Explosion", "Resource/Sound/explosion_d01.wav");
	SOUND->Add("SnakeAttack", "Resource/Sound/snake_atk.wav");
	SOUND->Add("SnakeDie", "Resource/Sound/snake_die01.wav");
	SOUND->Add("Spider", "Resource/Sound/spider01.wav");

	_curScene = make_shared<TileTestScene>();

	Timer::GetInstance()->LockFPS(60);
}

Program::~Program()
{
	Font::Delete();
}

void Program::Update()
{
	Timer::GetInstance()->Update();
	InputManager::GetInstance()->Update();
	CAMERA->Update();
	_curScene->Update();
	EFFECT->Update();
}

void Program::Render()
{
	Device::GetInstance()->Clear();

	CAMERA->SetViewBuffer();
	CAMERA->SetProjectionBuffer();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	FONT->GetDC()->BeginDraw();

	ALPHA->SetState();
	_curScene->Render();
	EFFECT->Render();

	ImGui::Text("FPS : %d", Timer::GetInstance()->GetFPS());
	//ImGui::Text("DeltaTime : %1f", Timer::GetInstance()->GetDeltaTime());
	//ImGui::Text("RunTime : %1f", Timer::GetInstance()->GetRunTime());
	ImGui::Text("mousePos X: %f, Y: %f", WIN_MOUSE_POS.x, WIN_MOUSE_POS.y);
	CAMERA->PostRender();

	FONT->GetDC()->EndDraw();

	CAMERA->SetUIViewBuffer();
	_curScene->PostRender();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	Device::GetInstance()->Present();
}
