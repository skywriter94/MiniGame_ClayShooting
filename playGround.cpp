#include "stdafx.h"
#include "playGround.h"

playGround::playGround()
{
}
playGround::~playGround()
{
}


HRESULT playGround::init()
{
	gameNode::init();
	init_SetObj();
	init_SetSprites();
	return S_OK;
}


void playGround::release()
{
	gameNode::release();
}


void playGround::update()
{
	gameNode::update();
	update_ChangeFP_andShooting();
	update_ClayFire();
	update_ClayMoving();
	update_Crosshair();
}


void playGround::render(HDC hdc)
{
	HDC backDC = this->getBackBuffer()->getMemDC();
	PatBlt(backDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//==================================================
	render_Sprites(backDC);
	//===================================================
	this->getBackBuffer()->render(hdc, 0, 0);
}


void playGround::init_SetObj()
{
	count_clayThrow = count_throwTimeInterval = player.score = player.repositionTime = player.isChangeImg = 0;
	player.leftCrosshair.radius = 20;
	player.leftCrosshair.x = WINSIZEX / 2.5; player.leftCrosshair.y = WINSIZEY / 2;

	player.rightCrosshair.radius = 20;
	player.rightCrosshair.x = WINSIZEX / 1.65f; player.rightCrosshair.y = WINSIZEY / 2;

	centerPos = (player.rightCrosshair.x + player.leftCrosshair.x) / 2;

	for (int i = 0; i < CLAY_MAX; i++)
	{
		int random = rand() % 2;
		clay[i].isHit = clay[i].isThrowing = clay[i].gravity = 0;
		clay[i].speed = CLAY_SPEED;
		clay[i].radius = 10;
		
		ptMin1.x = ptMin2.x = WINSIZEX;
		ptMin1.y = ptMin2.y = WINSIZEY;
		idx1 = idx2 = 0;
		if (random == 0) clay[i].leftThrowing = true;
		else clay[i].rightThrowing = true;
	}
}

void playGround::init_SetSprites()
{
	bg = new image;
	bg->init("bg.bmp", WINSIZEX, WINSIZEY, false, RGB(0,0,0));

	playerSprite[0] = new image;
	playerSprite[0]->init("lookLeft.bmp", 128, 179, true, RGB(255, 0, 255));
	playerSprite[1] = new image;
	playerSprite[1]->init("IdlePosition.bmp", 128, 281, true, RGB(255, 0, 255));
	playerSprite[2] = new image;
	playerSprite[2]->init("lookRight.bmp", 128, 199, true, RGB(255, 0, 255));

	playerFirePos = new image;
	playerFirePos = playerSprite[1];

	clayImg = new image;
	clayImg->init("clay.bmp", 20, 20, true, RGB(255, 0, 255));

	crosshair = new image;
	crosshair->init("crosshair.bmp", 40, 40, true, RGB(255, 0, 255));
}


void playGround::update_ChangeFP_andShooting()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
	{
		playerFirePos = playerSprite[0];
		player.repositionTime = 0;
		player.isChangeImg = true;
		player.isLeftShot = true;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
	{
		playerFirePos = playerSprite[2];
		player.repositionTime = 0;
		player.isChangeImg = true;
		player.isRightShot = true;
	}
	if (player.isChangeImg)
	{
		++player.repositionTime;
		if (player.repositionTime > PLAYER_REPOSITIONTIME)
		{
			playerFirePos = playerSprite[1];
			player.isChangeImg = false;
			player.repositionTime = 0;
		}
	}
}

void playGround::update_Crosshair()
{

	for (int i = 0; i < CLAY_MAX; i++)
	{
		if (clay[i].isThrowing)
		{
			float getBetweenLCH_X = player.leftCrosshair.x - clay[i].x > 0 ? player.leftCrosshair.x - clay[i].x : clay[i].x - player.leftCrosshair.x;
			float getBetweenLCH_Y = player.leftCrosshair.y - clay[i].y > 0 ? player.leftCrosshair.y - clay[i].y : clay[i].y - player.leftCrosshair.y;
			float getBetweenRCH_X = player.rightCrosshair.x - clay[i].x > 0 ? player.rightCrosshair.x - clay[i].x : clay[i].x - player.rightCrosshair.x;
			float getBetweenRCH_Y = player.rightCrosshair.y - clay[i].y > 0 ? player.rightCrosshair.y - clay[i].y : clay[i].y - player.rightCrosshair.y;

			if (ptMin1.x > getBetweenLCH_X && ptMin1.y > getBetweenLCH_Y && clay[i].pos == Left) idx1 = i;
			if (ptMin2.x > getBetweenRCH_X && ptMin2.y > getBetweenRCH_Y && clay[i].pos == Right) idx2 = i;
			ptMin1.x = clay[idx1].x, ptMin1.y = clay[idx1].y;
			ptMin2.y = clay[idx2].x, ptMin2.y = clay[idx2].y;
			player.leftCrosshair.y = ptMin1.y;
			player.rightCrosshair.y = ptMin2.y;
		}
	}
}

void playGround::update_ClayFire()
{
	++count_throwTimeInterval;
	if (count_throwTimeInterval > CLAY_THROWTIME && count_clayThrow < CLAY_MAX)
	{
		count_throwTimeInterval = 0;
		++count_clayThrow;
		
		if (clay[count_clayThrow - 1].leftThrowing)
		{
			clay[count_clayThrow - 1].x = WINSIZEX - clay[count_clayThrow - 1].radius * 2;
			clay[count_clayThrow - 1].y = WINSIZEY / 1.5f;
			clay[count_clayThrow - 1].angle = PI / 1.5f;
			clay[count_clayThrow - 1].pos = Left;
		}
		else if (clay[count_clayThrow - 1].rightThrowing)
		{
			clay[count_clayThrow - 1].x = clay[count_clayThrow - 1].radius * 2;
			clay[count_clayThrow - 1].y = WINSIZEY / 1.5f;
			clay[count_clayThrow - 1].angle = PI / 3;
			clay[count_clayThrow - 1].pos = Right;
		}
		clay[count_clayThrow - 1].isThrowing = true;
	}
}

void playGround::update_ClayMoving()
{
	player.leftCrosshair.left = player.leftCrosshair.x - player.leftCrosshair.radius;
	player.leftCrosshair.top = player.leftCrosshair.y - player.leftCrosshair.radius;
	player.leftCrosshair.right = player.leftCrosshair.x + player.leftCrosshair.radius;
	player.leftCrosshair.bottom = player.leftCrosshair.y + player.leftCrosshair.radius;

	player.rightCrosshair.left = player.rightCrosshair.x - player.rightCrosshair.radius;
	player.rightCrosshair.top = player.rightCrosshair.y - player.rightCrosshair.radius;
	player.rightCrosshair.right = player.rightCrosshair.x + player.rightCrosshair.radius;
	player.rightCrosshair.bottom = player.rightCrosshair.y + player.rightCrosshair.radius;
	for (int i = 0; i < CLAY_MAX; i++) // 발사된 클레이 움직임
	{
		if (clay[i].isThrowing)
		{
			clay[i].gravity += GRAVITY;
			clay[i].x += cosf(clay[i].angle) * clay[i].speed;
			clay[i].y += -sinf(clay[i].angle) * clay[i].speed + clay[i].gravity;
		}

		// 화면 바깥인 경우
		if (clay[i].x + clay[i].radius <= 0 || clay[i].y + clay[i].radius <= 0 || clay[i].x - clay[i].radius > WINSIZEX || clay[i].y - clay[i].radius > WINSIZEY)
		{
			clay[i].isThrowing = false; 
			clay[i].gravity = 0;
		}

		// 클레이 왼편 & 오른편 위치 판단
		if (clay[i].x < centerPos) clay[i].pos = Left;
		else if (clay[i].x > centerPos) clay[i].pos = Right;
	}

	for (int i = 0; i < CLAY_MAX; i++) // 클레이가 왼쪽 크로스헤어 안에 있는 경우
	{
		if (player.leftCrosshair.left < clay[i].x && player.leftCrosshair.right > clay[i].x && !clay[i].isHit && player.isLeftShot)
		{
			clay[i].isHit = true;
			clay[i].isThrowing = false;
			player.isLeftShot = false;
			++player.score;
		}
	}
	player.isLeftShot = false;

	for (int i = 0; i < CLAY_MAX; i++) // 클레이가 오른쪽 크로스헤어 안에 있는 경우
	{
		if (player.rightCrosshair.left < clay[i].x && player.rightCrosshair.right > clay[i].x && !clay[i].isHit && player.isRightShot)
		{
			clay[i].isHit = true;
			clay[i].isThrowing = false;
			player.isRightShot = false;
			++player.score;
		}
	}
	player.isRightShot = false;
}


void playGround::render_Sprites(HDC backDC)
{
	bg->render(backDC, 0, 0);
	playerFirePos->render(backDC, WINSIZEX/2 - 64, WINSIZEY - 100);

	for (int i = 0; i < CLAY_MAX; i++)
	{
		if (clay[i].isThrowing)
		{
			RectangleMakeCenter(backDC, clay[i].x, clay[i].y, clay[i].radius * 2, clay[i].radius * 2);
			clayImg->render(backDC, clay[i].x - clay[i].radius, clay[i].y - clay[i].radius);
		}
	}
	crosshair->render(backDC, player.leftCrosshair.x - player.leftCrosshair.radius, player.leftCrosshair.y - player.leftCrosshair.radius);
	crosshair->render(backDC, player.rightCrosshair.x - player.rightCrosshair.radius, player.rightCrosshair.y - player.rightCrosshair.radius);
}