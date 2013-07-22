// /////////////////////////////////////////////////////////////////
// @file Pool3dLogic.h
// @author PJ O Halloran.
// @date 01/11/2010
//
// Pool3D logic class.
//
// /////////////////////////////////////////////////////////////////

#ifndef __POOL_3D_LOGIC_H
#define __POOL_3D_LOGIC_H

// External Headers
#include <string>
#include <map>

#include <boost/shared_ptr.hpp>

#include <bullet/btBulletDynamicsCommon.h>

// Project Headers
#include "GameLogic.h"
#include "Actors.h"
#include "CProcess.h"
#include "GameOptions.h"
#include "GameLog.h"
#include "GameException.h"
#include "IGamePhysics.h"
#include "IGameView.h"
#include "CRandom.h"
#include "Matrix.h"
#include "EventManager.h"

#include "Pool3dEvents.h"
#include "Pool3dCommon.h"
#include "Pool3dActors.h"

namespace GameHalloran {

    class Pool3dLogic;

    // /////////////////////////////////////////////////////////////////
    // @class Pool3dLogicEventListener
    // @author PJ O Halloran
    //
    // This class holds the functionality for listening for and responding
    // to events for the Pool3D logic layer.
    //
    // /////////////////////////////////////////////////////////////////
    class Pool3dLogicEventListener : public IEventListener {
        // TODO: Add Pool3D specific event handlers here.....

    private:

        Pool3dLogic *m_logicPtr;                            ///< Pointer to the logic layer.

        // /////////////////////////////////////////////////////////////////
        // Create a new actor parameters object depending on the type of
        // actor requested in the lua script.
        //
        // @param srcData The lua actor data.
        //
        // @return NULL on error or a ActorParams pointer on success.
        //
        // /////////////////////////////////////////////////////////////////
        ActorParams *CreateNewActorParams(const LuaPlus::LuaObject &srcData);

    protected:

    public:

        // /////////////////////////////////////////////////////////////////
        // Custom constructor.
        //
        // @param logicPtr Pointer to the games logic layer.
        //
        // @throw GameException If the pointer to the game view is invalid.
        //
        // /////////////////////////////////////////////////////////////////
        explicit Pool3dLogicEventListener(Pool3dLogic *logicPtr) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~Pool3dLogicEventListener() { };

        // /////////////////////////////////////////////////////////////////
        // Returns ascii-text name for this listener, used mostly for
        // debugging
        //
        // /////////////////////////////////////////////////////////////////
        virtual char const *VGetName(void);

        // /////////////////////////////////////////////////////////////////
        // Return 'false' to indicate that this listener did NOT
        // consume the event, ( and it should continue to be
        // propogated )
        //
        // return 'true' to indicate that this listener consumed the
        // event, ( and it should NOT continue to be propgated )
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VHandleEvent(IEventData const &eventObj);

        // /////////////////////////////////////////////////////////////////
        /* ******* Various Callback handlers for the Logic layer ******** */
        // /////////////////////////////////////////////////////////////////

        // /////////////////////////////////////////////////////////////////
        // Callback triggered when a EvtData_Request_Pause_Game_Event event
        // is broadcast.
        //
        // @param eventData The event data object.
        //
        // @return bool True if the event is to be stopped now or false if the
        //                  event should be propagated further.
        //
        // /////////////////////////////////////////////////////////////////
        bool OnRequestPauseEvent(const EvtData_Request_Pause_Game_Event &eventData);

        // /////////////////////////////////////////////////////////////////
        // Callback triggered when a EvtData_Game_State event
        // is broadcast.
        //
        // @param eventData The event data object.
        //
        // @return bool True if the event is to be stopped now or false if the
        //                  event should be propagated further.
        //
        // /////////////////////////////////////////////////////////////////
        bool OnGameStateChangeEvent(const EvtData_Game_State &eventData);

