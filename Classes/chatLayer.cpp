
#include "chatLayer.h"
#include "chatData.h"
#include "chatShow.h"
#include "chatCell.h"
#include "globalfunc.h"
#define INPUT_MAX_SHOW_LEN     450

chatLayer::chatLayer()
:m_mainWidget(nullptr)
, m_pTableView(nullptr), m_assistNode(nullptr)
, m_recordPos(std::make_pair(0,0))
{
    m_iCellNum=0;
	m_fInputEnterPosX = 0;
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("TKChat/chatFace.plist");
}

chatLayer::~chatLayer()
{
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

bool chatLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    __NotificationCenter::getInstance()->addObserver(this, (SEL_CallFuncO)
		(&chatLayer::messageOnDataUpdate), REC_MSG_CHAT_UPDATE, NULL);
	
    m_mainWidget = CSLoader::createNode("TKChat/chatLayer.csb");
    this->addChild(m_mainWidget);
    
    this->initWithDisplay();
	this->initInputShow();
    this->addTableView();
    return true;
}

void chatLayer::initWithDisplay()
{
	auto sendBtn = static_cast<Button *>(Helper::seekWidgetByName(m_mainWidget, "Button_Send"));
	sendBtn->addClickEventListener(CC_CALLBACK_0(chatLayer::onSendClicked, this));

	auto faceBtn = static_cast<Button *>(Helper::seekWidgetByName(m_mainWidget, "Button_Face"));
	faceBtn->addClickEventListener(CC_CALLBACK_0(chatLayer::onFaceClicked, this));

	TextField *inputText = static_cast<TextField *>(Helper::seekWidgetByName(m_mainWidget, "TextField_InPut"));
	inputText->addEventListener(std::bind(&chatLayer::onInputClicked, this, inputText, std::placeholders::_2));
	inputText->setTextVerticalAlignment(TextVAlignment::CENTER);
	inputText->setPlaceHolder("click here input");
	inputText->setTouchSize(Size(inputText->getContentSize().width, inputText->getContentSize().height*0.2));
	m_fInputEnterPosX = inputText->getPositionX();

	auto pLayer = chatCell::create();
	this->addChild(pLayer);
	pLayer->setVisible(false);
	m_assistNode = pLayer->getRootNode();

	Text *lenT = static_cast<Text *>(Helper::seekWidgetByName(m_mainWidget, "Text_ForLen"));
	lenT->setVisible(false);

}

void chatLayer::initInputShow()
{
	auto node = Helper::seekWidgetByName(m_mainWidget, "Node_Contain");
	if (!node){
		return;
	}
	
	char buff[BUFFER_SIZE_1] = { 0 };
	for (int i = 1; i <= 50; ++i){
		Label *lbl = Label::createWithTTF("00", QJ_FONT_1, CHAT_DETAIL_FONT_SIZE);
		lbl->enableShadow();
		lbl->setAnchorPoint(Vec2(0, 0.5));
		node->addChild(lbl);
		sprintf(buff, "Label_Detail%d", i);
		lbl->setName(buff);
		lbl->setHorizontalAlignment(TextHAlignment::CENTER);
		lbl->setVisible(false);
	}

	for (int i = 1; i <= 50; ++i)
	{
		auto faceSp = Sprite::createWithSpriteFrameName(DATA_CHAT()->getChatFaceFile(1));
		faceSp->setVisible(false);
		sprintf(buff, "Sprite_Face%d", i);
		faceSp->setName(buff);
		faceSp->setScale(0.75);
		node->addChild(faceSp);
		faceSp->setVisible(false);
	}
}

