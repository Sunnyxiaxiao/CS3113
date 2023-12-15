#include "Level1.hpp"
#include "Utility.hpp"

#define LEVEL_WIDTH 31
#define LEVEL_HEIGHT 31
using namespace std;

unsigned int LEVEL_1_DATA[] =
{
    48,49,50,51,52,53,15,12,24,15,12,86,12,12,75,13,24,24,24,13,40,40,40,24,24,24,24,24,24,24,24,
    49,50,51,52,53,48,15,24,12,15,12,12,74,12,12,13,24,12,12,13,48,48,15,24,24,24,24,24,24,24,24,
    49,26,26,26,26,26,27,12,24,15,40,40,40,40,40,13,24,12,12,13,48,48,16,2,2,2,40,24,13,48,48,
    49,15,12,24,24,12,12,24,12,12,12,12,24,24,24,12,12,12,24,13,48,48,48,48,48,48,15,24,13,48,48,
    48,15,24,12,24,12,12,24,12,24,12,24,12,24,24,12,24,24,12,13,48,48,48,48,48,48,15,24,13,48,48,
    48,15,40,40,40,15,12,24,12,12,24,24,12,24,12,40,24,12,24,13,48,48,48,48,48,48,16,40,17,48,48,
    49,50,51,52,53,15,12,12,40,40,12,12,24,12,12,40,24,12,24,13,48,48,48,48,48,48,48,48,48,48,48,
    48,49,48,49,50,15,24,24,40,40,24,12,24,24,24,40,24,24,24,13,48,48,48,48,48,48,48,48,48,48,48,
    42,48,48,48,48,15,40,40,40,40,40,40,40,40,40,40,40,40,40,39,48,48,48,48,40,40,40,40,40,40,40,
    42,42,64,48,48,48,48,49,49,48,49,48,48,48,48,48,48,48,49,49,48,48,48,48,40,48,49,50,51,52,53,
    48,42,42,42,42,42,42,49,48,48,48,48,48,48,48,48,48,48,49,49,48,48,48,48,40,48,49,50,51,52,53,
    48,42,42,42,42,42,42,49,48,48,48,48,48,48,48,48,48,48,49,49,48,48,48,48,40,48,49,50,51,52,53,
    48,42,64,42,42,42,42,49,48,48,48,48,48,48,48,48,48,48,49,49,48,48,48,48,40,48,49,50,51,52,53,
    48,48,48,49,48,48,49,48,48,48,48,48,48,48,48,48,26,26,26,26,26,26,26,26,26,26,26,26,26,5,48,
    48,48,49,48,49,49,49,50,48,48,48,48,48,48,48,13,12,7,12,12,12,12,12,12,82,12,12,12,12,13,48,
    48,48,48,1,2,2,2,2,13,48,48,48,48,4,26,39,12,12,12,64,12,12,12,12,12,12,12,12,12,13,48,
    48,48,48,15,12,12,24,12,13,48,48,48,48,15,12,12,12,82,12,12,12,12,12,12,12,12,12,12,12,13,48,
    48,88,48,15,12,24,24,12,13,48,48,48,48,15,12,12,12,12,12,12,12,1,2,19,2,2,2,20,2,17,48,
    48,48,48,15,12,12,12,12,13,48,48,48,48,15,12,12,12,12,12,12,12,13,48,31,48,48,48,32,48,48,48,
    40,40,40,40,24,24,12,24,13,48,48,67,48,15,64,12,12,12,12,82,12,13,12,12,12,12,12,12,12,12,12,
    15,24,24,12,24,12,12,24,13,48,48,68,48,16,40,40,12,12,12,12,12,13,48,48,48,48,48,48,48,48,48,
    15,1,2,3,12,12,12,24,13,48,48,67,48,48,48,15,12,12,12,12,64,13,48,48,48,42,48,48,48,48,48,
    15,13,14,15,12,39,26,26,26,48,48,68,48,48,48,16,40,40,40,40,40,17,48,48,48,48,48,48,48,48,48,
    15,25,26,27,24,39,48,48,48,48,48,55,48,48,48,48,48,65,48,48,48,48,48,48,48,48,48,48,42,48,48,
    15,12,24,12,24,39,48,48,48,48,48,67,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,
    15,40,40,40,40,39,48,48,48,48,48,68,48,48,48,48,82,48,48,42,48,48,69,70,70,70,71,48,48,48,48,
    15,48,48,48,48,48,48,48,48,48,48,67,48,48,48,48,48,48,48,48,48,48,81,48,48,48,83,48,48,48,48,
    15,48,48,48,48,48,48,48,48,48,48,68,48,48,48,48,92,48,48,48,48,48,81,48,82,48,83,48,48,42,48,
    15,48,48,48,48,48,48,48,48,48,48,48,48,48,82,48,48,48,48,48,48,48,81,48,48,48,83,48,48,48,48,
    15,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,72,48,48,48,48,93,94,94,94,95,48,48,48,48,
    15,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48
};

