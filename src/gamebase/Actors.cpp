//========================================================================
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
// @file Actors.cpp
// @author Michael L. McShaffry (edited by PJ O Halloran)
// @date 09/07/2010
//
// File contains the implementation for the interfaces and classes for
// game actors.  An actor is defined as any object in the game which
// can change state.  e.g. Car in a racing game, candle in an action/
// adventure, etc.
// 
// Some important ground rules for Actors (from original code):
// "1. No destructors.
//  2. No pointers.
//  3. Never break rules one, two, or three.
// 
//  Why? These parameters are marshalled over god knows what. 
//  Could be a function call - or it could be the internet."
//
// This class was extracted from the Game Coding Complete 3 code.
// Originally written by Mike McShaffry, author of Game
// Coding Complete and games programmer.
// 
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.  Please see the associated header
// file for details on any changes i have made.
//
// /////////////////////////////////////////////////////////////////


// External Headers
#include <sstream>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include <LuaPlus/LuaObject.h>

#include <cstring>

// Project Headers
#include "Actors.h"

#include "GameMain.h"


using boost::shared_ptr;
using boost::optional;

using LuaPlus::LuaObject;

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// ********************* ActorParams *******************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	ActorParams::ActorParams() : m_Size(sizeof(ActorParams)), m_Id(0), m_Pos(), m_Type(AT_Unknown), m_Color()
	{
		strcpy(m_OnCreateLuaFunctionName, "");
		strcpy(m_OnDestroyLuaFunctionName, "");
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool ActorParams::VInit(std::istringstream &in)
	{ 
		I32 hasActorId = 0, x, y, z, r, g, b, a;

		in >> m_Size;
		in >> hasActorId;
		if (hasActorId)
		{
			in >> hasActorId;
			m_Id = hasActorId;
		}
		in >> x >> y >> z;
		m_Pos.SetX(F32(x));
		m_Pos.SetY(F32(y));
		m_Pos.SetZ(F32(z));
		in >> r >> g >> b >> a;
		m_Color.SetX(F32(r));
		m_Color.SetY(F32(b));
		m_Color.SetZ(F32(g));
		m_Color.SetW(F32(a));
		in >> m_OnCreateLuaFunctionName;
		in >> m_OnDestroyLuaFunctionName;
		return true;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool ActorParams::VInit(LuaPlus::LuaObject srcData, TErrorMessageList &errorMessages)
	{
		LuaPlus::LuaObject actorIDObj = srcData["ActorId"];
		if(actorIDObj.IsInteger())
		{
			m_Id = actorIDObj.GetInteger();
		}
	
		LuaPlus::LuaObject posObj = srcData["Pos"];
		if (posObj.IsTable())
		{
			const size_t tableCount = posObj.GetTableCount();
			if(3 != tableCount)
			{
				const std::string err("Incorrect number of parameters in the 'Pos' member.");
				errorMessages.push_back(err);
				return (false);
			}
			else
			{
				//Get the three values.
				m_Pos.SetX(posObj[1].GetFloat());
				m_Pos.SetY(posObj[2].GetFloat());
				m_Pos.SetZ(posObj[3].GetFloat());
			}
		}

		LuaPlus::LuaObject colorObj = srcData["Color"];
		if (colorObj.IsTable())
		{
			const size_t tableCount = colorObj.GetTableCount();
			if(4 != tableCount)
			{
				const std::string err("Incorrect number of parameters in the 'Color' member.");
				errorMessages.push_back(err);
				return (false);
			}
			else
			{
				//Get the RGBA off of it.
				LuaPlus::LuaObject r = colorObj["R"];
				if(r.IsNumber())
				{
					m_Color.SetX(r.GetFloat());
				}
				LuaPlus::LuaObject g = colorObj["G"];
				if(g.IsNumber())
				{
					m_Color.SetY(g.GetFloat());
				}
				LuaPlus::LuaObject b = colorObj["B"];
				if(b.IsNumber())
				{
					m_Color.SetZ(b.GetFloat());
				}
				LuaPlus::LuaObject a = colorObj["A"];
				if(a.IsNumber())
				{
					m_Color.SetW(a.GetFloat());
				}
			}
		}
	
		//See if we have any on create/destroy handlers.
		LuaPlus::LuaObject onCreateObj = srcData["OnCreateFunc"];
		if(onCreateObj.IsString())
		{
			const char *pString = onCreateObj.GetString();
			if(strlen(pString) > MAX_FUNC_NAME_SIZE)
			{
				const std::string err("The length of the 'OnCreateFunc' function name is too long.");
				errorMessages.push_back(err);
				return (false);
			}
			strcpy(m_OnCreateLuaFunctionName, onCreateObj.GetString());
		}
		LuaPlus::LuaObject onDestroyObj = srcData["OnDestroyFunc"];
		if(onDestroyObj.IsString())
		{
			const char *pString = onDestroyObj.GetString();
			if(strlen(pString) > MAX_FUNC_NAME_SIZE)
			{
				const std::string err("The length of the 'OnDestroyFunc' function name is too long.");
				errorMessages.push_back(err);
				return (false);
			}
			strcpy(m_OnDestroyLuaFunctionName, onDestroyObj.GetString());
		}
	
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void ActorParams::VSerialize(std::ostringstream &out) const
	{
		out << m_Type << " ";
		out << m_Size << " ";
		out << static_cast<I32>(*m_Id) << " ";
		if (m_Id.is_initialized())
		{
			out << *m_Id << " ";
		}
		out << m_Pos.GetX() << " " << m_Pos.GetY() << " " << m_Pos.GetZ() << " ";
		out << m_Color.GetX() << " " << m_Color.GetY() << " " << m_Color.GetZ() << " " << m_Color.GetW() << " ";
		out << m_OnCreateLuaFunctionName << " ";
		out << m_OnDestroyLuaFunctionName << " ";
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
    ActorParams *ActorParams::CreateFromStream(std::istringstream &in)
	{
		//I32 actorType;
		//in >> actorType;
		//
		//ActorParams *actor = NULL;
		//switch (actorType)
		//{
		//	case AT_Sphere:
		//	case AT_Teapot:
		//	case AT_TestObject:
		//	case AT_Grid:
		//	case AT_GenericMeshObject:
		//		break;
		//
		//	case AT_PP_Paddle:
		//		{
		//			// error: Can't see Game specific classes from here
		//		}
		//		break;
		//
		//	case AT_PP_Ball:
		//		{
		//
		//		}
		//		break;
		//
		//	default:
		//		SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, string("ActorParams::CreateFromStream()"), string("Attempted to create unimplemented actor type from a stream."));
		//		return 0;
		//}
		//
		//if (! actor->VInit(in))
		//{
		//	// something went wrong with the serialization...
		//	assert(0 && _T("Error in Actor stream initialization"));
		//	Delete(actor);
		//}
		//
		//return actor;

		return (NULL);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ActorParams *ActorParams::CreateFromLuaObj(LuaPlus::LuaObject &srcData)
	{
		return (NULL);
	}

	// /////////////////////////////////////////////////////////////////
	// ********************** BaseActor ********************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	BaseActor::BaseActor(const Matrix4 &mat, const I32 type, shared_ptr<ActorParams> params)\
		: m_id(0), m_Mat(mat), m_Type(type), m_Params(params)
	{
	}


	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseActor::VSetID(const ActorId id)
	{
		m_id = id;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseActor::VSetMat(const Matrix4 &newMat)
	{
		m_Mat = newMat;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4 BaseActor::VGetMat()
	{
		return (m_Mat);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	const I32 BaseActor::VGetType()
	{
		return (m_Type);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	const ActorId BaseActor::VGetID()
	{
		return (m_id);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	shared_ptr<IActorParams> BaseActor::VGetParams()
	{
		return (m_Params);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool BaseActor::VIsPhysical()
	{
		return (true);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool BaseActor::VIsGeometrical()
	{
		return (true);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseActor::VOnUpdate(const I32 deltaMilliseconds)
	{
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseActor::VRotateY(const F32 angleRadians)
	{
	}

}
