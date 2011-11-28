//========================================================================
// CProcess.cpp : Defines a simple cooperative multitasker
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
// @file CProcess.cpp
// @author Mike McShaffry (edited by PJ O Halloran)
// @date 08/07/2010
//
// File contains the implementation of the CProcess and 
// CProcessManager classes.  The few edits I have made to Mike 
// McShaffrys' code are detailed in the CProcess.h header file 
// comments.
//
// /////////////////////////////////////////////////////////////////

// External headers

// Project headers
#include "CProcess.h"

// Namespace Declarations
using boost::shared_ptr;


namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// ************************** CProcess *****************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	CProcess::CProcess(I32 nType, U32 uOrder) : m_uProcessFlags(0), m_iType(nType), m_bKill(false),\
		m_bActive(true), m_bPaused(false), m_bInitialUpdate(true), m_pNext()
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	CProcess::~CProcess()
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CProcess::IsDead(void) const
	{
		return (m_bKill);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CProcess::VKill()
	{
		m_bKill = true;
	}
	
	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	I32 CProcess::GetType(void) const
	{
		return (m_iType);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CProcess::SetType(const I32 type)
	{
		m_iType = type;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CProcess::IsActive(void) const
	{
		return (m_bActive);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CProcess::SetActive(const bool active)
	{
		m_bActive = active;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CProcess::IsAttached() const
	{
		return (m_uProcessFlags & PROCESS_FLAG_ATTACHED) ? true : false;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CProcess::SetAttached(const bool wantAttached)
	{
		if(wantAttached)
		{
			m_uProcessFlags |= CProcess::PROCESS_FLAG_ATTACHED;
		}
		else
		{
			m_uProcessFlags &= ~CProcess::PROCESS_FLAG_ATTACHED;
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CProcess::IsPaused(void) const
	{
		return (m_bPaused);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CProcess::VTogglePause()
	{
		m_bPaused = !m_bPaused;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CProcess::IsInitialized() const
	{
		return (!m_bInitialUpdate);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	shared_ptr<CProcess> const CProcess::GetNext(void) const
	{
		return (m_pNext);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CProcess::SetNext(shared_ptr<CProcess> nextProcessPtr)
	{
		m_pNext = nextProcessPtr;
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CProcess::VOnUpdate(const F32 elapsedTime)
	{
		if (m_bInitialUpdate)
		{
			VOnInitialize();
			m_bInitialUpdate = false;
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CProcess::VOnInitialize()
	{
	}

	// /////////////////////////////////////////////////////////////////
	// ********************** CProcessManager *************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CProcessManager::Detach(shared_ptr<CProcess> pProcess)
	{
		if(pProcess)
		{
			m_processList.remove(pProcess);
			pProcess->SetAttached(false);
		}
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	CProcessManager::CProcessManager() : m_processList()
	{
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	CProcessManager::~CProcessManager()
	{
		try
		{
			ClearAll();
		}
		catch(...)
		{
		}
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CProcessManager::UpdateProcesses(const F32 elapsedTime)
	{
		ProcessList::iterator i = m_processList.begin();		// Current position in list.
		ProcessList::iterator end = m_processList.end();		// End of list.
		shared_ptr<CProcess> pNext;								// The current processes NEXT process 
																//  to run after it has completed.

		// Iterate through all the processes in the list.
		while (i != end)
		{
			shared_ptr<CProcess> p(*i);
			++i;

			if(p->IsDead())
			{
				// Check for a child process and add if exists
				pNext = p->GetNext();
				if(pNext)
				{
					p->SetNext(shared_ptr<CProcess>((CProcess *)NULL));
					Attach(pNext);
				}
				Detach(p);
			}
			else if(p->IsActive() && !p->IsPaused())
			{
				p->VOnUpdate(elapsedTime);
			}
		}
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CProcessManager::IsProcessActive(const I32 nType)
	{
		bool foundActiveProcess = false;				// Have we found the process yet?

		// Iterate through the list until the list is exhausted or we have found what we are searching for.
		for(ProcessList::iterator i = m_processList.begin(); ((!foundActiveProcess) && (i != m_processList.end())); ++i)
		{
			// Check for living processes.  If they are dead, make sure no children 
			// are attached as they will be brought to life on next cycle.
			if ((*i)->GetType() == nType && 
				 (((*i)->IsDead() == false) || (*i)->GetNext()))
			{
				foundActiveProcess = true;
			}
		}

		return (foundActiveProcess);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CProcessManager::Attach(shared_ptr<CProcess> pProcess)
	{
		if(pProcess)
		{
			pProcess->SetAttached(true);
			m_processList.push_back(pProcess);
		}
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CProcessManager::HasProcesses()
	{
		return (!m_processList.empty());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CProcessManager::ClearAll(const bool killAll)
	{
		for(ProcessList::iterator i = m_processList.begin(); i != m_processList.end(); ++i)
		{
			Detach(*i);
			if(killAll)
			{
				(*i)->VKill();
			}
		}
	}

}
