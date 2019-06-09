//
// Created by don on 21.05.19.
//

#include <iostream>
#include "SceneObjetFactory.h"
#include "GameLogic.h"
#include "LightFactory.h"


void EnvoirementController::createSunFloorscene(ViewerSP &viewer, CameraSP &camera, GroupSP &scene) {

    KeyboardControllerSP controller = KeyboardController::create(camera);
    viewer->addController(controller);

    // camera controllers
    camera->translate(glm::vec3(0.f, 1.5f, -9.f))->rotate(180, glm::vec3(0.f, 1.f, 0.f))
            ->dolly(-1.f);

    viewer->addControllers(
            {

                    MouseController::create(camera)
            });

    SceneObjetFactory *insta = SceneObjetFactory::getInstance(viewer);
    LightFactory *lightFactory = LightFactory::getInstance();
    GameLogic::logic(camera, insta->createBullet(),viewer);
    camera->addChild(insta->createBullet());
    controller->setDing(insta->createBullet());
    scene->addChild(lightFactory->getSonne());
    scene->addChild(insta->getHimmel());

    lightFactory->getSonne()->addChild(insta->createFlugzeugGruppe());
    lightFactory->getSonne()->addChild(insta->getFloor());
    lightFactory->getSonne()->addChild(insta->getTurm());
    for (int i = 0; i < SceneObjetFactory::getInstance(viewer)->getZielscheiben().size(); i++) {
        lightFactory->getSonne()->addChild(SceneObjetFactory::getInstance(viewer)->getZielscheiben().at(i));
    }
    lightFactory->getSonne()->addChild(camera);
    camera->addChild(insta->getCamObject());

}

void EnvoirementController::createVideoScene(ViewerSP &viewer, CameraSP &camera, GroupSP &scene) {

    viewer->addController(KeyboardController::create(camera));
    camera->translate(glm::vec3(0.f, 1.5f, -9.f))->rotate(180, glm::vec3(0.f, 1.f, 0.f))
            ->dolly(-1.f);

    viewer->addControllers(
            {

                    MouseController::create(camera)
            });

    SceneObjetFactory *insta = SceneObjetFactory::getInstance(viewer);
    LightFactory *lightFactory = LightFactory::getInstance();
    scene->addChild(lightFactory->getVideoSonne());
 scene->addChild(insta->getHimmel());
    lightFactory->getVideoSonne()->addChild(insta->getFloor());
    lightFactory->getVideoSonne()->addChild(insta->createFlugzeugGruppe());
    lightFactory->getVideoSonne()->addChild(camera);

}