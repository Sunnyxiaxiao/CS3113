#include "Level2.hpp"
#include "Utility.hpp"

#define LEVEL_WIDTH 32
#define LEVEL_HEIGHT 32
using namespace std;

unsigned int LEVEL_2_DATA[] =
{
    40,40,40,42,42,42,40,24,84,116,24,24,24,40,42,42,42,40,12,12,12,12,12,40,40,40,40,40,40,40,40,40,40,40,40,42,42,42,40,24,24,24,24,24,24,40,42,42,42,40,12,12,12,12,12,40,42,42,42,39,42,42,42,42,40,40,40,42,42,42,40,40,40,24,124,24,24,40,42,42,42,40,12,12,12,40,40,40,42,42,42,39,42,42,42,42,40,40,40,42,42,42,40,24,24,24,24,24,24,40,42,42,42,40,12,12,12,40,42,42,42,42,42,39,42,42,42,42,40,40,40,42,42,42,40,24,24,24,24,24,24,40,42,42,42,40,40,40,40,40,42,42,42,42,42,39,42,42,42,42,40,40,40,42,42,42,40,40,40,40,40,40,40,40,42,42,42,42,42,42,42,42,42,42,42,42,42,39,42,42,42,42,40,40,40,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,39,42,42,42,42,40,40,40,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,40,40,40,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,14,14,14,14,14,14,14,42,42,42,42,42,40,40,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,14,42,42,42,42,42,14,42,42,42,42,42,40,40,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,14,42,42,42,42,42,14,42,42,42,42,42,40,40,42,42,42,42,42,42,42,42,42,14,14,14,14,14,42,42,42,42,14,42,42,42,42,42,14,42,42,42,42,42,40,40,42,42,42,42,42,42,42,42,42,14,42,42,42,14,42,42,42,42,14,42,42,42,42,42,14,42,42,42,42,42,40,40,69,70,70,70,71,42,42,42,42,14,42,42,42,14,42,42,42,42,14,42,42,42,42,42,14,42,42,42,42,42,40,40,81,48,48,48,83,42,42,42,42,14,42,42,42,14,42,42,42,42,14,42,42,42,42,42,14,42,42,42,42,42,40,40,81,48,82,48,83,42,42,42,42,14,42,42,42,14,42,42,42,42,14,42,42,42,42,42,14,42,42,42,42,42,40,40,81,48,48,48,83,42,42,42,42,14,42,42,42,14,42,42,42,42,14,42,42,42,42,42,14,42,42,42,42,42,40,40,93,94,94,94,95,42,42,42,42,14,42,42,42,14,42,42,42,42,14,42,42,42,42,42,14,42,42,42,42,42,40,40,42,42,42,42,42,42,42,42,42,14,42,42,42,14,42,42,42,42,14,42,42,42,42,42,14,42,42,42,42,42,40,40,42,42,42,42,42,42,42,42,42,14,42,42,42,14,42,42,42,42,14,42,42,42,42,42,14,14,14,14,14,14,40,40,42,42,42,42,42,42,42,42,42,14,42,42,42,14,42,42,42,42,14,42,42,42,42,42,42,42,42,42,42,14,40,40,42,42,42,42,42,42,42,42,42,14,42,42,42,14,42,42,42,42,14,42,42,42,42,42,42,42,42,42,42,14,40,40,14,14,14,14,14,42,42,42,42,14,42,42,42,14,42,42,42,42,14,42,42,42,42,42,42,42,42,42,42,14,40,40,42,42,42,42,14,42,42,42,42,14,42,42,42,14,42,42,42,42,14,42,42,42,42,42,42,42,42,42,42,14,40,42,42,42,42,42,14,42,42,42,42,14,42,42,42,14,42,42,42,42,14,42,42,42,42,42,42,42,42,42,42,14,40,42,110,42,42,42,14,42,42,42,42,14,42,42,42,14,14,14,14,14,14,42,42,42,42,42,42,42,42,42,42,14,40,42,42,42,42,42,14,42,42,42,42,14,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,14,40,42,42,42,42,42,14,42,42,42,42,14,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,14,40,42,42,42,42,42,14,14,14,14,14,14,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,14,40,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,14,40,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,14,40,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,14
};

unsigned int LEVEL_2_DECORATION[] = {
    40, 40, 40, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 40, 0, 0, 0, 0, 0, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 40, 0, 0, 0, 0, 0, 40, 0, 0, 0, 39, 0, 0, 0, 0, 40, 40, 40, 0, 0, 0, 40, 40, 40, 0, 0, 0, 0, 40, 0, 0, 0, 40, 0, 0, 0, 40, 40, 40, 0, 0, 0, 39, 0, 0, 0, 0, 40, 40, 40, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 40, 0, 0, 0, 40, 0, 0, 0, 0, 0, 39, 0, 0, 0, 0, 40, 40, 40, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 40, 40, 40, 40, 40, 0, 0, 0, 0, 0, 39, 0, 0, 0, 0, 40, 40, 40, 0, 0, 0, 40, 40, 40, 40, 40, 40, 40, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 39, 0, 0, 0, 0, 40, 40, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 39, 0, 0, 0, 0, 40, 40, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 40, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 14, 14, 14, 14, 14, 14, 0, 0, 0, 0, 0, 40, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 40, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 40, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 14, 14, 14, 14, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 40, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 40, 40, 69, 70, 70, 70, 71, 0, 0, 0, 0, 14, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 40, 40, 81, 0, 0, 0, 83, 0, 0, 0, 0, 14, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 40, 40, 81, 0, 82, 0, 83, 0, 0, 0, 0, 14, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 40, 40, 81, 0, 0, 0, 83, 0, 0, 0, 0, 14, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 40, 40, 93, 94, 94, 94, 95, 0, 0, 0, 0, 14, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 40, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 40, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 14, 14, 14, 14, 14, 14, 40, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 40, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 40, 40, 14, 14, 14, 14, 14, 0, 0, 0, 0, 14, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 40, 40, 0, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 40, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 40, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 0, 0, 14, 14, 14, 14, 14, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 40, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 40, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 40, 0, 0, 0, 0, 0, 14, 14, 14, 14, 14, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14
};

