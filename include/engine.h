#ifndef SENGINE_ENGINE_H
#define SENGINE_ENGINE_H

#include <SDL.h>
#include <list>
#include <memory>

#include "loop_runners/loop_runner.h"

#include "loop_runners/entity_loop_runner.h"


class Engine {
public:
    Engine();
    ~Engine();

    void Initialize();

    SDL_Renderer *GetRenderer();
    SDL_Window *GetWindow();
    SDL_GLContext *GetGlContext();

    std::shared_ptr<LoopRunner> GetLoopRunner(LoopRunnerType loopRunnerType);
    void AddLoopRunner(std::shared_ptr<LoopRunner> &loopRunner);

    void SetFps(int targetFps);


    void Update(double deltaTime);
    void Render();
    void RunLoop();
private:
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_GLContext glContext;

    std::list<std::shared_ptr<LoopRunner>> loopRunners;

    bool loopRunning;

    int fps;
    float cycleTime;

    int windowWidth;
    int windowHeight;

    void InitSdl();
    void InitOpenGl();
};

extern Engine *g_Engine;

#endif //SENGINE_ENGINE_H
