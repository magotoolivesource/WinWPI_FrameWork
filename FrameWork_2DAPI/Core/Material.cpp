#include "Material.h"

void Material::SetUV(int left, int top, int width, int height)
{
    uvRect = Gdiplus::Rect(left, top, width, height);
}