void chatLayer::updateInputShow()
{
	auto parentNode = Helper::seekWidgetByName(m_mainWidget, "Node_Contain");
	if (!parentNode){
		return;
	}
	vector<chatData::FaceData> faceVec;
	TextField *inputText = static_cast<TextField *>(Helper::seekWidgetByName(m_mainWidget, "TextField_InPut"));
	string str = inputText->getString();
	if (str.size()>0)
	{
		inputText->setOpacity(0);
	} else{
		inputText->setOpacity(255);
	}
	
	float max_show_w = 0xffff;
	float sp_w_ratio = 0.8;
	DATA_CHAT()->handleChatStr(str, CHAT_DETAIL_FONT_SIZE, max_show_w, faceVec);
	
	//-------
	vector<int> beginVec, endVec,faceBeginVec,faceEndVec;
	DATA_CHAT()->chatFaceSymbolPosVec(str, faceBeginVec, faceEndVec);
	m_recordPos.first = 0;
	m_recordPos.second = 0;
	if (beginVec.size()==endVec.size()&&beginVec.size()>0)
	{
		m_recordPos.first = beginVec[beginVec.size() - 1];
		m_recordPos.second = endVec[endVec.size() - 1];
	}
	if (faceBeginVec.size()==faceEndVec.size()&&faceBeginVec.size()>0)
	{
		if (m_recordPos.second<faceEndVec[faceEndVec.size()-1])
		{
			m_recordPos.first = faceBeginVec[faceBeginVec.size() - 1];
			m_recordPos.second = faceEndVec[faceEndVec.size() - 1];
		}
	}
	//-----
	char buff[BUFFER_SIZE_1] = { 0 };
	int lbl_num = 0;
	int face_num = 0;
	float sum_w = 0.0f;
	for (int i = 0; i < faceVec.size(); ++i)
	{
		auto info = faceVec[i];
		if (info.is_face)
		{
			++face_num;
			sprintf(buff, "Sprite_Face%d", face_num);
			auto sp = static_cast<Sprite *>(parentNode->getChildByName(buff));
			sp->setVisible(true);
			sp->initWithSpriteFrameName(DATA_CHAT()->getChatFaceFile(info.face_id));
			float w = sp->getContentSize().width*sp->getScale()*sp_w_ratio;
			sum_w += w;
			sp->setPositionX(sum_w - w*0.5);
		} else{
			++lbl_num;
			sprintf(buff, "Label_Detail%d", lbl_num);
			auto detailLbl = static_cast<Label *>(parentNode->getChildByName(buff));
			detailLbl->setVisible(true);
			detailLbl->setString(info.content);
			
			float w = detailLbl->getContentSize().width;
			sum_w += w;
			detailLbl->setPositionX(sum_w - w);
		}
	}
	if (sum_w > INPUT_MAX_SHOW_LEN){
		parentNode->setPositionX(m_fInputEnterPosX - (sum_w - INPUT_MAX_SHOW_LEN));
	} else{
		parentNode->setPositionX(m_fInputEnterPosX);
	}
	//----------------------------------
	for (int i = lbl_num + 1; true; ++i){
		sprintf(buff, "Label_Detail%d", i);
		auto detailLbl = static_cast<Label *>(parentNode->getChildByName(buff));
		if (detailLbl)
		{
			detailLbl->setVisible(false);
		} else{
			break;
		}
	}

	for (int k = face_num + 1; true; ++k)
	{
		sprintf(buff, "Sprite_Face%d", k);
		auto sp = static_cast<Sprite *>(parentNode->getChildByName(buff));
		if (sp){
			sp->setVisible(false);
		} else{
			break;
		}
	}
}

void chatLayer::onSendClicked()
{
	auto pLayer = this->getChildByName("TKChatShow__");
	if (pLayer){
		pLayer->removeFromParent();
	}

	TextField *inputText = static_cast<TextField *>(Helper::seekWidgetByName(m_mainWidget, "TextField_InPut"));
	if (!inputText->getString().empty()){
		chatData::ChatData info;
		info.role_lv = 66;
		info.send_name = "qinj";
		info.content = inputText->getString();
		DATA_CHAT()->reqChat(info);

		inputText->setString("");
	}
	this->updateInputShow();
}

void chatLayer::onFaceClicked()
{
	auto pLayer = this->getChildByName("TKChatShow__");
	if (pLayer){
		pLayer->removeFromParent();
	} else{
		pLayer = chatShow::create();
		((chatShow *)pLayer)->setShowClickFunc(CC_CALLBACK_1(chatLayer::setWillShowStr, this));
		this->addChild(pLayer);
		pLayer->setName("TKChatShow__");
	}
}

void chatLayer::setWillShowStr(std::string str)
{
	TextField *inputText = static_cast<TextField *>(Helper::seekWidgetByName(m_mainWidget, "TextField_InPut"));
	int num = inputText->getString().size() + str.size();
	if (!this->isCanContinueInput())
	{
		return;
	}
	
	inputText->setString(inputText->getString() + str);
	this->updateInputShow();
}