Level2::~Level2()
{
    delete [] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    delete    m_state.decoration;
    delete    m_state.potion;
    Mix_FreeChunk(m_state.potion_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void Level2::initialise()
{
    GLuint map_texture_id = Utility::load_texture("/Users/zouyurou/Desktop/CS3113/Final\ Project/Final\ Project/assets/images/tilemap.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_2_DATA, map_texture_id, 1.0f, 12, 11);
    m_state.decoration = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_2_DECORATION, map_texture_id, 1.0f, 12, 11);
    
    cout << "load map_texture_id ok" << endl;
    
    // player
    GLuint player_texture_id = Utility::load_texture("/Users/zouyurou/Desktop/CS3113/Final\ Project/Final\ Project/assets/images/player1.png");
    cout << player_texture_id << endl;
    m_state.player = new Entity();
    m_state.player->entity_type = PLAYER;
    m_state.player->set_position(glm::vec3(13.5f, 13.5f, 0.0f));
//    m_state.player->set_position(glm::vec3(-8.5f, -8.5f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->speed = 5.0f;
    m_state.player->width = 0.7f;
    m_state.player->height = 1;
    m_state.player->textureID = player_texture_id;
    
    cout << "load player_texture_id ok" << endl;

    // enemy
    GLuint enemy_texture_id = Utility::load_texture("/Users/zouyurou/Desktop/CS3113/Final\ Project/Final\ Project/assets/images/monster.png");
    m_state.enemies = new Entity[ENEMY_COUNT];
    for (int i = 0; i < ENEMY_COUNT; i++) {
        m_state.enemies[i].entity_type = ENEMY;
        m_state.enemies[i].ai_type = PATROL;
        m_state.enemies[i].ai_state = ACTIVE ;
        m_state.enemies[i].textureID = enemy_texture_id;
        m_state.enemies[i].width = 0.8;
        m_state.enemies[i].height = 0.8;
        m_state.enemies[i].speed = 2.5f;
    }
    // TODO set the enemy's position
    m_state.enemies[0].set_position(glm::vec3(-4.5f, 7.5f, 0.0f));
    m_state.enemies[1].set_position(glm::vec3(1.5f, 4.5f, 0.0f));
    m_state.enemies[2].set_position(glm::vec3(-7.5f, -6.5f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(0.0f, -1.0f, 0.0f));
    m_state.enemies[1].set_movement(glm::vec3(-1.0f, 0.0f, 0.0f));
    m_state.enemies[2].set_movement(glm::vec3(1.0f, 0.0f, 0.0f));
    m_state.enemies[0].enemy_direction = 1;
    m_state.enemies[1].enemy_direction = 0;
    m_state.enemies[2].enemy_direction = 0;
    
    cout << "load enemy_texture_id ok" << endl;
    
    // potion
    GLuint potion_texture_id = Utility::load_texture("/Users/zouyurou/Desktop/CS3113/Final\ Project/Final\ Project/assets/images/potion1.png");
    m_state.potion = new Entity();
    m_state.potion->entity_type = POTION;
    m_state.potion->set_position(glm::vec3(2.5f, -7.5f, 0.0f));
    m_state.potion->set_movement(glm::vec3(0.0f));
    m_state.potion->speed = 0.0f;
    m_state.potion->textureID = potion_texture_id;
    
    cout << "load potion_texture_id ok" << endl;
    
    GLuint mage_texture_id = Utility::load_texture("/Users/zouyurou/Desktop/CS3113/Final\ Project/Final\ Project/assets/images/mage.png");
    m_state.mage = new Entity();
    m_state.mage->entity_type = MAGE;
    m_state.mage->set_position(glm::vec3(-7.5f, -10.5f, 0.0f));
    m_state.mage->set_movement(glm::vec3(0.0f));
    m_state.mage->speed = 0.0f;
    m_state.mage->textureID = mage_texture_id;
    
    cout << "Load mage_texture_id ok" << endl;
        
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
//    m_state.jump_sfx = Mix_LoadWAV("/Users/zouyurou/Desktop/CS3113/SDL_HW5/SDL_HW5/assets/audio/jump.wav");
}

void Level2::update(float delta_time)
{
    cout << "before update, player's position: " << m_state.player->position.x << m_state.player->position.y << endl;
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.decoration);
    cout << "after update, player's position: " << m_state.player->position.x << m_state.player->position.y << endl;
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].update(delta_time, m_state.player, 0, 0, m_state.decoration);
    }
    
    m_state.potion->update(delta_time, m_state.player, 0, 0, m_state.decoration);
    m_state.mage->update(delta_time, m_state.player, 0, 0, m_state.decoration);
}


void Level2::render(ShaderProgram *program)
{
    m_state.map->render(program);
    m_state.decoration->render(program);
    m_state.player->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].render(program);
    }
    m_state.potion->render(program);
    m_state.mage->render(program);
}
