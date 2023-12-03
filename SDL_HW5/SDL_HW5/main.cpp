/**
* Author: Yurou Zou
* Assignment: Platformer
* Date due: 2023-12-02, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
using namespace std;

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define LEVEL1_LEFT_EDGE 5.0f
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "cmath"
#include <ctime>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "Entity.hpp"
#include "Map.hpp"
#include "Utility.hpp"
#include "Scene.hpp"
#include "Menu.hpp"
#include "Level1.hpp"
#include "Level2.hpp"
#include "Level3.hpp"

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

const char V_SHADER_PATH[] = "/Users/zouyurou/Desktop/CS3113/HW5/HW5/shaders/vertex_textured.glsl", F_SHADER_PATH[] = "/Users/zouyurou/Desktop/CS3113/HW5/HW5/shaders/fragment_textured.glsl";

Scene*  g_current_scene;
Menu*   g_menu;
Level1* g_level_1;
Level2* g_level_2;
Level3* g_level_3;
int lives = 3;

SDL_Window* g_display_window;
bool g_game_is_running = true;
bool g_game_start = false;
bool g_level_1_done = false;
bool g_level_2_done = false;
bool g_game_stop = false;
bool g_game_win = false;

ShaderProgram g_program;
glm::mat4 g_view_matrix, g_projection_matrix;

// Fixed timestemp for update
#define FIXED_TIMESTEP 0.0166666f
float g_time_accumulator = 0.0f;
const float MILLISECONDS_IN_SECOND = 1000.0;
float g_previous_ticks = 0.0f;



void switch_to_scene(Scene* scene)
{
    g_current_scene = scene;
    g_current_scene->initialise();
}

void initialise() {
    SDL_Init(SDL_INIT_VIDEO);
    g_display_window = SDL_CreateWindow("HW5", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    g_program.load(V_SHADER_PATH, F_SHADER_PATH);
    
    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f); // Defines the characteristics of your camera, such as clip planes, field of view, projection method etc.
    
    g_program.set_projection_matrix(g_projection_matrix);
    g_program.set_view_matrix(g_view_matrix);
    
    glUseProgram(g_program.get_program_id());
    
    g_menu    = new Menu();
    g_level_1 = new Level1();
    g_level_2 = new Level2();
    g_level_3 = new Level3();
    switch_to_scene(g_menu);
    
    Mix_Music* g_music = Mix_LoadMUS("/Users/zouyurou/Desktop/CS3113/SDL_HW5/SDL_HW5/assets/audio/background.mp3");
    Mix_PlayMusic(g_music, -1);
    Mix_VolumeMusic(50.0f);
    
    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(BG_RED, BG_GREEN, BG_BLUE, BG_OPACITY);
}


void process_input() {
    g_current_scene->m_state.player->set_movement(glm::vec3(0.0f));

    SDL_Event event;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if(!g_game_start && keys[SDL_SCANCODE_RETURN]){
        g_game_start = true;
        switch_to_scene(g_level_1);
    }
    
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            g_game_is_running = false;
        }
    }

    if (keys[SDL_SCANCODE_LEFT] && g_game_start)
    {
        g_current_scene->m_state.player->movement.x = -1.0f;
    } else if (keys[SDL_SCANCODE_RIGHT] && g_game_start)
    {
        g_current_scene->m_state.player->movement.x = 1.0f;
    } else if (keys[SDL_SCANCODE_SPACE] && g_game_start && g_current_scene->m_state.player->collidedBottom) {
        g_current_scene->m_state.player->jump = true;
        Mix_PlayChannel(-1, g_current_scene->m_state.jump_sfx, 0);
    }

    if (glm::length(g_current_scene->m_state.player->get_movement()) > 1.0f)
    {
        g_current_scene->m_state.player->movement = glm::normalize(g_current_scene->m_state.player->get_movement());
    }
}

void update()
{
    if (!g_game_stop){
        if (g_current_scene->m_state.player->position.x > 41){
            g_game_win = true;
            g_game_stop = true;
        }
        if (g_current_scene != g_level_2 && g_current_scene->m_state.player->position.x > 13 && g_current_scene->m_state.player->position.x < 26 && g_game_start && !g_level_2_done){
            switch_to_scene(g_level_2);
            g_level_1_done = true;
        }
        else if (g_current_scene->m_state.player->get_position().x > 26  && g_current_scene != g_level_3 && g_game_start){
            switch_to_scene(g_level_3);
            g_level_2_done = true;
        }
        // ————— DELTA TIME ————— //
        float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND; // get the current number of ticks
        float delta_time = ticks - g_previous_ticks; // the delta time is the difference from the last frame
        g_previous_ticks = ticks;
        
        // ————— FIXED TIMESTEP ————— //
        // STEP 1: Keep track of how much time has passed since last step
        delta_time += g_time_accumulator;
        
        // STEP 2: Accumulate the ammount of time passed while we're under our fixed timestep
        if (delta_time < FIXED_TIMESTEP)
        {
            g_time_accumulator = delta_time;
            return;
        }
        
        // STEP 3: Once we exceed our fixed timestep, apply that elapsed time into the objects' update function invocation
        while (delta_time >= FIXED_TIMESTEP)
        {
            g_current_scene->update(FIXED_TIMESTEP);
            
            delta_time -= FIXED_TIMESTEP;
        }
        g_time_accumulator = delta_time;
        
        g_view_matrix = glm::mat4(1.0f);
        if (g_current_scene->m_state.player->get_position().x > LEVEL1_LEFT_EDGE) {
            g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-g_current_scene->m_state.player->get_position().x, 3.75, 0));
        } else {
            g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-5, 3.75, 0));
        }
        if(g_current_scene != g_menu){
            if(g_current_scene->m_state.player->check_collisions_enemy(&g_current_scene->m_state.enemies[0], 1)){
                lives -= 1;
                cout << "lives -1: " << lives << endl;
                if(lives == 0){
                    g_game_stop = true;
                }
            }
        }
    }
}

void render() {
    g_program.set_view_matrix(g_view_matrix);
    glClear(GL_COLOR_BUFFER_BIT);
    g_current_scene->render(&g_program);
    if(g_game_stop){
        GLuint texture_font = Utility::load_texture("/Users/zouyurou/Desktop/CS3113/SDL_HW5/SDL_HW5/assets/fonts/font1.png");
        if(g_game_win){
            Utility::draw_text(&g_program, texture_font, "YOU WIN", 1.0f, 0.1f, glm::vec3(37.0f, g_current_scene->m_state.player->position.y, 0.0f));
        }
        else{
            Utility::draw_text(&g_program, texture_font, "YOU LOSE", 1.0f, 0.1f, glm::vec3(30.0f, g_current_scene->m_state.player->position.y, 0.0f));
        }
    }
    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    SDL_Quit();
    delete g_level_1;
    delete g_level_2;
    delete g_level_3;
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
