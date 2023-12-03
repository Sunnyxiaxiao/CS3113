#include "Level2.hpp"
#include "Utility.hpp"

#define LEVEL_WIDTH 36
#define LEVEL_HEIGHT 8

unsigned int LEVEL_2_DATA[] =
{
    4,  0,  0,  85,  85,  0,  0,  0,  0,  0,  29,  30,  31, 29, 30, 31, 0,  0,  0,  0,  0,  0,  0,  0,   0, 29, 30, 31,  0,  0,  0,  0,  0,  0,  0,  0,
    4,  0,  0,  101,  101,  0,  0,  0,  0,  0,  29,  30,  31, 29, 30, 31, 0,  0,  0,  0,  0,  0,  0,  0,   0, 29, 30, 31,  0,  0,  0,  0,  0,  0,  0,  0,
    4,  0,  0,  85,  85,  0,  0,  0,  0,  0,  29,  30,  31, 29, 30, 31, 0,  0,  0,  0,  0,  0,  0,  0,   0, 29, 30, 31,  0,  0,  0,  0,  0,  0,  0,  0,
    4,  0,  0,  101,  101,  0,  0,  0,  0,  0,  29,  30,  31, 29, 30, 31, 0,  0,  0,  0,  0,  8,  0,  0,   0, 29, 30, 31,  0,  0,  0,  0,  29,  30,  31,  0,
    4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8,  24, 8,  0,   0, 29, 30, 31,  0,  0,  0,  0,  0,  0,  0,  0,
    4,  0,  0,  18,  18,  0,  0,  0,  0, 17, 18, 18, 18, 19,  0,  0,  0,  0,  0, 8, 24, 0, 24,  8,   0,  0,  0,  0,  0,  0,  0, 29, 30, 30, 30, 31,
    4, 17, 18, 34, 34, 17, 18, 18, 19, 33, 34, 34, 34, 35,  17, 18, 18, 19, 0,  24,  0,  0, 0, 24, 17, 18, 18, 18, 19,  0,  0,  0,  0,  0,  0,  0,
    4, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 35,  34, 34, 34, 34, 0, 0,  0,  0, 0, 0, 33, 34, 34, 34, 35, 29, 30, 30, 30, 30, 30, 30
};

Level2::~Level2()
{
    delete [] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void Level2::initialise()
{
    GLuint map_texture_id = Utility::load_texture("/Users/zouyurou/Desktop/CS3113/SDL_HW5/SDL_HW5/assets/images/tilemap.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_2_DATA, map_texture_id, 1.0f, 16, 7);
    
    GLuint player_texture_id = Utility::load_texture("/Users/zouyurou/Desktop/CS3113/SDL_HW5/SDL_HW5/player.jpeg");
    m_state.player = new Entity();
    m_state.player->entity_type = PLAYER;
    m_state.player->set_position(glm::vec3(13.0f, -4.1f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->speed = 2.5f;
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->width = 0.7;
    m_state.player->textureID = player_texture_id;
    m_state.player->jump_power = 5.0f;

    GLuint enemy_texture_id = Utility::load_texture("/Users/zouyurou/Desktop/CS3113/SDL_HW5/SDL_HW5/assets/images/enemy.jpeg");
    m_state.enemies = new Entity[ENEMY_COUNT];
    m_state.enemies[0].entity_type = ENEMY;
    m_state.enemies[0].ai_type = GUARD;
    m_state.enemies[0].ai_state = IDLE;
    m_state.enemies[0].textureID = enemy_texture_id;
    m_state.enemies[0].set_position(glm::vec3(20.0f, 0.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_state.enemies[0].speed = 1.0f;
    m_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    m_state.jump_sfx = Mix_LoadWAV("/Users/zouyurou/Desktop/CS3113/SDL_HW5/SDL_HW5/assets/audio/jump.wav");
}

void Level2::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map);
    
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].update(delta_time, m_state.player, 0, 0, m_state.map);
    }
}


void Level2::render(ShaderProgram *program)
{
    m_state.map->render(program);
    m_state.player->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].render(program);
    }
}
