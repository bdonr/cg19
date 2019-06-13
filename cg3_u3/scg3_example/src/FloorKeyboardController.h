//
// Created by don on 11.06.19.
//

#ifndef CG3_U3_FLOORKEYBOARDCONTROLLER_H
#define CG3_U3_FLOORKEYBOARDCONTROLLER_H


#include "scg_glew.h"
#include "CameraController.h"
#include "scg_internals.h"
#include "StandardRenderer.h"
#include "Group.h"
namespace scg {

/**
 * \brief A camera controller using keyboard input.
 *
 * See implementation of constructor KeyboardController::KeyboardController(),
 * which prints a list of control actions.
 */
    class FloorKeyboardController: public CameraController {
    private:

        StandardRendererSP renderer;
        GroupSP videoScene;
        GroupSP gameScene;
        CameraSP currentCam;
    public:
        NodeSP bullet;
        bool movement=true;
        /**
         * Constructor with given camera transformation.
         */
        FloorKeyboardController(CameraSP camera);

        /**
         * Destructor.
         */
        virtual ~FloorKeyboardController();

        /**
         * Create shared pointer with given camera transformation.
         */
        static FloorKeyboardControllerSP create(CameraSP& camera);

        /**
         * Check input devices, called by Viewer::startMainLoop().
         *
         * \param viewState view state managed by Viewer, may be modified by controller
         */
        virtual void checkInput(ViewState* viewState);

        //wird  nicht benutzt möglicherweise kann es weg????
        virtual void setBullet(NodeSP N);

        /**
      *
      * set the renderer in controller
      *
      */
        void setRenderer(StandardRendererSP &renderer);
        /**
      *
      * set the flightShowScene in controller
      *
      */
        void setVideoScene(GroupSP videoScene);
        /**
        *
        * set the flightShowScene in controller
        *
        */
        void setGameScene(GroupSP gameScene);

        /**
       *
       * gif de Kamera toröch vun düsse controller
       *
        */
        CameraSP setCam(CameraSP camera) ;
    };

}

#endif //CG3_U3_FLOORKEYBOARDCONTROLLER_H
