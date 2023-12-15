#include "Scene.hpp"

class Level1 : public Scene {
public:
    int ENEMY_COUNT = 3;
    
    ~Level1();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
