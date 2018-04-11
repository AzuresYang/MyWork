#include "DXUT.h"
#include"Game_Map.h"
#include <ctime> 
MapClass::MapClass()
{
	_device = NULL;
	_pTexture = NULL;
	_numOfBuffer = 5;
	_fVelocity = 0;
	_height = 0;
	_screenWidth = 0;
	_elementWidth = 0;
	_level = 0;
	_lastMapPosition = 0;
	_firstMap = 0;
	_z = 0;
	for (int i = 0; i < 5; i++)
		_pBuffer[i] = NULL;
}

void MapClass::Clear()
{
	_device = NULL;
	_fVelocity = 0;
	_height = 0;
	_screenWidth = 0;
	_elementWidth = 0;
	SAFE_RELEASE(_pTexture);
	for (int i = 0; i < 5; i++)
		SAFE_RELEASE(_pBuffer[i]);
}

void MapClass::InitDigital()
{
	_level = 0;
}

bool MapClass::Init(LPDIRECT3DDEVICE9 * device, wchar_t* filename, int character_length, float h, float z, int screenWidth, float v)
{
	_device = *device;
	_height = h;
	_elementWidth = character_length;
	_screenWidth = screenWidth;
	_fVelocity = v;
	_lastMapPosition = 0;
	_time = 2000.0f;
	_level = 0;
	_z = z;
	HRESULT hr;
	hr = D3DXCreateTextureFromFile(_device, filename, &_pTexture);
	if (FAILED(hr))
	{
		::MessageBox(0, L"Create map texture failed", 0, 0);
		return false;
	}
	if(!CreateMap())
	{
		::MessageBox(0, L"Init:Create map failed", 0, 0);
		return false;
	}
	return true;
}
void MapClass::CreateWidth(float &width, float &blank_width)
{
	srand((time(0)));//设置随机数
	width = float((rand() % _screenWidth));//随机宽度
	if (width < _screenWidth/10)
		width = float(_screenWidth / 10);
	float factor ;
	if (_level == 3)
		factor = 4;
	else if (_level == 2)
		factor = 2;
	else
		factor = 0;
	blank_width = (rand() % 40)*factor;//一个简单的产生空白间距的办法
}

//lastposition 这个地方有问题
bool MapClass::CreateMap()
{
	_firstMap = 0;
	float w;
	float width;
	float blank_width;
	for (int i = 0; i < _numOfBuffer; i++)
	{
		CreateWidth(width, blank_width); //产生一个随机宽度和空白间距
		w = width + _lastMapPosition;
		_leftEdge[i] = _lastMapPosition;//记录下地图块的左右边界
		_rightEdge[i]= w;
		box[i].Init(D3DXVECTOR3((_lastMapPosition+w)/2,_height/2,_z),width,_height);
		MAPVERTEX obj[] =
		{
			{ _lastMapPosition,	 0,		  _z,  0.0f, 1.0f },
			{ _lastMapPosition,  _height, _z,  0.0f, 0.0f },
			{  w,				 0,		  _z,	 float(width/_elementWidth), 1.0f },
			{  w,				 _height, _z,	 float(width/_elementWidth), 0.0f },
		};
		_lastMapPosition = w + blank_width;//最后一块的长度就这样
		// 创建顶点缓存
		if (FAILED(_device->CreateVertexBuffer(sizeof(obj), 0, D3DFVF_GAMEMAP,
			D3DPOOL_MANAGED, &_pBuffer[i], NULL))) {
			::MessageBox(0, L"creat  map buffer error", 0, 0);
			return false;
		}
		// 填充顶点缓存
		void *ptr;
		if (FAILED(_pBuffer[i]->Lock(0, sizeof(obj), (void**)&ptr, 0)))
		{
			::MessageBox(0, L"lock buffer error", 0, 0);
			return false;
		}
		memcpy(ptr, obj, sizeof(obj));
		_pBuffer[i]->Unlock();
	}
	return true;
}

bool MapClass::UpdateOneMap(int i)
{
	float w;
	float width ;
	float blank_width = 50;//地图之间空白的地方

	CreateWidth(width, blank_width);
	w = width + _lastMapPosition;
	_leftEdge[i] = _lastMapPosition;
	_rightEdge[i] = w;
	box[i].Init(D3DXVECTOR3((_lastMapPosition + w) / 2, _height / 2, _z), width, _height);
	MAPVERTEX*	v;
	if (FAILED(_pBuffer[i]->Lock(0, 0, (void**)&v, 0)))
		return false;
	v[0]._x = v[1]._x = _lastMapPosition;
	v[2]._x = v[3]._x = w;
	v[2]._u = v[3]._u = float(width / _elementWidth);
	_pBuffer[i]->Unlock();
	_lastMapPosition = w + blank_width;//最后一块的长度就这样
									   // 创建顶点缓存
	return true;
}
void MapClass::UpdateMap(float timeDelta)
{
	MAPVERTEX*	v;
	_lastMapPosition -= _fVelocity*timeDelta;
	for (int i = 0; i < _numOfBuffer; i++)
	{
		_leftEdge[i] -= _fVelocity*timeDelta;
		_rightEdge[i] -= _fVelocity*timeDelta;
		_pBuffer[i]->Lock(0, 0, (void**)&v, 0);
		v[0]._x = v[0]._x - _fVelocity*timeDelta;
		v[1]._x = v[1]._x - _fVelocity*timeDelta;
		v[2]._x = v[2]._x - _fVelocity*timeDelta;
		v[3]._x = v[3]._x - _fVelocity*timeDelta;
		_pBuffer[i]->Unlock();
		box[i].Move(-_fVelocity*timeDelta, 0);
	}
	if (_rightEdge[_firstMap] < 0)
	{
		UpdateOneMap(_firstMap);
		_firstMap = (_firstMap + 1) % _numOfBuffer;
	}
}
void MapClass::Render()
{
	_device->SetTexture(0, _pTexture);
	for (int i = 0; i < _numOfBuffer; i++)
	{
		
		_device->SetStreamSource(0, _pBuffer[i], 0, sizeof(MAPVERTEX));
		_device->SetFVF(D3DFVF_GAMEMAP);
		_device->DrawPrimitive(D3DPT_TRIANGLESTRIP,  0, 2);
	}
	_device->SetTexture(0, 0);
}

