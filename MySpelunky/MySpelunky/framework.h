// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <time.h>
#include <tchar.h>
#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <fstream>
#include <algorithm>
#include <wrl/client.h>
#include <functional>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

//DirectXTex
#include "../DXTex/DirectXTex.h"

//ImGui
#include "../imGUI/imgui.h"
#include "../imGUI/imgui_impl_dx11.h"
#include "../imGUI/imgui_impl_win32.h"

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

// Framework
#include "Framework/Device/Device.h"

// Math
#include "Framework/Math/Vector2.h"
#include "Framework/Math/MathUtility.h"
#include "Framework/Math/Transform.h"

// Utility
#include "Framework/Utility/Timer.h"
#include "Framework/Utility/InputManager.h"
#include "Framework/Utility/tinyxml2.h"

// Render
#include "Framework/Render/ConstantBuffer.h"
#include "Framework/Render/GlobalBuffer.h"
#include "Framework/Render/VertexLayout.h"
#include "Framework/Render/VertexBuffer.h"
#include "Framework/Render/IndexBuffer.h"
#include "Framework/Render/Shader.h"
#include "Framework/Render/VertexShader.h"
#include "Framework/Render/PixelShader.h"
#include "Framework/Render/ShaderManager.h"

//TextureMapping
#include "Framework/TextureMapping/SRV.h"
#include "Framework/TextureMapping/SRVManager.h"
#include "Framework/TextureMapping/SamplerState.h"
#include "Framework/TextureMapping/BlendState.h"
#include "Framework/TextureMapping/StateManager.h"

// Collider
#include "Framework/Collider/Collider.h"
#include "Framework/Collider/RectCollider.h"
#include "Framework/Collider/CircleCollider.h"

// Camera
#include "Framework/Camera/Camera.h"

// Action
#include "Framework/Animation/Action.h"

// Obj
#include "Object/BasicObject/Quad.h"
#include "Object/BasicObject/Sprite.h"
#include "Object/BasicObject/Sprite_Clip.h"
#include "Object/BasicObject/Sprite_Frame.h"
#include "Object/BasicObject/Effect.h"
#include "Object/BasicObject/EffectManager.h"

// Spelunky
#include "Object/Obj/Tile/Tile.h"
#include "Object/Obj/Tile/Ladder.h"
#include "Object/Obj/Tile/Movable.h"
#include "Object/Obj/Tile/Normal.h"
#include "Object/Obj/Tile/OneWay.h"
#include "Object/Obj/Tile/Skeleton.h"
#include "Object/Obj/Tile/Spike.h"
#include "Object/Obj/Tile/Unbreakable.h"
#include "Object/Obj/Tile/Wooden.h"

#include "Object/Obj/Map.h"

#include "Object/Obj/Monster/Monster.h"
#include "Object/Obj/Monster/Mosquito.h"
#include "Object/Obj/Monster/Snake.h"
#include "Object/Obj/Monster/Spider.h"
#include "Object/Obj/Monster/MonsterManager.h"

#include "Object/Obj/Item/Item.h"
#include "Object/Obj/Item/Bomb.h"
#include "Object/Obj/Item/Rope.h"
#include "Object/Obj/Item/ItemManager.h"

#include "Object/Obj/Whip.h"
#include "Object/Obj/Player.h"

#include "Object/Obj/SpriteManager.h"

// Scene
#include "Scene/Scene.h"

// Program
#include "Program/Program.h"

#include "Types.h"

extern HWND hWnd;