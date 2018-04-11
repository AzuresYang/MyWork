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
	D3DXVECTOR3				v_RightVector;		  //�ҷ�������������x
	D3DXVECTOR3				v_UpVector;			  //�Ϸ���������,y
	D3DXVECTOR3             v_LookVector;         // �۲췽������  ,z
	D3DXVECTOR3             v_CameraPosition;     // �����λ�õ�����  
	D3DXVECTOR3             v_TargetPosition;     //Ŀ��۲�λ�õ�����  
	D3DXMATRIX              m_matView;            // ȡ���任����  
	D3DXMATRIX              m_matProj;            // ͶӰ�任����     
	LPDIRECT3DDEVICE9       m_pd3dDevice;		  //Direct3D�豸����  
	int						screen_width;
	int						screen_height;

public:
	//һ������ȡ���任�ĺ���  
	void CalculateViewMatrix(D3DXMATRIX *pMatrix);    //����ȡ���任����  

													  //����Get����  
	void GetProjMatrix(D3DXMATRIX *pMatrix) { *pMatrix = m_matProj; }  //���ص�ǰͶӰ����  
	void GetCameraPosition(D3DXVECTOR3 *pVector) { *pVector = v_CameraPosition; } //���ص�ǰ�����λ�þ���  
	void GetLookVector(D3DXVECTOR3 *pVector) { *pVector = v_LookVector; }  //���ص�ǰ�Ĺ۲����  

																		   //�ĸ�Setϵ�к�����ע�����Ƕ���������Ĭ��ֵNULL�ģ�����ʱ��д����Ҳ����  
	void SetTargetPosition(D3DXVECTOR3 *pLookat = NULL);  //�����������Ŀ��۲�λ������  
	void SetCameraPosition(D3DXVECTOR3 *pVector = NULL); //������������ڵ�λ������  
	void SetViewMatrix(D3DXMATRIX *pMatrix = NULL);  //����ȡ���任����  
	void SetProjMatrix(D3DXMATRIX *pMatrix = NULL);  //����ͶӰ�任����  

public:
	// �ظ�����ƽ�Ƶ���������  
	void MoveAlongRightVec(float units);		 // ��right�����ƶ�  
	void MoveAlongUpVec(float units);			 // ��up�����ƶ�  
	void MoveAlongLookVec(float units);    // ��look�����ƶ�  

										   // �Ƹ�������ת����������  
	void RotationRightVec(float fAngle);    // ��right����ѡ��  
	void RotationUpVec(float fAngle);       // ��up������ת  
	void RotationLookVec(float fAngle);     // ��look������ת  

public:
	//���캯������������
	Camera() {};
	Camera(IDirect3DDevice9 *pd3dDevice, int w, int h);  //���캯��  
	virtual ~Camera(void);  //��������  

};

void D3DVecNormal(D3DXVECTOR3* vec);
RECT MakeRect(LONG x,LONG y,LONG w, LONG h);

float GetRandomFloat(float lowBound, float highBound);

float CalDistanceSq(D3DXVECTOR3* d1, D3DXVECTOR3* d2);
#endif 