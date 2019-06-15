//
// Created by Arpad on 15.06.19.
//

//Diese cpp erweitert die Tranformation.cpp
#include "TransfomationExt.h"


    TransformationExtSP TransformationExt::create() {
        return std::make_shared<TransformationExt>();
    }

    Transformation* TransformationExt::translate(glm::vec3 translation) {
    matrix_ = glm::translate(matrix_, translation);
    position_ += translation;
        return this;
    }

   glm::vec3 TransformationExt::getPosition() {
      return position_;
   }







