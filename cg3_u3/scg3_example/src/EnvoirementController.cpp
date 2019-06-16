//
// Created by don on 21.05.19.
//

#include <iostream>
#include "SceneObjetFactory.h"
#include "GameLogic.h"
#include "LightFactory.h"

EnvoirementController* EnvoirementController::controllerInstance;

void EnvoirementController::createStandartScene(ViewerSP &viewer, CameraSP camera, GroupSP scene) {


    // camera controllers
    camera->translate(glm::vec3(0.f, 1.5f, -9.f))->rotate(180, glm::vec3(0.f, 1.f, 0.f))
            ->dolly(-1.f);
    GameLogic::logic(camera, insta->createBullet(),viewer);
    camera->addChild(insta->createBullet());
   // controller->setBullet(insta->createBullet());
    scene->addChild(lightFactory->getSonne());
    scene->addChild(insta->getSky());


    lightFactory->getSonne()->addChild(insta->getFloor());
    lightFactory->getSonne()->addChild(insta->getTower());

    for (int i = 0; i < SceneObjetFactory::getInstance(viewer)->getTargets().size(); i++) {
        lightFactory->getSonne()->addChild(insta->getTargets().at(i));
    }
    lightFactory->getSonne()->addChild(camera);
    camera->addChild(insta->getCamObject());

}

void EnvoirementController::createVideoScene(ViewerSP &viewer, CameraSP camera, GroupSP scene) {


    camera->translate(glm::vec3(0.f, 1.5f, -9.f))->rotate(180, glm::vec3(0.f, 1.f, 0.f))
            ->dolly(-1.f);

    //Scene bauen mittell helper factory

    //Licht in die Scene stellen
    scene->addChild(lightFactory->getVideoSonne());

    //camera ins licht stellen
    lightFactory->getVideoSonne()->addChild(camera);


    //Transformationen ins licht stellen
    lightFactory->getVideoSonne()->addChild(insta->createJetGroup());
    lightFactory->getVideoSonne()->addChild(insta->createJetGroup2());
    lightFactory->getVideoSonne()->addChild(insta->getVideoSky());
    lightFactory->getVideoSonne()->addChild(insta->getVideoFloor());

}

EnvoirementController::EnvoirementController(ViewerSP &viewer) {
    insta = SceneObjetFactory::getInstance(viewer);
    //Licht holen
    lightFactory = LightFactory::getInstance();
}


 EnvoirementController* EnvoirementController::getControllerInstance(ViewerSP &viewer){
    if(controllerInstance== nullptr){
        controllerInstance=new EnvoirementController(viewer);
    }
     return controllerInstance;
}

