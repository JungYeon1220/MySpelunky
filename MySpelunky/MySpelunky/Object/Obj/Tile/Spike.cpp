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

void Spike::InteractPlayer(shared_ptr<class Player> player)
{
	if (_isActive == false)
		return;
	Vector2 playerIdx = MathUtility::GetGridIndex(player->GetCollider()->GetWorldPos());

	if (_index.x > playerIdx.x + 1 || _index.x < playerIdx.x - 1)
		return;
	if (_index.y > playerIdx.y + 1 || _index.y < playerIdx.y - 1)
		return;

	if ((player->GetJumpPower() < 0.0f))
	{
		if (_col->IsCollision(player->GetCollider()) == true)
		{
			if (player->GetCollider()->GetWorldPos().y > _col->GetWorldPos().y)
			{
				if (player->GetCollider()->GetWorldPos().x >= _col->GetWorldPos().x - 50.0f
					&& player->GetCollider()->GetWorldPos().x <= _col->GetWorldPos().x + 50.0f)
				{
					if (_CanSpike == true)
					{
						player->Dead();
						SetBlood();
						Update();
					}

					_CanSpike = false;
				}
			}
		}

		if (_col->IsCollision(player->GetCollider()) == false)
			_CanSpike = true;
	}
}

void Spike::Update()
{
	if (_isActive == false)
		return;
	_col->Update();
	_spikeCol->Update();
}

void Spike::Render()
{
	if (_isActive == false)
		return;
	if (CAMERA->GetViewCollider()->IsCollision(_col) == false)
		return;

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
