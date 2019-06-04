//
// Created by don on 24.05.19.
//

#ifndef CG3_U3_GAMELOGIC_H
#define CG3_U3_GAMELOGIC_H
#include "../scg3/scg3.h"
#include "EnvoirementHelper.h"
#include "MatFactory.h"
#include "TexturFactory.h"
#include "ShaderFactory.h"
#include "SceneObjetFactory.h"

class GameLogic {
public:
    static void logic(CameraSP &camera,TransformationSP &bulletTrans, ViewerSP& viewer);
private:
    static double bulletTravel;
    static void bulletTravelAndTest();
     static void checkDurchflugZielscheibe(const glm::vec3 &camObjPos, double time);
     static void checkBulletTreffer(const glm::vec3 &bullet, double time,ViewerSP& viewer);

    static void checkDurchflugZielscheibe(const glm::vec3 &camObjPos, double time, ViewerSP& viewer);

};


#endif //CG3_U3_GAMELOGIC_H
