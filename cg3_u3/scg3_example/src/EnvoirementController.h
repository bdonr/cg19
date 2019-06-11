//
// Created by don on 21.05.19.
//

#ifndef CG3_U3_ENVOIREMENTCONTROLLER_H
#define CG3_U3_ENVOIREMENTCONTROLLER_H
#include "scg3.h"
using namespace scg;
class EnvoirementController {
public:
    static  void createSunFloorscene(ViewerSP& viewer, CameraSP& camera, GroupSP &scene);
    static void createVideoSceneHelper(ViewerSP &viewer, CameraSP &camera, GroupSP &scene);

};


#endif //CG3_U3_ENVOIREMENTCONTROLLER_H
