
#include "globalfunc.h"



static const char trailingBytesForUTF8[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5
};

vector<string> globalfunc::getCutOutString(const string str, const string font_name, float font_size, float row_width)
{
	vector<string> vec;
	auto pLabel = Label::createWithTTF("", font_name, font_size);
	string  _str = str;

	char buff[BUFFER_SIZE_3] = { 0 };
	while (1) {
		if (_str.size() == 0) {
			return vec;
		}
		pLabel->setString(_str);
		if (pLabel->getContentSize().width < row_width) {
			sprintf(buff, "%s", _str.c_str());
			_str = "\0";
			vec.push_back(buff);
			break;
		} else{
			int cutLoaction = 0;
			for (int i = 0; i < _str.size();) {
				int length = trailingBytesForUTF8[(unsigned char)_str[i]] + 1;
				i += length;
				cutLoaction = i;
				std::string mMessage = _str.substr(0, cutLoaction);
				pLabel->setString(mMessage.c_str());
				if (pLabel->getContentSize().width >= row_width - font_size) {
					sprintf(buff, "%s", mMessage.c_str());
					break;
				}
			}
			vec.push_back(_str.substr(0, cutLoaction).c_str());
			_str = _str.substr(cutLoaction, _str.size());
		}
	}
	return vec;
}


string globalfunc::intToString(const int value)
{
	char buff[BUFFER_SIZE_1] = { 0 };
	sprintf(buff, "%d", value);
	return buff;
}

void globalfunc::errorTip(const std::string str)
{
	auto size = Director::getInstance()->getVisibleSize();

	Label *lab = Label::createWithTTF(str, QJ_FONT_1, 24);
	lab->setTextColor(Color4B::RED);
	lab->enableOutline(Color4B(10, 10, 10, 100), 1);
	lab->setPosition(0.5*size);
	Director::getInstance()->getRunningScene()->addChild(lab);

	FadeIn *fadeIn = FadeIn::create(0.2);
	MoveBy *move = MoveBy::create(0.3, Vec2(0.0, 180));
	MoveBy *wait = MoveBy::create(1.0, Vec2::ZERO);
	FadeOut *fade = FadeOut::create(0.2);
	ScaleTo *scale = ScaleTo::create(0.2, 0.3);
	Spawn *spawn = Spawn::create(scale, fade, NULL);
	CallFunc *fx = CallFunc::create(std::bind(&Node::removeFromParent, lab));
	Sequence *seq = Sequence::create(DelayTime::create(0.1), fadeIn, move, wait, spawn, fx, NULL);
	lab->runAction(seq);
}