        // /////////////////////////////////////////////////////////////////
        // Callback triggered when a EvtData_Move_Actor event
        // is broadcast.
        //
        // @param eventData The event data object.
        //
        // @return bool True if the event is to be stopped now or false if the
        //                  event should be propagated further.
        //
        // /////////////////////////////////////////////////////////////////
        bool OnMoveActorEvent(const EvtData_Move_Actor &eventData);

        // /////////////////////////////////////////////////////////////////
        // Callback triggered when a EvtData_New_Actor event
        // is broadcast.
        //
        // @param eventData The event data object.
        //
        // @return bool True if the event is to be stopped now or false if the
        //                  event should be propagated further.
        //
        // /////////////////////////////////////////////////////////////////
        bool OnNewActorEvent(const EvtData_New_Actor &eventData);

        // /////////////////////////////////////////////////////////////////
        // Callback triggered when a EvtData_Request_New_Actor event
        // is broadcast.
        //
        // @param eventData The event data object.
        //
        // @return bool True if the event is to be stopped now or false if the
        //                  event should be propagated further.
        //
        // /////////////////////////////////////////////////////////////////
        bool OnRequestNewActorEvent(EvtData_Request_New_Actor &eventData);

        // /////////////////////////////////////////////////////////////////
        // Callback triggered when a EvtData_Request_Start_Game event
        // is broadcast.
        //
        // @param eventData The event data object.
        //
        // @return bool True if the event is to be stopped now or false if the
        //                  event should be propagated further.
        //
        // /////////////////////////////////////////////////////////////////
        bool OnRequestStartGameEvent(const EvtData_Request_Start_Game &eventData);

        // /////////////////////////////////////////////////////////////////
        // Callback triggered when a EvtData_UpdateActorParams event
        // is broadcast.
        //
        // @param eventData The event data object.
        //
        // @return bool True if the event is to be stopped now or false if the
        //                  event should be propagated further.
        //
        // /////////////////////////////////////////////////////////////////
        bool OnUpdateActorParametersEvent(const EvtData_UpdateActorParams &eventData);

        // /////////////////////////////////////////////////////////////////
        // Callback triggered when a EvtData_PhysCollision event
        // is broadcast.
        //
        // @param eventData The event data object.
        //
        // @return bool True if the event is to be stopped now or false if the
        //                  event should be propagated further.
        //
        // /////////////////////////////////////////////////////////////////
        bool OnActorCollisionEvent(const EvtData_PhysCollision &eventData);

        // /////////////////////////////////////////////////////////////////
        // Callback triggered when a EvtData_PhysTrigger_Leave event
        // is broadcast.
        //
        // @param eventData The event data object.
        //
        // @return bool True if the event is to be stopped now or false if the
        //                  event should be propagated further.
        //
        // /////////////////////////////////////////////////////////////////
        bool OnTriggerAreaLeaveEvent(const EvtData_PhysTrigger_Leave &eventData);

        // /////////////////////////////////////////////////////////////////
        // Callback triggered when a EvtData_End_Game event
        // is broadcast.
        //
        // @param eventData The event data object.
        //
        // @return bool True if the event is to be stopped now or false if the
        //                  event should be propagated further.
        //
        // /////////////////////////////////////////////////////////////////
        bool OnEndGameEvent(const EvtData_End_Game &eventData);

        // /////////////////////////////////////////////////////////////////
        // Callback triggered when a EvtData_Dialog_Open_Event event
        // is broadcast.
        //
        // @param eventData The event data object.
        //
        // @return bool True if the event is to be stopped now or false if the
        //                  event should be propagated further.
        //
        // /////////////////////////////////////////////////////////////////
        bool OnOpenDialogEvent(const EvtData_Dialog_Open_Event &eventData);

        // /////////////////////////////////////////////////////////////////
        // Callback triggered when a EvtData_Dialog_Close_Event event
        // is broadcast.
        //
        // @param eventData The event data object.
        //
        // @return bool True if the event is to be stopped now or false if the
        //                  event should be propagated further.
        //
        // /////////////////////////////////////////////////////////////////
        bool OnCloseDialogEvent(const EvtData_Dialog_Close_Event &eventData);

