#include "Renderer.h"

#include <cassert>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer.h>
#include <SDL_image.h>

namespace Application
{
	void Renderer::Init(SDL_Window* Window)
	{
		SDLRenderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

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
