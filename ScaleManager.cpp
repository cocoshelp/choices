#include "ScaleManager.h"
#include "cocos2d.h"
#include <cmath>

USING_NS_CC;


static cocos2d::Size designResolutionSize = cocos2d::Size(490, 840);

void ScaleManager::SetUpScaleFactors()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	_scaleX = visibleSize.width / designResolutionSize.width;
	_scaleY = visibleSize.height / designResolutionSize.height;

	if (abs(visibleSize.height - designResolutionSize.height) > abs(visibleSize.width - designResolutionSize.width))
	{
		_scalFactor = _scaleX;
	}
	else
	{
		_scalFactor = _scaleY;
	}
}