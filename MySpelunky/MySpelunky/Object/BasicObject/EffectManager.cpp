#include "framework.h"
#include "EffectManager.h"

EffectManager* EffectManager::_instance = nullptr;
EffectManager::EffectManager()
{
	AddEffect("Explosion", L"Resource/Texture/fx_explosion.png", Vector2(4, 4), Vector2(400.0f, 400.0f));
}

EffectManager::~EffectManager()
{
}

void EffectManager::Update()
{
	for (auto pair : _effectTable)
	{
		for (auto effect : pair.second)
			effect->Update();
	}
}

void EffectManager::Render()
{
	for (auto pair : _effectTable)
	{
		for (auto effect : pair.second)
			effect->Render();
	}
}

void EffectManager::AddEffect(string name, wstring file, Vector2 maxFrame, Vector2 size, float speed, Action::Type type)
{
	if (_effectTable.count(name) != 0)
		return;

	for (int i = 0; i < _poolCount; i++)
	{
		shared_ptr<Effect> effect = make_shared<Effect>(name, file, maxFrame, size, speed, type);
		_effectTable[name].push_back(effect);
	}
}

void EffectManager::Play(string name, Vector2 pos)
{
	if (_effectTable.count(name) == 0)
		return;

	for (auto effect : _effectTable[name])
	{
		if (effect->_isActive == false)
		{
			effect->Play(pos);
			return;
		}
	}

}
