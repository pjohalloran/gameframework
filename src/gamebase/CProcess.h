#pragma once
#ifndef __C_PROCESS_H
#define __C_PROCESS_H

//========================================================================
// CProcess.h : Defines a simple cooperative multitasker
//
// Part of the GameCode3 Application
//
// GameCode3 is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete - 3rd Edition" by Mike McShaffry, published by
// Charles River Media. ISBN-10: 1-58450-680-6   ISBN-13: 978-1-58450-680-5
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the author a favor and buy a new copy of the book - there are
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here:
//    http://www.amazon.com/gp/product/1584506806?ie=UTF8&tag=gamecodecompl-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=1584506806
//
// There's a companion web site at http://www.mcshaffry.com/GameCode/
//
// The source code is managed and maintained through Google Code:
// http://gamecode3.googlecode.com/svn/trunk/
//
// (c) Copyright 2009 Michael L. McShaffry
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License v2
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//========================================================================

// /////////////////////////////////////////////////////////////////
// @file CProcess.h
// @author Mike McShaffry (edited by PJ O Halloran)
// @date 08/07/2010
//
// File contains the header file for the CProcess and CProcessManager
// classes and all their helper functions and other utilities.
//
// Originally written by Mike McShaffry author of Game Coding Complete
// and games programmer.
//
// I have cleaned them up a little with better comments etc. to fit
// in with the rest of my code.
//
// I have added/modified the following functionality:
// - I moved all protected members to be private to make for
//      stronger encapsulation.  Some have public Getters methods
//      anyway.
// - I put the PROCESS_FLAG_ATTACHED as a private member of CProcess.
// - I put the ProcessList tpedef inside in CProcessManager in the
//      private section so it is known only to that class.
//
// /////////////////////////////////////////////////////////////////

#include <list>

#include "GameBase.h"

namespace GameHalloran {

    enum PROCESS_TYPE {
        PROC_NONE,
        PROC_WAIT,
        PROC_CONTROL,
        PROC_SCREEN,
        PROC_MUSIC,
        PROC_SOUNDFX,
        PROC_ACTOR,
        PROC_INTERPOLATOR,
        NUMBER_PROCESS_TYPES
    };

    // /////////////////////////////////////////////////////////////////
    // @class CProcess
    //
    // Represents a time shared process for a task that is updated once
    // per frame.
    //
    // /////////////////////////////////////////////////////////////////
    class CProcess : public NonCopyable {
    private:

        static const I32 PROCESS_FLAG_ATTACHED = 0x00000001;    ///< Flag to indicate the process is currently attached or not to the manager.

        U32 m_uProcessFlags;                            ///< The current value of the process flags.
        I32 m_iType;                                            ///< Type of process running
        bool m_bKill;                                           ///< Tells manager to kill and remove
        bool m_bActive;                                         ///< Is the process currently active.
        bool m_bPaused;                                         ///< Is the process currently paused.
        bool m_bInitialUpdate;                                  ///< Initial update?
        boost::shared_ptr<CProcess> m_pNext;                    ///< A pointer to the process that will run directly
        ///<  after this process.

    protected:

        // /////////////////////////////////////////////////////////////////
        // Set the initial update
        //
        // /////////////////////////////////////////////////////////////////
        void SetInitialUpdate(const bool initialUpdate) {
            m_bInitialUpdate = initialUpdate;
        };
        bool IsInitialUpdate() const {
            return (m_bInitialUpdate);
        };

    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param nType The type of the process.
        // @param uOrder ??
        //
        // /////////////////////////////////////////////////////////////////
        CProcess(I32 nType, U32 uOrder = 0);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~CProcess();

        // /////////////////////////////////////////////////////////////////
        // Tells the manager if it should kill and remove the process.
        //
        // /////////////////////////////////////////////////////////////////
        bool IsDead(void) const;

        // /////////////////////////////////////////////////////////////////
        // Kills a process.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VKill();

        // /////////////////////////////////////////////////////////////////
        // Get the type of process running.
        //
        // @return I32 The type of process running.
        //
        // /////////////////////////////////////////////////////////////////
        I32 GetType(void) const;

        // /////////////////////////////////////////////////////////////////
        // Set the type of the process.
        //
        // @param type The new type of the process.
        //
        // /////////////////////////////////////////////////////////////////
        void SetType(const I32 type);

