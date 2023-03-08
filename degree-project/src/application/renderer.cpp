#include "renderer.h"

#include <cassert>
#include <SDL_image.h>

namespace application
{
	void renderer::init(SDL_Window* window)
	{
		SDL_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		SDL_SetRenderDrawColor(SDL_renderer, 203, 225, 239, 255);
	}

	void renderer::clear()
	{
		SDL_DestroyRenderer(SDL_renderer);

		while (!textures.empty())
		{
			const auto texture = textures.begin();
			SDL_DestroyTexture(texture->second);
			textures.erase(texture->first);
		}
	}

	void renderer::draw_texture(SDL_Texture* texture, const SDL_Rect& image_rect, double angle) const
	{
		const SDL_FPoint center { image_rect.w / 2.f, image_rect.h / 2.f };
		const SDL_FRect rect {
			static_cast<float>(image_rect.x),
			static_cast<float>(image_rect.y),
			static_cast<float>(image_rect.w),
			static_cast<float>(image_rect.h),
		};
		SDL_RenderCopyExF(SDL_renderer, texture, nullptr, &rect, angle, &center, SDL_FLIP_NONE);
	}

	void renderer::draw_canvas() const
	{
		SDL_RenderPresent(SDL_renderer);
	}

	void renderer::clear_canvas() const
	{
		SDL_RenderClear(SDL_renderer);
	}

	SDL_Texture* renderer::get_image(const std::string& file_path)
	{
		if (textures.contains(file_path)) { return textures.at(file_path); }

		SDL_Surface* surface = IMG_Load(file_path.c_str());
		assert(surface);

		SDL_Texture* texture = SDL_CreateTextureFromSurface(SDL_renderer, surface);
		assert(texture);

		SDL_FreeSurface(surface);
		textures[file_path] = texture;

		return texture;
	}
}
