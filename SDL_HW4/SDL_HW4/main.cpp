/**
* Author: Yurou Zou
* Assignment: Lunar Lander
* Date due: 2023-11-08, 11:59pm
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
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.hpp"
//#include "Map.hpp"
#include <vector>

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


// textureId for game success or fail
GLuint successTextureID, failTextureID;
glm::mat4 g_model_success;
glm::mat4 g_model_fail;


#define PLATFORM_COUNT 19
#define ENEMY_COUNT 3

// create Game State struct
struct GameState {
    Entity *player;
    Entity *platforms;
    Entity *enemies;
//    Map *map;
};

GameState state;

SDL_Window* g_display_window;
bool g_game_is_running = true;

ShaderProgram program;
glm::mat4 g_view_matrix, g_model_matrix, g_projection_matrix;

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
    g_display_window = SDL_CreateWindow("HW3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    program.load(V_SHADER_PATH, F_SHADER_PATH);
    g_model_success = glm::mat4(1.0f);
    g_model_fail = glm::mat4(1.0f);
    
    g_view_matrix = glm::mat4(1.0f);
    g_model_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f); // Defines the characteristics of your camera, such as clip planes, field of view, projection method etc.
    
    program.set_projection_matrix(g_projection_matrix);
    program.set_view_matrix(g_view_matrix);
    
    glUseProgram(program.get_program_id());
    
    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(BG_RED, BG_GREEN, BG_BLUE, BG_OPACITY);
    
    // Craete ball entity
    state.player = new Entity();
    state.player->entity_type = PLAYER;
    state.player->width = 0.5;
    state.player->set_position(glm::vec3(-4.0f, -1.0f, 0.0f));
    state.player->set_movement(glm::vec3(0));
    state.player->set_acceleration(glm::vec3(0, -2.0f, 0));
    state.player->speed = 2.0f;
    state.player->jump_power = 3.0f;
    state.player->textureID = LoadTexture(PLAYER_TEXTURE);
        
    // create enemy entities
    GLuint enemy_texture_id = LoadTexture(ENEMY_TEXTURE);
    state.enemies = new Entity[ENEMY_COUNT];
    state.enemies[0].entity_type = ENEMY;
    state.enemies[0].ai_type = GUARD;
    state.enemies[0].ai_state = IDLE;
    state.enemies[0].textureID = enemy_texture_id;
    state.enemies[0].position = glm::vec3(-4.5f, 1.0f, 0.0f); // need to change
    state.enemies[0].movement = glm::vec3(0.0f);
    state.enemies[0].speed = 0.5f;
    state.enemies[0].acceleration = glm::vec3(0.0f, -9.81f, 0.0f);
    state.enemies[1].entity_type = ENEMY;
    state.enemies[1].ai_type = PATROL;
    state.enemies[1].ai_state = ACTIVE;
    state.enemies[1].textureID = enemy_texture_id;
    state.enemies[1].position = glm::vec3(-2.0f, -2.5f, 0.0f); // need to change
    state.enemies[1].movement = glm::vec3(0.0f);
    state.enemies[1].speed = 0.5f;
    state.enemies[1].acceleration = glm::vec3(0.0f, -9.81f, 0.0f);
    state.enemies[2].entity_type = ENEMY;
    state.enemies[2].ai_type = JUMPER;
    state.enemies[2].ai_state = ACTIVE;
    state.enemies[2].textureID = enemy_texture_id;
    state.enemies[2].position = glm::vec3(2.5f, -1.25f, 0.0f); // need to change
    state.enemies[2].movement = glm::vec3(0.0f);
    state.enemies[2].speed = 0.5f;
    state.enemies[2].acceleration = glm::vec3(0.0f, -9.81f, 0.0f);
    
    // create platform entities
    state.platforms = new Entity[PLATFORM_COUNT];
    GLuint basePlatformTextureID = LoadTexture(OBS_TEXTURE);
    
    for (int i = 0; i < 12; i++) {
        state.platforms[i].entity_type = PLATFORM;
        state.platforms[i].textureID = basePlatformTextureID;
        if (i < 7) {
            state.platforms[i].position = glm::vec3(-4.5f + i, -3.0f, 0.0f);
            state.platforms[i+12].position = glm::vec3(-4.5f + i, 0.7f, 0.0f);
            state.platforms[i+12].textureID = basePlatformTextureID;
            state.platforms[i+12].entity_type = PLATFORM;
        } else {
            state.platforms[i].position = glm::vec3(-4.5f + i, -1.5f, 0.0f);
        }
    }
    
    for (int i = 0; i < (PLATFORM_COUNT); i++ ){
        state.platforms[i].update(0, NULL, NULL, 0, NULL, NULL);
    }
//    // ————— MAP SET-UP ————— //
//    GLuint map_texture_id = LoadTexture(OBS_TEXTURE);
//    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, LEVEL_1_DATA, map_texture_id, 4.0f, 16, 7);  // Look at this beautiful initialisation. That's literally it
}


void process_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            g_game_is_running = false;
        }
    }
    state.player->movement = glm::vec3(0);
    
    // get the key input
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
//    std::cout << "safe platform 2: " << state.platforms[2].position.x << " " << state.platforms[2].position.y << std::endl;
//    std::cout << "player: " << state.player->position.x << " " << state.player->position.y << std::endl;
    
    if (keys[SDL_SCANCODE_A]) {
        state.player->movement.x = -1.0f;
    }
    else if (keys[SDL_SCANCODE_D]) {
        state.player->movement.x = 1.0f;
    } else if (keys[SDL_SCANCODE_SPACE] && state.player->collidedBottom) {
        state.player->jump = true;
    }
    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
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
        state.player->update(FIXED_TIMESTEP, state.player, state.platforms, PLATFORM_COUNT, state.enemies, ENEMY_COUNT);
                
        for (int i = 0; i < ENEMY_COUNT; i++) {
            state.enemies[i].update(FIXED_TIMESTEP, state.player, state.platforms, PLATFORM_COUNT, state.enemies, ENEMY_COUNT);
        }
        
        delta_time -= FIXED_TIMESTEP;
    }

    g_time_accumulator = delta_time;
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
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].render(&program);
    }
//    state.map->render(&program);
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].render(&program);
    }
    state.player->render(&program);
    if (state.enemies[0].defeated && state.enemies[1].defeated && state.enemies[2].defeated) {
        std::cout << "Game end, WIN" << std::endl;
        glClear(GL_COLOR_BUFFER_BIT);
        draw_text(&program, LoadTexture(FONT_TEXTURE), "WIN", 0.5f, -.25f, glm::vec3(0.0f, 0.0f, 0.0f));
        state.player->isActive = false;
    }
    if (state.player->defeated) {
        std::cout << "Game end, LOSE" << std::endl;
        glClear(GL_COLOR_BUFFER_BIT);
        draw_text(&program, LoadTexture(FONT_TEXTURE), "LOSE", 0.5f, -.25f, glm::vec3(0.0f, 0.0f, 0.0f));
        state.player->isActive = false;
    }
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