unsigned int LEVEL_1_DECORATION[] = {
0, 0, 0, 0, 0, 0, 15, 0, 0, 15, 0, 0, 0, 0, 0, 13, 0, 0, 0, 13, 40, 40, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 15, 0, 0, 0, 0, 0, 13, 0, 0, 0, 13, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26, 26, 26, 26, 26, 27, 0, 0, 15, 40, 40, 40, 40, 40, 13, 0, 0, 0, 13, 0, 0, 16, 2, 2, 2, 40, 0, 13, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 15, 0, 13, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 15, 0, 13, 0, 0, 0, 15, 40, 40, 40, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 16, 40, 17, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 40, 40, 0, 0, 0, 0, 0, 40, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 40, 40, 0, 0, 0, 0, 0, 40, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 39, 0, 0, 0, 0, 40, 40, 40, 40, 40, 40, 40, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 82, 0, 0, 0, 0, 13, 0, 0, 0, 0, 1, 2, 2, 2, 2, 13, 0, 0, 0, 0, 4, 26, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 15, 0, 0, 0, 0, 13, 0, 0, 0, 0, 15, 0, 0, 0, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 15, 0, 0, 0, 0, 13, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 1, 2, 19, 2, 2, 2, 20, 2, 17, 0, 0, 0, 0, 15, 0, 0, 0, 0, 13, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 13, 0, 31, 0, 0, 0, 32, 0, 0, 0, 40, 40, 40, 40, 0, 0, 0, 0, 13, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 82, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 16, 40, 40, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 1, 2, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 13, 0, 15, 0, 39, 26, 26, 26, 0, 0, 0, 0, 0, 0, 16, 40, 40, 40, 40, 40, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 26, 27, 0, 39, 0, 0, 0, 0, 0, 55, 0, 0, 0, 0, 0, 65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 40, 40, 40, 40, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 0, 0, 0, 0, 0, 69, 70, 70, 70, 71, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 81, 0, 0, 0, 83, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 92, 0, 0, 0, 0, 0, 81, 0, 82, 0, 83, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 0, 0, 0, 0, 0, 0, 0, 81, 0, 0, 0, 83, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 93, 94, 94, 94, 95, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

Level1::~Level1()
{
    delete [] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    delete    m_state.decoration;
    delete    m_state.potion;
    Mix_FreeChunk(m_state.potion_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void Level1::initialise()
{
    GLuint map_texture_id = Utility::load_texture("/Users/zouyurou/Desktop/CS3113/Final\ Project/Final\ Project/assets/images/tilemap.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_1_DATA, map_texture_id, 1.0f, 12, 11);
    m_state.decoration = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_1_DECORATION, map_texture_id, 1.0f, 12, 11);
    
    cout << "load map_texture_id ok" << endl;
    
    // player
    GLuint player_texture_id = Utility::load_texture("/Users/zouyurou/Desktop/CS3113/Final\ Project/Final\ Project/assets/images/player1.png");
    cout << player_texture_id << endl;
    m_state.player = new Entity();
    m_state.player->entity_type = PLAYER;
    m_state.player->set_position(glm::vec3(14.5f, 11.5f, 0.0f));
//    m_state.player->set_position(glm::vec3(-1.5f, -9.5f, 0.0f));
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
        m_state.enemies[i].ai_state = IDLE ;
        m_state.enemies[i].textureID = enemy_texture_id;
        m_state.enemies[i].width = 0.9;
        m_state.enemies[i].height = 0.9;
        m_state.enemies[i].speed = 2.0f;
    }
    // TODO set the enemy's position
    m_state.enemies[0].set_position(glm::vec3(9.5f, 9.5f, 0.0f));
    m_state.enemies[1].set_position(glm::vec3(-1.5f, -8.5f, 0.0f));
    m_state.enemies[2].set_position(glm::vec3(-9.5f, 2.5f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(0.0f, -1.0f, 0.0f));
    m_state.enemies[1].set_movement(glm::vec3(-1.0f, 0.0f, 0.0f));
    m_state.enemies[2].set_movement(glm::vec3(0.0f, -1.0f, 0.0f));
    m_state.enemies[0].enemy_direction = 1;
    m_state.enemies[1].enemy_direction = 0;
    m_state.enemies[2].enemy_direction = 1;
    
    cout << "load enemy_texture_id ok" << endl;
    
    // potion
    GLuint potion_texture_id = Utility::load_texture("/Users/zouyurou/Desktop/CS3113/Final\ Project/Final\ Project/assets/images/potion1.png");
    m_state.potion = new Entity();
    m_state.potion->entity_type = POTION;
    m_state.potion->set_position(glm::vec3(10.5f, -3.5f, 0.0f));
    m_state.potion->set_movement(glm::vec3(0.0f));
    m_state.potion->speed = 0.0f;
    m_state.potion->textureID = potion_texture_id;
    
    cout << "load potion_texture_id ok" << endl;
    
    GLuint mage_texture_id = Utility::load_texture("/Users/zouyurou/Desktop/CS3113/Final\ Project/Final\ Project/assets/images/mage.png");
    m_state.mage = new Entity();
    m_state.mage->entity_type = MAGE;
    m_state.mage->set_position(glm::vec3(-13.5f, -2.5f, 0.0f));
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

void Level1::update(float delta_time)
{
    cout << "before update, player's position: " << m_state.player->position.x << m_state.player->position.y << endl;
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.decoration);
    cout << "after update, player's position: " << m_state.player->position.x << m_state.player->position.y << endl;
    for (int i = 0; i < 3; i++)
    {
        m_state.enemies[i].update(delta_time, m_state.player, 0, 0, m_state.decoration);
    }
    
    m_state.potion->update(delta_time, m_state.player, 0, 0, m_state.decoration);
    m_state.mage->update(delta_time, m_state.player, 0, 0, m_state.decoration);
}


void Level1::render(ShaderProgram *program)
{
    m_state.map->render(program);
    m_state.decoration->render(program);
    m_state.player->render(program);
    for (int i = 0; i < 3; i++)
    {
        m_state.enemies[i].render(program);
    }
    m_state.potion->render(program);
    m_state.mage->render(program);
}
