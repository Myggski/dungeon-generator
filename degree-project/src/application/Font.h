#pragma once

#include <memory>
#include <string>
#include "Renderer.h"
#include <SDL_ttf.h>

namespace Application
{
	class Font
	{
	public:
		static Font& GetInstance();
		static void CreateInstance(const Renderer& Renderer);

		bool Load(const std::string& Name, const std::string& FilePath, int FontSize);
		void Draw(const std::string& Name, const std::string& Text, float X, float Y) const;
		void Clear();

		TTF_Font* Get(const std::string& Name) const;

	private:
		Font(const Renderer& Renderer);

	private:
		static std::unique_ptr<Font> Instance;
		const Renderer& Renderer;
		std::unordered_map<std::string, TTF_Font*> Fonts;
		
	};
}
