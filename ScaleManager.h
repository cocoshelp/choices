#pragma once

#include "cocos2d.h"

USING_NS_CC;

class ScaleManager
{
public:
	static ScaleManager* Instance()
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

#define GETSCALEX ( ScaleManager::Instance()->getScaleX() )
#define GETSCALEY ( ScaleManager::Instance()->getScaleY() )
#define GETSCALEFAC ( ScaleManager::Instance()->getScaleFactor() )

#define SCALEX(p) ( (p) * GETSCALEX )
#define SCALEY(p) ( (p) * GETSCALEY )
#define SCALE(p) ( (p) * GETSCALEFAC )

#define SCALEDPOS(x,y) ( Vec2 (  GETSCALEX * (x), GETSCALEY * (y)) )

#define SCALEFONT(p) ( (p) * SCALEY * GETSCALEFAC )

#define SCALENODE(n) \
	n->setScaleX(GETSCALEX); \
	n->setScaleY(GETSCALEY)

#define SCALEBYRADIUS(n) \
	n->setScaleX(GETSCALEX); \
	n->setScaleY(GETSCALEX)


