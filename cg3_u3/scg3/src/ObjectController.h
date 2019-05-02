//
// Created by don on 5/1/19.
//

#ifndef CG3_U3_OBJECTCONTROLLER_H

#define CG3_U3_OBJECTCONTROLLER_H

#include "KeyboardController.h"
#include "Transformation.h"
using namespace scg;
namespace scg {
    class ObjectController : public KeyboardController {

    private: CameraSP camera_;
        TransformationSP obj;
    public:
        ObjectController(CameraSP camera,TransformationSP obj);

        ~ObjectController() override;


        static KeyboardControllerSP create(CameraSP camera,Transformation obj);

        virtual void checkInput(ViewState* viewState);
    };
}

#endif //CG3_U3_OBJECTCONTROLLER_H
