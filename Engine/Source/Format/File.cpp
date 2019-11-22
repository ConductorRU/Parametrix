#include "stdafx.h"
#include "File.h"
namespace Led
{
	File::File()
	{
	
	}
	File::~File()
	{
		Close();
	}
	void File::Open(const wstring& filename, uint flag)
	{
		Close();
		_file.open(filename, flag);
	}
	void File::Close()
	{
		if(_file.is_open())
			_file.close();
	}
}