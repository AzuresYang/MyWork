#ifndef __SkyboxH__
#define __SkyboxH__

#include <d3d9.h>  
#include <d3dx9.h> 
struct SkyBoxTexture
{
	float x, y, z;
	float u, v;
};
#define FVF_SKYBOXTEXTURE (D3DFVF_XYZ | D3DFVF_TEX1)

class SkyBoxClass
{
private:
	IDirect3DDevice9*			_device;
	IDirect3DVertexBuffer9*		_vertexbuffer;
	IDirect3DTexture9*			_tex[6];
	float						_length;

public:
	SkyBoxClass(IDirect3DDevice9* device);
	 ~SkyBoxClass();

public:
	bool	InitSkyBox(float length,	IDirect3DTexture9* *tex	);
	void	RenderSkyBox(D3DXMATRIX *pMatWorld, bool bRenderFrame);
};
#endif