#ifndef D3DUTILITY_H
#define D3DUTILITY_H

#include "d3dx11Effect.h"
#include "DDSTextureLoader.h"
#include "dxerr.h"
#include "LightHelper.h"
#include "MathHelper.h"
#include <cassert>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <DirectXColors.h>

// Simple error checker for debugging. 
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)													\
		{                                                           \
			HRESULT hr = (x);                                       \
			if(FAILED(hr))                                          \
			{                                                       \
				DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true); \
			}                                                       \
		}
	#endif // HR
	
	#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif 

// Convenience macro for releasing COM objects. 
#define ReleaseCOM(x) { if(x){ x->Release(); x = nullptr; } }

// Convenience macro for deleting objects.
#define SafeDelete(x) { delete x; x = nullptr; }

#endif // D3DUTILITY_H
