#include "framework.h"
#include "Spike.h"

Spike::Spike()
    :Tile()
{
	int random = MathUtility::RandomInt(1, 3);
	_name = "Spike" + to_string(random);
	_type = Tile::Type::SPIKE;

	_spikeCol = make_shared<RectCollider>(Vector2(100.0f, 90.0f));
	_spikeCol->GetTransform()->SetParent(_col->GetTransform());
	_spikeCol->GetTransform()->SetPosition(Vector2(0.0f, -5.0f));

	_bloodName = "Blood" + to_string(random);
}

Spike::Spike(Vector2 pos)
    :Tile(pos)
{
	int random = MathUtility::RandomInt(1, 3);
	_name = "Spike" + to_string(random);
	_type = Tile::Type::SPIKE;

	_spikeCol = make_shared<RectCollider>(Vector2(100.0f, 90.0f));
	_spikeCol->GetTransform()->SetParent(_col->GetTransform());
	_spikeCol->GetTransform()->SetPosition(Vector2(0.0f, -5.0f));

	_bloodName = "Blood" + to_string(random);
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
	_col->Update();
	_spikeCol->Update();
}

void Spike::Render()
{
	_transform->SetPosition(Vector2(0.0f, 0.0f));
	_transform->Update();
	_transform->SetWorldBuffer(0);
	SPRITEMANAGER->Render("Cave", _name);

	if (_isBlood == true)
	{
		SPRITEMANAGER->Render("Cave", _bloodName);
	}
}

void Spike::SetSkeletonSpike()
{
	_isSkeleton = true;
	_name = "SkeletonSpike";
	_bloodName = "SkeletonBlood";
}