void chatLayer::onInputClicked(TextField *inputT, TextField::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
		break;
	case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
		break;
	case cocos2d::ui::TextField::EventType::INSERT_TEXT:
		if (!inputT->getString().empty())
		{
			if (!this->isCanContinueInput())
			{
				return;
			}
			
			Text *lenT = static_cast<Text *>(Helper::seekWidgetByName(m_mainWidget, "Text_ForLen"));			
			if (lenT->getContentSize().width>INPUT_MAX_SHOW_LEN){
				inputT->setPositionX(m_fInputEnterPosX - (lenT->getContentSize().width - INPUT_MAX_SHOW_LEN));
			} else{
				inputT->setPositionX(m_fInputEnterPosX);
			}
			this->updateInputShow();
		}
		break;
	case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
	{
		   Text *lenT = static_cast<Text *>(Helper::seekWidgetByName(m_mainWidget, "Text_ForLen"));
		   string str = inputT->getString();
		   lenT->setString(str);
		   if (lenT->getContentSize().width >INPUT_MAX_SHOW_LEN){
			   inputT->setPositionX(m_fInputEnterPosX - (lenT->getContentSize().width - INPUT_MAX_SHOW_LEN));
		   } else{
			 inputT->setPositionX(m_fInputEnterPosX);
		  }
		  if (str.size()<=m_recordPos.second)
		  {
			  str = str.substr(0, m_recordPos.first);
		  }
		  inputT->setString(str);
		  this->updateInputShow();
	}
		break;
	default:
		break;
	}
}

void chatLayer::messageOnDataUpdate(Ref *ref)
{
    this->refreshTableView();
}

void chatLayer::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
}

void chatLayer::scrollViewDidZoom(cocos2d::extension::ScrollView* view){}


Size chatLayer::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(m_CellSize.width,this->getCellHeight(idx));
}

TableViewCell* chatLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell * Cell = table->dequeueCell();
    if (!Cell) {
        Cell=new TableViewCell();
        Cell->autorelease();
		auto cCell = chatCell::create();
		if (cCell != NULL) {
			cCell->setCascadeOpacityEnabled(true);
			cCell->setTag(TABLE_VIEW_CELL_TAG);
			cCell->setUpdateIdx(static_cast<int>(idx));
			Cell->addChild(cCell);
		}
    }else {
		auto  cCell = (chatCell *)Cell->getChildByTag(TABLE_VIEW_CELL_TAG);
		cCell->setUpdateIdx(static_cast<int>(idx));
    }
    return Cell;
}

ssize_t chatLayer::numberOfCellsInTableView(TableView *table)
{
	m_iCellNum = DATA_CHAT()->getChatData().size();
	return m_iCellNum;
}

void chatLayer::addTableView()
{
	auto tableLayout = Helper::seekWidgetByName(m_mainWidget, "Panel_Table");
    m_TableSize=tableLayout->getContentSize();
    m_CellSize=Size(m_TableSize.width, 130);
    m_pTableView=TableView::create(this,m_TableSize);
    m_pTableView->setAnchorPoint(Vec2(0.5, 0.5));
	m_pTableView->ignoreAnchorPointForPosition(false);
    m_pTableView->setPosition(m_TableSize*0.5);
    m_pTableView->setBounceable(true);
    tableLayout->addChild(m_pTableView);
	m_pTableView->setCascadeOpacityEnabled(true);

    this->refreshTableView();
}

void chatLayer::refreshTableView()
{
	m_pTableView->reloadData();
}

float chatLayer::getCellHeight(int index)
{
	float height = CHAT_DESIGN_HEIGHT;
	float rowDis = 35;
	auto infoVec = DATA_CHAT()->getHandleChatVec();
	if (infoVec.size() < index)
	{
		return 0.f;
	}
	auto faceVec = infoVec[index];
	int rowNum = DATA_CHAT()->getChatDetailsNum(m_assistNode, CHAT_CONTENT_WIDTH, faceVec, 0.7, CHAT_DETAIL_FONT_SIZE);
	height += (rowNum - 1)*rowDis;
	return height;
}

bool chatLayer::isCanContinueInput()
{
	Text *lenT = static_cast<Text *>(Helper::seekWidgetByName(m_mainWidget, "Text_ForLen"));
	if (lenT->getContentSize().width > 1000.f)
	{
		globalfunc::errorTip("too long can't input");
		return false;
	}
	return true;
}
