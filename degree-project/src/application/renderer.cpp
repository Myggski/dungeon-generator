#include "Renderer.h"

#include <cassert>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer.h>
#include <SDL_image.h>

#include "Camera.h"

namespace Application
{
	void Renderer::Init(SDL_Window* Window)
	{
		SDLRenderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui::StyleColorsDark();
		ImGui_ImplSDL2_InitForSDLRenderer(Window, SDLRenderer);
		ImGui_ImplSDLRenderer_Init(SDLRenderer);
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

	void Renderer::DrawTexture(SDL_Texture* Texture, const SDL_FRect& ImageRect, double Angle) const
	{
		const SDL_FPoint Center {
			ImageRect.w / 2.f,
			ImageRect.h / 2.f
		};
		const SDL_FRect DrawRect{
			ImageRect.x * TILE_SIZE - Camera::GetInstance().PositionX,
			ImageRect.y * TILE_SIZE - Camera::GetInstance().PositionY,
			ImageRect.w * TILE_SIZE,
			ImageRect.h * TILE_SIZE,
		};

		SDL_RenderCopyExF(SDLRenderer, Texture, nullptr, &DrawRect, Angle, &Center, SDL_FLIP_NONE);
		SDL_SetTextureBlendMode(Texture, SDL_BLENDMODE_BLEND);
	}

	void Renderer::DrawCanvas() const
	{
		ImGui::Render();
		SDL_SetRenderDrawColor(SDLRenderer, 199, 220, 208, 255);
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(SDLRenderer);
	}

	void Renderer::ClearCanvas() const
	{
		SDL_RenderClear(SDLRenderer);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
	}

	void Renderer::SetDrawColor(Uint8 R, Uint8 G, Uint8 B) const
	{
		SDL_SetRenderDrawColor(SDLRenderer, R, G, B, 255);
	}

	void Renderer::DrawRectangle(const SDL_FRect& Rect, std::tuple<float, float> TileSize) const
	{
		auto [Width, Height] = TileSize;

		SDL_FRect DrawRect{
			Rect.x * Width - Camera::GetInstance().PositionX,
			Rect.y * Height - Camera::GetInstance().PositionY,
			Rect.w * Width,
			Rect.h * Height,
		};

		SDL_RenderDrawRectF(SDLRenderer, &DrawRect);
	}

	void Renderer::DrawFilledRectangle(const SDL_FRect& Rect, std::tuple<float, float> TileSize) const
	{
		auto [Width, Height] = TileSize;

		SDL_FRect DrawRect{
			Rect.x * Width - Camera::GetInstance().PositionX,
			Rect.y * Height - Camera::GetInstance().PositionY,
			Rect.w * Width,
			Rect.h * Height,
		};

		SDL_RenderFillRectF(SDLRenderer, &DrawRect);
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
