#include "Scene.hpp"

class Level3 : public Scene {
public:
    int ENEMY_COUNT = 4;
    
    ~Level3();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
