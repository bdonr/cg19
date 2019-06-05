//
// Created by don on 21.05.19.
//

#ifndef CG3_U3_ENVOIREMENTHELPER_H
#define CG3_U3_ENVOIREMENTHELPER_H
#include "../scg3/scg3.h"
using namespace scg;
class EnvoirementHelper {
public:
    static  void createSunFloorscene(ViewerSP& viewer, CameraSP& camera, GroupSP &scene);
    static void createVideoScene(ViewerSP &viewer, CameraSP &camera, GroupSP &scene);

};


#endif //CG3_U3_ENVOIREMENTHELPER_H
