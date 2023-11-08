/**
* Author: Yurou Zou
* Assignment: Lunar Lander
* Date due: 2023-11-08, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

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
    GLuint textureID;
    glm::mat4 g_model_matrix;
    bool isActive = true;
    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    bool collidedSafe = false;
    float isSafe = true;
    Entity();
    
    bool check_collision(Entity *other);
    void check_collisions_y(Entity *objects, int objectCount);
    void check_collisions_x(Entity *objects, int objectCount);
    void update(float deltaTime, Entity *platforms, int platformCount);
    void render(ShaderProgram *program);
};
