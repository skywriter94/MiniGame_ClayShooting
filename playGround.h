#pragma once
#include "gameNode.h"

#define PI 3.14f
#define PI2 PI*2
#define GRAVITY 0.1f

#define PLAYER_REPOSITIONTIME 28
#define CLAY_MAX 20
#define CLAY_SPEED 10
#define CLAY_THROWTIME 60

class playGround : public gameNode
{
	enum ClayPos
	{
		Left, Right
	};

	struct Crosshair
	{
		float x, y;
		float radius;
		float left, top, right, bottom;
	};

	struct Clay
	{
		float x, y;
		float angle;
		float radius;
		float speed;
		float gravity;
		ClayPos pos;
		bool isThrowing;
		bool leftThrowing, rightThrowing;
		bool isHit;
	};
	struct Player
	{
		float repositionTime;
		bool isChangeImg, isLeftShot, isRightShot;
		Crosshair leftCrosshair, rightCrosshair;
		int score;
	};

private:
	image* bg;
	image* playerFirePos;
	image* playerSprite[3]; // [0] == look left, [1] == look center, [2] == look right
	image* clayImg;
	image* crosshair;

	Player player;
	Clay clay[CLAY_MAX];
	float count_throwTimeInterval;
	int count_clayThrow;
	int centerPos;

	POINT ptMin1, ptMin2;
	int idx1, idx2;

public:
	playGround();
	~playGround();

	virtual HRESULT init();			//�ʱ�ȭ ���� �Լ�
	virtual void release();			//�޸� ���� �Լ�
	virtual void update();			//���� ���� �Լ�
	virtual void render(HDC hdc);	//�׸��� ���� �Լ�

	void init_SetObj();
	void init_SetSprites();

	void update_ChangeFP_andShooting();
	void update_Crosshair();
	void update_ClayFire();
	void update_ClayMoving();

	void render_Sprites(HDC backDC);
};