#pragma once
namespace Led
{
	enum DEBUG_TYPE
	{
		DEBUG_NONE    = 0,
		DEBUG_ERROR   = 1,
		DEBUG_WARNING = 2,
		DEBUG_INFO    = 4,
		DEBUG_ALL     = 7,
	};
	class Debug
	{
	private:
		static DEBUG_TYPE _type;
	public:
		static void Log(const wstring &message, DEBUG_TYPE type = DEBUG_ALL)
		{
			if(type == DEBUG_ALL || (_type & type))
				OutputDebugString(message.c_str()); 
		}
		static void Log(const string &message, DEBUG_TYPE type = DEBUG_ALL)
		{
			if(type == DEBUG_ALL || (_type & type))
				OutputDebugStringA((message + "\n").c_str()); 
		}
		static inline void ThrowIfFailed(HRESULT hr)
    {
      if (FAILED(hr))
      {
        throw std::exception();
      }
    }
	};
}