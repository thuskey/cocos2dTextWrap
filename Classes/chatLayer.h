

#ifndef __chatLayer__
#define __chatLayer__

#include "publicheaders.h"

/*
聊天类
*/
class chatLayer:public Layer
,cocos2d::extension::TableViewDataSource
{
public:
    chatLayer();
    ~chatLayer();

	CREATE_FUNC(chatLayer);

	float  getCellHeight(int index);
protected:
	bool init();
    
    void initWithDisplay();
	void initInputShow();
	//初始化一个（伪）输入节点 节点内容与聊天显示的content一致
	void updateInputShow();
	//根据输入框输入的内容（或表情） 更新（伪）输入节点的内容
	//让人感觉输入框可以输入/删除 表情

	void onSendClicked();
	void onFaceClicked();
	void setWillShowStr(std::string str);
	void onInputClicked(TextField *inputT, TextField::EventType type);

    void messageOnDataUpdate(Ref *ref);
    //////tableView
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view);
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view);
    
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    void addTableView();
    void refreshTableView();
	bool isCanContinueInput();
private:
    Node *m_mainWidget;
	Node *m_assistNode;
	//辅助节点 用来计算每一行聊天内容的高度

    ///tableview
    Size         m_CellSize;
    Size         m_TableSize;
    int          m_iCellNum;
    TableView   *m_pTableView;
    
	float        m_fInputEnterPosX;

	std::pair<int, int>   m_recordPos;
	//记录输入框中 最后一个表情字标识的 起始位置与结束位置
};

#endif /* defined(__chatLayer__) */
