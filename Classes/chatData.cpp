
#include "chatData.h"
#include "globalfunc.h"

const int face_symbol_len = 4;

static chatData *instance = nullptr;
chatData *chatData::getInstance()
{
	if (!instance){
		instance = new chatData();
	}
	return instance;
}

chatData::chatData()
{
	//格式化表情标识
	m_faceForChatVec.clear();
	char buff[BUFFER_SIZE_1] = { 0 };
	for (int i = 1; i <= this->getMaxFaceSysbolNum();++i)
	{
		if (i<10)
		{
			sprintf(buff, "#0%d#", i);
		} else{
			sprintf(buff, "#%d#", i);
		}
		m_faceForChatVec.push_back(buff);
	}
}

chatData::~chatData()
{

}

void chatData::reqChat(ChatData &info)
{
	vector<FaceData> faceVec;
	
	//处理聊天内容
	DATA_CHAT()->handleChatStr(info.content, CHAT_DETAIL_FONT_SIZE, CHAT_CONTENT_WIDTH, faceVec);

	m_chatData.push_back(info);
	m_handledChatData.push_back(faceVec);
	__NotificationCenter::getInstance()->postNotification(REC_MSG_CHAT_UPDATE);
}

int chatData::getMaxFaceSysbolNum()
{
	return 30;
}

void chatData::handleChatStr(string pStr, float fontSize, float fRowWidth, vector<FaceData> &faceVec)
{
	Label * pLabel = Label::createWithTTF("", QJ_FONT_1, fontSize);
	string  _strForFace = pStr;
	if (_strForFace.size() < face_symbol_len)
	{
		FaceData info;
		info.is_face = false;
		info.content = _strForFace;
		faceVec.push_back(info);
		return;
	}
	int iLastCutLocation = 0;
	///search face symbol
	const auto &tmpData = m_faceForChatVec;
	for (int i = 0; i < (_strForFace.size() - face_symbol_len + 1); ++i){
		int count = 1;
		for (auto it_s = tmpData.begin(); it_s != tmpData.end(); ++it_s, ++count){
			if (strcmp((*it_s).c_str(), _strForFace.substr(i, face_symbol_len).c_str()) == 0) {
				//---------------
				FaceData info;

				string tmp_str = _strForFace.substr(iLastCutLocation, i - iLastCutLocation);
				auto strVec = globalfunc::getCutOutString(tmp_str, QJ_FONT_1, fontSize, fRowWidth);
				//限制content的字符串长度
				for (auto aa : strVec){
					info.is_face = false;
					info.content = aa;
					faceVec.push_back(info);
				}

				info.is_face = true;
				info.face_id = count;
				info.content = "";
				faceVec.push_back(info);
				
				iLastCutLocation = i+face_symbol_len;
				i += face_symbol_len-1;
				break;
			}
		}
	}
	if (iLastCutLocation!=_strForFace.size()-1)
	{
		FaceData info;
		string tmp_str = _strForFace.substr(iLastCutLocation, _strForFace.size() - iLastCutLocation);
		auto strVec = globalfunc::getCutOutString(tmp_str, QJ_FONT_1, fontSize, fRowWidth);
		for (auto aa : strVec){
			info.is_face = false;
			info.content = aa;
			faceVec.push_back(info);
		}
	}
}

void chatData::chatFaceSymbolPosVec(string str, vector<int> &beginVec, vector<int> &endVec)
{
	if (str.size() < face_symbol_len)
	{
		return;
	}

	auto tmpData = DATA_CHAT()->getFaceSysbolVec();
	for (int i = 0; i < (str.size() - face_symbol_len + 1); ++i){
		int count = 1;
		for (auto it_s = tmpData.begin(); it_s != tmpData.end(); ++it_s, ++count){
			if (strcmp((*it_s).c_str(), str.substr(i, face_symbol_len).c_str()) == 0) {
				beginVec.push_back(i);
				endVec.push_back(i + face_symbol_len - 1);
				i += face_symbol_len - 1;
				break;
			}
		}
	}
}

string chatData::getChatFaceFile(int index)
{
	index = MAX(MIN(this->getMaxFaceSysbolNum(), index), 1);
	char buff[BUFFER_SIZE_1] = { 0 };
	sprintf(buff, "TKChat/face/icon_bq%d.png", index);
	return buff;
}

