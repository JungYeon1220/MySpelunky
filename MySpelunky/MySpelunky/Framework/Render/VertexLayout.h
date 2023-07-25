#pragma once

struct Vertex
{
    XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
};

struct Vertex_Texture
{
    XMFLOAT3 pos;
    XMFLOAT4 color;
    XMFLOAT2 uv;
};
