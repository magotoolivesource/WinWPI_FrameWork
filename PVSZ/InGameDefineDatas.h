#pragma once


#define WIDEN2W(x) L##x
#define WIDENW(x) WIDEN2W(x)
#define IMAGEPATHW(x) L"Assets/Images/" WIDENW(x)

#define WIDEN2(x) L##x
#define WIDEN(x) WIDEN2(x)
#define IMAGEPATH(x) "Assets/Images/" ##x
//#define IMAGEPATH(x) L"Assets/Images/"L##x

#define ONEBLOCKSIZEW 80
#define ONEBLOCKSIZEH 98
const Vec2 OneBlockSize = { 80, 98 };

#define SUNIMAGESIZEW 50.f
#define SUNIMAGESIZEH 50.f

#define SHOOTERBLOCKSIZEW 56 * 1.f
#define SHOOTERBLOCKSIZEH 34 * 1.f


#define SCREEN_W 1280
#define SCREEN_H 720


#define TAG_PLANT "Plant"
#define TAG_ZOMBIE "Zombie"
#define TAG_SUN "Sun"
#define TAG_BULLET "Bullet"
#define TAG_EFFECT "Effect"


enum class E_ALLLayerType
{
	BG = -1000,

	Front = 0,

	Palnts = 1000,

	Zombie = 2000,

	Shooter = 3000,

	SUN = 4000,

	Effect = 5000,

};


enum class E_ZOMBIE_TYPE
{
	Zombie_Normal = 0,
	Zombie_Fast,
	Zombie_Strong,
	Zombie_Flying,
	Zombie_Boss,
	// 추가적인 좀비 타입이 필요할 경우 여기에 추가


	MAX
};