        // /////////////////////////////////////////////////////////////////
        // Callback triggered when a EvtData_Complex_Mesh_Loaded event
        // is broadcast.
        //
        // @param eventData The event data object.
        //
        // @return bool True if the event is to be stopped now or false if the
        //                  event should be propagated further.
        //
        // /////////////////////////////////////////////////////////////////
        bool OnComplexMeshLoadedEvent(const EvtData_Complex_Mesh_Loaded &eventData);

        // /////////////////////////////////////////////////////////////////
        // Callback triggered when a EvtData_Camera_Aim_Mode_Request event
        // is broadcast.
        //
        // @param eventData The event data object.
        //
        // @return bool True if the event is to be stopped now or false if the
        //                  event should be propagated further.
        //
        // /////////////////////////////////////////////////////////////////
        bool OnCameraAimModeRequest(const EvtData_Camera_Aim_Mode_Request &eventData);

        // /////////////////////////////////////////////////////////////////
        // Callback triggered when a EvtData_Shot_Start event
        // is broadcast.
        //
        // @param eventData The event data object.
        //
        // @return bool True if the event is to be stopped now or false if the
        //                  event should be propagated further.
        //
        // /////////////////////////////////////////////////////////////////
        bool OnPoolShotStart(const EvtData_Shot_Start &eventData);

        // /////////////////////////////////////////////////////////////////
        // Callback triggered when a EvtData_ResetCueBall event
        // is broadcast.
        //
        // @param eventData The event data object.
        //
        // @return bool True if the event is to be stopped now or false if the
        //                  event should be propagated further.
        //
        // /////////////////////////////////////////////////////////////////
        bool OnResetCueBall(const EvtData_ResetCueBall &eventData);

        // /////////////////////////////////////////////////////////////////
        // Callback triggered when a EvtData_Move_Kinematic_Actor event
        // is broadcast.
        //
        // @param eventData The event data object.
        //
        // @return bool True if the event is to be stopped now or false if the
        //                  event should be propagated further.
        //
        // /////////////////////////////////////////////////////////////////
        bool OnPlayerMoveActor(const EvtData_Move_Kinematic_Actor &eventData);

        // /////////////////////////////////////////////////////////////////
        // Callback triggered when a EvtData_Physics_Diagnostics event
        // is broadcast.
        //
        // @param eventData The event data object.
        //
        // @return bool True if the event is to be stopped now or false if the
        //                  event should be propagated further.
        //
        // /////////////////////////////////////////////////////////////////
        bool OnPhysicsDiagnosticUpdate(const EvtData_Physics_Diagnostics &eventData);

    };

    // /////////////////////////////////////////////////////////////////
    // @class Pool3dLogic
    // @author PJ O Halloran.
    //
    // This is a class is the Pool3D games logic layer class.
    //
    // /////////////////////////////////////////////////////////////////
    class Pool3dLogic : public BaseLuaGameLogic {
        friend class Pool3dLogicEventListener;

    private:

        // /////////////////////////////////////////////////////////////////
        // @class Pool3dGameState
        // @author PJ O Halloran.
        //
        // Holds a record of the current state of a game of Pool/Billards.
        // Used only by the logic layer to keep track of game status and
        // player score.
        //
        // /////////////////////////////////////////////////////////////////
        class Pool3dGameState {
        private:

            // /////////////////////////////////////////////////////////////////
            // @struct Player.
            // @author PJ O Halloran.
            //
            // Holds information on the current status of a player.
            //
            // /////////////////////////////////////////////////////////////////
            struct Player {
                I32 m_id;                           ///< ID of player.
                I32 m_ballGroup;                    ///< The ball group the player is assigned.
                U32 m_score;                ///< The players score or how many balls they have potted.
                I32 m_turnShots;                    ///< The number of shots the player has for this turn.

                // /////////////////////////////////////////////////////////////////
                // Constructor.
                //
                // /////////////////////////////////////////////////////////////////
                Player() : m_id(0), m_ballGroup(BallActorParams::eNone), m_score(0), m_turnShots(0) {};

