//
//  StartPage.hpp
//  SDL_HW5
//
//  Created by 邹雨柔 on 12/1/23.
//

#ifndef StartPage_hpp
#define StartPage_hpp

#include <stdio.h>

#endif /* StartPage_hpp */

#include "Scene.hpp"

class StartPage : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //
    int ENEMY_COUNT = 0;
    
    // ————— CONSTRUCTOR ————— //
    ~StartPage();
    
    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
