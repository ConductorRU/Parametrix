#include "stdafx.h"
#include "Math/Vector.h"
#include "FreeType.h"
namespace Led
{
	FreeType::FreeType()
	{
		if (FT_Init_FreeType(&ft))
		{
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		}
	}

    FreeType::~FreeType()
    {
        FT_Done_FreeType(ft);
    }

	FreeTypeFont* FreeType::LoadFont(const string& filename)
	{
		FreeTypeFont* font = new FreeTypeFont(this, filename);
		return font;
	}

	FreeTypeFont::FreeTypeFont(FreeType* freeType, const string& filename)
	{
		if (FT_New_Face(freeType->ft, filename.c_str(), 0, &face))
		{
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		}
	}

    FreeTypeFont::~FreeTypeFont()
    {
        FT_Done_Face(face);
    }

	void FreeTypeFont::SetPixelSize(uint width, uint height)
	{
		FT_Set_Pixel_Sizes(face, width, height);
	}

	void FreeTypeFont::Update()
	{
        for (unsigned char c = 0; c < 128; c++)
        {
            // load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            /*// generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            Characters.insert(std::pair<char, Character>(c, character));*/
        }
	}
}
