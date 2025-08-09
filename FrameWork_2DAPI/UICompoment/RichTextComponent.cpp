//#pragma once
#include "RichTextComponent.h"
#include "../Core/GameObject.h"
#include "../Compoment/Transform.h"
#include "../Core/DefineHeader.h"


RichTextComponent::~RichTextComponent( )
{
	transform = nullptr;
}

void RichTextComponent::Initialize( )
{

}
void RichTextComponent::Initialize_AddCompoment( )
{
	transform = this->owner->GetComponent<Transform>();

	//Gdiplus::FontFamily family(fontFamily.c_str());
	//Gdiplus::Font font(&family, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	family = new Gdiplus::FontFamily(fontFamily.c_str( ));
	font = new Gdiplus::Font(family, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

}
void RichTextComponent::Release( )
{
	SAFEDELETE(font);
	SAFEDELETE(family);
}


Gdiplus::Color RichTextComponent::ParseColor(const std::wstring& hex)
{
	unsigned int value = 0;
	std::wstringstream ss;
	ss << std::hex << hex;
	ss >> value;
	BYTE a = ( value >> 24 ) & 0xFF;
	BYTE r = ( value >> 16 ) & 0xFF;
	BYTE g = ( value >> 8 ) & 0xFF;
	BYTE b = value & 0xFF;
	return Gdiplus::Color(a, r, g, b);
}

void RichTextComponent::ParseRichText( )
{
	parsedSegments.clear( );
	std::wregex tag(L"<color=#([0-9A-Fa-f]{8})>(.*?)</color>");
	std::wsregex_iterator iter(rawText.begin( ), rawText.end( ), tag);
	std::wsregex_iterator end;

	size_t lastPos = 0;
	for ( ; iter != end; ++iter ) {
		size_t matchPos = iter->position( );
		if ( matchPos > lastPos ) {
			parsedSegments.push_back({
				rawText.substr(lastPos, matchPos - lastPos),
				Gdiplus::Color(255, 0, 0, 0)
				});
		}
		parsedSegments.push_back({
			( *iter )[ 2 ].str( ),
			ParseColor(( *iter )[ 1 ].str( ))
			});
		lastPos = matchPos + iter->length( );
	}

	if ( lastPos < rawText.size( ) ) {
		parsedSegments.push_back({
			rawText.substr(lastPos),
			Gdiplus::Color(255, 0, 0, 0)
			});
	}
}

void RichTextComponent::Update(float dt)
{
}
void RichTextComponent::Render(HDC hdc)
{
	Draw(hdc);
}


void RichTextComponent::Draw(HDC hdc) {
	//if ( !transform ) return;

	//Gdiplus::Graphics graphics(hdc);
	//graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);

	////Gdiplus::FontFamily family(fontFamily.c_str( ));
	////Gdiplus::Font font(&family, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

	//Gdiplus::StringFormat format;
	//format.SetAlignment(hAlign);
	//format.SetLineAlignment(vAlign);
	//format.SetFormatFlags(Gdiplus::StringFormatFlagsLineLimit);

	//float lineHeight = fontSize + 4.0f;
	//float y = transform->position.y;

	//for ( const auto& seg : parsedSegments ) {
	//	std::wistringstream stream(seg.text);
	//	std::wstring line;
	//	while ( std::getline(stream, line, L'\n') ) {
	//		if ( y > transform->position.y + height ) break;

	//		Gdiplus::RectF layoutRect(transform->position.x, y, ( Gdiplus::REAL ) width, ( Gdiplus::REAL ) lineHeight);
	//		Gdiplus::SolidBrush brush(seg.color);
	//		graphics.DrawString( line.c_str( ), -1, font, layoutRect, &format, &brush);

	//		y += lineHeight;
	//	}
	//}



	if ( !transform ) return;

	Gdiplus::Graphics graphics(hdc);
	graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);

	//Gdiplus::FontFamily family(fontFamily.c_str( ));
	//Gdiplus::Font font(&family, fontSize, fontStyle, Gdiplus::UnitPixel);

	Gdiplus::StringFormat format;
	format.SetAlignment(hAlign);
	format.SetLineAlignment(vAlign);
	format.SetFormatFlags(Gdiplus::StringFormatFlagsLineLimit);

	float lineHeight = fontSize + 4.0f;

	Vec2 localpos = transform->getLocalPosition( );

	float y = localpos.y;// transform->localposition.y;

	for ( const auto& seg : parsedSegments ) {
		std::wistringstream stream(seg.text);
		std::wstring line;
		while ( std::getline(stream, line, L'\n') ) {
			if ( y > localpos.y + height ) break;
			Gdiplus::RectF layoutRect(localpos.x, y, ( Gdiplus::REAL ) width, ( Gdiplus::REAL ) lineHeight);
			if ( useOutline ) {
				for ( int dx = -1; dx <= 1; ++dx ) {
					for ( int dy = -1; dy <= 1; ++dy ) {
						if ( dx == 0 && dy == 0 ) continue;
						Gdiplus::RectF outlineRect = layoutRect;
						outlineRect.X += dx;
						outlineRect.Y += dy;
						Gdiplus::SolidBrush outlineBrush(outlineColor);
						graphics.DrawString(line.c_str( ), -1, font, outlineRect, &format, &outlineBrush);
					}
				}
			}

			Gdiplus::SolidBrush brush(seg.color);
			graphics.DrawString(line.c_str( ), -1, font, layoutRect, &format, &brush);
			y += lineHeight;
		}
	}
}
