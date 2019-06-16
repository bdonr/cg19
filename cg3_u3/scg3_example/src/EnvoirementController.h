//
// Created by don on 21.05.19.
//

#ifndef CG3_U3_ENVOIREMENTCONTROLLER_H
#define CG3_U3_ENVOIREMENTCONTROLLER_H
#include "scg3.h"
#include "LightFactory.h"
#include "SceneObjetFactory.h"

using namespace scg;

/**
 * forward declaration Of SceneObjetFactory
 */
class SceneObjetFactory;
class LightFactory;
class EnvoirementController {

private:
     SceneObjetFactory *insta;
     LightFactory *lightFactory;
     static EnvoirementController* controllerInstance;
    EnvoirementController(ViewerSP &viewer);
public:
    /**
     * Create instance of EnvoirementController
     * @param viewer
     * @return
     */
    static EnvoirementController* getControllerInstance(ViewerSP &viewer);

    /**
     * Creates all Objects of the StandartScene
     * @param viewer
     * @param camera
     * @param scene
     */
    void createStandartScene(ViewerSP &viewer, CameraSP camera, GroupSP scene);

    /**
     * Creates all Objects of the VideoScene
     * @param viewer
     * @param camera
     * @param scene
     */
     void createVideoScene(ViewerSP &viewer, CameraSP camera, GroupSP scene);

};


#endif //CG3_U3_ENVOIREMENTCONTROLLER_H
