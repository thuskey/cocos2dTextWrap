
#ifndef __chatCell__
#define __chatCell__

#include "publicheaders.h"

class chatCell:public Node
{
public:
    chatCell();
    ~chatCell();
    CREATE_FUNC(chatCell);
    bool init();
    
    void setUpdateIdx(int idx);

	Node *getRootNode(){
		return m_pRootNode;
	}
protected:
    void showDetails(int idx);
	void onBtnClicked(Ref *ref);
private:
    Node              * m_pRootNode;
};

#endif /* defined(__ProjectTank__chatCell__) */
