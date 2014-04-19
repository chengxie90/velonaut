#include "inputtest.h"
#include <SDL2/SDL.h>
#include "gametest.h"

#include <bullet/btBulletDynamicsCommon.h>

InputTest::InputTest()
{
}

void InputTest::init()
{

}

void InputTest::update()
{
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_UP]) {
        double scale = .01; if (!keys[SDL_SCANCODE_SPACE]) scale *= 1.1;
        std::vector<double> torque = GameTest::GetApp()->getPlayerRight();
        torque[0] *= scale; torque[1] *= scale; torque[2] *= scale;
        GameTest::GetApp()->applyPlayerTorque(torque);
    }

    if (keys[SDL_SCANCODE_DOWN]) {
        double scale = -.01; if (!keys[SDL_SCANCODE_SPACE]) scale *= 1.1;
        std::vector<double> torque = GameTest::GetApp()->getPlayerRight();
        torque[0] *= scale; torque[1] *= scale; torque[2] *= scale;
        GameTest::GetApp()->applyPlayerTorque(torque);
    }

    if (keys[SDL_SCANCODE_RIGHT]) {
        double scale = -.02; if (!keys[SDL_SCANCODE_SPACE]) scale *= 1.1;
        std::vector<double> torque = GameTest::GetApp()->getPlayerUp();
        torque[0] *= scale; torque[1] *= scale; torque[2] *= scale;
        GameTest::GetApp()->applyPlayerTorque(torque);
    }

    if (keys[SDL_SCANCODE_LEFT]) {
        double scale = .02; if (!keys[SDL_SCANCODE_SPACE]) scale *= 1.1;
        std::vector<double> torque = GameTest::GetApp()->getPlayerUp();
        torque[0] *= scale; torque[1] *= scale; torque[2] *= scale;
        GameTest::GetApp()->applyPlayerTorque(torque);
    }

    if (keys[SDL_SCANCODE_A]) {
        double scale = -.03;
        std::vector<double> torque = GameTest::GetApp()->getPlayerLook();
        torque[0] *= scale; torque[1] *= scale; torque[2] *= scale;
        GameTest::GetApp()->applyPlayerTorque(torque);
    }

    if (keys[SDL_SCANCODE_D]) {
        double scale = .03;
        std::vector<double> torque = GameTest::GetApp()->getPlayerLook();
        torque[0] *= scale; torque[1] *= scale; torque[2] *= scale;
        GameTest::GetApp()->applyPlayerTorque(torque);
    }
    if (keys[SDL_SCANCODE_SPACE]) {
        double scale = 10000;
        std::vector<double> force = GameTest::GetApp()->getPlayerLook();
        force[0] *= scale; force[1] *= scale; force[2] *= scale;
        GameTest::GetApp()->applyPlayerForce(force);
    }


    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                GameTest::GetApp()->terminate();
            }
            /*
            if (e.key.keysym.sym == SDLK_UP) {
                std::vector<double> camPos = GraphicsTest::GetInstance()->getCameraPosition();
                std::vector<double> camDir = GraphicsTest::GetInstance()->getCameraDerivedUp();
                double moveRate = 0.1;
                std::vector<double> newPos = std::vector<double>(3);
                newPos[0] = camPos[0] + (moveRate * camDir[0]);
                newPos[1] = camPos[1] + (moveRate * camDir[1]);
                newPos[2] = camPos[2] + (moveRate * camDir[2]);
                GraphicsTest::GetInstance()->setCameraPostion(newPos);
            }
            if (e.key.keysym.sym == SDLK_DOWN) {
                std::vector<double> camPos = GraphicsTest::GetInstance()->getCameraPosition();
                std::vector<double> camDir = GraphicsTest::GetInstance()->getCameraDerivedUp();
                double moveRate = -0.1;
                std::vector<double> newPos = std::vector<double>(3);
                newPos[0] = camPos[0] + (moveRate * camDir[0]);
                newPos[1] = camPos[1] + (moveRate * camDir[1]);
                newPos[2] = camPos[2] + (moveRate * camDir[2]);
                GraphicsTest::GetInstance()->setCameraPostion(newPos);
            }
            if (e.key.keysym.sym == SDLK_RIGHT) {
                std::vector<double> camPos = GraphicsTest::GetInstance()->getCameraPosition();
                std::vector<double> camDir = GraphicsTest::GetInstance()->getCameraDerivedDirection();
                std::vector<double> up = GraphicsTest::GetInstance()->getCameraDerivedUp();
                btVector3 rightVec = (btVector3(up[0], up[1], up[2]).cross(btVector3(camDir[0], camDir[1], camDir[2]))).normalized();
                double moveRate = -0.1;
                std::vector<double> newPos = std::vector<double>(3);
                newPos[0] = camPos[0] + (moveRate * rightVec.x());
                newPos[1] = camPos[1] + (moveRate * rightVec.y());
                newPos[2] = camPos[2] + (moveRate * rightVec.z());
                GraphicsTest::GetInstance()->setCameraPostion(newPos);
            }
            if (e.key.keysym.sym == SDLK_LEFT) {
                std::vector<double> camPos = GraphicsTest::GetInstance()->getCameraPosition();
                std::vector<double> camDir = GraphicsTest::GetInstance()->getCameraDerivedDirection();
                std::vector<double> up = GraphicsTest::GetInstance()->getCameraDerivedUp();
                btVector3 rightVec = (btVector3(up[0], up[1], up[2]).cross(btVector3(camDir[0], camDir[1], camDir[2]))).normalized();
                double moveRate = 0.1;
                std::vector<double> newPos = std::vector<double>(3);
                newPos[0] = camPos[0] + (moveRate * rightVec.x());
                newPos[1] = camPos[1] + (moveRate * rightVec.y());
                newPos[2] = camPos[2] + (moveRate * rightVec.z());
                GraphicsTest::GetInstance()->setCameraPostion(newPos);
            }
            if (e.key.keysym.sym == SDLK_EQUALS) {
                std::vector<double> camPos = GraphicsTest::GetInstance()->getCameraPosition();
                std::vector<double> camDir = GraphicsTest::GetInstance()->getCameraDerivedDirection();
                double moveRate = 0.1;
                std::vector<double> newPos = std::vector<double>(3);
                newPos[0] = camPos[0] + (moveRate * camDir[0]);
                newPos[1] = camPos[1] + (moveRate * camDir[1]);
                newPos[2] = camPos[2] + (moveRate * camDir[2]);
                GraphicsTest::GetInstance()->setCameraPostion(newPos);
            }
            if (e.key.keysym.sym == SDLK_MINUS) {
                std::vector<double> camPos = GraphicsTest::GetInstance()->getCameraPosition();
                std::vector<double> camDir = GraphicsTest::GetInstance()->getCameraDerivedDirection();
                double moveRate = -0.1;
                std::vector<double> newPos = std::vector<double>(3);
                newPos[0] = camPos[0] + (moveRate * camDir[0]);
                newPos[1] = camPos[1] + (moveRate * camDir[1]);
                newPos[2] = camPos[2] + (moveRate * camDir[2]);
                GraphicsTest::GetInstance()->setCameraPostion(newPos);
            }
    */

        }
        if (e.type == SDL_QUIT) {
            GameTest::GetApp()->terminate();
        }
    }
}

void InputTest::shutdown()
{

}

InputTest *InputTest::GetInstance()
{
    return GameTest::GetApp()->GetInput();
}
