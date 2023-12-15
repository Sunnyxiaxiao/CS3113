#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Utility.hpp"
#include "Entity.hpp"
#include "Map.hpp"


struct GameState
{
    // ————— GAME OBJECTS ————— //
    Map *map;
    Map *decoration;
    Entity *player;
    Entity *enemies;
    Entity *potion;
    Entity *mage;
    
    // ————— AUDIO ————— //
    Mix_Music *bgm;
    Mix_Chunk *potion_sfx;
    
    // ————— POINTERS TO OTHER SCENES ————— //
    int next_scene_id;
};

class Scene {
public:
    // ————— ATTRIBUTES ————— //
    int m_number_of_enemies = 1;
    int m_number_of_potions = 1;
    
    GameState m_state;
    
    // ————— METHODS ————— //
    virtual void initialise() = 0;
    virtual void update(float delta_time) = 0;
    virtual void render(ShaderProgram *program) = 0;
    
    // ————— GETTERS ————— //
    GameState const get_state()             const { return m_state;             }
    int       const get_number_of_enemies() const { return m_number_of_enemies; }
    int       const get_number_of_potions() const { return m_number_of_potions; }
};
