#include "Scene.hpp"

class Level2 : public Scene {
public:
    int ENEMY_COUNT = 3;
    
    ~Level2();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
