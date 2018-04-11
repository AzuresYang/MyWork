#include "DXUT.h"
#include "Myd3d.h"
#include<tchar.h>



void D3DVecNormal(D3DXVECTOR3* vec)
{
	float sum = vec->x*vec->x + vec->y*vec->y + vec->z*vec->z;
	sum = sqrt(sum);
	vec->x /= sum;
	vec->y /= sum;
	vec->z /= sum;
}
D3DLIGHT9 d3d::InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type      = D3DLIGHT_DIRECTIONAL;
	light.Ambient   = *color * 0.4f;
	light.Diffuse   = *color;
	light.Specular  = *color * 0.6f;
	light.Direction = *direction;
	return light;
}

D3DLIGHT9 d3d::InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type      = D3DLIGHT_POINT;
	light.Ambient   = *color * 0.4f;
	light.Diffuse   = *color;
	light.Specular  = *color * 0.6f;
	light.Position  = *position;
	light.Range        = 1000.0f;
	light.Falloff      = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;

	return light;
}

D3DLIGHT9 d3d::InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type      = D3DLIGHT_SPOT;
	light.Ambient   = *color * 0.4f;
	light.Diffuse   = *color;
	light.Specular  = *color * 0.6f;
	light.Position  = *position;
	light.Direction = *direction;
	light.Range        = 1000.0f;
	light.Falloff      = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;
	light.Theta        = 0.5f;
	light.Phi          = 0.7f;

	return light;
}

D3DMATERIAL9 d3d::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient  = a;
	mtrl.Diffuse  = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power    = p;
	return mtrl;
}
VOID Light_Set(LPDIRECT3DDEVICE9 pd3dDevice, UINT nType)
{
	static D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	switch (nType)
	{
	case 1:     //���Դ  
		light.Type = D3DLIGHT_POINT;
		light.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		light.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		light.Position = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
		light.Attenuation0 = 1.0f;
		light.Attenuation1 = 0.0f;
		light.Attenuation2 = 0.0f;
		light.Range = 300.0f;
		break;
	case 2:     //ƽ�й�  
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		light.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		light.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		break;
	case 3:     //�۹��  
		light.Type = D3DLIGHT_SPOT;
		light.Position = D3DXVECTOR3(0.0f, 250.0f, 0.0f);
		light.Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		light.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		light.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.3f);
		light.Attenuation0 = 1.0f;
		light.Attenuation1 = 0.0f;
		light.Attenuation2 = 0.0f;
		light.Range = 300.0f;
		light.Falloff = 1.2f;
		light.Phi = D3DX_PI / 3.0f;
		light.Theta = D3DX_PI / 8.0f;
		break;
	}
	pd3dDevice->SetLight(0, &light); //���ù�Դ  
	pd3dDevice->LightEnable(0, true);//���ù���  
	pd3dDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(36, 36, 36));   //����һ�»�����  
}
bool Fond::Init(IDirect3DDevice9*  * device, wchar_t * type)
{
	_width = 100;
	_height = 50;
	_rec.top = 0;
	_rec.left = 0;
	_rec.bottom = _rec.top + _height;
	_rec.right = _rec.left + _width;
	_device = *device;
	if (type)
		D3DXCreateFont(_device, 0, 0, 0,
			0, 0, 0, 0, 0, 0, type, &_fond);
	else
		D3DXCreateFont(_device, 0, 0, 0,
			0, 0, 0, 0, 0, 0, _T("����"), &_fond);
	return true;
}

