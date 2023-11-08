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

const char BALL_TEXTURE[] = "/Users/zouyurou/Desktop/CS3113/SDL_HW3/HW3/HW3/soph.png";
const char OBS_TEXTURE[] = "/Users/zouyurou/Desktop/CS3113/SDL_HW3/HW3/HW3/test.png";
const char SAFE_TEXTURE[] = "/Users/zouyurou/Desktop/CS3113/SDL_HW3/HW3/HW3/safe.jpeg";
const char SUCCESS_TEXTURE[] ="/Users/zouyurou/Desktop/CS3113/SDL_HW3/HW3/HW3/success.jpeg";
const char FAIL_TEXTURE[] ="/Users/zouyurou/Desktop/CS3113/SDL_HW3/HW3/HW3/fail.png";

// textureId for game success or fail
GLuint successTextureID, failTextureID;
glm::mat4 g_model_success;
glm::mat4 g_model_fail;

// define total number of platforms, including safe platforms,
// obstacle platforms, and regular base platforms
#define PLATFORM_COUNT 13

// create Game State struct
struct GameState {
    Entity *player;
    Entity *platforms;
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
    
    // create success and fail texture id
    successTextureID = LoadTexture(SUCCESS_TEXTURE);
    failTextureID = LoadTexture(FAIL_TEXTURE);
    
    // Craete ball entity
    state.player = new Entity();
    state.player->width = 0.5;
    state.player->set_position(glm::vec3(0, 4.5f, 0));
    state.player->set_movement(glm::vec3(0));
    state.player->set_acceleration(glm::vec3(0, -0.75f, 0));
    state.player->speed = 1.5f;
    state.player->textureID = LoadTexture(BALL_TEXTURE);
        
    // create platform entities
    state.platforms = new Entity[PLATFORM_COUNT];
    GLuint basePlatformTextureID = LoadTexture(OBS_TEXTURE);
    GLuint safePlatformTextureID = LoadTexture(SAFE_TEXTURE);
    GLuint obstaclePlatformTextureID = LoadTexture(OBS_TEXTURE);
    
    float base_x = -4.5f, base_y = -3.25f, obs_x = -2.5f, obs_y = 0.2f, temp_y = 0.8f;
    
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        // set the safe platforms
        if (i == 2 || i == 3 || i == 7 || i == 8) {
            state.platforms[i].textureID = safePlatformTextureID;
            state.platforms[i].set_position(glm::vec3(base_x, base_y, 0));
            state.platforms[i].height = 0.35;
            base_x += 1;
        }
        // set the obstacle platforms on top of the base platforms
        else if (i > 9) {
            state.platforms[i].textureID = obstaclePlatformTextureID;
            state.platforms[i].set_position(glm::vec3(obs_x, obs_y, 0));
            obs_x += 2.5;
            obs_y += temp_y;
            temp_y = -temp_y;
        }
        // set the regular base platforms
        else {
            state.platforms[i].textureID = basePlatformTextureID;
            state.platforms[i].set_position(glm::vec3(base_x, base_y, 0));
            base_x += 1;
        }
    }
    
    for (int i = 0; i < (PLATFORM_COUNT); i++ ){
        state.platforms[i].update(0, NULL, 0);
    }
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
    
    if (state.player->collidedSafe) {
        
    }
    else if (state.player->collidedBottom == false) {
        if (keys[SDL_SCANCODE_A]) {
            state.player->movement.x = -1.0f;
            state.player->acceleration.x = -1.0f;
            state.player->acceleration.y = -0.3f;
            
        }
        else if (keys[SDL_SCANCODE_D]) {
            state.player->movement.x = 1.0f;
            state.player->acceleration.x = 1.0f;
            state.player->acceleration.y = -0.3f;
        }
        
//        if (glm::length(state.player->movement) > 1.0f) {
//            state.player->movement = glm::normalize(state.player->movement);
//        }
    }
    else if (state.player->check_collision(&state.platforms[2]) || state.player->check_collision(&state.platforms[3]) || state.player->check_collision(&state.platforms[7]) || state.player->check_collision(&state.platforms[8])) {
        
        state.player->collidedSafe = true;
        std::cout << "Soph is safe" << std::endl;
    }
    else {
        state.player->isSafe = false;
        std::cout << "Soph fails" << std::endl;
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
        // Notice that we're using FIXED_TIMESTEP as our delta time
        state.player->update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
        delta_time -= FIXED_TIMESTEP;
    }

    g_time_accumulator = delta_time;
}

void draw_object(glm::mat4 &object_model_matrix, GLuint &object_texture_id)
{
    program.set_model_matrix(object_model_matrix);
    glBindTexture(GL_TEXTURE_2D, object_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].render(&program);
    }
    
    // soph successes
    if (state.player->collidedSafe == true) {
        float text1[] =
        {
            -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f
        };
        
        glVertexAttribPointer(program.get_position_attribute(), 2, GL_FLOAT, false, 0, text1);
        glEnableVertexAttribArray(program.get_position_attribute());
            
        float text1_texture[] = {
            0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        };
        
        glVertexAttribPointer(program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, text1_texture);
        glEnableVertexAttribArray(program.get_tex_coordinate_attribute());
        draw_object(g_model_success, successTextureID);
        state.player->isActive = false;
    }
    // soph fails
    else if (state.player->isSafe == false) {
        float text1[] =
        {
            -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f
        };
        
        glVertexAttribPointer(program.get_position_attribute(), 2, GL_FLOAT, false, 0, text1);
        glEnableVertexAttribArray(program.get_position_attribute());
            
        float text1_texture[] = {
            0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        };
        
        glVertexAttribPointer(program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, text1_texture);
        glEnableVertexAttribArray(program.get_tex_coordinate_attribute());
        draw_object(g_model_fail, failTextureID);
        state.player->isActive = false;
    }
    
    state.player->render(&program);
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
