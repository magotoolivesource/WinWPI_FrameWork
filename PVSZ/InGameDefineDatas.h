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