bool MapClass::IsOnMap(float lx, float rx)
{
	int j = _firstMap;
	for (int i = 0; i < _numOfBuffer; i++)
	{
		if ( rx > _leftEdge[j] &&lx < _rightEdge[j])//是否在地图块上的判断标准是，人物的左边是否在一块地图的
			return true;
		j= (j + 1) % _numOfBuffer;
	}
	return false;
}

bool BackGroudClass::CreateBuffer()
{
	float w;
	//w = _screenHeight/_screenHeight;
	w = 1.0f / 2.5;
	_screenHeight = 50;
	_screenWidth = 100;
	float u = 1.0f / _totalNum;
	for (int i = 0; i < _totalNum; i++)
	{
		MAPVERTEX obj[] = 
		{
			{ 0,				0,				_z[i],  0.0f,  	u*(i + 1) },
			{ 0,				_screenHeight,  _z[i],  0.0f,  u*i},
			{ _screenWidth,		0,				_z[i],	 w,	  	u*(i+1)},
			{ _screenWidth,	_screenHeight,		_z[i],	 w,     u*i },

		};
		if (FAILED(_device->CreateVertexBuffer(sizeof(obj), 0, D3DFVF_GAMEMAP,
			D3DPOOL_MANAGED, &_pBuffer[i], NULL))) {
			::MessageBox(0, L"Backgound:creat buffer error", 0, 0);
			return false;
		}
		// 填充顶点缓存
		void *ptr;
		if (FAILED(_pBuffer[i]->Lock(0, sizeof(obj), (void**)&ptr, 0)))
		{
			::MessageBox(0, L"Backgroud: Lock buffer error", 0, 0);
			return false;
		}
		memcpy(ptr, obj, sizeof(obj));
		_pBuffer[i]->Unlock();
	}
	return true;
}

HRESULT BackGroudClass::Init(LPDIRECT3DDEVICE9* device, wchar_t * filename, float w, float h,unsigned int num, float v)
{
	if (num > 7)
	{
		::MessageBox(0, L"BackGroudClass.Init:目前背景不能创建超过七个图层", 0, 0);
		return E_FAIL;
	}
	_device = *device;
	_totalNum = num;
	_screenWidth = w, _screenHeight = h;
	_z[0] = -0.3f;
	_fVelocity[0] = v;
	for (int i = 1; i < _totalNum; i++)
		_z[i] = i*0.01, _fVelocity[i] = i*v/3;
	if (FAILED(D3DXCreateTextureFromFile(_device, filename, &_pTexture)))
	{
		::MessageBox(0, L"Create Button texture[Up] error", 0, 0);
		return E_FAIL;
	}
	if (!CreateBuffer())
		return E_FAIL;
	return S_OK;
}

void BackGroudClass::Update(float timeDelta)
{
	MAPVERTEX*	v;
	for (int i = 0; i < _totalNum; i++)
	{
		_pBuffer[i]->Lock(0, 0, (void**)&v, 0);
// 		v[0]._u =fmodf(v[0]._u + _fVelocity[i] * timeDelta, 1.0f);
// 		v[1]._u =fmodf(v[1]._u + _fVelocity[i] * timeDelta, 1.0f);
// 		v[2]._u =fmodf(v[2]._u + _fVelocity[i] * timeDelta, 1.0f);
// 		v[3]._u =fmodf(v[3]._u + _fVelocity[i] * timeDelta, 1.0f);
		v[0]._u = v[0]._u + _fVelocity[i] * timeDelta;
		v[1]._u = v[1]._u + _fVelocity[i] * timeDelta;
		v[2]._u = v[2]._u + _fVelocity[i] * timeDelta;
		v[3]._u = v[3]._u + _fVelocity[i] * timeDelta;
		_pBuffer[i]->Unlock();
	}
}

void BackGroudClass::Render()
{
	for (int i = _totalNum-1; i >=0; i--)
	{
		_device->SetTexture(0, _pTexture);
		_device->SetStreamSource(0, _pBuffer[i], 0, sizeof(MAPVERTEX));
		_device->SetFVF(D3DFVF_GAMEMAP);
		_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
	_device->SetTexture(0, 0);
}

void	BackGroudClass::Clear()
{
	for (int i = 0; i < 7; i++)
	{
		_z[i]=_fVelocity[i] = 0;
		SAFE_RELEASE(_pBuffer[i]);
	}
	SAFE_RELEASE(_pTexture);
	_totalNum = 0;
}