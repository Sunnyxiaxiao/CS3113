enum EntityType { PLATFORM, PLAYER, ENEMY   };
enum AIType     { WALKER, GUARD, PATROL, JUMPER };
enum AIState    { ACTIVE, IDLE, ATTACKING  };

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>

#endif /* Entity_hpp */

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

class Entity {
public:
    EntityType entity_type;
    AIType ai_type;
    AIState ai_state;
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    // ————— GETTERS ————— //
    glm::vec3 const get_position()     const { return position; };
    glm::vec3 const get_velocity()     const { return velocity; };
    glm::vec3 const get_acceleration() const { return acceleration; };
    glm::vec3 const get_movement()     const { return movement; };
    
    // ————— SETTERS ————— //
    void const set_position(glm::vec3 new_position) { position = new_position; };
    void const set_velocity(glm::vec3 new_velocity) { velocity = new_velocity; };
    void const set_acceleration(glm::vec3 new_position) { acceleration = new_position; };
    void const set_movement(glm::vec3 new_movement) { movement = new_movement; };
    
    float width = 1;
    float height = 1;
    float speed;
    bool forward;
    bool jump = false;
    float jump_power = 0;
    GLuint textureID;
    glm::mat4 g_model_matrix;
    bool isActive = true;
    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    bool defeated = false;
    
    Entity();
    
    bool check_collision(Entity *other);
    void check_collisions_y(Entity *objects, int objectCount);
    void check_collisions_x(Entity *objects, int objectCount);
    void check_collisions_enemy(Entity *enemies, int enemy_count);
    void update(float deltaTime, Entity *player, Entity *platforms, int platformCount, Entity *enemies, int enemyCount);
    void render(ShaderProgram *program);
    void draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index);
    void ai_activate(Entity *player);
    void ai_walk();
    void ai_guard(Entity *player);
    void ai_patrol();
    void ai_jump(Entity *player);

};
