#include "Renderer.h"

#include <cassert>
#include <SDL_image.h>

namespace Application
{
	void Renderer::Init(SDL_Window* Window)
	{
		SDLRenderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	}

	void Renderer::Clear()
	{
		SDL_DestroyRenderer(SDLRenderer);

		while (!Textures.empty())
		{
			const auto Texture = Textures.begin();
			SDL_DestroyTexture(Texture->second);
			Textures.erase(Texture->first);
		}
	}

	void Renderer::DrawTexture(SDL_Texture* Texture, const SDL_Rect& ImageRect, double Angle) const
	{
		const SDL_FPoint Center { ImageRect.w / 2.f, ImageRect.h / 2.f };
		const SDL_FRect Rect {
			static_cast<float>(ImageRect.x),
			static_cast<float>(ImageRect.y),
			static_cast<float>(ImageRect.w),
			static_cast<float>(ImageRect.h),
		};
		SDL_RenderCopyExF(SDLRenderer, Texture, nullptr, &Rect, Angle, &Center, SDL_FLIP_NONE);
	}

	void Renderer::DrawCanvas() const
	{
		SDL_SetRenderDrawColor(SDLRenderer, 203, 225, 239, 255);
		SDL_RenderPresent(SDLRenderer);
	}

	void Renderer::ClearCanvas() const
	{
		SDL_RenderClear(SDLRenderer);
	}

	void Renderer::SetDrawColor(Uint8 R, Uint8 G, Uint8 B) const
	{
		SDL_SetRenderDrawColor(SDLRenderer, R, G, B, 255);
	}

	void Renderer::DrawRectangle(const SDL_Rect& Rect) const
	{
		SDL_Rect DrawRect{
			Rect.x * 32,
			Rect.y * 32,
			Rect.w * 32,
			Rect.h * 32,
		};

		SDL_RenderDrawRect(SDLRenderer, &DrawRect);
	}


	SDL_Texture* Renderer::GetImage(const std::string& FilePath)
	{
		if (Textures.contains(FilePath)) { return Textures.at(FilePath); }

		SDL_Surface* Surface = IMG_Load(FilePath.c_str());
		assert(Surface);

		SDL_Texture* Texture = SDL_CreateTextureFromSurface(SDLRenderer, Surface);
		assert(Texture);

		SDL_FreeSurface(Surface);
		Textures[FilePath] = Texture;

		return Texture;
	}
}
