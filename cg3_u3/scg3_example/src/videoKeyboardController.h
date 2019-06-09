//
// Created by don on 09.06.19.
//

#ifndef CG3_U3_VIDEOKEYBOARDCONTROLLER_H
#define CG3_U3_VIDEOKEYBOARDCONTROLLER_H

#include "scg3.h"

using namespace scg;

class videoKeyboardController : public KeyboardController {
    static videoKeyboardControllerSP create(CameraSP& camera);
    void checkInput(ViewState* viewState);

public:
    videoKeyboardController(const CameraSP &camera);


};


#endif //CG3_U3_VIDEOKEYBOARDCONTROLLER_H
