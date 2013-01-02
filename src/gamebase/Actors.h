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
// @file Actors.h
// @author Michael L. McShaffry (edited by PJ O Halloran)
// @date 09/07/2010
//
// File contains the header for the interfaces and classes for the
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
// in with the rest of my code.
//
// I have added/modified the following functionality:
// - I commented out game specific code and DirectX code for now.
// - I commented out the LuaState header until i get scripting
//		into the framework.
// - I moved all implementation code to the .cpp file.
// 
// BaseActor:
// - I removed the reference to the BaseGameState friend.
//
// ActorParams:
// - I changed it to be a class instead of a struct. Why was it a struct??
// - I used boost::optional instead of the custom one used in GCC3.
// - I commeneted out the position and color members for now as
//		they are DirectX specific.
//
// /////////////////////////////////////////////////////////////////

#ifndef __ACTORS_H
#define __ACTORS_H

// External Headers
#include <sstream>
#include <string>
#include <cstring>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include <LuaPlus/LuaLink.h>
#include <LuaPlus/LuaPlus.h>
#include <LuaPlus/LuaObject.h>

// Project Headers
#include "IActors.h"
#include "Matrix.h"
#include "Vector.h"
#include "ISceneNode.h"

// Disable depreciated function strcpy() warning as we dont want to use strcpy_s to keep our code 
//  platform independant.
#pragma warning( disable:4996 )

namespace GameHalloran
{
	
	// /////////////////////////////////////////////////////////////////
	// @class ActorParams
	// @author Mike McShaffry & PJ O Halloran
	//
	// This class defines the parameters belonging to an actor.
	// It is meant to be extended for different actor types.
	//
	// /////////////////////////////////////////////////////////////////
	class ActorParams : public IActorParams
	{
	private:

		static const I32 MAX_FUNC_NAME_SIZE = 64;				///< Max length of a LUA function name.

		I32 m_Size;												///< The number of bytes required to store the class (sizeof(ActorParams)).
		boost::optional<ActorId> m_Id;							///< The ID of the actor.
		Point3 m_Pos;											///< The position.
		ActorType m_Type;										///< The type of actor.
		Vector4 m_Color;										///< The Color
		char m_OnCreateLuaFunctionName[MAX_FUNC_NAME_SIZE];		///< Lua function name to call for this actor, upon creation.
		char m_OnDestroyLuaFunctionName[MAX_FUNC_NAME_SIZE];	///< As above, but on destruction.

	protected:

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		ActorParams();

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~ActorParams() { };

		// /////////////////////////////////////////////////////////////////
		// Get/Set the ID of the actor associated with these parameters.
		//
		// /////////////////////////////////////////////////////////////////
		virtual boost::optional<ActorId> VGetId()
		{
			return (m_Id);
		};
		virtual void VSetId(boost::optional<ActorId> id)
		{
			m_Id = id;
		};

		// /////////////////////////////////////////////////////////////////
		// Get/Set the size of the parameters.
		//
		// /////////////////////////////////////////////////////////////////
		virtual I32 VGetSize() const
		{
			return (m_Size);
		};
		virtual void VSetSize(const I32 size)
		{
			m_Size = size;
		};

		// /////////////////////////////////////////////////////////////////
		// Get/Set the position.
		//
		// /////////////////////////////////////////////////////////////////
		virtual Point3 VGetPos() const
		{
			return (m_Pos);
		};
		virtual void VSetPos(const Point3 &position)
		{
			m_Pos = position;
		};

		// /////////////////////////////////////////////////////////////////
		// Get/Set the color.
		//
		// /////////////////////////////////////////////////////////////////
		virtual Vector4 VGetColor() const
		{
			return (m_Color);
		};
		virtual void VSetColor(const Vector4 &color)
		{
			m_Color = color;
		};

		// /////////////////////////////////////////////////////////////////
		// Get/Set the actor type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ActorType VGetType() const
		{
			return (m_Type);
		};
		virtual void VSetType(const ActorType type)
		{
			m_Type = type;
		};

		// /////////////////////////////////////////////////////////////////
		// Get/Set the Create Function name.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const char * const VGetCreateFuncName() const
		{
			return (m_OnCreateLuaFunctionName);
		};
		virtual bool VSetCreateFuncName(const char * const createFuncName)
		{
			if(strlen(createFuncName) > MAX_FUNC_NAME_SIZE)
			{
				return (false);
			}
			strcpy(m_OnCreateLuaFunctionName, createFuncName);
			return (true);
		};

		// /////////////////////////////////////////////////////////////////
		// Get/Set the Destroy Function name.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const char * const VGetDestroyFuncName() const
		{
			return (m_OnDestroyLuaFunctionName);
		};
		virtual bool VSetDestroyFuncName(const char * const destroyFuncName)
		{
			if(strlen(destroyFuncName) > MAX_FUNC_NAME_SIZE)
			{
				return (false);
			}
			strcpy(m_OnDestroyLuaFunctionName, destroyFuncName);
			return (true);
		};

		// /////////////////////////////////////////////////////////////////
		// Initialize the parameters from a stream.
		//
		// @param in The input stream to initialize the parameters from.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VInit(std::istringstream &in);

