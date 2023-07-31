#include "framework.h"
#include "Spike.h"

Spike::Spike()
    :Tile()
{
    _sprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
    int random = rand() % 3;
    _sprite->SetCurClip(Vector2(5 + random, 9));
    _type = Tile::Type::SPIKE;

    _spikeCol = make_shared<RectCollider>(Vector2(100.0f, 90.0f));
    _spikeCol->GetTransform()->SetParent(_col->GetTransform());
    _spikeCol->GetTransform()->SetPosition(Vector2(0.0f, -5.0f));

    _bloodSprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
    _bloodSprite->SetCurClip(Vector2(5 + random, 10));
}

Spike::Spike(Vector2 pos)
    :Tile(pos)
{
    _sprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
    int random = rand() % 3;
    _sprite->SetCurClip(Vector2(5 + random, 9));
    _type = Tile::Type::SPIKE;

    _spikeCol = make_shared<RectCollider>(Vector2(100.0f, 90.0f));
    _spikeCol->GetTransform()->SetParent(_col->GetTransform());
    _spikeCol->GetTransform()->SetPosition(Vector2(0.0f, -5.0f));

    _bloodSprite = make_shared<Sprite_Frame>(L"Resource/Texture/floor_cave.png", Vector2(12, 12), Vector2(100.0f, 100.0f));
    _bloodSprite->SetCurClip(Vector2(5 + random, 10));
}

Spike::~Spike()
{
}

bool Spike::Block(shared_ptr<Collider> col)
{
    return false;
}

void Spike::Update()
{
    Tile::Update();
    if (_isBlood == true)
    {
        _bloodSprite->Update();
    }
}

void Spike::Render()
{
    Tile::Render();
    if (_isBlood == true)
    {
        _bloodSprite->Render();
    }
}

void Spike::SetSkeletonSpike()
{
    _isSkeleton = true;
    _sprite->SetCurClip(Vector2(8, 9));
    _bloodSprite->SetCurClip(Vector2(5, 10));
}
