//
// Created by don on 21.05.19.
//

#include <iostream>
#include "SceneObjetFactory.h"
#include "GameLogic.h"


void EnvoirementHelper::createSunFloorscene(ViewerSP &viewer, CameraSP &camera, GroupSP &scene) {

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
    GameLogic::logic(camera, insta->createBullet(),viewer);
    camera->addChild(insta->createBullet());
    controller->setDing(insta->createBullet());
    scene->addChild(insta->getSonne());
    scene->addChild(insta->getHimmel());

    insta->getSonne()->addChild(insta->createFlugzeugGruppe());
    insta->getSonne()->addChild(insta->getFloor());
    insta->getSonne()->addChild(insta->getTurm());
    for (int i = 0; i < SceneObjetFactory::getInstance(viewer)->getZielscheiben().size(); i++) {
        SceneObjetFactory::getInstance(viewer)->getSonne()->addChild(
                SceneObjetFactory::getInstance(viewer)->getZielscheiben().at(i));
    }
    insta->getSonne()->addChild(camera);
    camera->addChild(insta->getCamObject());

}

void EnvoirementHelper::createVideoScene(ViewerSP &viewer, CameraSP &camera, GroupSP &scene) {

    viewer->addController(KeyboardController::create(camera));
    camera->translate(glm::vec3(0.f, 1.5f, -9.f))->rotate(180, glm::vec3(0.f, 1.f, 0.f))
            ->dolly(-1.f);

    viewer->addControllers(
            {

                    MouseController::create(camera)
            });

    SceneObjetFactory *insta = SceneObjetFactory::getInstance(viewer);
    scene->addChild(insta->getVideoSonne());
 scene->addChild(insta->getHimmel());
    insta->getVideoSonne()->addChild(insta->getFloor());
    insta->getVideoSonne()->addChild(insta->createFlugzeugGruppe());
    SceneObjetFactory::getInstance(viewer)->getVideoSonne()->addChild(camera);

}