        // /////////////////////////////////////////////////////////////////
        // Check if the process is currently active.
        //
        // /////////////////////////////////////////////////////////////////
        bool IsActive(void) const;

        // /////////////////////////////////////////////////////////////////
        // Set the active status of the process.
        //
        // @param active True if the process is to be marked as currently
        //                  active and false if not.
        //
        // /////////////////////////////////////////////////////////////////
        void SetActive(const bool active);

        // /////////////////////////////////////////////////////////////////
        // Is the process attached to the process manager currently.
        //
        // /////////////////////////////////////////////////////////////////
        bool IsAttached() const;

        // /////////////////////////////////////////////////////////////////
        // Sets if the process should currently be attached to the process
        // manager.
        //
        // @param wantAttached True if the process should be marked as
        //                      attached and false otherwise.
        //
        // /////////////////////////////////////////////////////////////////
        void SetAttached(const bool wantAttached);

        // /////////////////////////////////////////////////////////////////
        // Check if the process is currently paused.
        //
        // /////////////////////////////////////////////////////////////////
        bool IsPaused(void) const;

        // /////////////////////////////////////////////////////////////////
        // Pauses a process.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VTogglePause();

        // /////////////////////////////////////////////////////////////////
        // Has the process been initialized.
        //
        // /////////////////////////////////////////////////////////////////
        bool IsInitialized() const;

        // /////////////////////////////////////////////////////////////////
        // Get a shared pointer to the process to be run directly after this
        // process.
        //
        // @return boost::shared_ptr<CProcess> Pointer to the next process
        //                                      to run.
        //
        // /////////////////////////////////////////////////////////////////
        boost::shared_ptr<CProcess> const GetNext(void) const;

        // /////////////////////////////////////////////////////////////////
        // Sets the next process to run after this process has completed.
        //
        // @param nextProcessPtr The next process to run.
        //
        // /////////////////////////////////////////////////////////////////
        void SetNext(boost::shared_ptr<CProcess> nextProcessPtr);

        // /////////////////////////////////////////////////////////////////
        // The main task of the process.  This should be overridden in
        // derived classes.
        //
        // @param elapsedTime The number of seconds since the last update.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VOnUpdate(const F32 elapsedTime);

        // /////////////////////////////////////////////////////////////////
        // Performs initialization for the process.  The first action of a
        // process.  This should be overridden in derived classes.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VOnInitialize();
    };

    // /////////////////////////////////////////////////////////////////
    // @class CProcessManager
    //
    // CProcessManager is a container for CProcess objects.
    //
    // /////////////////////////////////////////////////////////////////
    class CProcessManager {
    private:

        typedef std::list<boost::shared_ptr<CProcess> > ProcessList;
        ProcessList m_processList;                          ///< A list of CProcess objects.

        // /////////////////////////////////////////////////////////////////
        // Removes a process from the list.
        //
        // @param pProcess A pointer to the CProcess object to remove from
        //                  the list.
        //
        // /////////////////////////////////////////////////////////////////
        void Detach(boost::shared_ptr<CProcess> pProcess);

    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // /////////////////////////////////////////////////////////////////
        CProcessManager();

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        ~CProcessManager();

        // /////////////////////////////////////////////////////////////////
        // Update all process.  This is intended to be called once per game
        // loop.
        //
        // @param elapsedTime The number of seconds since the last update.
        //
        // /////////////////////////////////////////////////////////////////
        void UpdateProcesses(const F32 elapsedTime);

        // /////////////////////////////////////////////////////////////////
        // Check if a process identified by the type supplied is currently
        // running or active.
        //
        // /////////////////////////////////////////////////////////////////
        bool IsProcessActive(const I32 nType);

        // /////////////////////////////////////////////////////////////////
        // Attach a process to the list.
        //
        // /////////////////////////////////////////////////////////////////
        void Attach(boost::shared_ptr<CProcess> pProcess);

        // /////////////////////////////////////////////////////////////////
        // Does the manager have any processes?
        //
        // /////////////////////////////////////////////////////////////////
        bool HasProcesses();

        // /////////////////////////////////////////////////////////////////
        // Clear all currently running processes.
        //
        // @param killAll Should all processes be terminated as well as being
        //                  cleared.
        //
        // /////////////////////////////////////////////////////////////////
        void ClearAll(const bool killAll = true);

    };

}

#endif