                // /////////////////////////////////////////////////////////////////
                // Constructor.
                //
                // /////////////////////////////////////////////////////////////////
                Player(const I32 id) : m_id(id), m_ballGroup(BallActorParams::eNone), m_score(0), m_turnShots(0) {};

                // /////////////////////////////////////////////////////////////////
                // Reset a player info.
                //
                // /////////////////////////////////////////////////////////////////
                void Reset() {
                    m_ballGroup = BallActorParams::eNone;
                    m_score = 0;
                    m_turnShots = 0;
                };
            };


            // /////////////////////////////////////////////////////////////////
            // @struct BallPottedState
            // @author PJ O Halloran
            //
            // Holds information on a potted ball.
            //
            // /////////////////////////////////////////////////////////////////
            struct BallPottedState {
                ActorId m_ballId;                           ///< ID of the ball actor.
                I32 m_ballGroup;                            ///< Group the ball belongs to.
                I32 m_playerId;                             ///< ID of the player who potted the ball.
                F64 m_potTimestamp;                     ///< Time the ball was potted.
                I32 m_pocketId;                             ///< ID of the pocket the ball fell into.

                BallPottedState() : m_ballId(0), m_ballGroup(0), m_playerId(0), m_potTimestamp(0.0), m_pocketId(0) {};

                BallPottedState(const ActorId aid, const I32 bgId, const I32 pid, const F64 ts, const I32 pockId) : m_ballId(aid), m_ballGroup(bgId), m_playerId(pid), m_potTimestamp(ts), m_pocketId(pockId) {};
            };

            // /////////////////////////////////////////////////////////////////
            // @struct ShotState
            // @author PJ O Halloran
            //
            // Information on the current state of a shot.
            //
            // /////////////////////////////////////////////////////////////////
            struct ShotState {
                U32 m_bbCollisionsCount;                    ///< Number of ball-ball collisions during shot.
                U32 m_btCollisionCount;                 ///< Number of ball-table collisions during shot.
                U32 m_cbpbCollisionCount;                   ///< Number of cue ball - player ball group collisions during shot.
                bool m_validFirstHit;                               ///< Did the cue ball hit a ball of the same group as the players group on the first ball-ball collision?
                std::vector<BallPottedState> m_pottedBallVec;       ///< Array of potted balls.
                bool m_cueBallPotted;                               ///< Has cue ball been potted?
                bool m_blackBallPotted;                             ///< Has black ball been potted?
                F64 m_startTime;                                    ///< Shots start time.
                F64 m_endTime;                                  ///< Shots end time.

                // /////////////////////////////////////////////////////////////////
                // Constructor.
                //
                // /////////////////////////////////////////////////////////////////
                ShotState() {
                    Reset();
                };

                // /////////////////////////////////////////////////////////////////
                // Add a potted ball to the list.
                //
                // /////////////////////////////////////////////////////////////////
                void AddPottedBall(const ActorId ballId, const I32 ballGroup, const I32 playerId, const I32 pocketId, const F32 ts) {
                    if(ballGroup == BallActorParams::eBlack) {
                        m_blackBallPotted = true;
                    } else if(ballGroup == BallActorParams::eCue) {
                        m_cueBallPotted = true;
                    }

                    BallPottedState newBall;
                    newBall.m_ballGroup = ballGroup;
                    newBall.m_ballId = ballId;
                    newBall.m_playerId = playerId;
                    newBall.m_pocketId = pocketId;
                    newBall.m_potTimestamp = ts;
                    m_pottedBallVec.push_back(newBall);
                };

                // /////////////////////////////////////////////////////////////////
                // Get a count of how many balls were potted to date belonging to a
                // ball group.
                //
                // /////////////////////////////////////////////////////////////////
                U32 GetPottedBallsCount(const I32 ballGroup) const {
                    if(m_pottedBallVec.empty() || ballGroup >= BallActorParams::eNumberBallGroupTypes) {
                        return (0);
                    }

                    U32 count = 0;
                    for(std::vector<BallPottedState>::const_iterator i = m_pottedBallVec.begin(), end = m_pottedBallVec.end(); i != end; ++i) {
                        if((*i).m_ballGroup == ballGroup) {
                            ++count;
                        }
                    }

                    return (count);
                };

