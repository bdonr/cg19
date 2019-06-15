//
// Created by m7f-nt8 on 15.06.19.
//

#ifndef CG3_U3_POSITIONHELPER_H
#define CG3_U3_POSITIONHELPER_H

#include "Transformation.h"

using namespace scg;

class TransformationExt;
typedef std::shared_ptr<TransformationExt> TransformationExtSP;

class TransformationExt : public Transformation{

    private:

    glm::vec3 position_ = glm::vec3(0, 0, 0);

    public:

    static TransformationExtSP create();

    Transformation* translate(glm::vec3 posObject);

    virtual glm::vec3 getPosition();




};

#endif //CG3_U3_POSITIONHELPER_H
