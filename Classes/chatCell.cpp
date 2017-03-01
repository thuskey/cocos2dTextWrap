

#include "chatCell.h"
#include "chatData.h"
#include "globalfunc.h"

const  float sp_w_ratio = 0.7f;
chatCell::chatCell()
:m_pRootNode(nullptr)
{
}

chatCell::~chatCell(){

}

bool chatCell::init(){
	m_pRootNode = Node::create();
	m_pRootNode->setContentSize(Size(CHAT_CONTENT_WIDTH, CHAT_DESIGN_HEIGHT));
	m_pRootNode->setAnchorPoint(Vec2::ZERO);
	m_pRootNode->ignoreAnchorPointForPosition(false);
	m_pRootNode->setPosition(Vec2::ZERO);
	this->setCascadeOpacityEnabled(true);
	m_pRootNode->setCascadeOpacityEnabled(true);
	this->addChild(m_pRootNode);

	auto infoNode = Node::create();
	infoNode->setPosition(Vec2(0, 90));
	infoNode->setName("Node_Info");
	infoNode->setCascadeOpacityEnabled(true);
	m_pRootNode->addChild(infoNode);

	Label *nameLbl = Label::createWithTTF("00", QJ_FONT_1, CHAT_DETAIL_FONT_SIZE);
	nameLbl->enableShadow();
	nameLbl->setAnchorPoint(Vec2(0, 0.5));
	nameLbl->setPosition(Vec2(10, 19));
	infoNode->addChild(nameLbl);
	nameLbl->setName("Label_Name");

	Label *lvLbl = Label::createWithTTF("Lv.99", QJ_FONT_1, CHAT_DETAIL_FONT_SIZE);
	lvLbl->setAnchorPoint(Vec2(0, .5));
	lvLbl->enableShadow();
	lvLbl->setPosition(Vec2(231, nameLbl->getPositionY()));
	infoNode->addChild(lvLbl);
	lvLbl->setName("Label_Lv");
    return true;
}

void chatCell::setUpdateIdx(int idx)
{
    this->showDetails(idx);
}

void chatCell::showDetails(int idx)
{
	auto vec = DATA_CHAT()->getChatData();
	if (idx >= vec.size()){
		return;
	}
	char buff[BUFFER_SIZE_1] = { 0 };
	auto infoBg = m_pRootNode->getChildByName("Node_Info");

	auto nameLbl = static_cast<Label *>(infoBg->getChildByName("Label_Name"));
	auto lvLbl = static_cast<Label *>(infoBg->getChildByName("Label_Lv"));
	auto &infoData = vec[idx];
	nameLbl->setString(infoData.send_name);
	infoBg->setTag(idx);

	lvLbl->setString("Lv." + globalfunc::intToString(infoData.role_lv));
	lvLbl->setPositionX(nameLbl->getPositionX() + nameLbl->getContentSize().width + 30);


	vector<chatData::FaceData> faceVec;
	faceVec = DATA_CHAT()->getHandleChatVec()[idx];
	int num = DATA_CHAT()->getChatDetailsNum(m_pRootNode, CHAT_CONTENT_WIDTH, faceVec, sp_w_ratio, CHAT_DETAIL_FONT_SIZE);
	//********
	m_pRootNode->setContentSize(Size(CHAT_CONTENT_WIDTH, CHAT_DESIGN_HEIGHT));
	if (num>1){
		m_pRootNode->setContentSize(Size(CHAT_CONTENT_WIDTH, CHAT_DESIGN_HEIGHT + 35 * (num - 1)));
	}
	m_pRootNode->setPosition(Vec2::ZERO);
	
	infoBg->setPositionY(m_pRootNode->getContentSize().height - 35);
	//********
	DATA_CHAT()->handleShowChatDetail(m_pRootNode, 20, infoBg->getPositionY() - 25, CHAT_CONTENT_WIDTH, faceVec, sp_w_ratio, 35, CHAT_DETAIL_FONT_SIZE);
}