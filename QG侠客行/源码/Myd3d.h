#ifndef __Myd3dH__
#define __Myd3dH__

#include <d3dx9.h>
#include <string>

namespace d3d
{
	const D3DXCOLOR      WHITE( D3DCOLOR_XRGB(255, 255, 255) );
	const D3DXCOLOR      BLACK( D3DCOLOR_XRGB(  0,   0,   0) );
	const D3DXCOLOR        RED( D3DCOLOR_XRGB(255,   0,   0) );
	const D3DXCOLOR      GREEN( D3DCOLOR_XRGB(  0, 255,   0) );
	const D3DXCOLOR       BLUE( D3DCOLOR_XRGB(  0,   0, 255) );
	const D3DXCOLOR     YELLOW( D3DCOLOR_XRGB(255, 255,   0) );
	const D3DXCOLOR       CYAN( D3DCOLOR_XRGB(  0, 255, 255) );
	const D3DXCOLOR    MAGENTA( D3DCOLOR_XRGB(255,   0, 255) );
	//
	// Lights
	//
	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
	D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);
	//
	// Materials
	//
	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);
	const D3DMATERIAL9 WHITE_MTRL  = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
	const D3DMATERIAL9 RED_MTRL    = InitMtrl(RED, RED, RED, BLACK, 2.0f);
	const D3DMATERIAL9 GREEN_MTRL  = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
	const D3DMATERIAL9 BLUE_MTRL   = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);
}

class Fond
{
public:
	ID3DXFont * _fond;
	IDirect3DDevice9*     _device;
	long _width;
	long _height;
	RECT _rec;
	bool Init(IDirect3DDevice9*  * device, wchar_t * type = NULL);
	void draw(wchar_t * text, long x, long y);
	void draw(float digital, long x, long y);
	void Clear() {if (_fond!= NULL) _fond->Release();}
	~Fond() { Clear(); }
};

VOID Light_Set(LPDIRECT3DDEVICE9 pd3dDevice, UINT nType);

class Camera
{
public:
	D3DXVECTOR3				v_RightVector;		  //右分量方向向量，x
	D3DXVECTOR3				v_UpVector;			  //上方向方向向量,y
	D3DXVECTOR3             v_LookVector;         // 观察方向向量  ,z
	D3DXVECTOR3             v_CameraPosition;     // 摄像机位置的向量  
	D3DXVECTOR3             v_TargetPosition;     //目标观察位置的向量  
	D3DXMATRIX              m_matView;            // 取景变换矩阵  
	D3DXMATRIX              m_matProj;            // 投影变换矩阵     
	LPDIRECT3DDEVICE9       m_pd3dDevice;		  //Direct3D设备对象  
	int						screen_width;
	int						screen_height;

public:
	//一个计算取景变换的函数  
	void CalculateViewMatrix(D3DXMATRIX *pMatrix);    //计算取景变换矩阵  

													  //三个Get函数  
	void GetProjMatrix(D3DXMATRIX *pMatrix) { *pMatrix = m_matProj; }  //返回当前投影矩阵  
	void GetCameraPosition(D3DXVECTOR3 *pVector) { *pVector = v_CameraPosition; } //返回当前摄像机位置矩阵  
	void GetLookVector(D3DXVECTOR3 *pVector) { *pVector = v_LookVector; }  //返回当前的观察矩阵  

																		   //四个Set系列函数，注意他们都参数都有默认值NULL的，调用时不写参数也可以  
	void SetTargetPosition(D3DXVECTOR3 *pLookat = NULL);  //设置摄像机的目标观察位置向量  
	void SetCameraPosition(D3DXVECTOR3 *pVector = NULL); //设置摄像机所在的位置向量  
	void SetViewMatrix(D3DXMATRIX *pMatrix = NULL);  //设置取景变换矩阵  
	void SetProjMatrix(D3DXMATRIX *pMatrix = NULL);  //设置投影变换矩阵  

public:
	// 沿各分量平移的三个函数  
	void MoveAlongRightVec(float units);		 // 沿right向量移动  
	void MoveAlongUpVec(float units);			 // 沿up向量移动  
	void MoveAlongLookVec(float units);    // 沿look向量移动  

										   // 绕各分量旋转的三个函数  
	void RotationRightVec(float fAngle);    // 绕right向量选择  
	void RotationUpVec(float fAngle);       // 绕up向量旋转  
	void RotationLookVec(float fAngle);     // 绕look向量旋转  

public:
	//构造函数和析构函数
	Camera() {};
	Camera(IDirect3DDevice9 *pd3dDevice, int w, int h);  //构造函数  
	virtual ~Camera(void);  //析构函数  

};

void D3DVecNormal(D3DXVECTOR3* vec);
RECT MakeRect(LONG x,LONG y,LONG w, LONG h);

float GetRandomFloat(float lowBound, float highBound);

float CalDistanceSq(D3DXVECTOR3* d1, D3DXVECTOR3* d2);
#endif 