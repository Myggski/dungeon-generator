#pragma once
#include <string>

#include <unordered_map>
#include <SDL_video.h>
#include <SDL.h>

namespace application
{
	class renderer
	{
	public:
		void init(SDL_Window* window);
		void clear();
		void draw_texture(SDL_Texture* texture, const SDL_Rect& image_rect, double angle = 0) const;
		void draw_canvas() const;
		void clear_canvas() const;
		void set_draw_color(Uint8 r, Uint8 g, Uint8 b) const;
		void draw_rectangle(const SDL_Rect& rect) const;
		SDL_Texture* get_image(const std::string& file_path);

	private:
		SDL_Renderer* SDL_renderer;
		std::unordered_map<std::string, SDL_Texture*> textures;
	};
}