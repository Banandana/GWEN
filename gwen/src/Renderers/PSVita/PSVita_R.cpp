#include "Gwen/Gwen.h"
#include "Gwen/BaseRender.h"
#include "Gwen/Utility.h"
#include "Gwen/Font.h"
#include "Gwen/Texture.h"
#include "Gwen/Renderers/PSVita.h"

#include <vita2d.h>
#include <psp2/gxm.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/io/fcntl.h>
namespace Gwen
{
	namespace Renderer
	{

		vita2d_font *defaultFont;

		PSVita::PSVita(const char* fontPath)
		{
			//pgf = vita2d_load_default_pgf();
			SceUID fd = sceIoOpen(fontPath, SCE_O_RDONLY, 0);
			int fileSize = sceIoLseek(fd, 0, SCE_SEEK_END);
			sceIoClose(fd);
			fd = sceIoOpen(fontPath, SCE_O_RDONLY, 0);
			void* buffer = malloc(fileSize);
			sceIoRead(fd, buffer, fileSize);
			sceIoClose(fd);

			defaultFont = vita2d_load_font_mem(buffer, fileSize);
		}

		PSVita::~PSVita()
		{
		}

		void PSVita::SetDrawColor( Gwen::Color dcolor )
		{
			color = RGBA8(dcolor.r, dcolor.g, dcolor.b, dcolor.a);
		}

		void PSVita::LoadFont( Gwen::Font* pFont )
		{
			pFont->data = defaultFont;
		}

		void PSVita::FreeFont( Gwen::Font* pFont )
		{
			pFont->data = NULL;
		}

		void PSVita::RenderText( Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString & text )
		{
			Translate(pos.x, pos.y);
			std::string str = Gwen::Utility::UnicodeToString(text);
			int w = vita2d_font_text_width(defaultFont, (int)pFont->size, str.c_str());
			int h = vita2d_font_text_height(defaultFont, (int)pFont->size, str.c_str());
			vita2d_font_draw_text(defaultFont, pos.x, pos.y + (int)pFont->size, color, (int)pFont->size, str.c_str());
		}

		Gwen::Point PSVita::MeasureText( Gwen::Font* pFont, const Gwen::UnicodeString & text )
		{
			std::string str = Gwen::Utility::UnicodeToString(text);

			int w = vita2d_font_text_width(defaultFont, (int)pFont->size, str.c_str());
			int h = vita2d_font_text_height(defaultFont, (int)pFont->size, str.c_str());
			return Gwen::Point( w, h );
		}

		void PSVita::StartClip()
		{
			Gwen::Rect rect = ClipRegion();
			vita2d_set_region_clip(SCE_GXM_REGION_CLIP_INSIDE, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h);
		}


		void PSVita::EndClip()
		{
			vita2d_set_region_clip(SCE_GXM_REGION_CLIP_NONE, 0, 0, 960, 544);
		}

		void PSVita::LoadTexture( Gwen::Texture* pTexture )
		{
			std::string path = pTexture->name.m_String;

			vita2d_texture* texture = NULL;

			if (path.substr(path.find_last_of(".") + 1) == "png") {
				texture = vita2d_load_PNG_file(path.c_str());
			} else if (path.substr(path.find_last_of(".") + 1) == "jpg") {
				texture = vita2d_load_JPEG_file(path.c_str());
			} else if (path.substr(path.find_last_of(".") + 1) == "jpeg") {
				texture = vita2d_load_JPEG_file(path.c_str());
			} else if (path.substr(path.find_last_of(".") + 1) == "bmp") {
				texture = vita2d_load_BMP_file(path.c_str());
			} else {
				pTexture->failed = true;
				return;
			}

			pTexture->width = (int)vita2d_texture_get_width(texture);
			pTexture->height = (int)vita2d_texture_get_height(texture);
			pTexture->data = texture;
		}

		void PSVita::FreeTexture( Gwen::Texture* pTexture )
		{
			vita2d_free_texture(reinterpret_cast<vita2d_texture*>(pTexture->data));
		}

		void PSVita::DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect rect, float u1, float v1, float u2, float v2 )
		{
			Translate( rect );
			rect.x += 0.5f;	// Draw at pixel centre.
			rect.y += 0.5f;

			const unsigned int w = pTexture->width;
			const unsigned int h = pTexture->height;

			vita2d_draw_texture_part_scale(
				reinterpret_cast<vita2d_texture*>(pTexture->data),
				// Destination X
				rect.x,
				// Destination Y
				rect.y,
				// Texture X
				u1 * w,
				// Texture Y
				v1 * h,
				// Texture W
				( u2 - u1 ) * (float)w,
				// Texture H
				( v2 - v1 ) * (float)h,
				// X scale
				(float)rect.w / (( u2 - u1 ) * (float)w),
				// Y scale
				(float)rect.h / (( v2 - v1 ) * (float)h)
			);
		}

		Gwen::Color PSVita::PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color & col_default )
		{
			Gwen::Color gcol;

			return gcol;
		}

		void PSVita::DrawFilledRect( Gwen::Rect rect )
		{
			Translate( rect );
			rect.x += 0.5f;	// Draw at pixel centre.
			rect.y += 0.5f;
			vita2d_draw_rectangle(rect.x, rect.y, rect.w, rect.h, color);
		}

		void PSVita::DrawLinedRect( Gwen::Rect rect )
		{
			Translate( rect );
			rect.x += 0.5f;	// Draw at pixel centre.
			rect.y += 0.5f;

			// Top
			vita2d_draw_line(rect.x, rect.y, rect.x + rect.w, rect.y, color);

			// Bottom
			vita2d_draw_line(rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h, color);

			// Left
			vita2d_draw_line(rect.x, rect.y, rect.x, rect.y + rect.h, color);

			// Right
			vita2d_draw_line(rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h, color);
		}

		void PSVita::DrawPixel( int x, int y )
		{
			vita2d_draw_pixel(x, y, color);
		}

		bool PSVita::BeginContext( Gwen::WindowProvider* pWindow )
		{
			vita2d_clear_screen();
			return true;
		}

		bool PSVita::EndContext( Gwen::WindowProvider* pWindow )
		{
			return true;
		}

		bool PSVita::PresentContext( Gwen::WindowProvider* pWindow )
		{
			return true;
		}

	}
}
