#pragma once
namespace Led
{
	struct FreeTypeGlyph
	{
		uint textureID; // ID handle of the glyph texture
		Point2 size; // Size of glyph
		Point2 bearing; // Offset from baseline to left/top of glyph
		uint advance; // Offset to advance to next glyph
	};

	class FreeType
	{
		friend class FreeTypeFont;
	private:
		FT_Library ft;

	public:
		FreeType();
		~FreeType();
		FreeTypeFont *LoadFont(const string &filename);
	};

	class FreeTypeFont
	{
	private:
		FT_Face face;
	public:
		FreeTypeFont(FreeType *freeType, const string& filename);
		~FreeTypeFont();
		void SetPixelSize(uint width, uint height);
		void Update();
	};
}
