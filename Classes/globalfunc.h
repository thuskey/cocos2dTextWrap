
#include "publicheaders.h"

class globalfunc
{
public:
	static vector<string> getCutOutString(const string str, const string font_name, float font_size, float row_width);
	//根据显示宽度动态裁剪字符串
	static string intToString(const int value);

	static void errorTip(const std::string str);
private:
};