#pragma once

#include <wx/wx.h>
#include <string>

class DrawingsParserApp : public wxApp
{
public:
	virtual bool OnInit();

private:
	enum ProcessingFileState 
	{
		NONE = 0,                     // 0000
		LOAD = 1,                     // 0001
		LOAD_PARSE = 1 << 1,          // 0010
		LOAD_PARSE_WRITE = 1 << 2     // 0100
	};

	//������� ��� ������������ ��������� ������
	void processingOneFile(std::string& fileName, unsigned int state);
};

