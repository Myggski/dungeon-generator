#include "Keyboard.h"

namespace Application
{
    KeyboardKeyData::KeyboardKeyData(SDL_Scancode Scancode) : Scancode(Scancode) { }

    void Keyboard::OnKeyPressed(SDL_Event Event)
    {
        if (IsKeyReleased(Event.key.keysym.scancode))
        {
            Keys[Event.key.keysym.scancode] = KeyboardKeyData(Event.key.keysym.scancode);
        }
        else
        {
            Keys.at(Event.key.keysym.scancode).NumberOfRepeats += 1;
        }
    }

    void Keyboard::OnKeyReleased(SDL_Event Event)
    {
        Keys.erase(Event.key.keysym.scancode);
    }

    bool Keyboard::IsKeyPressed(const SDL_Scancode Scancode) const
    {
        return Keys.contains(Scancode);
    }

    bool Keyboard::IsKeyPressedOnce(const SDL_Scancode Scancode)
    {
        if (!IsKeyPressed(Scancode))
        {
            return false;
        }

        Keys.at(Scancode).NumberOfRepeats += 1;

        return IsKeyPressed(Scancode) && Keys.at(Scancode).NumberOfRepeats == 1;
    }


    bool Keyboard::IsKeyReleased(const SDL_Scancode Scancode) const
    {
        return !IsKeyPressed(Scancode);
    }

    bool Keyboard::IsKeyHeld(const SDL_Scancode Scancode) const
    {
        return Keys.contains(Scancode);
    }
}