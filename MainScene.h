#pragma once
#include "cocos2d.h"
#include "State.h"

class MainScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MainScene);

private:
	
	// Nodes
	cocos2d::Sprite* ball;
	cocos2d::Label* interactText;
	cocos2d::Label* helperText;
	cocos2d::Label* forTest;
	cocos2d::Layer* interactionLayer;
	cocos2d::PhysicsBody* circleBody;
	cocos2d::Vec2 acc;
	cocos2d::Vec2 accPrev;
	cocos2d::Vec2 handStartAcc;

	// Cocos methods
	void OnAcceleration(cocos2d::Acceleration* acc, cocos2d::Event* _event);
	void interactScreen(cocos2d::Touch* touch, cocos2d::Event* event);
	void handleApp(float dt);
	void handleStates(float dt);

	// Vars
	int counter;
	bool screenInteract;
	int chosenindx;
	std::vector <cocos2d::Vec2> midPositions;

	// Help methods
	void updateText(std::string mainLabel, std::string helperLabel);
	std::string getAnswer(int n);
	std::string to_string(int n);
};