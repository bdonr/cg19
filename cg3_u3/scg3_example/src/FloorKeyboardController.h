//
// Created by don on 11.06.19.
//

#ifndef CG3_U3_FLOORKEYBOARDCONTROLLER_H
#define CG3_U3_FLOORKEYBOARDCONTROLLER_H


#include "scg_glew.h"
#include "scg_internals.h"
#include "StandardRenderer.h"
#include "Group.h"
#include "scg3.h"
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
        GroupSP flightShowScene;
        GroupSP gameScene;
        CameraSP gameCam;
        CameraSP flightShowCam;
        bool movement;
        bool pitch;
        ViewerSP viewer;
    public:
        NodeSP bullet;


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
        static FloorKeyboardControllerSP create(CameraSP& camera );

        /**
         * Check input devices, called by Viewer::startMainLoop().
         *
         * \param viewState view state managed by Viewer, may be modified by controller
         */
        virtual void checkInput(ViewState* viewState);


        virtual void setBullet(NodeSP N);

        /**
      *
      * set the renderer in controller
      *
      */
        void setRenderer(StandardRendererSP &renderer);
        /**
      *
      * set the flightshow Scene in controller
      *
      */
        void setVideoScene(GroupSP videoScene);
        /**
        *
        * set the Game scene  in controller
        *
        */
        void setGameScene(GroupSP gameScene);

        /**
       *
       * set the camera of the Game scene
       *
        */
        void setGameCam(CameraSP gameCam) ;

        /**
       *
       * set the camera of the fligthshow scene
       *
       */
        void setFlightShowCam(CameraSP flightShowCam) ;
       /**
        *
        * set the viewer in the controller
        *
        */
        void setViewer(ViewerSP viewer);

    };


}

#endif //CG3_U3_FLOORKEYBOARDCONTROLLER_H
