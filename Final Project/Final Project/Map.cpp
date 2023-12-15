/**
* Author: Yurou Zou
* Assignment: Rise of the AI
* Date due: 2023-11-18, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#include "Map.hpp"
using namespace std;

Map::Map(int width, int height, unsigned int *level_data, GLuint texture_id, float tile_size, int tile_count_x, int tile_count_y)
{
    m_width = width;
    m_height = height;
    cout << m_width << m_height << endl;
    
    m_level_data = level_data;
    m_texture_id = texture_id;
    
    m_tile_size = tile_size;
    m_tile_count_x = tile_count_x;
    m_tile_count_y = tile_count_y;
    
    build();
}

void Map::build()
{
    for(int y = 0; y < m_height; y++)
    {
        for(int x = 0; x < m_width; x++) {
            int tile = m_level_data[y * m_width + x];
            
            if (tile == 0) continue;
            
            float u = (float) (tile % m_tile_count_x) / (float) m_tile_count_x;
            float v = (float) (tile / m_tile_count_x) / (float) m_tile_count_y;
            
            float tile_width  = 1.0f / (float) m_tile_count_x;
            float tile_height = 1.0f / (float) m_tile_count_y;
            
            float x_offset = -(m_tile_size / 2); // From center of tile
            float y_offset =  (m_tile_size / 2); // From center of tile
            
            this->m_vertices.insert(m_vertices.end(), {
                x_offset + (m_tile_size * x) - m_width / 2 + m_tile_size / 2,  y_offset +  -m_tile_size * y + m_height / 2  - m_tile_size / 2,
                x_offset + (m_tile_size * x) - m_width / 2  + m_tile_size / 2,  y_offset + (-m_tile_size * y) - m_tile_size + m_height / 2  - m_tile_size / 2,
                x_offset + (m_tile_size * x) + m_tile_size - m_width / 2 + m_tile_size / 2, y_offset + (-m_tile_size * y) - m_tile_size + m_height / 2 - m_tile_size / 2,
                x_offset + (m_tile_size * x) - m_width / 2 + m_tile_size / 2, y_offset + -m_tile_size * y + m_height / 2 - m_tile_size / 2,
                x_offset + (m_tile_size * x) + m_tile_size - m_width / 2 + m_tile_size / 2, y_offset + (-m_tile_size * y) - m_tile_size + m_height / 2 - m_tile_size / 2,
                x_offset + (m_tile_size * x) + m_tile_size - m_width / 2 + m_tile_size / 2, y_offset +  -m_tile_size * y + m_height / 2 - m_tile_size / 2
            });
            this->m_texture_coordinates.insert(m_texture_coordinates.end(), {
                u, v,
                u, v + (tile_height),
                u + tile_width, v + (tile_height),
                u, v,
                u + tile_width, v + (tile_height),
                u + tile_width, v
            });
        }
    }
    
    m_left_bound   = -m_tile_size*m_width/2;
    m_right_bound  = m_tile_size*m_width/2;
    m_top_bound    = m_tile_size*m_height/2;
    m_bottom_bound = -m_tile_size*m_height/2;
    cout << "tile_size: " << m_tile_size << endl;
    cout << "width: " << m_width << " height: " << m_height << endl;
    cout << "bound: " << m_left_bound << " " << m_right_bound << " " << m_top_bound << " " << m_bottom_bound << endl;
}

void Map::render(ShaderProgram* program)
{
    glm::mat4 model_matrix = glm::mat4(1.0f);
    program->set_model_matrix(model_matrix);

    glUseProgram(program->get_program_id());

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, m_vertices.data());
    glEnableVertexAttribArray(program->get_position_attribute());
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, m_texture_coordinates.data());
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glDrawArrays(GL_TRIANGLES, 0, (int)m_vertices.size() / 2);
    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_position_attribute());
}

bool Map::is_solid(glm::vec3 position, float *penetration_x, float *penetration_y)
{
    *penetration_x = 0;
    *penetration_y = 0;

    int tile_x = 0;
    int tile_y = 0;

    tile_x = floor((((int(floor(position.x))+int(floor(m_width/2))) % m_width)));
    tile_y = m_height/2 - ceil(position.y);
    
    int tile = m_level_data[(tile_y) * m_height + tile_x];

    if (tile != 0) return true;

    return false;
}
