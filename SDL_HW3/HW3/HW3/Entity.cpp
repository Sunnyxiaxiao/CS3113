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
            if (i != 2 && i != 3&& i != 7 && i != 8) {
                isSafe = false;
                std::cout << "Game Over on y and the object is " << i << std::endl;
                
            }
            if (i == 2 || i == 3 || i == 7 || i == 8) {
                std::cout << "Lanuched on the object is " << i << std::endl;
            }
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
            if (i != 2 && i != 3 && i != 7 && i != 8) {
                isSafe = false;
                std::cout << "Game Over on x and the object is " << i << std::endl;
                
            }
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

void Entity::update(float deltaTime, Entity *platforms, int platform_count)
{
    if (!isActive) return;
    
    // Check for collisions
    for (int i = 0; i < platform_count; i++)
    {
        if (check_collision(&platforms[i])) return;
    }
    
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    collidedSafe = false;
    
    if (movement.x == 1.0f)
    {
//        std::cout << "left key pressed" << std::endl;
        acceleration.x += -1.0f * deltaTime; // Replace with actual acceleration value
    }
    else if (movement.x == -1.0f)
    {
        acceleration.x += 1.0f * deltaTime; // Replace with actual acceleration value
    }
    else
    {
        if (velocity.x > 0.5f) {
            acceleration.x -= 0.5f;
        } else if (velocity.x < -0.5f) {
            acceleration.x += 0.5f;
        }
        else
        {
            velocity.x = 0.0f; // If velocity is very low, just set it to 0
            acceleration.x = 0.0f;
        }
    }
    
//    velocity.x = movement.x * speed;
    velocity.x += acceleration.x * deltaTime;
    velocity.y += acceleration.y * deltaTime;
    
    position.y += velocity.y * deltaTime;           // Move on Y
    check_collisions_y(platforms, platform_count);     // Fix if needed

    position.x += velocity.x * deltaTime;           // Move on X
    check_collisions_x(platforms, platform_count);     // Fix if needed
    
    g_model_matrix = glm::mat4(1.0f);
    g_model_matrix = glm::translate(g_model_matrix, position);
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
