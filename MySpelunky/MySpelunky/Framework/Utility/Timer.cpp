#include "framework.h"
#include "Timer.h"

Timer* Timer::_instance = nullptr;
Timer::Timer()
{
	// 1초에 몇번 진동하는지
	QueryPerformanceFrequency((LARGE_INTEGER*) &_periodFrequency);

	// 프로그램이 시작할 때 지금까지 CPU의 진동수
	QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);

	_timeScale = 1.0f / (float)_periodFrequency;
}

Timer::~Timer()
{
}

void Timer::Update()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&_curDamageTime);
	_deltaTime = (float)(_curDamageTime - _lastTime) * _timeScale;

	if (_lockFPS != 0)
	{
		while (_deltaTime < (1.0 / _lockFPS))
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&_curDamageTime);
			_deltaTime = (float)(_curDamageTime - _lastTime) * _timeScale;
		}
	}

	_lastTime = _curDamageTime;

	_frameCount++;
	_oneSecCount += _deltaTime;

	if (_oneSecCount >= 1)
	{
		_frameRate = _frameCount;
		_oneSecCount = 0;
		_frameCount = 0;
	}

	_runTime += _deltaTime;
}
