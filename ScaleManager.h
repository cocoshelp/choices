#pragma once

#include "cocos2d.h"

USING_NS_CC;

class ScaleManager
{
public:
	static ScaleManager* getInstance()
	{
		static ScaleManager instance;
		return &instance;
	}

	void SetUpScaleFactors();
	float getScaleX() { return _scaleX; }
	float getScaleY() { return _scaleX; }
	float getScaleFactor() { return _scaleX; }

private:

	float _scaleX;
	float _scaleY;
	float _scalFactor;
};

#define GETSCALEX ( ScaleManager::getInstance()->getScaleX() )
#define GETSCALEY ( ScaleManager::getInstance()->getScaleY() )
#define GETSCALEFAC ( ScaleManager::getInstance()->getScaleFactor() )

#define SCALENODE(n) \
	n->setScaleX(GETSCALEX); \
	n->setScaleY(GETSCALEY)

#define SCALEBYRADIUS(n) \
	n->setScaleX(GETSCALEFAC); \
	n->setScaleY(GETSCALEFAC)


