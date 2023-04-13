#pragma once
#include <string>

#include <unordered_map>
#include <SDL_video.h>
#include <SDL.h>

namespace Application
{
	class Font;

	class Renderer
	{
	public:
		void Init(SDL_Window* Window);
		void Clear();
		void DrawTexture(SDL_Texture* Texture, const SDL_FRect& ImageRect, double Angle = 0) const;
		void DrawCanvas() const;
		void ClearCanvas() const;
		void SetDrawColor(Uint8 R, Uint8 G, Uint8 B) const;
		void DrawRectangle(const SDL_FRect& Rect, std::tuple<float, float> TileSize = std::make_tuple(TILE_SIZE, TILE_SIZE)) const;
		void DrawFilledRectangle(const SDL_FRect& Rect, std::tuple<float, float> TileSize = std::make_tuple(TILE_SIZE, TILE_SIZE)) const;
		SDL_Texture* GetImage(const std::string& FilePath);
		inline float GetTileSize() const
		{
			return TILE_SIZE;
		}

	private:
		static constexpr float TILE_SIZE { 64.f };
		SDL_Renderer* SDLRenderer = nullptr;
		std::unordered_map<std::string, SDL_Texture*> Textures;

		friend class Font;
	};
}
