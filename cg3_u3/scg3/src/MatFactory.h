//
// Created by don on 21.05.19.
//

#ifndef CG3_U3_MATFACTORY_H
#define CG3_U3_MATFACTORY_H
#include "../scg3/scg3.h"
using namespace scg;
class MatFactory {

private:
    static MaterialCoreSP red;
    static MaterialCoreSP white;
    static MaterialCoreSP black;
    static MaterialCoreSP tag;
    static MaterialCoreSP stadt;

public:
    static const MaterialCoreSP &getRed();

    static const MaterialCoreSP &getWhite();

    static const MaterialCoreSP &getBlack();

    static const MaterialCoreSP &getTag();
    static const MaterialCoreSP &getStadt();


private:


     static MaterialCoreSP createMat(const glm::vec4 &ambient, const glm::vec4 &specular, const glm::vec4 &diffuse, const float &shine);
     static MaterialCoreSP createMat(const glm::vec4 &ambient, const glm::vec4 &specular, const glm::vec4 &diffuse, const float &shine,const glm::vec4& emission);
};


#endif //CG3_U3_MATFACTORY_H
