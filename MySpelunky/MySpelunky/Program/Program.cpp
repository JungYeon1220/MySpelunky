#include "framework.h"
#include "Program.h"

#include "../Scene/BasicScene/TutorialScene.h"
#include "../Scene/BasicScene/TileMapScene.h"

Program::Program()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	_curScene = make_shared<TileMapScene>();

	Timer::GetInstance()->LockFPS(60);
}

Program::~Program()
{
}

void Program::Update()
{
	Timer::GetInstance()->Update();
	InputManager::GetInstance()->Update();
	CAMERA->Update();
	_curScene->Update();
}

void Program::Render()
{
	Device::GetInstance()->Clear();

	CAMERA->SetViewBuffer();
	CAMERA->SetProjectionBuffer();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ALPHA->SetState();
	_curScene->Render();

	ImGui::Text("FPS : %d", Timer::GetInstance()->GetFPS());
	//ImGui::Text("DeltaTime : %1f", Timer::GetInstance()->GetDeltaTime());
	//ImGui::Text("RunTime : %1f", Timer::GetInstance()->GetRunTime());
	ImGui::Text("mousePos X: %f, Y: %f", WIN_MOUSE_POS.x, WIN_MOUSE_POS.y);
	CAMERA->PostRender();

	CAMERA->SetUIViewBuffer();
	_curScene->PostRender();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	Device::GetInstance()->Present();
}
