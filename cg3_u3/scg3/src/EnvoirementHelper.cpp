//
// Created by don on 21.05.19.
//

#include <iostream>
#include "SceneObjetFactory.h"


void createSterne(LightSP *mats, LightPositionSP *lightPositionSp) {

    for (int j = 0; j < 50; j++) {
        float sx1 = rand() % 800 + 1;
        float sy1 = rand() % 180 + 19;
        float sz1 = rand() % 800 + 1;
        std::cout << sx1 << " " << sy1 << " " << sz1 << std::endl;
        mats[j] = Light::create();
        mats[j]->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))->setDiffuse(glm::vec4(.0, 0, 0, 1))->setAmbient(
                        glm::vec4(.1, .1, .1, 1))

                ->init();
        mats[j]->setPosition(glm::vec4(sx1, sy1, sz1, 1));
        lightPositionSp[j] = LightPosition::create(mats[j]);
    }
}

int dmod(double time, double d) {
    return time - (int) (time / d) * d;
}


void EnvoirementHelper::createSunFloorscene(ViewerSP& viewer, CameraSP& camera, GroupSP &scene) {


    // camera controllers
    camera->translate(glm::vec3(0.f, 1.5f, -9.f))->rotate(180, glm::vec3(0.f, 1.f, 0.f))
            ->dolly(-1.f);

    viewer->addControllers(
            {

                    MouseController::create(camera)
            });

    SceneObjetFactory* insta= SceneObjetFactory::getInstance(viewer);

  /**  auto mondAnim = TransformAnimation::create();
    float angularVelMond = 1.f;
    glm::vec3 axisMond(0.f, 1.f, 0.f);
    teapotAnim->setUpdateFunc(
            [angularVelMond, axisMond](TransformAnimation *animation,
                               double currTime, double diffTime, double totalTime) {
                animation->rotate(angularVelMond, axisMond);
            });
    viewer->addAnimation(mondAnim);

    auto mondAnimTrans = Transformation::create();
    mondAnimTrans->translate(glm::vec3(0.3f, 0.f, 0.f));


    auto mondTrans = Transformation::create();
    mondTrans->rotate(-90.f, glm::vec3(1.f, 0.f, 0.f));

    mondAnim->addChild(mondAnimTrans);
    mondAnimTrans->addChild(SceneObjetFactory::getMond());

**/
    scene->addChild(insta->getSonne());
    scene->addChild(insta->getHimmel());

    insta->getSonne()->addChild(insta->getGroup());
    insta->getSonne()->addChild(insta->getFloor());
    insta->getSonne()->addChild(insta->getTurm());
    camera->addChild(insta->getCamObject());

   /** auto links = insta->getKugel();
    links->translate(glm::vec3(1,0,0));

    auto rechts = insta->getKugel();
    rechts->translate(glm::vec3(-1,0,0));

    insta->getLinks()->addChild(links);


    camera->addChild(insta->getRechts());
    insta->getRechts()->addChild(rechts);
    **/
    viewer->startAnimations();
}