                // /////////////////////////////////////////////////////////////////
                // Reset shot state.
                //
                // /////////////////////////////////////////////////////////////////
                void Reset() {
                    m_bbCollisionsCount = 0;
                    m_btCollisionCount = 0;
                    m_cbpbCollisionCount = 0;
                    m_validFirstHit = false;
                    m_pottedBallVec.clear();
                    m_cueBallPotted = false;
                    m_blackBallPotted = false;
                    m_startTime = 0.0;
                    m_endTime = -1.0;
                };

                // /////////////////////////////////////////////////////////////////
                // Check if the shot has been recorded as ended yet.
                //
                // /////////////////////////////////////////////////////////////////
                bool IsComplete() const {
                    return (m_startTime < m_endTime);
                };

                // /////////////////////////////////////////////////////////////////
                // Check if there is a shot currently in progress.
                //
                // /////////////////////////////////////////////////////////////////
                bool IsInProgress() const {
                    return (m_startTime > 0.0 && m_endTime < 0.0);
                };

            };

        public:

            // /////////////////////////////////////////////////////////////////
            // @enum PlayerId
            //
            // Safe array access for the Pool3dGameState m_playersArr array
            // class member.
            //
            // ePlayerOne:          Index to access information on Player One.
            // ePlayerTwo:          Index to access information on Player Two.
            // eNumberOfPlayers:    Total number of players.
            //
            // /////////////////////////////////////////////////////////////////
            enum PlayerId {
                ePlayerOne = 0,
                ePlayerTwo,
                eNumberOfPlayers
            };

            ShotState m_shotState;                                      ///< State of the current shot if any.
            Player m_playersArr[2];                                     ///< Score and turn information for the pool players.
            I32 m_currentPlayerIndex;                                   ///< ID of the player whose turn it is.
            U32 m_shotCount;                                    ///< Total number of shots taken to date.
            F64 m_gameStartTime;                                        ///< Time the game was started.

            // /////////////////////////////////////////////////////////////////
            // Constructor.
            //
            // /////////////////////////////////////////////////////////////////
            Pool3dGameState() : m_currentPlayerIndex(ePlayerOne), m_shotCount(0), m_gameStartTime(0.0), m_shotState() {
                m_playersArr[ePlayerOne] = Player(ePlayerOne);
                m_playersArr[ePlayerTwo] = Player(ePlayerTwo);
            };

            // /////////////////////////////////////////////////////////////////
            // Destructor.
            //
            // /////////////////////////////////////////////////////////////////
            ~Pool3dGameState() {};

            // /////////////////////////////////////////////////////////////////
            // New players turn.
            //
            // /////////////////////////////////////////////////////////////////
            void NextPlayer() {
                if(m_currentPlayerIndex < eNumberOfPlayers - 1) {
                    ++m_currentPlayerIndex;
                } else {
                    m_currentPlayerIndex = 0;
                }
            };

#ifdef DEBUG
            // /////////////////////////////////////////////////////////////////
            // Log the current state of the game.
            //
            // /////////////////////////////////////////////////////////////////
            void LogState();
#endif

        };

        boost::shared_ptr<IEventListener> m_listener;                               ///< Logic layers Event listener.
        ViewMode m_viewMode;                                                        ///< Current view mode state.
        Pool3dGameState m_gameStateObj;                                             ///< Current game state.
        ActorId m_cueBallActorId;                                                   ///< Actor ID of the cue ball actor as it is needed often for various game logic events.
        ActorId m_tableActorId;                                                     ///< ID of the table actor.

        // /////////////////////////////////////////////////////////////////
        // Called once a shot has started once per logic update to check if
        // the ball actors have stopped moving yet.
        //
        // /////////////////////////////////////////////////////////////////
        bool HasShotEnded();

