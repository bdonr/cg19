//
// Created by don on 24.05.19.
//

#ifndef CG3_U3_GAMELOGIC_H
#define CG3_U3_GAMELOGIC_H
#include "../scg3/scg3.h"
#include "EnvoirementHelper.h"
#include "MatFactory.h"
#include "TexturHelper.h"
#include "ShaderFactory.h"
#include "SceneObjetFactory.h"

class GameLogic {
public:
   static void logic(CameraSP &camera, TransformationSP &ZielKugelTrans1, TransformationSP &ZielKugelTrans2,
                          TransformationSP &ZielKugelTrans3, ShapeSP &kugel1, ShapeSP &kugel2, ShapeSP &kugel3,
                          TransformationSP &bulletTrans,LightSP light,ViewerSP viewer);
private:
    static double bulletTravel;
    static void bulletTravelAndTest();
    static void checkDurchflugZielscheibe(const glm::vec3 &camObjPos,double time);

    };


#endif //CG3_U3_GAMELOGIC_H
