#include "HelloWorldScene.h"
#include "chatLayer.h"

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	m_actionFinished = true;

	auto node = CSLoader::createNode("TKChat/MainScene.csb");
	this->addChild(node);

	auto bgLayout = static_cast<Layout *>(Helper::seekWidgetByName(node,"Panel_1"));
	bgLayout->addClickEventListener(CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
	
	this->menuCloseCallback(nullptr);
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
	if (!m_actionFinished){
		return;
	}
	m_actionFinished = false;
	auto pLayer = this->getChildByName("chatLayer__");
	if (!pLayer)
	{
		pLayer = chatLayer::create();
		pLayer->setName("chatLayer__");
		this->addChild(pLayer);

		pLayer->setPositionX(-640);
		auto func = [=]{
			m_actionFinished = true;
		};
		pLayer->runAction(Sequence::create(MoveTo::create(0.5, Vec2(0, 0)),
			CallFunc::create(func), nullptr));
	} else{
		auto func = [=]{
			m_actionFinished = true;
			pLayer->removeFromParent();
		};
		pLayer->runAction(Sequence::create(MoveTo::create(0.5, Vec2(-640, 0)),
			CallFunc::create(func), nullptr));
	}
}
