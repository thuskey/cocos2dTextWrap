
#include "publicheaders.h"

class globalfunc
{
public:
	static vector<string> getCutOutString(const string str, const string font_name, float font_size, float row_width);
	//������ʾ��ȶ�̬�ü��ַ���
	static string intToString(const int value);

	static void errorTip(const std::string str);
private:
};