		// /////////////////////////////////////////////////////////////////
		// Serialize the parameters out to a stream.
		//
		// @param out The output stream 
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const;

		// /////////////////////////////////////////////////////////////////
		// Initialize the parameters from a Lua script.
		// 
		// @param srcData The Lua object containing the parameter data.
		// @param errorMessages The error messages (maybe passed back from the script?)
		// 
		// /////////////////////////////////////////////////////////////////
		virtual bool VInit(LuaPlus::LuaObject &srcData, TErrorMessageList &errorMessages);

		// /////////////////////////////////////////////////////////////////
		// Initialize the parameters from a stream.
		//
		// @param in The stream to initlialize the stream from.
		//
		// @return ActorParams* A pointer to an ActorParams object initialized
		//						from the stream.
		//
		// /////////////////////////////////////////////////////////////////
		static ActorParams *CreateFromStream(std::istringstream &in);

		// /////////////////////////////////////////////////////////////////
		// Initialize an ActorParams object from a Lua script.
		// 
		// @param srcData The Lua object table containing the actor data.
		//
		// @return ActorParams* A pointer to an ActorParams object initialized
		//						from the stream.
		//
		// /////////////////////////////////////////////////////////////////
		static ActorParams *CreateFromLuaObj(LuaPlus::LuaObject &srcData);

		// /////////////////////////////////////////////////////////////////
		// The logic layers create function.  Creates an Actor in respect to
		// the logic layer from the parameters.
		//
		// The actor created should be added manually to the logic layer.
		//
		// @return boost::shared_ptr<IActor> A pointer to an actor or NULL
		//										on error.
		// 
		// /////////////////////////////////////////////////////////////////
		virtual boost::shared_ptr<IActor> VCreate() { return (boost::shared_ptr<IActor>()); };

		// /////////////////////////////////////////////////////////////////
		// The view layers create function.  Creates an SceneNode representing
		// the Actor in respect to the view layer from the parameters.
		//
		// @return boost::shared_ptr<ISceneNode> A pointer to a scene node.
		//
		// /////////////////////////////////////////////////////////////////
		virtual boost::shared_ptr<ISceneNode> VCreateSceneNode() { return(boost::shared_ptr<ISceneNode>()); };
	};

	// /////////////////////////////////////////////////////////////////
	// @class BaseActor
	// @author Michael L. McShaffry (edited by PJ O Halloran).
	//
	// This is the base implementation of an actor that all actors will
	// inherit from and extend.
	//
	// /////////////////////////////////////////////////////////////////
	class BaseActor : public IActor
	{
	private:

		ActorId m_id;								///< The ID of the actor.
		Matrix4 m_Mat;								///< The position/orientation of the actor.
		I32 m_Type;									///< The type of the actor.
		boost::shared_ptr<ActorParams> m_Params;	///< Pointer to the actors parameter object.

	protected:

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param mat The position of the actor.
		// @param type The type of actor.
		// @param params The parameters belonging to the actor.
		//
		// /////////////////////////////////////////////////////////////////
		BaseActor(const Matrix4 &mat, const I32 type, boost::shared_ptr<ActorParams> params);

		// /////////////////////////////////////////////////////////////////
		// Set the ID of the actor.
		//
		// @param id The new ID of the actor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetID(const ActorId id);

		// /////////////////////////////////////////////////////////////////
		// Set the matrix encapsulating the actors orientation and position.
		//
		// @param newMat
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetMat(const Matrix4 &newMat);

		// /////////////////////////////////////////////////////////////////
		// Get the matrix encapsulating the actors orientation and position.
		//
		// @return Matrix4
		//
		// /////////////////////////////////////////////////////////////////
		virtual Matrix4 VGetMat();
		
		// /////////////////////////////////////////////////////////////////
		// Get the type of actor.
		//
		// @return const I32 The actor type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const I32 VGetType();
		
		// /////////////////////////////////////////////////////////////////
		// Get the ID of the actor.
		//
		// @return ActorId The actor ID.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const ActorId VGetID();
		
		// /////////////////////////////////////////////////////////////////
		// Get the parameters associated with the actor.
		//
		// @return boost::shared_ptr<ActorParams> The parameters belonging to
		//											the actor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual boost::shared_ptr<IActorParams> VGetParams();
		
		// /////////////////////////////////////////////////////////////////
		// Check if the actor is physical.
		//
		// @return bool Is the actor physical?
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VIsPhysical();
		
		// /////////////////////////////////////////////////////////////////
		// Check if the actor is geometrical.
		//
		// @return bool Is the actor geometrical?
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VIsGeometrical();
		
		// /////////////////////////////////////////////////////////////////
		// Called once per main loop to update the state of the actor.
		//
		// @param deltaMilliseconds The number of milliseconds since the last
		//								update.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnUpdate(const I32 deltaMilliseconds);
		
		// /////////////////////////////////////////////////////////////////
		// Rotate the actor along the Y axis.
		//
		// @param angleRadians The angle to rotate the actor by.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VRotateY(const F32 angleRadians);
	};
}

#endif
