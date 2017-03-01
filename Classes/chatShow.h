
#ifndef __chatShow__
#define __chatShow__

#include "publicheaders.h"

/*
聊天表情类
*/
class chatShow:public Layer
,cocos2d::extension::TableViewDataSource
{
public:
    chatShow();
    ~chatShow();
   
	CREATE_FUNC(chatShow);

	void setShowClickFunc(std::function<void(std::string)> fx){
		m_fx=fx;
	}
protected:
	bool init();

    void exit();
    void initWithDisplay();

	void setHandleDetail(int index);

    //////tableView
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view);
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view);
    
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    void addTableView();
    void refreshTableView();
private:
    Node *m_mainNode;
   
    ///tableview
    Size         m_CellSize;
    Size         m_TableSize;
    int          m_iCellNum;
    TableView   *m_pTableView;
    bool         m_bIsDragging;

	std::function<void(std::string)> m_fx;
};

#endif /* defined(__chatShow__) */


class chatShowFace :public Node
{
public:
	chatShowFace();
	~chatShowFace();
	CREATE_FUNC(chatShowFace);
	bool init();

	void setUpdateIdx(int idx);

	void setClickeFunc(std::function<void(int/*index*/)> fx){
		m_Fx = fx;
	}
protected:
	void showDetails(int idx);
	void onBtnClicked(Ref *ref);
private:
	Node              * m_pRootNode;
	std::function<void(int)> m_Fx;
};