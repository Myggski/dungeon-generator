#pragma once

#include <memory>
#include <SDL_events.h>
#include <SDL_scancode.h>
#include <unordered_map>

namespace Application
{
    struct KeyboardKeyData
    {
    public:
        bool IsHeld() const { return NumberOfRepeats > 0; }
        KeyboardKeyData() = default;
        KeyboardKeyData(SDL_Scancode Scancode);

    public:
        size_t NumberOfRepeats{ 0 };
        SDL_Scancode Scancode{ SDL_SCANCODE_UNKNOWN };
    };

    class Keyboard
    {
    public:
        static void CreateInstance();
        static Keyboard& GetInstance();

        void OnKeyPressed(SDL_Event Event);
        void OnKeyReleased(SDL_Event Event);
        bool IsKeyPressed(const SDL_Scancode Scancode) const;
        bool IsKeyPressedOnce(const SDL_Scancode Scancode);
        bool IsKeyReleased(const SDL_Scancode Scancode) const;
        bool IsKeyHeld(const SDL_Scancode Scancode) const;

    private:
        Keyboard() = default;

    private:
        static std::unique_ptr<Keyboard> Instance;
        std::unordered_map<SDL_Scancode, KeyboardKeyData> Keys;
    };
}