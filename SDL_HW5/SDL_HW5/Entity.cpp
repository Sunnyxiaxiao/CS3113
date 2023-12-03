/**
* Author: Yurou Zou
* Assignment: Rise of the AI
* Date due: 2023-11-18, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Entity.hpp"

using namespace std;

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    forward = true;
//    collidedBottom = true;
    g_model_matrix = glm::mat4(1.0f);
}

//void Entity::draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index)
//{
//    // Step 1: Calculate the UV location of the indexed frame
//    float u_coord = (float)(index % m_animation_cols) / (float)m_animation_cols;
//    float v_coord = (float)(index / m_animation_cols) / (float)m_animation_rows;
//
//    // Step 2: Calculate its UV size
//    float width = 1.0f / (float)m_animation_cols;
//    float height = 1.0f / (float)m_animation_rows;
//
//    // Step 3: Just as we have done before, match the texture coordinates to the vertices
//    float tex_coords[] =
//    {
//        u_coord, v_coord + height, u_coord + width, v_coord + height, u_coord + width, v_coord,
//        u_coord, v_coord + height, u_coord + width, v_coord, u_coord, v_coord
//    };
//
//    float vertices[] =
//    {
//        -0.5, -0.5, 0.5, -0.5,  0.5, 0.5,
//        -0.5, -0.5, 0.5,  0.5, -0.5, 0.5
//    };
//
//    // Step 4: And render
//    glBindTexture(GL_TEXTURE_2D, texture_id);
//
//    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
//    glEnableVertexAttribArray(program->get_position_attribute());
//
//    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
//    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());
//
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//
//    glDisableVertexAttribArray(program->get_position_attribute());
//    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
//}

void Entity::update(float delta_time, Entity* player, Entity* objects, int object_count, Map* map)
{
    if (!isActive) return;
    
//    // Check for collisions
//    for (int i = 0; i < platform_count; i++)
//    {
//        if (check_collision(&platforms[i])) return;
//    }
    
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    
    if (entity_type == ENEMY) ai_activate(player);
    else check_collisions_enemy(objects, object_count);
//    cout << entity_type << " " << speed << endl;
    
    if (jump) {
        jump = false;
        velocity.y += jump_power;
    }
    
    velocity.x = movement.x * speed;
    velocity += acceleration * delta_time;

    position.y += velocity.y * delta_time;
    check_collisions_y(objects, object_count);
    check_collision_y(map);

    position.x += velocity.x * delta_time;
    check_collisions_x(objects, object_count);
    check_collision_x(map);
    
    g_model_matrix = glm::mat4(1.0f);
    g_model_matrix = glm::translate(g_model_matrix, position);
}

bool Entity::check_collision(Entity *other) {
    if (other == this) return false;
    if (!isActive || !other->isActive) {
//        std::cout << "isActive is false" << std::endl;
        return false;
    }
    
//    std::cout << "x: " << position.x << " " << other->position.x << std::endl;
//    std::cout << "y: " << position.y << " " << other->position.y << std::endl;
    
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0);
//    std::cout << "xdist: " << xdist << std::endl;
//    std::cout << "ydist: " << ydist << std::endl;
    
    return xdist < 0.0f && ydist <   0.0f;
}

void const Entity::check_collisions_y(Entity *objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity *object = &objects[i];

        if (check_collision(object))
        {
            cout << "here" << endl;
            float ydist = fabs(position.y - object->position.y);
            float y_overlap = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            
            if (velocity.y > 0) {
                position.y -= y_overlap;
                velocity.y = 0;
                collidedTop = true;
            }
            else if (velocity.y < 0) {
                position.y += y_overlap;
                velocity.y = 0;
                collidedBottom = true;
            }
        }
    }
}

void const Entity::check_collisions_x(Entity *objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity *object = &objects[i];

        if (check_collision(object))
        {
            float xdist = fabs(position.x - object->position.x);
            float x_overlap = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= x_overlap;
                velocity.x = 0;
                collidedRight = true;
            }
            else if (velocity.x < 0) {
                position.x += x_overlap;
                velocity.x = 0;
                collidedLeft = true;
            }
        }
    }
}

void const Entity::check_collision_y(Map* map)
{
    // Probes for tiles above
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);

    // Probes for tiles below
    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    // If the map is solid, check the top three points
    if (map->is_solid(top, &penetration_x, &penetration_y) && velocity.y > 0)
    {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->is_solid(top_left, &penetration_x, &penetration_y) && velocity.y > 0)
    {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->is_solid(top_right, &penetration_x, &penetration_y) && velocity.y > 0)
    {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }

    // And the bottom three points
    if (map->is_solid(bottom, &penetration_x, &penetration_y) && velocity.y < 0)
    {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->is_solid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0)
    {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->is_solid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0)
    {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;

    }
}

void const Entity::check_collision_x(Map* map)
{
    // Probes for tiles; the x-checking is much simpler
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    if (map->is_solid(left, &penetration_x, &penetration_y) && velocity.x < 0)
    {
        position.x += penetration_x;
        velocity.x = 0;
        collidedLeft = true;
    }
    if (map->is_solid(right, &penetration_x, &penetration_y) && velocity.x > 0)
    {
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRight = true;
    }
}


bool Entity::check_collisions_enemy(Entity *enemies, int enemy_count) {
    for (int i = 0; i < enemy_count; i++) {
        Entity *enemy = &enemies[i];
//        if (enemy->position.y <= position.y) {
//            cout << "enemy position y: " << enemy->position.y << endl;
//            cout << "player position y: " << position.y << endl;
//            cout << "difference: " << fabs(position.y - enemy->position.y) - ((height + enemy->height) / 2.0);
//        }
        if (fabs(position.x - enemy->position.x) - ((width + enemy->width) / 2.0) < 0 && fabs(position.y - enemy->position.y) - ((height + enemy->height) / 2.0) <= 0) {
            cout << "difference: " << fabs(position.y - enemy->position.y) - ((height + enemy->height) / 2.0) << endl;
            
            if (fabs(position.y - enemy->position.y) - ((height + enemy->height) / 2.0) == 0 && enemy->position.y <= position.y) {
                enemy->be_defeated();
            }
            
            else {
                enemy->be_defeated();
                std::cout << "Player fails by colliding to an enemy" << std::endl;
                return true;
            }
        }
    }
    return false;
}

void Entity::ai_activate(Entity *player) {
    switch(ai_type) {
            
        case GUARD:
            ai_guard(player);
            break;
            
    }
}

void Entity::ai_guard(Entity* player)
{
    switch (ai_state) {
    case IDLE:
        if (glm::distance(position, player->get_position()) < 2.0f) ai_state = ACTIVE;
        break;
    case ACTIVE:
        if (glm::distance(position, player->position) >= 2.0f) {
            ai_state = IDLE;
            movement = glm::vec3(0.0f, 0.0f, 0.0f);
            std::cout << ai_state << std::endl;
        }
        else if (position.x > player->get_position().x) {
            movement = glm::vec3(-1.0f, 0.0f, 0.0f);
        }
        else {
            movement = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        break;
    case ATTACKING:
        break;
    default:
        break;
    }
}

void Entity::draw_sprite_from_texture_atlas(ShaderProgram *program, GLuint textureID, int index)
{
    float u = 0;
    float v = 0;

    float width = 1.0f;
    float height = 1.0f;
    
    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v};
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());
    
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_position_attribute());
}

void Entity::render(ShaderProgram *program) {
    if (!isActive) return;
    
    program->set_model_matrix(g_model_matrix);
    
    float vertices[]  = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
    float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());
    
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}

void Entity::be_defeated(){
    position.x = 0;
    position.y = 100;
    width = 0.0f;
    height = 0.0f;
    defeated = true;
    isActive = false;
}
