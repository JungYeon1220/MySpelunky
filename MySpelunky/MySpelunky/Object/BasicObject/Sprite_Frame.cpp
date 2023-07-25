#include "framework.h"
#include "Sprite_Frame.h"

Sprite_Frame::Sprite_Frame(wstring path, Vector2 maxFrame)
	: Sprite(path)
{
	_size = Vector2(_srv.lock()->GetImageSize().x / maxFrame.x, _srv.lock()->GetImageSize().y / maxFrame.y);
	_actionBuffer->_data.size = _size;
	Sprite::CreateVertices();
	Sprite::CreateData(path);
}

Sprite_Frame::Sprite_Frame(wstring path, Vector2 maxFrame, Vector2 size)
	: Sprite(path, Vector2(size.x, size.y))
{
	_actionBuffer->_data.size = Vector2(_actionBuffer->_data.imageSize.x / maxFrame.x, _actionBuffer->_data.imageSize.y / maxFrame.y);
	Sprite::CreateVertices();
	Sprite::CreateData(path);
}

Sprite_Frame::~Sprite_Frame()
{
}

void Sprite_Frame::Update()
{
	Sprite::Update();
}

void Sprite_Frame::Render()
{
	Sprite::Render();
}

void Sprite_Frame::SetCurClip(Vector2 frame)
{
	Vector2 tempSize = _actionBuffer->_data.size;
	_actionBuffer->_data.startPos.x = (frame.x * tempSize.x);
	_actionBuffer->_data.startPos.y = (frame.y * tempSize.y);
}

void Sprite_Frame::SetCurClip(Action::Clip clip)
{
	_actionBuffer->_data.startPos = clip.startPos;
	_actionBuffer->_data.size = clip.size;
}
