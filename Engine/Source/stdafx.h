#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#endif

#include <iostream>
#include <fstream>
#include <locale>
#include <clocale>
#include <ctime>
#include <stdio.h>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <set>
#include <typeinfo>
#include <cmath>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <functional>
#include <assert.h>
#include <thread>
#include <cstdlib>
#include <algorithm>
#include <sstream> 
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
//#define D3D10_IGNORE_SDK_LAYERS
#ifdef _DEBUG
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#include <d3d12.h>
#include <d3d12shader.h>
#include <d3dcompiler.h>
#include <dxgi1_4.h>
#include <windows.h>
#include <windowsx.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxgi.lib")

using namespace std;
typedef unsigned __int8 uchar;
typedef unsigned __int16 ushort;
typedef unsigned __int32 uint;
typedef unsigned __int64 ulong;
typedef size_t sint;