void Fond::draw(wchar_t* text, long x, long y)
{
	_rec.top = y;
	_rec.left = x;
	_rec.bottom = _rec.top + _height;
	_rec.right = _rec.left + _width;
	_fond->DrawText(NULL, text, -1, &_rec, DT_TOP | DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

}

void Fond::draw(float digital, long x, long y)
{
	wchar_t w_text[20];
	swprintf(w_text, L"%.2f", digital);
	_rec.top = y;
	_rec.left = x;
	_rec.bottom = _rec.top + _height;
	_rec.right = _rec.left + _width;
	_fond->DrawText(NULL, w_text, -1, &_rec, DT_TOP | DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

}



Camera::Camera(IDirect3DDevice9 *pd3dDevice, int w, int h)
{
	m_pd3dDevice = pd3dDevice;
	v_RightVector = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	v_UpVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	v_LookVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	v_CameraPosition = D3DXVECTOR3(0.0f, 0.0f, -25.0f);
	v_TargetPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	screen_width = w;
	screen_height = h;
}


void Camera::CalculateViewMatrix(D3DXMATRIX *pMatrix)
{
	//1.�Ȱ�3���������淶����ʹ���໥��ֱ����Ϊһ����������  
	D3DXVec3Normalize(&v_LookVector, &v_LookVector);  //�淶���۲����  

	D3DXVec3Cross(&v_UpVector, &v_LookVector, &v_RightVector);    // ��������۲�������ֱ  
	D3DXVec3Normalize(&v_UpVector, &v_UpVector);
	D3DXVec3Cross(&v_RightVector, &v_UpVector, &v_LookVector);    // ����������������ֱ  
	D3DXVec3Normalize(&v_RightVector, &v_RightVector);              // �淶��������
																	// �淶�������� 

																	// 2.������ȡ���任����
	pMatrix->_11 = v_RightVector.x;
	pMatrix->_12 = v_UpVector.x;
	pMatrix->_13 = v_LookVector.x;
	pMatrix->_14 = 0.0f;

	pMatrix->_21 = v_RightVector.y;
	pMatrix->_22 = v_UpVector.y;
	pMatrix->_23 = v_LookVector.y;
	pMatrix->_24 = 0.0f;

	pMatrix->_31 = v_RightVector.z;
	pMatrix->_32 = v_UpVector.z;
	pMatrix->_33 = v_LookVector.z;
	pMatrix->_34 = 0.0f;

	pMatrix->_41 = -D3DXVec3Dot(&v_RightVector, &v_CameraPosition);
	pMatrix->_42 = -D3DXVec3Dot(&v_UpVector, &v_CameraPosition);
	pMatrix->_43 = -D3DXVec3Dot(&v_LookVector, &v_CameraPosition);
	pMatrix->_44 = 1.0f;
}

//-----------------------------------------------------------------------------  
// Name��Camera::SetTargetPosition( )  
// Desc: ����������Ĺ۲�λ��  
//-----------------------------------------------------------------------------  
void Camera::SetTargetPosition(D3DXVECTOR3 *pLookat)
{
	//�ȿ���pLookat�Ƿ�ΪĬ��ֵNULL  
	if (pLookat != NULL)  v_TargetPosition = (*pLookat);
	else v_TargetPosition = D3DXVECTOR3(0.0f, 0.0f, 1.0f);


	v_LookVector = v_TargetPosition - v_CameraPosition;		//�۲췽������ = �۲��λ�� - �����λ��

															//���������淶����������������Ϊ���������������⣬ʹ���ǲ�һ������
	D3DXVec3Normalize(&v_LookVector, &v_LookVector);


	D3DXVec3Cross(&v_UpVector, &v_LookVector, &v_RightVector);
	D3DXVec3Normalize(&v_UpVector, &v_UpVector);
	D3DXVec3Cross(&v_RightVector, &v_UpVector, &v_LookVector);
	D3DXVec3Normalize(&v_RightVector, &v_RightVector);



}

//-----------------------------------------------------------------------------  
// Name��Camera::SetCameraPosition( )  
// Desc: ������������ڵ�λ��  
//-----------------------------------------------------------------------------  
void Camera::SetCameraPosition(D3DXVECTOR3 *pVector)
{
	D3DXVECTOR3 v = D3DXVECTOR3(0.0f, 0.0f, -250.0f);
	if (pVector != NULL)
		v_CameraPosition = (*pVector);
	else
		v_CameraPosition = v;
}

//-----------------------------------------------------------------------------  
// Name��Camera::SetViewMatrix( )  
// Desc: ����ȡ���任����  
//-----------------------------------------------------------------------------  
void Camera::SetViewMatrix(D3DXMATRIX *pMatrix)
{
	if (pMatrix != NULL)
		m_matView = (*pMatrix);
	else
		CalculateViewMatrix(&m_matView);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matView);

	//��ȡ���任�����ֵ�������ֱ���ҷ������Ϸ������͹۲����  
	v_RightVector = D3DXVECTOR3(m_matView._11, m_matView._12, m_matView._13);
	v_UpVector = D3DXVECTOR3(m_matView._21, m_matView._22, m_matView._23);
	v_LookVector = D3DXVECTOR3(m_matView._31, m_matView._32, m_matView._33);

}

//-----------------------------------------------------------------------------  
// Name��Camera::SetProjMatrix( )  
// Desc: ����ͶӰ�任����  
//-----------------------------------------------------------------------------  
VOID Camera::SetProjMatrix(D3DXMATRIX *pMatrix)
{
	//�ж�ֵ��û�У�û�еĻ��ͼ���һ��  
	if (pMatrix != NULL)
		m_matProj = *pMatrix;
	else
		D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4.0f, (float)((double)screen_width / screen_height), 1.0f, 30000.0f);//�ӽ���Զ����Ϊ30000.0f�������Ͳ��¿�����Զ����������  
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);//����ͶӰ�任����  
}

