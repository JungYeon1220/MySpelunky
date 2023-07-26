#include "framework.h"
#include "Timer.h"

Timer* Timer::_instance = nullptr;
Timer::Timer()
{
	// 1�ʿ� ��� �����ϴ���
	QueryPerformanceFrequency((LARGE_INTEGER*) &_periodFrequency);

	// ���α׷��� ������ �� ���ݱ��� CPU�� ������
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