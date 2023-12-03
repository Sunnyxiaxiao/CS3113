/**
* Author: Yurou Zou
* Assignment: Platformer
* Date due: 2023-12-02, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

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
#include "stb_image.h"

#include "Entity.hpp"
#include "Map.hpp"
#include "Utility.hpp"
#include "Scene.hpp"
#include "Menu.hpp"
//#include "LevelA.h"
//#include "LevelB.h"
//#include "LevelC.h"

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

const int NUMBER_OF_TEXTURES = 1; // to be generated, that is
const GLint LEVEL_OF_DETAIL = 0;  // base image level; Level n is the nth mipmap reduction image
const GLint TEXTURE_BORDER = 0;   // this value MUST be zero

const char V_SHADER_PATH[] = "/Users/zouyurou/Desktop/CS3113/SDL_HW2/HW2/HW2/shaders/vertex_textured.glsl", F_SHADER_PATH[] = "/Users/zouyurou/Desktop/CS3113/SDL_HW2/HW2/HW2/shaders/fragment_textured.glsl";

const char PLAYER_TEXTURE[] = "/Users/zouyurou/Desktop/CS3113/SDL_HW4/SDL_HW4/player.jpeg";
const char ENEMY_TEXTURE[] = "/Users/zouyurou/Desktop/CS3113/SDL_HW4/SDL_HW4/enemy.jpeg";
const char OBS_TEXTURE[] = "/Users/zouyurou/Desktop/CS3113/SDL_HW4/SDL_HW4/safe.jpeg";
const char FONT_TEXTURE[] = "/Users/zouyurou/Desktop/CS3113/SDL_HW4/SDL_HW4/font.png";


Scene*  g_current_scene;
Menu*   g_menu;
//LevelA* g_level_a;
//LevelB* g_level_b;
//LevelC* g_level_c;
int lives = 3;

SDL_Window* g_display_window;
bool g_game_is_running = true;
bool g_game_start = false;
bool g_level_a_done = false;
bool g_level_b_done = false;
bool g_game_stop = false;
bool g_game_win = false;

ShaderProgram program;
glm::mat4 g_view_matrix, g_projection_matrix;



void switch_to_scene(Scene* scene)
{
    g_current_scene = scene;
    g_current_scene->initialise();
}

GameState state;

// Fixed timestemp for update
#define FIXED_TIMESTEP 0.0166666f
float g_time_accumulator = 0.0f;
const float MILLISECONDS_IN_SECOND = 1000.0;
float g_previous_ticks = 0.0f;

// load texture by passing an filepath
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
    g_display_window = SDL_CreateWindow("HW5", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    program.load(V_SHADER_PATH, F_SHADER_PATH);
    
    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f); // Defines the characteristics of your camera, such as clip planes, field of view, projection method etc.
    
    program.set_projection_matrix(g_projection_matrix);
    program.set_view_matrix(g_view_matrix);
    
    glUseProgram(program.get_program_id());
    
    // ————— SCENE SETUP ————— //
    g_menu    = new Menu();
//    g_level_a = new LevelA();
//    g_level_b = new LevelB();
//    g_level_c = new LevelC();
    switch_to_scene(g_menu);
    
    // ————— LOOPING BACKGROUND MUSIC FOR WHOLE GAME ————— //
//    Mix_Music* g_music = Mix_LoadMUS("assets/audio/background.mp3");
//    Mix_PlayMusic(g_music, -1);
//    Mix_VolumeMusic(50.0f);
    
    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(BG_RED, BG_GREEN, BG_BLUE, BG_OPACITY);
}


void process_input() {
    g_current_scene->m_state.player->set_movement(glm::vec3(0.0f));

    SDL_Event event;
    const Uint8* key_state = SDL_GetKeyboardState(NULL);
    if(!g_game_start && key_state[SDL_SCANCODE_RETURN]){
        g_game_start = true;
//        switch_to_scene(g_level_a);
    }
    
    while (SDL_PollEvent(&event))
    {
        // ————— KEYSTROKES ————— //
        switch (event.type) {
            // ————— END GAME ————— //
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            g_game_is_running = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_q:
                // Quit the game with a keystroke
                g_game_is_running = false;
                break;

            case SDLK_SPACE:
                // ————— JUMPING ————— //
                if (g_current_scene->m_state.player->collidedBottom)
                {
                    g_current_scene->m_state.player->jump = true;
                    Mix_PlayChannel(-1, g_current_scene->m_state.jump_sfx, 0);
                }
                break;

            default:
                break;
            }

        default:
            break;
        }
        if (key_state[SDL_SCANCODE_LEFT] && g_game_start)
        {
            g_current_scene->m_state.player->move_left();
        }
        else if (key_state[SDL_SCANCODE_RIGHT] && g_game_start)
        {
            g_current_scene->m_state.player->move_right();
        }

        // ————— NORMALISATION ————— //
        if (glm::length(g_current_scene->m_state.player->get_movement()) > 1.0f)
        {
            g_current_scene->m_state.player->set_movement(glm::normalize(g_current_scene->m_state.player->get_movement()));
        }
    }
}

void update()
{
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
    
    // ————— PLAYER CAMERA ————— //
    g_view_matrix = glm::mat4(1.0f);

    if (g_current_scene->m_state.player->get_position().x > LEVEL1_LEFT_EDGE) {
            g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-g_current_scene->m_state.player->get_position().x, 3.75, 0));
    } else {
            g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-5, 3.75, 0));
    }
    
    // ————— UPDATE LIVES OF PLAYER ————— //
    if(g_current_scene != g_menu){
        if(g_current_scene->m_state.player->check_collisions_enemy(&g_current_scene->m_state.enemies[0], 1)){
            lives -= 1;
            g_current_scene->m_state.player->set_position(glm::vec3(g_current_scene->m_state.player->get_position().x - 2.0f, g_current_scene->m_state.player->get_position().y, 0.0f)); //get away from AI
            if(lives == 0){
                g_game_stop = true;
            }
        }
    }
}

void draw_text(ShaderProgram *program, GLuint font_texture_id, std::string text, float size, float spacing, glm::vec3 position)
{
    // Scale the size of the fontbank in the UV-plane
    // We will use this for spacing and positioning
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;

    // Instead of having a single pair of arrays, we'll have a series of pairs—one for each character
    // Don't forget to include <vector>!
    std::vector<float> vertices;
    std::vector<float> texture_coordinates;

    // For every character...
    for (int i = 0; i < text.size(); i++) {
        // 1. Get their index in the spritesheet, as well as their offset (i.e. their position
        //    relative to the whole sentence)
        int index = (int) text[i];  // ascii value of character
        float offset = (size + spacing) * i;
        
        // 2. Using the spritesheet index, we can calculate our U- and V-coordinates
        float u_coordinate = (float) (index % 16) / 16.0f;
        float v_coordinate = (float) (index / 16) / 16.0f;

        // 3. Inset the current pair in both vectors
        vertices.insert(vertices.end(), {
            offset + (-0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
        });
        texture_coordinates.insert(texture_coordinates.end(), {
                u_coordinate, v_coordinate,
                u_coordinate, v_coordinate + height,
                u_coordinate + width, v_coordinate,
                u_coordinate + width, v_coordinate + height,
                u_coordinate + width, v_coordinate,
                u_coordinate, v_coordinate + height,
            });
        }
    glm::mat4 g_model_matrix = glm::mat4(1.0f);
    g_model_matrix = glm::translate(g_model_matrix, position);
    
    program->set_model_matrix(g_model_matrix);
    glUseProgram(program->get_program_id());
    
    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->get_position_attribute());
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texture_coordinates.data());
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());
    
    glBindTexture(GL_TEXTURE_2D, font_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, (int) (text.size() * 6));
    
    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}

void render() {
    program.set_view_matrix(g_view_matrix);

    glClear(GL_COLOR_BUFFER_BIT);

    // ————— RENDERING THE SCENE (i.e. map, character, enemies...) ————— //
    g_current_scene->render(&program);
    
    // ————— WIN/LOSE TEXT ————— //
    if(g_game_stop){
        if(g_game_win){
            Utility::draw_text(&program, Utility::load_texture("assets/fonts/font1.png"), "WIN", 2.0f, 0.1f, glm::vec3(39.0, g_current_scene->m_state.player->get_position().y, 0.0f));
        }
        else{
            Utility::draw_text(&program, Utility::load_texture("assets/fonts/font1.png"), "LOSE", 2.0f, 0.1f, glm::vec3(g_current_scene->m_state.player->get_position().x - 2.0f, g_current_scene->m_state.player->get_position().y, 0.0f));
        }
    }
    
    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    SDL_Quit();

    // ————— DELETING LEVEL DATA (i.e. map, character, enemies...) ————— //
//    delete g_level_a;
//    delete g_level_b;
//    delete g_level_c;
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
