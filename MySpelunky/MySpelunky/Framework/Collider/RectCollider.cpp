#include "framework.h"
#include "RectCollider.h"

RectCollider::RectCollider(Vector2 size)
: _size(size)
{
	CreateVertices();
	CreateData();

    _type = Type::RECT;
}

RectCollider::~RectCollider()
{
}

void RectCollider::Update()
{
    _transform->Update();

    _colorBuffer->SetColor(_color);
    _colorBuffer->Update_Resource();
}

void RectCollider::Render()
{
    _transform->SetWorldBuffer(0);
    _colorBuffer->SetPS_Buffer(0);

    _vertexBuffer->SetIA_VertexBuffer();
    _vs->SetIA_InputLayOut();

    DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

    _vs->Set();
    _ps->Set();

    DC->Draw(_vertices.size(),0);
}

void RectCollider::CreateVertices()
{
    Vertex v;

    Vector2 halfSize = _size * 0.5f;

    v.pos = { -halfSize.x, halfSize.y, 0.0f }; // 왼쪽 위
    _vertices.push_back(v);

    v.pos = { halfSize.x, halfSize.y, 0.0f }; // 오른쪽 위
    _vertices.push_back(v);

    v.pos = { halfSize.x, -halfSize.y, 0.0f }; // 오른쪽 아래
    _vertices.push_back(v);

    v.pos = { -halfSize.x, -halfSize.y, 0.0f }; // 왼쪽 아래
    _vertices.push_back(v);

    v.pos = { -halfSize.x, halfSize.y, 0.0f }; // 왼쪽 위
    _vertices.push_back(v);
}

void RectCollider::CreateData()
{
    _vertexBuffer = make_shared<VertexBuffer>(_vertices.data(), sizeof(Vertex), _vertices.size());
    _colorBuffer = make_shared<ColorBuffer>();

    _vs = make_shared<VertexShader>(L"Shader/ColliderVS.hlsl");
    _ps = make_shared<PixelShader>(L"Shader/ColliderPS.hlsl");
}

bool RectCollider::IsCollision(shared_ptr<CircleCollider> col)
{
    AABB_Info info = GetAABB_Info();

    Vector2 circleWorldPos = col->GetWorldPos();

    float left = col->GetWorldPos().x - col->GetWorldRadius();
    float right = col->GetWorldPos().x + col->GetWorldRadius();
    float top = col->GetWorldPos().y + col->GetWorldRadius();
    float bottom = col->GetWorldPos().y - col->GetWorldRadius();

    if (circleWorldPos.x < info.right && circleWorldPos.x > info.left)
    {
        if (bottom < info.top && top > info.bottom)
        {
            return true;
        }
    }

    if (circleWorldPos.y > info.bottom && circleWorldPos.y < info.top)
    {
        if (left < info.right && right > info.left)
        {
            return true;
        }
    }

    if (col->IsCollision(Vector2(info.left, info.top))
        || col->IsCollision(Vector2(info.right, info.top))
        || col->IsCollision(Vector2(info.left, info.bottom))
        || col->IsCollision(Vector2(info.right, info.bottom)))
    {
        return true;
    }

    return false;
}

bool RectCollider::IsCollision(shared_ptr<RectCollider> col)
{
    AABB_Info aInfo = GetAABB_Info();
    AABB_Info bInfo = col->GetAABB_Info();

    if (aInfo.left > bInfo.right || aInfo.right < bInfo.left
        || aInfo.top < bInfo.bottom || aInfo.bottom > bInfo.top)
        return false;

    return true;
}

bool RectCollider::IsOBB(shared_ptr<RectCollider> col)
{
    OBB_Info aInfo = GetOBB_Info();
    OBB_Info bInfo = col->GetOBB_Info();

    Vector2 aToB = aInfo.pos - bInfo.pos;

    Vector2 nea1 = aInfo.direction[0];
    Vector2 nea2 = aInfo.direction[1];
    Vector2 ea1 = nea1 * aInfo.length[0];
    Vector2 ea2 = nea2 * aInfo.length[1];

    Vector2 neb1 = bInfo.direction[0];
    Vector2 neb2 = bInfo.direction[1];
    Vector2 eb1 = neb1 * bInfo.length[0];
    Vector2 eb2 = neb2 * bInfo.length[1];

    // nea1 기준으로 투영
    float length = abs(nea1.Dot(aToB));
    float lengthA = ea1.Length();
    float lengthB = SeparateAxis(nea1, eb1, eb2);

    if (length > lengthA + lengthB)
        return false;

    // nea2 기준으로 투영
    length = abs(nea2.Dot(aToB));
    lengthA = ea2.Length();
    lengthB = SeparateAxis(nea2, eb1, eb2);

    if (length > lengthA + lengthB)
        return false;

    // neb1 기준으로 투영
    length = abs(neb1.Dot(aToB));
    lengthA = eb1.Length();
    lengthB = SeparateAxis(neb1, ea1, ea2);

    if (length > lengthA + lengthB)
        return false;

    // neb2 기준으로 투영
    length = abs(neb2.Dot(aToB));
    lengthA = eb2.Length();
    lengthB = SeparateAxis(neb2, ea1, ea2);

    if (length > lengthA + lengthB)
        return false;

    return true;
}

