//
// Created by don on 09.06.19.
//

#ifndef CG3_U3_VIDEOKEYBOARDCONTROLLER_H
#define CG3_U3_VIDEOKEYBOARDCONTROLLER_H

#include "scg3.h"
#include "scg_internals.h"
#include "Group.h"

namespace scg {

    class VideoKeyboardController : public FloorKeyboardController {
    private:
        bool enable;
        bool chooseScene;
        GroupSP gameScene;
        StandardRendererSP renderer;


    public:
        static VideoKeyboardControllerSP create(CameraSP &camera);

        virtual void checkInput(ViewState *viewState);


        VideoKeyboardController(const CameraSP &camera);

        virtual ~VideoKeyboardController();


        /**
       *
       * set the gameScene in controller
       *
         */
        void setGameScene(GroupSP gameScene);

        /**
      *
      * set the renderer in controller
      *
        */
        void setRenderer(StandardRendererSP &renderer);
    };

}
#endif //CG3_U3_VIDEOKEYBOARDCONTROLLER_H
