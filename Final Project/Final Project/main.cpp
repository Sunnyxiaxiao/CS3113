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
#include "StartPage.hpp"
#include "HelpPage.hpp"
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

const char V_SHADER_PATH[] = "/Users/zouyurou/Desktop/CS3113/Final \Project/Final \Project/shaders/vertex_textured.glsl", F_SHADER_PATH[] = "/Users/zouyurou/Desktop/CS3113/Final \Project/Final \Project/shaders/fragment_textured.glsl";

Scene*  g_current_scene;
Menu*   g_menu;
StartPage* g_start_page;
HelpPage* g_help_page;
Level1* g_level_1;
Level2* g_level_2;
Level3* g_level_3;

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
    g_display_window = SDL_CreateWindow("Final Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    g_program.load(V_SHADER_PATH, F_SHADER_PATH);
    
    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-8.0f, 8.0f, -6.0f, 6.0f, -1.0f, 1.0f); // Defines the characteristics of your camera, such as clip planes, field of view, projection method etc.
    
    g_program.set_projection_matrix(g_projection_matrix);
    g_program.set_view_matrix(g_view_matrix);
    
    glUseProgram(g_program.get_program_id());
    
    g_start_page    = new StartPage();
    g_menu          = new Menu();
    g_help_page     = new HelpPage();
    g_level_1 = new Level1();
    g_level_2 = new Level2();
    g_level_3 = new Level3();
    switch_to_scene(g_start_page);
    
    Mix_Music* g_music = Mix_LoadMUS("/Users/zouyurou/Desktop/CS3113/Final \Project/Final \Project/assets/audio/background.mp3");
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
    const Uint8* key = SDL_GetKeyboardState(NULL);
    if(g_current_scene == g_start_page && key[SDL_SCANCODE_RETURN]){
        switch_to_scene(g_menu);
    }
    if(key[SDL_SCANCODE_H]) {
        switch_to_scene(g_help_page);
    }
    else if(key[SDL_SCANCODE_1]){
        g_game_start = true;
        g_game_stop = false;
        switch_to_scene(g_level_1);
    }
    else if(key[SDL_SCANCODE_2]){
        g_game_start = true;
        g_game_stop = false;
        switch_to_scene(g_level_2);
    }
    else if(key[SDL_SCANCODE_3]){
        g_game_start = true;
        g_game_stop = false;
        switch_to_scene(g_level_3);
    }
    if (key[SDL_SCANCODE_R]) {
        switch_to_scene(g_menu);
        g_game_stop = false;
    }

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            g_game_is_running = false;
        }
    }

    // ————— KEY HOLD ————— //
    if (key[SDL_SCANCODE_A] && g_game_start) {
        g_current_scene->m_state.player->movement.x = -1.0f;
    }
    else if (key[SDL_SCANCODE_D] && g_game_start) {
        g_current_scene->m_state.player->movement.x = 1.0f;
    }
    if (key[SDL_SCANCODE_W] && g_game_start) {
        g_current_scene->m_state.player->movement.y = 1.0f;
    }
    else if (key[SDL_SCANCODE_S] && g_game_start) {
        g_current_scene->m_state.player->movement.y = -1.0f;
    }

    if (glm::length(g_current_scene->m_state.player->movement) > 1.0f) {
        g_current_scene->m_state.player->movement = glm::normalize(g_current_scene->m_state.player->movement);
    }
}

void update()
{
    if(g_current_scene->m_state.player->defeated){
        cout << "player is defeated" << endl;
        g_game_stop = true;
        g_game_win = false;
    }
    else if(g_current_scene->m_state.player->has_potion && g_current_scene->m_state.player->give_potion){
        cout << "player wins" << endl;
        g_game_stop = true;
        g_game_win = true;
    }
    if (!g_game_stop){
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
        g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-g_current_scene->m_state.player->get_position().x, -g_current_scene->m_state.player->get_position().y, 0));
//        if (g_current_scene == g_level_1) {
//            g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-g_current_scene->m_state.player->get_position().x, -g_current_scene->m_state.player->get_position().y, 0));
//        } else {
//            g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-7.5, 5.5, 0));
//        }
        
//        if(g_current_scene == g_level_1 || g_current_scene == g_level_2 || g_current_scene == g_level_3) {
//            g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-g_current_scene->m_state.player->get_position().x, -g_current_scene->m_state.player->get_position().y, 0));
//        } else {
//            g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-8, 6, 0));
//        }
        
//        g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-7.5, 5.5, 0));
        
        if (g_current_scene == g_level_1 || g_current_scene == g_level_2) {
            if(g_current_scene->m_state.player->check_collisions_enemy(&g_current_scene->m_state.enemies[0], 1)) {
                    g_game_stop = true;
                    g_game_win = false;
                    g_current_scene->m_state.player->isActive = false;
                    g_current_scene->m_state.player->defeated = true;
            }
        }
        
//        if(g_current_scene == g_level_1 || g_current_scene == g_level_2 || g_current_scene == g_level_3) {
//            if(g_current_scene->m_state.player->check_collisions_enemy(&g_current_scene->m_state.enemies[0], 1)) {
//                g_game_stop = true;
//                g_game_win = false;
//                g_current_scene->m_state.player->isActive = false;
//                g_current_scene->m_state.player->defeated = true;
//            }
//        }
    }
}

void render() {
    g_program.set_view_matrix(g_view_matrix);
    glClear(GL_COLOR_BUFFER_BIT);
    g_current_scene->render(&g_program);
    if(g_game_stop){
        GLuint texture_font = Utility::load_texture("/Users/zouyurou/Desktop/CS3113/SDL_HW5/SDL_HW5/assets/fonts/font1.png");
        if(g_game_win){
            Utility::draw_text(&g_program, texture_font, "YOU WIN", 1.0f, 0.1f, glm::vec3(g_current_scene->m_state.player->get_position().x - 2.0f, g_current_scene->m_state.player->get_position().y, 0.0f));
        }
        else{
            Utility::draw_text(&g_program, texture_font, "YOU LOSE", 1.0f, 0.1f, glm::vec3(g_current_scene->m_state.player->get_position().x - 2.0f, g_current_scene->m_state.player->get_position().y, 0.0f));
        }
    }
    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    SDL_Quit();
//    delete g_level_1;
//    delete g_level_2;
//    delete g_level_3;
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