        // /////////////////////////////////////////////////////////////////
        // Get the position of the nearest ball to the cue ball.
        //
        // @param groupId The groupId of the ball type we are looking for.
        // @param cueBallPos The current position of the cue ball.
        // @param posRef The object to store the nearest ball position inside.
        //                  (Will be uninitialized if no ball was found).
        //
        // /////////////////////////////////////////////////////////////////
        void GetNearestPlayerBallPosition(const BallActorParams::BallGroup groupId, const Point3 &cueBallPos, boost::optional<Point3> &posRef);

        // /////////////////////////////////////////////////////////////////
        // OnUpdate() helper function.
        // Updates the Pool game logic state while a pool game is in progress.
        //
        // /////////////////////////////////////////////////////////////////
        void UpdatePoolGameLogic();

    public:

        typedef std::map<std::string, btCollisionShape*> CollisionShapeMap;
        CollisionShapeMap m_collisionShapeLookupTable;                                  ///< List of physics collision shapes in a lookup table.

        // Keys used to lookup pool game collision shapes...
        static const std::string PGL_POOL_BALL_COLLISION_SHAPE;
        static const std::string PGL_POOL_CUE_COLLISION_SHAPE;
        static const std::string PGL_POOL_TABLE_FLOOR_MIDDLE_COLLISION_SHAPE;
        static const std::string PGL_POOL_TABLE_SIDE_COLLISION_SHAPE;
        static const std::string PGL_POOL_TABLE_FRONT_COLLISION_SHAPE;
        static const std::string PGL_POOL_TABLE_FRONT_BOUNDARY_COLLISION_SHAPE;
        static const std::string PGL_POOL_TABLE_SIDE_BOUNDARY_COLLISION_SHAPE;

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param optionObj The global game options pointer.
        // @param loggerPtr The global logging object.
        //
        // @throw GameException& If the options pointer is NULL.
        // @throw GameException& If we fail to allocate memory for the internal
        //                          process manager.
        //
        // /////////////////////////////////////////////////////////////////
        explicit Pool3dLogic(boost::shared_ptr<GameOptions> optionsPtr, boost::shared_ptr<GameLog> loggerPtr, boost::shared_ptr<ModelViewProjStackManager> stackManagerPtr) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~Pool3dLogic();

        // /////////////////////////////////////////////////////////////////
        // Build the initial scene.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VBuildInitialScene();

        // /////////////////////////////////////////////////////////////////
        // Load a game state.
        //
        // @param gameNameRef The name of the game.
        //
        // @return bool True on success or false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VLoadGame(const std::string &gameNameRef);

        // /////////////////////////////////////////////////////////////////
        // Save the game state.
        //
        // @return bool True on success or false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VSaveGame();

        // /////////////////////////////////////////////////////////////////
        // Called once per main loop to update the logic state of the game.
        //
        // @param time The total application time so far.
        // @param elapsedTime The time elapsed since the last update.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VOnUpdate(const F64 time, const F32 elapsedTime);

        // /////////////////////////////////////////////////////////////////
        // Changing Game Logic State.
        //
        // @param newState The new game state.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VChangeState(const enum BaseGameState newState);

        // /////////////////////////////////////////////////////////////////
        // Reset the game logic, clearing it of all game actors (including
        // physics objects) and current state including any running logic
        // processes.  Should be called to end the current game or on
        // application exit.
        //
        // /////////////////////////////////////////////////////////////////
        void ResetGameLogic();

        // /////////////////////////////////////////////////////////////////
        // Get the current view mode.
        //
        // /////////////////////////////////////////////////////////////////
        ViewMode GetViewMode() const {
            return (m_viewMode);
        };

        // /////////////////////////////////////////////////////////////////
        // Handle an aim mode request from a script or other game layer.
        //
        // /////////////////////////////////////////////////////////////////
        void AimModeRequest(const enum ViewMode viewMode);

    };

}

#endif
