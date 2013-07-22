// /////////////////////////////////////////////////////////////////
// @file Timer.cpp
// @author PJ O Halloran
// @date 16/08/2010
//
// File contains the implementation of the Timer class.
//
// /////////////////////////////////////////////////////////////////

#include "Timer.h"

namespace GameHalloran {
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Timer::Start()
    {
        // Get the current clock time and start the timer.
        m_started = true;
        m_paused = false;
        m_startTicks = GetTicks();
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Timer::Stop()
    {
        // Stop and unpause the timer
        m_started = false;
        m_paused = false;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Timer::Pause()
    {
        if((m_started) && (!m_paused)) {
            //Pause the timer and calculate the paused ticks
            m_paused = true;
            m_pausedTicks = GetTicks() - m_startTicks;
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Timer::UnPause()
    {
        if(m_paused) {
            // Unpause the timer, reset the paused ticks and recalculate the start ticks
            m_paused = false;
            m_startTicks = GetTicks() - m_pausedTicks;
            m_pausedTicks = 0;
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    F64 Timer::GetTime()
    {
        if(m_started) {
            if(m_paused) {
                return (m_pausedTicks);
            } else {
                // Return the current time minus the start time
                F64 currTime = GetTicks();
                return (currTime - m_startTicks);
            }
        }

        // The timer is not currently running
        return (0.0);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool Timer::IsStarted() const
    {
        return (m_started);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool Timer::IsPaused() const
    {
        return (m_paused);
    }
}
