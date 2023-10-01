/**
* Author: Yurou Zou
* Assignment: Simple 2D Scene
* Date due: 2023-09-30, 11:59pm
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

const char V_SHADER_PATH[] = "/Users/zouyurou/Desktop/CS3113/SDL_HW1/SDL_HW1/shaders/vertex_textured.glsl", F_SHADER_PATH[] = "/Users/zouyurou/Desktop/CS3113/SDL_HW1/SDL_HW1/shaders/fragment_textured.glsl";

const char file1[] = "/Users/zouyurou/Desktop/CS3113/SDL_HW1/test.png";
const char file2[] =  "/Users/zouyurou/Desktop/CS3113/SDL_HW1/soph.png";
GLuint tex1;
GLuint tex2;

const int NUMBER_OF_TEXTURES = 1; // to be generated, that is
const GLint LEVEL_OF_DETAIL = 0;  // base image level; Level n is the nth mipmap reduction image
const GLint TEXTURE_BORDER = 0;   // this value MUST be zero

SDL_Window* g_display_window;
ShaderProgram g_shader_program;
glm::mat4 view_matrix, m_model_matrix1, m_model_matrix2, m_projection_matrix, m_trans_matrix;

bool g_game_is_running = true;
bool g_is_growing1 = true, g_is_growing2 = true;
int g_frame_counter1 = 0, g_frame_counter2 = 0;

const float MILLISECONDS_IN_SECOND = 1000.0;
float m_previous_ticks = 0.0f;
const float GROWTH_FACTOR = 1.05f;
const float SHRINK_FACTOR = 0.95f;
const float ROT_ANGLE = glm::radians(1.5f);
const int MAX_FRAME1 = 40, MAX_FRAME2 = 40;
float m1_translate = 0.0f;
float m2_translate = 0.0f;
float m2_rotate = 0.0f;


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
    g_display_window = SDL_CreateWindow("HW1",
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
    
    tex1 = LoadTexture(file1);
    tex2 = LoadTexture(file2);
    
    view_matrix       = glm::mat4(1.0f);
    m_model_matrix1     = glm::mat4(1.0f); // Defines the position (location and orientation) of the camera
    m_model_matrix2     = glm::mat4(1.0f);
    m_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f); // Defines the characteristics of your camera, such as clip planes, field of view, projection method etc.
    
    g_shader_program.set_projection_matrix(m_projection_matrix);
    g_shader_program.set_view_matrix(view_matrix);
    
    glUseProgram(g_shader_program.get_program_id());
    
    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(BG_RED, BG_GREEN, BG_BLUE, BG_OPACITY);
}

void process_input(){
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            g_game_is_running = false;
        }
    }
};

void update() {
    float ticks = (float) SDL_GetTicks() / MILLISECONDS_IN_SECOND; // get the current number of ticks
    float delta_time = ticks - m_previous_ticks; // the delta time is the difference from the last frame
    m_previous_ticks = ticks;
    
    m1_translate += 0.5f * delta_time;
    
    // need this line for reset the matrix1 to the origin scale
    m_model_matrix1 = glm::mat4(1.0f);
    
    glm::vec3 scale_vector1;
    g_frame_counter1 += 1;
    
    // STEP 2
    if (g_frame_counter1 >= MAX_FRAME1)
    {
        g_is_growing1 = !g_is_growing1;
        g_frame_counter1 = 0;
    }
    
    // STEP 4
    scale_vector1 = glm::vec3(g_is_growing1 ? GROWTH_FACTOR : SHRINK_FACTOR,
                             g_is_growing1 ? GROWTH_FACTOR : SHRINK_FACTOR,
                            1.0f);
    
    // STEP 4
    m_model_matrix1 = glm::scale(m_model_matrix1, scale_vector1);
    m_model_matrix1 = glm::translate(m_model_matrix1, glm::vec3(m1_translate, m1_translate, 0.0f));
    
//    m_model_matrix2 = glm::mat4(1.0f); No need this line for rotating
    glm::vec3 scale_vector2;
    g_frame_counter2 += 1;
    m_model_matrix2 = glm::rotate(m_model_matrix2, ROT_ANGLE, glm::vec3(0.0f, 0.0f, 1.0f));
    if (g_frame_counter2 >= MAX_FRAME2) {
        g_is_growing2 = !g_is_growing2;
        g_frame_counter2 = 0;
    }
    scale_vector2 = glm::vec3(g_is_growing2 ? GROWTH_FACTOR : SHRINK_FACTOR,
                             g_is_growing2 ? GROWTH_FACTOR : SHRINK_FACTOR,
                            1.0f);
    m_model_matrix2 = glm::scale(m_model_matrix2, scale_vector2);
}

void draw_object(glm::mat4 &object_model_matrix, GLuint &object_texture_id, const int k)
{
    g_shader_program.set_model_matrix(object_model_matrix);
    glBindTexture(GL_TEXTURE_2D, object_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, k);
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Vertices: rect
    float vertices1[] = {
        -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,  // triangle 1
        -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f   // triangle 2
    };
    
    // Textures: rect
    float texture_coordinates1[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,     // triangle 1
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,     // triangle 2
    };
    
    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices1);
    glEnableVertexAttribArray(g_shader_program.get_position_attribute());
    
    glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texture_coordinates1);
    glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());
    
    draw_object(m_model_matrix1, tex1, 6);
    
    // Vertices: triangle
    float vertices2[] = {
        -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f
    };
    
    // Textures: triangle
    float texture_coordinates2[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f
    };
    
    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices2);
    glEnableVertexAttribArray(g_shader_program.get_position_attribute());
    
    glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texture_coordinates2);
    glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());
    
    draw_object(m_model_matrix2, tex2, 3);
    
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
