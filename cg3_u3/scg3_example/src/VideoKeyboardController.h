//
// Created by don on 09.06.19.
//

#ifndef CG3_U3_VIDEOKEYBOARDCONTROLLER_H
#define CG3_U3_VIDEOKEYBOARDCONTROLLER_H

#include "scg3.h"
#include "scg_internals.h"
namespace scg {

    class VideoKeyboardController : public CameraController {
    private:
        bool enable;
        bool chooseScene;
    public:
        bool isChooseScene() const;

        void setChooseScene(bool chooseScene);

    public:
        static VideoKeyboardControllerSP create(CameraSP &camera);

        virtual void checkInput(ViewState *viewState);


        VideoKeyboardController(const CameraSP &camera);

        virtual ~VideoKeyboardController();

        bool isEnable() const;

        void setEnable(bool enable);


    };

}
#endif //CG3_U3_VIDEOKEYBOARDCONTROLLER_H
