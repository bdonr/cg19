//
// Created by don on 21.05.19.
//

#include <iostream>
#include "SceneObjetFactory.h"
#include "GameLogic.h"
#include "LightFactory.h"


void EnvoirementController::createSunFloorscene(ViewerSP &viewer, CameraSP camera, GroupSP scene) {



    // camera controllers
    camera->translate(glm::vec3(0.f, 1.5f, -9.f))->rotate(180, glm::vec3(0.f, 1.f, 0.f))
            ->dolly(-1.f);



    SceneObjetFactory *insta = SceneObjetFactory::getInstance(viewer);
    LightFactory *lightFactory = LightFactory::getInstance();
    GameLogic::logic(camera, insta->createBullet(),viewer);
    camera->addChild(insta->createBullet());
   //controller->setBullet(insta->createBullet());
    scene->addChild(lightFactory->getSonne());
    ShapeSP h=insta->getHimmel();
    scene->addChild(h);

    lightFactory->getSonne()->addChild(insta->createFlugzeugGruppe());
    lightFactory->getSonne()->addChild(insta->getFloor());
    TransformationSP turm=insta->getTurm();
    lightFactory->getSonne()->addChild(turm);

    for (int i = 0; i < SceneObjetFactory::getInstance(viewer)->getZielscheiben().size(); i++) {
        lightFactory->getSonne()->addChild(SceneObjetFactory::getInstance(viewer)->getZielscheiben().at(i));
    }

    lightFactory->getSonne()->addChild(camera);
    camera->addChild(insta->getCamObject());

    lightFactory->getSonne()->addChild(lightFactory->getVideoSonne2());

}

void EnvoirementController::createVideoSceneHelper(ViewerSP &viewer, CameraSP videoCam, GroupSP scene) {


    videoCam->translate(glm::vec3(0.f, 1.5f, -9.f))->rotate(180, glm::vec3(0.f, 1.f, 0.f))
            ->dolly(-1.f);

    SceneObjetFactory *insta = SceneObjetFactory::getInstance(viewer);
    LightFactory *lightFactory = LightFactory::getInstance();
    scene->addChild(lightFactory->getVideoSonne());

    lightFactory->getVideoSonne()->addChild(insta->createFlugzeugGruppe());

}