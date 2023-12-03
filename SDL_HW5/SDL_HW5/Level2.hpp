#include "Scene.hpp"

class Level2 : public Scene {
public:
    int ENEMY_COUNT = 1;
    
    ~Level2();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
