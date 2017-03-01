
#ifndef __chatData__
#define __chatData__

#include "publicheaders.h"

#define REC_MSG_CHAT_UPDATE               "rec_msg_chat_update"

#define DATA_CHAT()                       (chatData::getInstance())
#define CHAT_DETAIL_FONT_SIZE             24
#define CHAT_CONTENT_WIDTH                580
#define CHAT_DESIGN_HEIGHT                100
class chatData 
{
public:
	chatData();
	~chatData();

	struct ChatData{
		int send_role_id = 0;
		int role_lv = 0;
		string content = "";
		string send_name = "";
	};

	struct FaceData{
		bool is_face = false;
		int face_id = 0;
		std::string content = "";
	};

	static chatData *getInstance();
	void reqChat(ChatData &info);
private:
	vector<string>       m_faceForChatVec;
	std::deque<ChatData> m_chatData;

	std::deque<vector<FaceData>> m_handledChatData;
public:
    void handleChatStr(string pStr, float fontSize, float fRowWidth, vector<FaceData> &faceVec);
	//将聊天中包含的表情解析出来 聊天内容转化成 FaceData 数据 
	void chatFaceSymbolPosVec(string str, vector<int> &beginVec, vector<int> &endVec);
	//找出str 中每个表情标识的起始位置/结束位置
	int getChatDetailsNum(cocos2d::Node *parentNode, float max_show_w, vector<FaceData> &faceVec, float sp_w_ratio, int fontSize);
	//计算每条聊天内容 显示的行数
	int handleShowChatDetail(cocos2d::Node *parentNode, float basic_posx, float up_pos_y, float max_show_w, vector<FaceData> &faceVec, float sp_w_ratio, float row_dis, int fontSize);
	//图文混排

	int  getMaxFaceSysbolNum();
	string getChatFaceFile(int index);

	const std::deque<ChatData> &getChatData(){
		return m_chatData;
	}

	const vector<string>& getFaceSysbolVec(){
		return m_faceForChatVec;
	}

	const std::deque<vector<FaceData>>&getHandleChatVec(){
		return m_handledChatData;
	}
};
#endif