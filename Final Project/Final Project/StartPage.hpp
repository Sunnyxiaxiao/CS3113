#include "Scene.hpp"

class StartPage : public Scene {
public:
    int ENEMY_COUNT = 0;
    int potion = 0;
    
    ~StartPage();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
