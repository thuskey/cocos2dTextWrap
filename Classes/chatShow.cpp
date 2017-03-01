

#include "chatShow.h"
#include "chatData.h"

chatShow::chatShow()
:m_mainNode(nullptr)
,m_pTableView(nullptr)
, m_iCellNum(0)
, m_fx(nullptr)
, m_bIsDragging(false)
{
}

chatShow::~chatShow()
{
   
}

bool chatShow::init()
{
    if (!Layer::init()) {
        return false;
    }
    m_mainNode = CSLoader::createNode("TKChat/chatShow.csb");

	this->addChild(m_mainNode);
    this->initWithDisplay();
    this->addTableView();
    return true;
}


void chatShow::exit()
{
	this->refreshTableView();
}

void chatShow::initWithDisplay()
{
	auto midLayout = static_cast<Layout *>(Helper::seekWidgetByName(m_mainNode, "Panel_Mid"));
	midLayout->setSwallowTouches(false);
}

void chatShow::setHandleDetail(int index)
{
	if (m_bIsDragging){
		return;
	}

	string gola_str;
	auto vec = DATA_CHAT()->getFaceSysbolVec();
	if (index > 0 && index <= vec.size())
	{
		gola_str = vec[index - 1];
	}
	if (m_fx)
	{
		m_fx(gola_str);
	}
}

void chatShow::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
	m_bIsDragging = view->isTouchMoved();
}

void chatShow::scrollViewDidZoom(cocos2d::extension::ScrollView* view){}

Size chatShow::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return m_CellSize;
}

TableViewCell* chatShow::tableCellAtIndex(TableView *table, ssize_t idx)
{
	auto Cell = new TableViewCell();
	Cell->autorelease();
	auto  chatCell = chatShowFace::create();
	chatCell->setClickeFunc(CC_CALLBACK_1(chatShow::setHandleDetail, this));
	chatCell->setTag(TABLE_VIEW_CELL_TAG);
	chatCell->setUpdateIdx(static_cast<int>(idx));
	Cell->addChild(chatCell);
    return Cell;
}

ssize_t chatShow::numberOfCellsInTableView(TableView *table)
{
	int num = (DATA_CHAT()->getMaxFaceSysbolNum() - 1) / 2 + 1;
    return num;
}

void chatShow::addTableView()
{
	auto tableLayout = Helper::seekWidgetByName(m_mainNode, "Panel_Table");
    m_TableSize=tableLayout->getContentSize();
	m_CellSize = Size(86.f,m_TableSize.height);
    m_pTableView=TableView::create(this,m_TableSize);
    m_pTableView->setAnchorPoint(Vec2(0.5, 0.5));
	m_pTableView->ignoreAnchorPointForPosition(false);
    m_pTableView->setPosition(m_TableSize*0.5);
	m_pTableView->setDirection(cocos2d::extension::ScrollView::Direction::HORIZONTAL);
    m_pTableView->setBounceable(true);
    m_pTableView->setTouchEnabled(true);
    tableLayout->addChild(m_pTableView);
    
    this->refreshTableView();
}

void chatShow::refreshTableView()
{
    m_pTableView->reloadData();
}




/************************************************************************/
/*                                                                      */
/************************************************************************/

chatShowFace::chatShowFace()
:m_pRootNode(nullptr)
, m_Fx(nullptr)
{
}

chatShowFace::~chatShowFace(){

}

bool chatShowFace::init(){

	if (!Node::init())
	{
		return false;
	}
	m_pRootNode = CSLoader::createNode("TKChat/chatFaceCell.csb");
	this->addChild(m_pRootNode);

	auto bgLayout = static_cast<Layout *>(Helper::seekWidgetByName(m_pRootNode, "Panel_1"));
	bgLayout->setSwallowTouches(false);
	m_pRootNode->setContentSize(bgLayout->getContentSize());
	m_pRootNode->setPosition(bgLayout->getContentSize()*0.5);

	char buff[BUFFER_SIZE_1] = { 0 };
	for (int i = 1; i <= 2; ++i)
	{
		sprintf(buff, "Image_Face%d", i);
		auto img = static_cast<ImageView *>(Helper::seekWidgetByName(m_pRootNode, buff));
		if (img)
		{
			img->addClickEventListener(CC_CALLBACK_1(chatShowFace::onBtnClicked, this));
			img->setTouchEnabled(true);
			img->setSwallowTouches(false);
		}
	}
	return true;
}

void chatShowFace::setUpdateIdx(int idx)
{
	this->showDetails(idx);
}

void chatShowFace::showDetails(int idx)
{

	char buff[BUFFER_SIZE_1] = { 0 };
	for (int i = 1; i <= 2; ++i)
	{
		sprintf(buff, "Image_Face%d", i);
		auto img = static_cast<ImageView *>(Helper::seekWidgetByName(m_pRootNode, buff));
		if (img)
		{
			int tag = idx * 2 + i;
			img->setTag(tag);
			img->loadTexture(DATA_CHAT()->getChatFaceFile(tag), Widget::TextureResType::PLIST);
			img->setVisible(tag <= DATA_CHAT()->getMaxFaceSysbolNum());
		}
	}
}

void chatShowFace::onBtnClicked(Ref *ref)
{
	auto index = static_cast<ImageView *>(ref)->getTag();
	if (m_Fx){
		m_Fx(index);
	}
}
