#ifndef _Players_H_
#define _Players_H_
#include"Game_BasicClass.h"

class PlayerClass :public RoleBasic
{
	PProp  props;
	int frameNum;
	float boxw, boxh;
	D3DXMATRIX world;	
	CSound *wav[5];
	UINT   _sworkKeeNum;
	
	void (*regist)(RoleBasic* role);
public:
	int jumpnum;
	D3DXVECTOR3	 velocity;
	PlayerClass() { _tag = PLAYER; }
	virtual HRESULT Init(LPDIRECT3DDEVICE9	device, wchar_t *filename, int nrow, int ncow, float frame, 
						float w, float h, float z,void(*regis)(RoleBasic* role));
	virtual void InitDigital();
	virtual HRESULT LoadWav() { return S_OK; }
	virtual void Update(float timeDelta);
	virtual void Move(float x, float y) { _position.x += x, _position.y += y; box->Move(x, y); }
 	virtual void ChangeAction(ActionState act);
	virtual void Render();
	virtual void Clear() { RoleBasic::Clear(); SAFE_DELETE(box); }
	virtual void MoveToY(float y) { _position.y = y; box->Init(_position, boxw, boxh); }
	virtual float GetBoxLy() { SquareBox* b = (SquareBox*)box; return b->ly; }
	virtual D3DXVECTOR3 GetPos() { return _position; }
	virtual UINT GetSkillNum(SkillTag tag);
	virtual void  ChangerSkillNum(SkillTag tag, int num );
	virtual ~PlayerClass() { Clear(); }
};


#endif