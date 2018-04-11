#include<d3d9.h>
#include<d3dx9.h>
#include<string.h>
#define SNOW_NUMBER     2000
#define SNOW_WIDTH_Z	1000
#define SNOW_LENGTH_X   1000
#define SNOW_HEIGHT_Y   1000


struct SnowTexture
{
	float x, y, z;
	float u, v;
};
#define FVF_SNOWTEXTURE (D3DFVF_XYZ | D3DFVF_TEX1)

struct SnowParticle
{
	float x, y, z;
	
	float RotationY;
	float RotationX;
	float FallSpeed;
	float RotationSpeed;
	int TextureIndex;
};

class SnowClass
{
private:
	IDirect3DDevice9*				p_device;
	SnowParticle					m_Snows[SNOW_NUMBER];	//ѩ������
	IDirect3DVertexBuffer9*			p_vertexbuffer;			//�����������ݵĶ��㻺��
	IDirect3DTexture9*				p_texture[6];	//ѩ��������
	float wind_X;			

public:
	SnowClass(IDirect3DDevice9 *pd3dDevice);
	~SnowClass();

	HRESULT InitSnow(IDirect3DTexture9* *tex);
	HRESULT UpdateSnow(float fElapsedTime, int wind);				//����ϵͳ�ĸ���
	HRESULT RenderSnow(int flag);							//������Ⱦ
};