//-----------------------------------------------------------------------------  
// Name��Camera::MoveAlongRightVec( )  
// Desc: ��������ƽ��units����λ  ---ɨ��
//-----------------------------------------------------------------------------  
VOID Camera::MoveAlongRightVec(float units)
{
	//ֱ�ӳ���units�������ۼӾ�����  
	v_CameraPosition += v_RightVector * units;
	v_TargetPosition += v_RightVector * units;
}

//-----------------------------------------------------------------------------  
// Name��Camera::MoveAlongUpVec( )  
// Desc:  ��������ƽ��fUnits����λ --- ����
//-----------------------------------------------------------------------------  
VOID Camera::MoveAlongUpVec(float units)
{
	//ֱ�ӳ���fUnits�������ۼӾ�����  
	v_CameraPosition += v_UpVector * units;
	v_TargetPosition += v_UpVector * units;
}

//-----------------------------------------------------------------------------  
// Name��Camera::MoveAlongLookVec( )  
// Desc:  �ع۲�����ƽ��units����λ  ---����
//-----------------------------------------------------------------------------  
VOID Camera::MoveAlongLookVec(float units)
{
	//ֱ�ӳ���fUnits�������ۼӾ�����  
	v_CameraPosition += v_LookVector * units;
	v_TargetPosition += v_LookVector * units;

}


//-----------------------------------------------------------------------------  
// Name��Camera::RotationRightVec( )  
// Desc:  ����������תfAngle�����ȵ�λ�ĽǶ�  
//-----------------------------------------------------------------------------  
VOID Camera::RotationRightVec(float fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &v_RightVector, fAngle);//��������v_RightVector��תfAngle���Ƕȵ�R����  
	D3DXVec3TransformCoord(&v_UpVector, &v_UpVector, &R);//��v_UpVector������v_RightVector��תfAngle���Ƕ�  
	D3DXVec3TransformCoord(&v_LookVector, &v_LookVector, &R);//��v_LookVector������v_RightVector��תfAngle���Ƕ�  

	v_TargetPosition = v_LookVector * D3DXVec3Length(&v_CameraPosition);//����һ�¹۲�����λ�ã��������ģ=������  
}

//-----------------------------------------------------------------------------  
// Name��Camera::RotationUpVec( )  
// Desc:  ����������תfAngle�����ȵ�λ�ĽǶ�  
//-----------------------------------------------------------------------------  
VOID Camera::RotationUpVec(float fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &v_UpVector, fAngle);//��������v_UpVector��תfAngle���Ƕȵ�R����  
	D3DXVec3TransformCoord(&v_RightVector, &v_RightVector, &R);//��v_RightVector������v_UpVector��תfAngle���Ƕ�  
	D3DXVec3TransformCoord(&v_LookVector, &v_LookVector, &R);//��v_LookVector������v_UpVector��תfAngle���Ƕ�  

	v_TargetPosition = v_LookVector * D3DXVec3Length(&v_CameraPosition);//����һ�¹۲�����λ�ã��������ģ=������  
}

//-----------------------------------------------------------------------------  
// Name��Camera::RotationLookVec( )  
// Desc:  �ع۲�������תfAngle�����ȵ�λ�ĽǶ�  
//-----------------------------------------------------------------------------  
VOID Camera::RotationLookVec(FLOAT fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &v_LookVector, fAngle);//��������v_LookVector��תfAngle���Ƕȵ�R����  
	D3DXVec3TransformCoord(&v_RightVector, &v_RightVector, &R);//��v_RightVector������v_LookVector��תfAngle���Ƕ�  
	D3DXVec3TransformCoord(&v_UpVector, &v_UpVector, &R);//��v_UpVector������v_LookVector��תfAngle���Ƕ�  

	v_TargetPosition = v_LookVector * D3DXVec3Length(&v_CameraPosition);//����һ�¹۲�����λ�ã��������ģ=������  
}


//-----------------------------------------------------------------------------  
// Desc: ��������  
//-----------------------------------------------------------------------------  
Camera::~Camera(void)
{
}

RECT MakeRect(LONG x, LONG y, LONG w, LONG h)
{
	RECT rec;
	rec.top = y;
	rec.bottom = y + h;
	rec.left = x;
	rec.right = x + w;
	return rec;
}

float GetRandomFloat(float lowBound, float highBound)
{
	if (lowBound >= highBound) // 
		return lowBound;

	// ���0-1���ϵ��
	float f = (rand() % 10000) * 0.0001f;

	// ����
	return (f * (highBound - lowBound)) + lowBound;
}

float CalDistanceSq(D3DXVECTOR3* d1, D3DXVECTOR3* d2)
{
	double dx = d1->x - d2->x;
	dx *= dx;

	double dy = d1->y - d2->y;
	dy *= dy;


	double dz = d1->z - d2->z;
	dz *= dz;
	return (dx + dz+dy);
}

