#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Math.hpp"

class Entity {
    public:
        Entity(Vector2f p_pos, SDL_Texture* p_tex);
        Vector2f& getPos();
        double getX();
        double getY();
        SDL_Texture* getTex();
        SDL_Rect getCurrentFrame();
    private:
        Vector2f pos;
        SDL_Rect currentFrame;
        SDL_Texture* tex;
};
