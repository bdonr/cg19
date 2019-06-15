//
// Created by m7f-nt8 on 15.06.19.
//

#include "TransfomationExt.h"




    TransformationExtSP TransformationExt::create() {
        return std::make_shared<TransformationExt>();
    }

    Transformation* TransformationExt::translate(glm::vec3 translation) {
    matrix_ = glm::translate(matrix_, translation);
    position_ += translation;
        return this;
    }