bool RectCollider::IsOBB(shared_ptr<CircleCollider> col)
{
    OBB_Info info = GetOBB_Info();

    Vector2 circlePos = col->GetTransform()->GetWorldPos();
    float radius = col->GetWorldRadius();

    Vector2 aToB = info.pos - circlePos;

    Vector2 nea1 = info.direction[0];
    Vector2 nea2 = info.direction[1];
    Vector2 ea1 = nea1 * info.length[0];
    Vector2 ea2 = nea2 * info.length[1];

    float vertexLength = (ea1 + ea2).Length();

    if (aToB.Length() > radius + vertexLength)
        return false;

    float length = abs(nea1.Dot(aToB));
    float lengthA = ea1.Length();

    if (length > lengthA + radius)
        return false;

    length = abs(nea2.Dot(aToB));
    lengthA = ea2.Length();

    if (length > lengthA + radius)
        return false;

    return true;
}

float RectCollider::SeparateAxis(Vector2 separate, Vector2 e1, Vector2 e2)
{
    float r1 = abs(separate.Dot(e1));
    float r2 = abs(separate.Dot(e2));
    
    return r1 + r2;
}

bool RectCollider::Block(shared_ptr<RectCollider> col)
{
    if (!IsCollision(col))
        return false;

    AABB_Info infoA = GetAABB_Info();
    AABB_Info infoB = col->GetAABB_Info();

    Vector2 halfSizeA = Vector2(infoA.right - infoA.left, infoA.top - infoA.bottom) * 0.5f;
    Vector2 halfSizeB = Vector2(infoB.right - infoB.left, infoB.top - infoB.bottom) * 0.5f;

    Vector2 dir = col->GetWorldPos() - GetWorldPos();

    Vector2 overlap;
    overlap.x = (halfSizeA.x + halfSizeB.x) - abs(dir.x);
    overlap.y = (halfSizeA.y + halfSizeB.y) - abs(dir.y);

    if (overlap.y > overlap.x)
    {
        Vector2 temp = col->GetWorldPos();
        dir.y = 0;
        dir.Normalize();
        temp.x += dir.x * overlap.x;

        col->GetTransform()->SetPosition(temp);
    }
    else
    {
        Vector2 temp = col->GetWorldPos();
        dir.x = 0;
        dir.Normalize();
        temp.y += dir.y * overlap.y;

        col->GetTransform()->SetPosition(temp);
    }

    return true;
}

bool RectCollider::Block(shared_ptr<CircleCollider> col)
{
    if (!IsCollision(col))
        return false;

    AABB_Info info = GetAABB_Info();
    float radius = col->GetWorldRadius();

    Vector2 halfSize = Vector2(info.right - info.left, info.top - info.bottom) * 0.5f;

    Vector2 dir = col->GetWorldPos() - GetWorldPos();

    Vector2 overlap;
    overlap.x = (halfSize.x + radius) - abs(dir.x);
    overlap.y = (halfSize.y + radius) - abs(dir.y);

    if (overlap.y > overlap.x)
    {
        Vector2 temp = col->GetWorldPos();
        dir.y = 0;
        dir.Normalize();
        temp.x += dir.x * overlap.x;

        col->GetTransform()->SetPosition(temp);
    }
    else
    {
        Vector2 temp = col->GetWorldPos();
        dir.x = 0;
        dir.Normalize();
        temp.y += dir.y * overlap.y;

        col->GetTransform()->SetPosition(temp);
    }

    return true;
}

bool RectCollider::Block(shared_ptr<Collider> col)
{
    switch (col->GetType())
    {
    case Collider::Type::RECT:
        return Block(dynamic_pointer_cast<RectCollider>(col));
    case Collider::Type::CIRCLE:
        return Block(dynamic_pointer_cast<CircleCollider>(col));
    case Collider::Type::NONE:
        return false;
    default:
        return false;
    }
}

RectCollider::AABB_Info RectCollider::GetAABB_Info()
{
    AABB_Info info;
    info.left =     _transform->GetWorldPos().x - _size.x * _transform->GetWorldScale().x * 0.5f;
    info.right =    _transform->GetWorldPos().x + _size.x * _transform->GetWorldScale().x * 0.5f;
    info.top =      _transform->GetWorldPos().y + _size.y * _transform->GetWorldScale().y * 0.5f;
    info.bottom =   _transform->GetWorldPos().y - _size.y * _transform->GetWorldScale().y * 0.5f;

    return info;
}

RectCollider::OBB_Info RectCollider::GetOBB_Info()
{
    OBB_Info info;
    info.pos = _transform->GetWorldPos();

    XMFLOAT4X4 matrix;
    XMStoreFloat4x4(&matrix, _transform->GetSRT());
    info.direction[0] = { matrix._11, matrix._12 };
    info.direction[1] = { matrix._21, matrix._22 };

    info.direction[0].Normalize();
    info.direction[1].Normalize();

    Vector2 halfSize = _size * 0.5f;

    info.length[0] = halfSize.x * _transform->GetWorldScale().x;
    info.length[1] = halfSize.y * _transform->GetWorldScale().y;

    return info;
}

bool RectCollider::IsCollision(const Vector2& pos)
{
    AABB_Info info = GetAABB_Info();

    if (pos.x > info.left && pos.x < info.right)
    {
        if (pos.y > info.bottom && pos.y < info.top)
            return true;
    }
    return false;
}
