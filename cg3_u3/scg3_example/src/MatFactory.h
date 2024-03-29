//
// Created by don on 21.05.19.
//

#ifndef CG3_U3_MATFACTORY_H
#define CG3_U3_MATFACTORY_H

#include "scg3.h"

using namespace scg;

class MatFactory {

private:
    MaterialCoreSP red;
    MaterialCoreSP white;
    MaterialCoreSP black;
    MaterialCoreSP cyan;
    MaterialCoreSP stadt;
    MaterialCoreSP gold;
    static MatFactory *instance;

    MatFactory();

public:
    static MatFactory *getInstance();

    const MaterialCoreSP &getRed();

    const MaterialCoreSP &getWhite();

    /**
     * Black Material
     * @return MaterialCoreSP
     */
    const MaterialCoreSP &getBlack();
    const MaterialCoreSP &getCyan();

    const MaterialCoreSP &getStadt();

    const MaterialCoreSP &getGold();

private:

    MaterialCoreSP
    createMat(const glm::vec4 &ambient, const glm::vec4 &specular, const glm::vec4 &diffuse, const float &shine);

    MaterialCoreSP
    createMat(const glm::vec4 &ambient, const glm::vec4 &specular, const glm::vec4 &diffuse, const float &shine,
              const glm::vec4 &emission);


};


#endif //CG3_U3_MATFACTORY_H
