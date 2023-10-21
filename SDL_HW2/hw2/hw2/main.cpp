/**
* Author: Yurou Zou
* Assignment: Pong
* Date due: 2023-10-21, 11:59pm
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
#include "stb_image.h"
#include "iostream"

const int WINDOW_WIDTH = 640,
          WINDOW_HEIGHT = 480;

const int VIEWPORT_X = 0,
          VIEWPORT_Y = 0,
          VIEWPORT_WIDTH = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const float BG_RED = 0.1922f,
            BG_BLUE = 0.549f,
            BG_GREEN = 0.9059f,
            BG_OPACITY = 1.0f;

const char V_SHADER_PATH[] = "/Users/zouyurou/Desktop/CS3113/SDL_HW2/HW2/HW2/shaders/vertex_textured.glsl", F_SHADER_PATH[] = "/Users/zouyurou/Desktop/CS3113/SDL_HW2/HW2/HW2/shaders/fragment_textured.glsl";

const char BALL_TEXTURE[] = "/Users/zouyurou/Desktop/CS3113/SDL_HW2/HW2/HW2/soph.png";
const char PLAYER_TEXTURE[] = "/Users/zouyurou/Desktop/CS3113/SDL_HW2/HW2/HW2/test.png";


const int NUMBER_OF_TEXTURES = 1; // to be generated, that is
const GLint LEVEL_OF_DETAIL = 0;  // base image level; Level n is the nth mipmap reduction image
const GLint TEXTURE_BORDER = 0;   // this value MUST be zero

SDL_Window* g_display_window;
ShaderProgram g_shader_program;
glm::mat4 g_view_matrix, g_model_p1, g_model_p2, g_model_ball, g_projection_matrix, g_trans_matrix;

bool g_game_is_running = true, g_game_is_end = false;
bool g_game_is_multi_players = true;

GLuint ball_texture_id, player_texture_id;

glm::vec3 p1_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 p1_movement = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 p2_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 p2_movement = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 ball_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 ball_movement = glm::vec3(0.0f, 0.0f, 0.0f);

float ball_speed_x = 0.1f, ball_speed_y = 0.06f, player_speed = 50.0f;
const float MILLISECONDS_IN_SECOND = 1000.0;
float m_previous_ticks = 0.0f;

GLuint LoadTexture(const char* filePath) {
    int width, height, number_of_components;
    unsigned char* image = stbi_load(filePath, &width, &height, &number_of_components, STBI_rgb_alpha);
    if(image == nullptr) {
        std::cerr << "Failed to load texture." << std::endl;
        return 0;
    }
    
    // Generating and binding a texture ID to the image
    GLuint textureID;
    glGenTextures(NUMBER_OF_TEXTURES, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA, width, height, TEXTURE_BORDER, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    // Setting texture filter parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // Releasing memory and returning texture id
    stbi_image_free(image);
    return textureID;
}

void initialise() {
    SDL_Init(SDL_INIT_VIDEO);
    g_display_window = SDL_CreateWindow("HW2",
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT,
                                        SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);
    
    ball_texture_id = LoadTexture(BALL_TEXTURE);
    player_texture_id = LoadTexture(PLAYER_TEXTURE);
    
    g_view_matrix        = glm::mat4(1.0f);
    g_model_p1      = glm::mat4(1.0f);
    g_model_p2      = glm::mat4(1.0f);
    g_model_ball         = glm::mat4(1.0f);

    g_projection_matrix = glm::ortho(-64.0f, 64.0f, -48.0f, 48.0f, -1.0f, 1.0f); // Defines the characteristics of your camera, such as clip planes, field of view, projection method etc.
    
    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);
    
    glUseProgram(g_shader_program.get_program_id());
    
    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(BG_RED, BG_GREEN, BG_BLUE, BG_OPACITY);
}

void process_input(){
    SDL_Event event;
    p1_movement = glm::vec3(0.0f);
    p2_movement = glm::vec3(0.0f);
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            g_game_is_running = false;
        }
    }
    // get the key input
    const Uint8 *key_state = SDL_GetKeyboardState(NULL);
    if (key_state[SDL_SCANCODE_T]){ // switch from multi players to one player
        g_game_is_multi_players = !g_game_is_multi_players;
    }
    
    if(g_game_is_multi_players){
        if (key_state[SDL_SCANCODE_S]) {
            p1_movement.y = -1.0f;
        } else if (key_state[SDL_SCANCODE_W]) {
            p1_movement.y = 1.0f;
        }
        if (key_state[SDL_SCANCODE_UP]) {
            p2_movement.y = 1.0f;
        } else if (key_state[SDL_SCANCODE_DOWN]) {
            p2_movement.y = -1.0f;
        }
    }
    else{
        if (key_state[SDL_SCANCODE_W]) {
            p1_movement.y = 1.0f;
            p2_movement.y = 1.0f;
        } else if (key_state[SDL_SCANCODE_S]) {
            p1_movement.y = -1.0f;
            p2_movement.y = -1.0f;
        }
    }
};

void update() {
    float ticks = (float) SDL_GetTicks() / MILLISECONDS_IN_SECOND; // get the current number of ticks
    float delta_time = ticks - m_previous_ticks; // the delta time is the difference from the last frame
    m_previous_ticks = ticks;
    
    if (!g_game_is_end) {
        // calculate the movement of the ball
        ball_movement.x += (ball_movement.x > 0) ? ball_speed_x * delta_time : -ball_speed_x * delta_time;
        ball_movement.y += (ball_movement.y > 0) ? ball_speed_y * delta_time : -ball_speed_y * delta_time;
        // if the ball is hit by one of the players, rebound
        if (ball_position.x < -52 && ball_position.x > -56 && ball_position.y <= p1_position.y + 8 && ball_position.y >= p1_position.y - 8) {
            ball_movement.x = -ball_movement.x;
        }
        if (ball_position.x > 52 && ball_position.x < 56 && ball_position.y <= p2_position.y + 8 && ball_position.y >= p2_position.y - 8) {
            ball_movement.x = -ball_movement.x;
        }
        // if the ball hit the top or bottom, rebound
        if (ball_position.y < -46 || ball_position.y > 46) {
            ball_movement.y = -ball_movement.y;
            if (ball_position.y > 46) ball_position.y = 46;
            else ball_position.y = -46;
        }
        // if the ball is out of the boundary, end the game
        if (ball_position.x < -64 || ball_position.x > 64) {
            std::cout << "End Game" << std::endl;
            g_game_is_end = true;
            
        } else {
            // translate through movement
            ball_position += ball_movement;
            std::cout << ball_position.x << std::endl;
            g_model_ball = glm::translate(g_model_ball, ball_movement);
        }
        
        // control the players' movement and translate use the position
        g_model_p1 = glm::mat4(1.0f);
        g_model_p2 = glm::mat4(1.0f);
        if (p1_position.y > 40 || p1_position.y < -40) {
            if (p1_position.y > 40) p1_position.y = 40;
            else p1_position.y = -40;
        } else {
            p1_position.y += p1_movement.y * player_speed * delta_time;
        }
        g_model_p1 = glm::translate(g_model_p1, p1_position);
        
        if (p2_position.y > 40 || p2_position.y < -40) {
            if (p2_position.y > 40) p2_position.y = 40;
            else p2_position.y = -40;
        } else {
            p2_position.y += p2_movement.y * player_speed * delta_time;
        }
        g_model_p2 = glm::translate(g_model_p2, p2_position);
    }
}

void draw_object(glm::mat4 &object_model_matrix, GLuint &object_texture_id, const int k)
{
    g_shader_program.set_model_matrix(object_model_matrix);
    glBindTexture(GL_TEXTURE_2D, object_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, k);
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    float ball[] = {
        -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f
    };
    
    float ball_texture[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    };
    
    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, ball);
    glEnableVertexAttribArray(g_shader_program.get_position_attribute());
    glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, ball_texture);
    glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());
    draw_object(g_model_ball, ball_texture_id, 6);
    
    float p1[] = {
        -58.0f, -8.0f, -50.0f, -8.0f, -50.0f, 8.0f,
        -58.0f, -8.0f, -50.0f, 8.0f, -58.0f, 8.0f
    };
    
    float p1_texture[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    };
    
    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, p1);
    glEnableVertexAttribArray(g_shader_program.get_position_attribute());
    glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, p1_texture);
    glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());
    draw_object(g_model_p1, player_texture_id, 6);
    
    float p2[] = {
        58.0f, -8.0f, 50.0f, -8.0f, 50.0f, 8.0f,
        58.0f, -8.0f, 50.0f, 8.0f, 58.0f, 8.0f
    };
    
    float p2_texture[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    };
    
    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, p2);
    glEnableVertexAttribArray(g_shader_program.get_position_attribute());
    glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, p2_texture);
    glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());
    draw_object(g_model_p2, player_texture_id, 6);
    
    
    // We disable two attribute arrays now
    glDisableVertexAttribArray(g_shader_program.get_position_attribute());
    glDisableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());
    
    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    // Part 1: Initialise our program—whatever that means
    initialise();

    while (g_game_is_running)
    {
        // Part 2: If the player did anything—press a button, move the joystick—process it
        process_input();

        // Part 3: Using the game's previous state, and whatever new input we have, update the game's state
        update();

        // Part 4: Once updated, render those changes onto the screen
        render();
    }

    // Part 5: The game is over, so let's perform any shutdown protocols
    shutdown();
    return 0;
}

