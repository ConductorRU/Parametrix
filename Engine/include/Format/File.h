#pragma once
namespace Led
{
	class File
	{
	public:
		enum FILE: uint
		{
			READ = ios_base::in,
			WRITE = ios_base::out,
			BIN = ios_base::binary,
			ATEND = ios_base::ate,
			APPEND = ios_base::app,
			TRUNCATE = ios_base::trunc,
			READ_BIN = READ | WRITE,
			WRITE_BIN = WRITE | WRITE,
		};
	private:
		fstream _file;
	public:
		File();
		~File();
		void Open(const wstring& filename, uint flag);
		void Close();
	};
}