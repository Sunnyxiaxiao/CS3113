enum EntityType { PLATFORM, PLAYER, ENEMY, POTION, MAGE   };
enum AIType     { WALKER, GUARD, PATROL, JUMPER };
enum AIState    { ACTIVE, IDLE, ATTACKING  };

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>

#endif /* Entity_hpp */

#pragma once
#include "Map.hpp"

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
    bool has_potion = false;
    bool give_potion = false;
    int enemy_direction = 0; // 0 for horizontal, 1 for vertical
    
    Entity();
    
    bool check_collision(Entity *other);
    void const check_collisions_y(Entity *objects, int objectCount);
    void const check_collisions_x(Entity *objects, int objectCount);
    bool check_collisions_enemy(Entity *enemies, int enemy_count);
    void const check_collision_y(Map* map);
    void const check_collision_x(Map* map);
//    void move_left()    { movement.x = -1.0f; };
//    void move_right()   { movement.x = 1.0f; };
//    void move_up()      { movement.y = 1.0f; };
//    void move_down()    { movement.y = -1.0f; };
    void update(float delta_time, Entity* player, Entity* objects, int object_count, Map* map);
    void render(ShaderProgram *program);
    void draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index);
    void ai_activate(Entity *player);
    void ai_guard(Entity *player);
//    void ai_activate2(Entity *player, int enemy_index);
    void ai_patrol();
    void be_defeated();
};
