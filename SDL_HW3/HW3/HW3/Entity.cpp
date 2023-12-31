/**
* Author: Yurou Zou
* Assignment: Lunar Lander
* Date due: 2023-11-08, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Entity.hpp"
#include <vector>

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    forward = true;
    
    g_model_matrix = glm::mat4(1.0f);
}

bool Entity::check_collision(Entity *other) {
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
    
    return xdist < 0.0f && ydist < 0.0f;
}

void Entity::check_collisions_y(Entity *objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity *object = &objects[i];

        if (check_collision(object))
        {
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

void Entity::check_collisions_x(Entity *objects, int objectCount)
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

void Entity::check_collision_enemy(Entity *enemies, int enemy_count) {
    for (int i = 0; i < enemy_count; i++) {
        Entity *enemy = &enemies[i];
        
        if (check_collision(enemy)) {
            if (velocity.y < 0) {
                if (enemy->position.y <= position.y) {
                    enemy->defeated = true;
                    enemy->isActive = false;
                }
            }
            else {
                defeated = true;
            }
        }
    }
}

void Entity::ai_activate(Entity *player) {
    switch(ai_type) {
        case WALKER:
            ai_walk();
            break;
            
        case GUARD:
            ai_guard(player);
            break;
            
        case PATROL:
            ai_patrol();
            break;
            
        case JUMP:
            ai_jump(player);
            break;
            
    }
}

void Entity::ai_walk() {
    movement = glm::vec3(-1.0f, 0.0f, 0.0f);
}

void Entity::ai_guard(Entity* player)
{
    switch (ai_state) {
    case IDLE:
        if (glm::distance(m_position, player->get_position()) < 3.0f) ai_state = ACTIVE;
        break;

    case ACTIVE:
        if (glm::distance(position, player->position) > 3.5f) {
            aiState = IDLE;
        }
        if (m_position.x > player->get_position().x) {
            m_movement = glm::vec3(-1.0f, 0.0f, 0.0f);
        }
        else {
            m_movement = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        break;

    case ATTACKING:
        break;

    default:
        break;
    }
}

void Entity::ai_patrol() {
    switch(ai_state) {
        case IDLE:
            break;
            
        case ACTIVE:
            if (forward && position.x <= 0.5f) {
                movement = glm::vec3(1.0f, 0.0f, 0.0f);
                if (position == 0.5f) forward = !forward;
            }
            else if (!forward && position.x > 0.5f){
                movement = glm::vec3(-1.0f, 0.0f, 0.0f);
                if (position < -4.5f) foward = !forward;
            }
            break;
    }
}

void Entity::ai_jump(Entity *player) {
    switch(ai_state) {
        case IDLE:
            std::cout << ai_state << "\n";
            if (glm::distance(position, player->position) < 1.5f) {
                ai_state = ACTIVE;
            }
            else {
                velocity.y = -4.75;
            }
            break;
            
        case ACTIVE:
            std::cout << ai_state << "\n";
            velocity.y = 4.0;
            
            if (glm::distance(position, player->position) > 1.5f) {
                ai_state = IDLE;
            }
            
            break;
    }
}

void Entity::update(float delta_time, Entity *player, Entity *platforms, int platform_count, Entity *enemies, int enemy_count)
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
    collidedSafe = false;
    
    if (entity_type == ENEMY) ai_activate(player);
    else check_collision_enemy(enemies, eneymy_count);
    
    if (jump) {
        jump = false;
        velocity.y += jump_power;
    }
    
    velocity.x = movement.x * speed;
    velocity += acceleration * delta_time;
    
    position.y += velocity.y * delta_time;           // Move on Y
    check_collisions_y(platforms, platform_count);     // Fix if needed

    position.x += velocity.x * delta_time;           // Move on X
    check_collisions_x(platforms, platform_count);     // Fix if needed
    
    g_model_matrix = glm::mat4(1.0f);
    g_model_matrix = glm::translate(g_model_matrix, position);
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