int  chatData::getChatDetailsNum(Node *parentNode, float max_show_w, vector<FaceData> &faceVec, float sp_w_ratio, int fontSize)
{
	if (parentNode)
	{
		return DATA_CHAT()->handleShowChatDetail(parentNode, 0, 0, max_show_w, faceVec, sp_w_ratio, 0,fontSize);
	}
	return 0;
}

int chatData::handleShowChatDetail(Node *parentNode, float basic_posx, float up_pos_y, float max_show_w, vector<FaceData> &faceVec, float sp_w_ratio, float row_dis, int fontSize)
{
	char buff[BUFFER_SIZE_1] = { 0 };
	int lbl_num = 0;
	int face_num = 0;
	float sum_w = 0.0f;
	int row_index = 1;
	for (int i = 0; i < faceVec.size(); ++i)
	{
		auto info = faceVec[i];
		if (info.is_face)
		{
			++face_num;
			sprintf(buff, "Sprite_Face%d", face_num);
			auto sp = static_cast<Sprite *>(parentNode->getChildByName(buff));
			if (!sp)
			{
				sp = Sprite::createWithSpriteFrameName(DATA_CHAT()->getChatFaceFile(info.face_id));
				sp->setName(buff);
				sp->setScale(0.75*0.75);
				parentNode->addChild(sp);
			} else{
				sp->initWithSpriteFrameName(DATA_CHAT()->getChatFaceFile(info.face_id));
			}

			sp->setVisible(true);

			float w = sp->getContentSize().width*sp->getScale()*sp_w_ratio;
			if (sum_w + w > max_show_w)
			{
				//换行
				++row_index;
				sum_w = w;
			} else{
				sum_w += w;
			}
			sp->setPosition(Vec2(basic_posx + sum_w - w*0.5, up_pos_y - (row_index - 1) * row_dis));
		} else{
			++lbl_num;
			sprintf(buff, "Label_Detail%d", lbl_num);
			auto detailLbl = static_cast<Label *>(parentNode->getChildByName(buff));
			if (!detailLbl)
			{
				detailLbl = Label::create(info.content, QJ_FONT_1, fontSize);
				detailLbl->setAnchorPoint(Vec2(0, 0.5));
				detailLbl->setName(buff);
				parentNode->addChild(detailLbl);
			} else{
				detailLbl->setString(info.content);
			}
			
			detailLbl->setVisible(true);
			float w = detailLbl->getContentSize().width;
			if (sum_w + w > max_show_w)
			{
				//换行
				auto vec = globalfunc::getCutOutString(info.content, QJ_FONT_1, 24, max_show_w - sum_w);
				if (vec.size() > 0)
				{
					detailLbl->setString(vec[0]);
					detailLbl->setPosition(Vec2(basic_posx + sum_w, up_pos_y - (row_index - 1) * row_dis));
				}

				string tmp_str = "";
				for (int m = 1; m < vec.size(); ++m){
					tmp_str += vec[m];
				}

				++row_index;

				if (tmp_str.size()>0)
				{
					/*
					新的一行显示上一行最后的一个lbl 没有显示完的内容
					此处传进来的faceVec 数据是经过 handleChatStr 处理的
					tmp_str的显示宽度必然不会越界
					*/
					++lbl_num;
					sprintf(buff, "Label_Detail%d", lbl_num);
					auto lbl = static_cast<Label *>(parentNode->getChildByName(buff));
					if (!lbl)
					{
						lbl = Label::create(tmp_str, QJ_FONT_1, fontSize);
						lbl->setName(buff);
						lbl->setAnchorPoint(Vec2(0, 0.5));
						parentNode->addChild(lbl);
					} else{
						lbl->setString(tmp_str);
					}
					
					lbl->setVisible(true);

					w = lbl->getContentSize().width;
					sum_w = w;
					lbl->setPosition(Vec2(basic_posx + sum_w - w, up_pos_y - (row_index - 1) * row_dis));
				} else{
					sum_w = 0;
				}
			} else{
				sum_w += w;
				detailLbl->setPosition(Vec2(basic_posx + sum_w - w, up_pos_y - (row_index - 1) * row_dis));
			}
		}
	}

	//----------------------------------
	for (int i = lbl_num + 1; true; ++i){
		sprintf(buff, "Label_Detail%d", i);
		auto detailLbl = static_cast<Label *>(parentNode->getChildByName(buff));
		if (detailLbl)
		{
			detailLbl->setVisible(false);
		} else{
			//lbl为nullptr 无须继续遍历
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
			//sp为nullptr 无须继续遍历
			break;
		}
	}
	return row_index;
}
