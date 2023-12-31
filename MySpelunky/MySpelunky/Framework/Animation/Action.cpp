#include "framework.h"
#include "Action.h"

Action::Action(vector<Clip> clips, string name, Type type, float speed)
	: _clips(clips)
	, _name(name)
	, _repeatType(type)
	, _curSpeed(speed)
{

}

Action::~Action()
{
}

void Action::Update()
{
	if (_isPlay == false)
		return;

	_time += DELTA_TIME;

	if (_time > _curSpeed)
	{
		_time = 0.0f;

		switch (_repeatType)
		{
		case Action::END:
		{
			_curClipIndex++;

			if (_curClipIndex == 4 && _fourIndexEvent != nullptr)
				_fourIndexEvent();

			if (_curClipIndex >= _clips.size())
				Stop();
		}
		break;
		case Action::LOOP:
		{
			if (_isReverse == true)
			{
				if (_curClipIndex <= 0)
					_curClipIndex = _clips.size() - 1;
				_curClipIndex--;
			}
			else
			{
				_curClipIndex++;
				_curClipIndex %= _clips.size();
			}
		}
		break;
		case Action::PINGPONG:
		{
			if (_isReverse)
			{
				_curClipIndex--;
				if (_curClipIndex <= 0)
					_isReverse = false;
			}
			else
			{
				_curClipIndex++;
				if (_curClipIndex >= _clips.size() - 1)
					_isReverse = true;
			}
		}
		break;
		default:
			break;
		}
	}
}

void Action::Play()
{
	_isPlay = true;
	_time = 0.0f;
}

void Action::Pause()
{
	_isPlay = false;
}

void Action::Resume()
{
	_isPlay = true;
}

void Action::Stop()
{
	_isPlay = false;
	_time = 0.0f;
	_curClipIndex = 0;

	if (_endEvent != nullptr)
		_endEvent();
}

void Action::Reset()
{
	_isPlay = false;
	_curClipIndex = 0;
	_time = 0.0f;
}
