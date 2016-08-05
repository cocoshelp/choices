#include "MainScene.h"
#include "ScaleManager.h"
#include "State.h"
#include <vector>
#include <string>
#include <math.h> 

USING_NS_CC;

#define TILTSECONDS 6
#define HANDLESECONDS 0.1f
#define BORDER 1000


Scene* MainScene::createScene() 
{
	auto scene = Scene::createWithPhysics();

	auto layer = MainScene::create();

	scene->addChild(layer);

	return scene;
}

bool MainScene::init() 
{
	if (!Layer::init()) {
		return false;
	}

	Device::setAccelerometerEnabled(true);
	Device::setKeepScreenOn(true);

	auto accListener = EventListenerAcceleration::create(CC_CALLBACK_2(MainScene::OnAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(accListener, this);

	// Back button handler
	auto touchListener = EventListenerKeyboard::create();
	touchListener->onKeyReleased = [](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
		{
			Director::getInstance()->end();
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto choiseWidth = visibleSize.width / 2;
	auto choiseHeight = visibleSize.height / 4;

	interactionLayer = LayerColor::create(Color4B(197, 31, 36, 255), visibleSize.width, choiseHeight);
	interactionLayer->setAnchorPoint(Vec2(0,0));
	interactionLayer->setPosition(0, 0);
	addChild(interactionLayer);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [](Touch* touch, Event* event) { return true; };
	listener->onTouchMoved = [](Touch* touch, Event* event) {};
	listener->onTouchEnded = CC_CALLBACK_2(MainScene::interactScreen, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, interactionLayer);

	auto blue = LayerColor::create(Color4B(0, 63, 106, 255), choiseWidth, choiseHeight);
	blue->setAnchorPoint(Vec2(0, 0));
	blue->setPosition(0, choiseHeight);
	addChild(blue);

	auto green = LayerColor::create(Color4B(151, 189, 61, 255), choiseWidth, choiseHeight);
	green->setAnchorPoint(Vec2(0, 0));
	green->setPosition(choiseWidth, choiseHeight);
	addChild(green);

	auto orange = LayerColor::create(Color4B(240, 171, 0, 255), choiseWidth, choiseHeight);
	orange->setAnchorPoint(Vec2(0, 0));
	orange->setPosition(0, 2 * choiseHeight);
	addChild(orange);

	auto purple = LayerColor::create(Color4B(125, 0, 99, 255), choiseWidth, choiseHeight);
	purple->setAnchorPoint(Vec2(0, 0));
	purple->setPosition(choiseWidth, 2 * choiseHeight);
	addChild(purple);

	auto sky = LayerColor::create(Color4B(0, 156, 204, 255), choiseWidth, choiseHeight);
	sky->setAnchorPoint(Vec2(0, 0));
	sky->setPosition(0, 3 * choiseHeight);
	addChild(sky);

	auto yellow = LayerColor::create(Color4B(252, 217, 0, 255), choiseWidth, choiseHeight);
	yellow->setAnchorPoint(Vec2(0, 0));
	yellow->setPosition(choiseWidth, 3 * choiseHeight);
	addChild(yellow);

	midPositions.reserve(6);
	midPositions.push_back(Vec2(blue->getBoundingBox().getMidX(), blue->getBoundingBox().getMidY()));
	midPositions.push_back(Vec2(green->getBoundingBox().getMidX(), green->getBoundingBox().getMidY()));
	midPositions.push_back(Vec2(orange->getBoundingBox().getMidX(), orange->getBoundingBox().getMidY()));
	midPositions.push_back(Vec2(purple->getBoundingBox().getMidX(), purple->getBoundingBox().getMidY()));
	midPositions.push_back(Vec2(sky->getBoundingBox().getMidX(), sky->getBoundingBox().getMidY()));
	midPositions.push_back(Vec2(yellow->getBoundingBox().getMidX(), yellow->getBoundingBox().getMidY()));


	// Creating the screen edge
	auto edgeBody = PhysicsBody::createEdgeBox(Size(visibleSize.width + BORDER * 2, choiseHeight * 3 + BORDER * 2), PhysicsMaterial(100, 0.4, 10), BORDER);
	auto edgeNode = Node::create();
	edgeNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + choiseHeight / 2));
	edgeNode->setPhysicsBody(edgeBody);
	addChild(edgeNode);

	ball = Sprite::create("ball.png");
	ball->setPosition(0, 0);
	ball->setPosition(choiseWidth, 2 * choiseHeight);
	circleBody = PhysicsBody::createCircle(ball->getContentSize().height / 2, PhysicsMaterial(0, 1, 1));
	circleBody->setGravityEnable(false);
	ball->setPhysicsBody(circleBody);
	addChild(ball);
	SCALEBYRADIUS(ball);

	this->schedule(schedule_selector(MainScene::handleApp), 0.05f);
	this->schedule(schedule_selector(MainScene::handleStates), HANDLESECONDS);

	// Creating the interaction text with the user
	counter = 0;
	interactText = Label::createWithSystemFont("", "Ariel", 36);
	addChild(interactText);
	interactText->setPosition(choiseWidth, choiseHeight / 2);
	SCALENODE(interactText);

	helperText = Label::createWithSystemFont("", "Ariel", 20);
	addChild(helperText);
	helperText->setPosition(visibleSize.width - 15, 10);
	helperText->setAnchorPoint(Vec2(1, 0));
	SCALENODE(helperText);

	return true;
}

void MainScene::OnAcceleration(cocos2d::Acceleration *_acc, cocos2d::Event *_event) 
{
	// If the current state of the app is not tilt do not geather info
	if (State::getInstance()->getState() != StatesInfo::Tilt)
	{
		return;
	}

	acc = Vec2(_acc->x * 2000, _acc->y * 2000);

	// Setting the starting point 
	if (accPrev == Vec2())
	{
		accPrev = acc;
	}
}

void MainScene::interactScreen(cocos2d::Touch *touch, cocos2d::Event *event)
{	
	auto _state = State::getInstance()->getState();

	if (_state == StatesInfo::Start || 
		_state == StatesInfo::Question || 
		_state == StatesInfo::Finish)
	{
		screenInteract = true;
	}
}

void MainScene::handleStates(float dt) 
{
	auto _state = State::getInstance()->getState();

	switch (_state)
	{
	case StatesInfo::Begin:
	{
		updateText("Welcome to the Choices app", "Tap on screen");
		State::getInstance()->setState(StatesInfo::Start);
	}break;
	case StatesInfo::Start:
	{
		chosenindx = -1;
		if (screenInteract)
		{
			updateText("Think about some question \nyou wish to get answer...", "Tap on screen and start to tilt the device");
			screenInteract = false;
			State::getInstance()->setState(StatesInfo::Question);
		}
	}break;
	case StatesInfo::Question:
	{
		if (screenInteract)
		{
			// Setting counter for tilting
			counter = TILTSECONDS / HANDLESECONDS;
			screenInteract = false;
			State::getInstance()->setState(StatesInfo::Tilt);
		}
	}break;
	case StatesInfo::Tilt:
	{
		counter--;
		if (counter % static_cast<int>(ceilf( 1 / HANDLESECONDS)) == 0)
		{
			updateText(to_string( static_cast<int>(ceilf(counter * HANDLESECONDS))), "Tilt the device");
		}
		//circleBody->applyImpulse(Vec2(2000, 3000));
		if (counter < 0)
		{
			State::getInstance()->setState(StatesInfo::Wait);
			updateText("Wait until the ball stops", "");
		}
	}break;
	case StatesInfo::Wait:
	{
		auto velocity = abs(circleBody->getAngularVelocity());
		if (velocity < 1.5f)
		{
			State::getInstance()->setState(StatesInfo::Answer);
		}
	}break;
	case StatesInfo::Answer: 
	{
		runAction(Sequence::create(
			DelayTime::create(1.0f),
			CallFunc::create([&]()
		{
			State::getInstance()->setState(StatesInfo::Finish);
			updateText(getAnswer(chosenindx), "Tap to ask another question");
		}),
			nullptr));
	}break;
	case StatesInfo::Finish:
	{
		if (screenInteract)
		{
			State::getInstance()->setState(StatesInfo::Start);
		}
	}break;
	default:
		break;
	}
}

void MainScene::handleApp(float dt)
{
	auto _state = State::getInstance()->getState();

	if (_state == StatesInfo::Tilt)
	{
		circleBody->applyImpulse(Vec2(acc.x - accPrev.x, acc.y - accPrev.y));
		accPrev = acc;
		return;
	}

	if (_state != StatesInfo::Answer || chosenindx != -1)
	{
		return;
	}

	accPrev = Vec2();

	int nearIndex = 0;
	ball->setAnchorPoint(Vec2(0.5, 0.5));
	auto circlePosition = ball->getPosition();
	for (size_t i = 1; i < midPositions.size(); i++)
	{
		auto nearPos = midPositions.at(nearIndex);
		auto candidatePos = midPositions.at(i);
		auto nearValue = sqrt(pow(nearPos.x - circlePosition.x, 2) + pow(nearPos.y - circlePosition.y, 2));
		auto candidateValue = sqrt(pow(candidatePos.x - circlePosition.x, 2) + pow(candidatePos.y - circlePosition.y, 2));
		if (nearValue > candidateValue)
		{
			nearIndex = i;
		}
	}

	chosenindx = nearIndex;

	ball->runAction(MoveTo::create(0.5f, midPositions.at(nearIndex)));

	circleBody->setVelocity(Vec2());
}

void MainScene::updateText(std::string mainLabel, std::string helperLabel) 
{
	interactText->setString(mainLabel);
	helperText->setString(helperLabel);
}

std::string MainScene::to_string(int n)
{
	std::ostringstream ss;
	ss << n;
	return ss.str();
}

std::string MainScene::getAnswer(int n) 
{
	switch (n)
	{
	case 0: return "I believe you \nknow the answer";
	case 1: return "Get deep in your soul \nand figure it out";
	case 2: return "Sometimes the needs of the \nother becomes before your needs";
	case 3: return "Think about something that \naffected your life for answer";
	case 4: return "Yes";
	default:
		return "No";
	}
}