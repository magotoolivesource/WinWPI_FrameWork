#pragma once


#define SAFEDELETE(x) if (x) { delete x; x = nullptr; }
#define SAFEARRAYDELETE(x) if (x) { delete[] x; x = nullptr; }
#define SAFEDELETEFOR(x) if (x) { for (auto& item : x) { delete item; } x.clear(); }


#define PI 3.14159265f
