//
// Created by don on 24.05.19.
//

#include "GameLogic.h"

double GameLogic::bulletTravel = 0;
void GameLogic::logic(CameraSP &camera, TransformationSP &ZielKugelTrans1, TransformationSP &ZielKugelTrans2,
           TransformationSP &ZielKugelTrans3, ShapeSP &kugel1, ShapeSP &kugel2, ShapeSP &kugel3,
           TransformationSP &bulletTrans,LightSP light,ViewerSP viewer) {
    ZielKugelTrans1 = SceneObjetFactory::getZielscheiben()[0];
    ZielKugelTrans2 = SceneObjetFactory::getZielscheiben()[1];
    ZielKugelTrans3 = SceneObjetFactory::getZielscheiben()[2];

    bulletTrans = Transformation::create();
    auto TransAni = TransformAnimation::create();
    ZielKugelTrans2->setVisible(false);
    ZielKugelTrans3->setVisible(false);
    auto camObjectTrans = Transformation::create();
    bulletTrans->translate(glm::vec3(0.02f, -0.08f, -0.2f));
    TransAni->setUpdateFunc(
            [camera, light, bulletTrans, ZielKugelTrans1, ZielKugelTrans2, ZielKugelTrans3, camObjectTrans](
                    TransformAnimation *anim, double currTime, double diffTime, double totalTime) {
/* die projektile bewegen sich die ganze zeit, sie laufen vorwärts und springen dan beim errreichen
 * der maximal distanz zurück, die kugeln sind daurhaft unsichbar nur beim drücken von SPACE werden
 * diese sichtbar und können interagieren
 /            std::cout<<camera->getMatrix()[3][0];
             std::cout<<camera->getMatrix()[3][1]<<;
             std::cout<<camera->getMatrix()[3][2];
 **/               bulletTrans->translate(glm::vec3(0, 0., -0.02));
                bulletTravelAndTest();
                if (bulletTravel > 25) {
                    bulletTrans->translate(glm::vec3(0, 0, 2.5));
/*
 * 0.1 * 5 = 0.5 * 5 = 2.5 rechung für den zurücksprung
 */
                }
/*kolision detection beim rammen des flugzeugs gegen die objekte*/
                glm::mat4 tempCamObjMat = camera->getMatrix();
                tempCamObjMat = glm::translate(tempCamObjMat, glm::vec3(0, -0.08, -0.2));

                float CamObjX = tempCamObjMat[3][0];
                float CamObjY = tempCamObjMat[3][1];
                float CamObjZ = tempCamObjMat[3][2];

                glm::vec3 camObjPos = glm::vec3(CamObjX, CamObjY, CamObjZ);
                glm::vec3 kugelObjPos1 = glm::vec3(ZielKugelTrans1->getMatrix()[3][0],
                                                   ZielKugelTrans1->getMatrix()[3][1],
                                                   ZielKugelTrans1->getMatrix()[3][2]);
                glm::vec3 kugelObjPos2 = glm::vec3(ZielKugelTrans2->getMatrix()[3][0],
                                                   ZielKugelTrans2->getMatrix()[3][1],
                                                   ZielKugelTrans2->getMatrix()[3][2]);
                glm::vec3 kugelObjPos3 = glm::vec3(ZielKugelTrans3->getMatrix()[3][0],
                                                   ZielKugelTrans3->getMatrix()[3][1],
                                                   ZielKugelTrans3->getMatrix()[3][2]);


                checkDurchflugZielscheibe(camObjPos,totalTime);


                /*kolision detection beim treffen der projektile auf objecte*/
                glm::mat4 tempBulletMat = camera->getMatrix();
                tempBulletMat *= bulletTrans->getMatrix();

                float bulletX = tempBulletMat[3][0];
                float bulletY = tempBulletMat[3][1];
                float bulletZ = tempBulletMat[3][2];

                glm::vec3 bullObjPos = glm::vec3(bulletX, bulletY, bulletZ);
                glm::vec3 bullSomeDif1 = bullObjPos;
                bullSomeDif1 -= kugelObjPos1;
                glm::vec3 bullSomeDif2 = bullObjPos;
                bullSomeDif2 -= kugelObjPos2;
                glm::vec3 bullSomeDif3 = bullObjPos;
                bullSomeDif3 -= kugelObjPos3;

                float bulldiff1 = sqrt(pow(bullSomeDif1.x, 2) + pow(bullSomeDif1.y, 2) + pow(bullSomeDif1.z, 2));
                float bulldiff2 = sqrt(pow(bullSomeDif2.x, 2) + pow(bullSomeDif2.y, 2) + pow(bullSomeDif2.z, 2));
                float bulldiff3 = sqrt(pow(bullSomeDif3.x, 2) + pow(bullSomeDif3.y, 2) + pow(bullSomeDif3.z, 2));

                double bullrad1 = 0.1;
                double bullrad2 = 0.1;
                if (ZielKugelTrans1->isVisible()) {
                    if (glm::abs(bullrad1 - bullrad2) < bulldiff1 && bulldiff1 < (bullrad1 + bullrad2) &&
                        bulletTrans->isVisible()) {
                        ZielKugelTrans1->setVisible(false);
                        ZielKugelTrans2->setVisible(true);
                    }
                } else if (ZielKugelTrans2->isVisible()) {
                    if (glm::abs(bullrad1 - bullrad2) < bulldiff2 && bulldiff2 < (bullrad1 + bullrad2) &&
                        bulletTrans->isVisible()) {
                        ZielKugelTrans2->setVisible(false);
                        ZielKugelTrans3->setVisible(true);
                    }
                } else if (ZielKugelTrans3->isVisible()) {
                    if (glm::abs(bullrad1 - bullrad2) < bulldiff3 && bulldiff3 < (bullrad1 + bullrad2) &&
                        bulletTrans->isVisible()) {
                        ZielKugelTrans3->setVisible(false);
                        ZielKugelTrans1->setVisible(true);
                    }
                }


                /**        if (camera->getPosition().x > 15) {
                            camera->rotate(15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                        } else if (camera->getPosition().x < -15) {
                            camera->rotate(15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                        } else if (camera->getPosition().z > 15) {
                            camera->rotate(15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                        } else if (camera->getPosition().z < -15) {
                            camera->rotate(15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                        } else if (camera->getPosition().y < 1.0) {
                            camera->rotate(-15.0f, glm::vec3(1.0f, 0.f, 0.0f));
                        } else if (camera->getPosition().y > 10.0) {
                            camera->rotate(15.0f, glm::vec3(1.0f, 0.f, 0.0f));
                        }
        **/
                /*
                 * interesant: die bewegung der kamera ist spiegelverkehrt wegen dem blick in die negative Z-Achse
                 * also bewegt sich die camera nach -Z
                 * aber
                 * auch die geschwindigkeit der transformation ist anders etwa halb so stark
                 * die rotation skalierung ist gleich allerdings richtet sich die cammere nach den weltcoordinaten aus
                 * und die objekte nach ihren eigenen UND die rations richtung ist spiegelverkehrt.
                 */
                // camera->rotate(-0.7f, glm::vec3(1.0f, 0.0f, 0.0f))->translate(glm::vec3(0, 0, -0.001));


                // std::cout << totalTime << std::endl;

            });
    viewer->addAnimation(TransAni);

}

void GameLogic::checkDurchflugZielscheibe(const glm::vec3 &camObjPos,double time) {
    double rad1 = 0.2;
    double rad2 = 0.3;


    std::vector<TransformationSP> x = SceneObjetFactory::getZielscheiben();
    for (int i = 0; i < x.size(); i++) {
        glm::vec3 kk = glm::vec3(x[i]->getMatrix()[3][0],
                                 x[i]->getMatrix()[3][1],
                                 x[i]->getMatrix()[3][2]);
        float diff1 = sqrt(pow(camObjPos.x - kk.x, 2) + pow(camObjPos.y - kk.y, 2) + pow(camObjPos.z - kk.z, 2));
        if (x[i]->isVisible()) {
            if (glm::abs(rad1 - rad2) < diff1 && diff1 < (rad1 + rad2)) {
                x[i]->setVisible(false);
                x[i + 1]->setVisible(true);
            }
        }

    }
}

void GameLogic::bulletTravelAndTest() {
    if (bulletTravel > 25) {
        bulletTravel = 0;
    }
    bulletTravel = bulletTravel + 0.2;




}

