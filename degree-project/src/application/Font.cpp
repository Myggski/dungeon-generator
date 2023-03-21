
#include "Font.h"
#include <stdexcept>

namespace Application
{
    std::unique_ptr<Font> Font::Instance = nullptr;

    Font::Font(const Application::Renderer& Renderer)
		: Renderer(Renderer) { }


    Font& Font::GetInstance()
    {
        if (Instance == nullptr)
        {
            throw std::logic_error("Instance of Font has not been created yet, or has been destroyed!");
        }

        return *Instance;
    }

    void Font::CreateInstance(const Application::Renderer& Renderer)
    {
        if (TTF_Init() >= 0)
        {
            if (Instance != nullptr) {
                throw std::logic_error("Instance of Font has already been created!");
            }

            Instance = std::make_unique<Font>(Font(Renderer));
        }
    }

    bool Font::Load(const std::string& Name, const std::string& FilePath, int FontSize)
    {
        TTF_Font* font = TTF_OpenFont(FilePath.c_str(), FontSize);

        if (!font)
        {
            return false;
        }

        Fonts[Name] = font;

        return true;
    }

    void Font::Draw(const std::string& Name, const std::string& Text, float X, float Y) const
    {
        TTF_Font* font = Fonts.at(Name);
        if (!font) return;  // Error

        const SDL_Color color = { 35, 35, 35, 255 };  // White color
        SDL_Surface* surface = TTF_RenderText_Blended(font, Text.c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer.SDLRenderer, surface);

        int w, h;
        TTF_SizeText(font, Text.c_str(), &w, &h);

        const SDL_FRect rect{ X, Y, static_cast<float>(w), static_cast<float>(h) };

        SDL_RenderCopyF(Renderer.SDLRenderer, texture, nullptr, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    TTF_Font* Font::Get(const std::string& Name) const
    {
        return Fonts.at(Name);
    }

    void Font::Clear()
    {
        for (const auto& Font : Fonts) {
            TTF_CloseFont(Font.second);
        }

        Fonts.clear();
        TTF_Quit();
    }
}
