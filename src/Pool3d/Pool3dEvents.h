// /////////////////////////////////////////////////////////////////
// @file Pool3dEvents.h
// @author PJ O Halloran
// @date 01/11/2010
//
// File contains the header for the Pool3d event classes.
//
// /////////////////////////////////////////////////////////////////

#ifndef __POOL_3D_EVENTS_H
#define __POOL_3D_EVENTS_H

// External Headers
#include <strstream>
#include <string>

#include <boost/shared_ptr.hpp>


// Project Headers
#include "Events.h"
#include "PhysicsEvents.h"

#include "Pool3dCommon.h"


namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Complex_Mesh_Loaded
	// @author PJ O' Halloran
	//
	// Event fired when the View layer has completed loading a complex
	// mesh from which important dimension attributes may be calculated.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Complex_Mesh_Loaded : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		};

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param params The parameters of the actor the mesh belongs to.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Complex_Mesh_Loaded(const ActorParams *params)\
			: m_params(NULL)
		{
			if(params)
			{
				m_params = reinterpret_cast<ActorParams *>(GCC_NEW char[params->VGetSize()]);
				memcpy(m_params, params, params->VGetSize());
			}
		};

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Complex_Mesh_Loaded(std::istringstream &in)
		{
			// Note: Actor parameters are not serialized!
		};

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Complex_Mesh_Loaded(m_params));
		};

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Complex_Mesh_Loaded() { try { Delete(m_params); } catch(...) {}};

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
			return (m_LuaEventData);
		};

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable(pState);
			
			// Serialize the data necessary.
			m_bHasLuaEventData = true;
		};

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			// Note: parameters are not serialized!
		};

		// /////////////////////////////////////////////////////////////////
		// Get the actor parameters.
		//
		// /////////////////////////////////////////////////////////////////
		ActorParams *GetActorParameters() const
		{
			return (m_params);
		};

	private:
		ActorParams *m_params;							///< Actor parameters of the mesh.
		LuaPlus::LuaObject m_LuaEventData;				///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Load_Game
	// @author PJ O' Halloran
	//
	// Event fired when the user attempts to load a game.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Load_Game : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param filename The name of the file to load.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Load_Game(const std::string &filename)\
			: m_filename(filename)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Load_Game(std::istringstream &in)
		{
			in >> m_filename;
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Load_Game(m_filename));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Load_Game() {}

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
			return m_LuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );
			
			// Serialize the data necessary.
			m_LuaEventData.SetString("Filename", m_filename.c_str());

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_filename << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the filename to load.
		//
		// /////////////////////////////////////////////////////////////////
		std::string GetFilename() const
		{
			return (m_filename);
		};

	private:
		std::string m_filename;							///< The file attempt to load.
		LuaPlus::LuaObject m_LuaEventData;				///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Save_Game
	// @author PJ O' Halloran
	//
	// Event fired when the user attempts to save a game.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Save_Game : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param filename The name of the file to save.
		// @param saveType The type of save action (binary or human readable).
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Save_Game(const std::string &filename, const I32 saveType)\
			: m_filename(filename), m_saveType(saveType)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Save_Game(std::istringstream &in)
		{
			in >> m_filename;
			in >> m_saveType;
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Save_Game(m_filename, m_saveType));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Save_Game() {}

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
			return m_LuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );
			
			// Serialize the data necessary.
			m_LuaEventData.SetString("Filename", m_filename.c_str());
			m_LuaEventData.SetInteger("Type", m_saveType);

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_filename << " ";
			out << m_saveType << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the filename to load.
		//
		// /////////////////////////////////////////////////////////////////
		std::string GetFilename() const
		{
			return (m_filename);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the type of save file to create.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetSaveType() const
		{
			return (m_saveType);
		};

	private:
		std::string m_filename;							///< The file attempt to load.
		I32 m_saveType;									///< The type of save (e.g. binary, human readable).
		LuaPlus::LuaObject m_LuaEventData;				///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Graphics_Config_Change
	// @author PJ O' Halloran
	//
	// Event fired when the user changes the display parameters.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Graphics_Config_Change : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param width The screen width.
		// @param height The screen height.
		// @param filteringMode Texture filtering mode.
		// @param multisampling Use multisampling.
		// @param shadows Use shadows.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Graphics_Config_Change(const I32 width, const I32 height, const I32 filteringMode, const bool multisampling, const bool shadows)\
			: m_width(width), m_height(height), m_filteringMode(filteringMode), m_multisampling(multisampling), m_shadows(shadows)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Graphics_Config_Change(std::istringstream &in)
			: m_width(0), m_height(0), m_filteringMode(0), m_multisampling(false), m_shadows(false)
		{
			in >> m_width;
			in >> m_height;
			in >> m_filteringMode;
			in >> m_multisampling;
			in >> m_shadows;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param srcData Lua script data describing the event.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Graphics_Config_Change(LuaPlus::LuaObject &srcData)
			: m_width(0), m_height(0), m_filteringMode(0), m_multisampling(false), m_shadows(false)
		{
			GameHalloran::SetIntFromLua(srcData["Width"], m_width);
			GameHalloran::SetIntFromLua(srcData["Height"], m_height);
			GameHalloran::SetIntFromLua(srcData["Filtering"], m_filteringMode);
			GameHalloran::SetBoolFromLua(srcData["Multisampling"], m_multisampling);
			GameHalloran::SetBoolFromLua(srcData["Shadows"], m_shadows);
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Graphics_Config_Change(m_width, m_height, m_filteringMode, m_multisampling, m_shadows));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Graphics_Config_Change() {}

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
			return m_LuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );
			
			// Serialize the data necessary.
			m_LuaEventData.SetInteger("Width", m_width);
			m_LuaEventData.SetInteger("Height", m_height);
			m_LuaEventData.SetInteger("Filtering", m_filteringMode);
			m_LuaEventData.SetBoolean("Multisampling", m_multisampling);
			m_LuaEventData.SetBoolean("Shadows", m_shadows);

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_width << " ";
			out << m_height << " ";
			out << m_filteringMode << " ";
			out << (m_multisampling ? 1 : 0) << " ";
			out << (m_shadows ? 1 : 0) << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the screen width.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetWidth() const
		{
			return (m_width);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the screen height.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetHeight() const
		{
			return (m_height);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the texture filtering mode.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetTexFilteringMode() const
		{
			return (m_filteringMode);
		};

		// /////////////////////////////////////////////////////////////////
		// Turn on multisampling?
		//
		// /////////////////////////////////////////////////////////////////
		bool UseMultisampling() const
		{
			return (m_multisampling);
		};

		// /////////////////////////////////////////////////////////////////
		// Turn on shadows?
		//
		// /////////////////////////////////////////////////////////////////
		bool UseShadows() const
		{
			return (m_shadows);
		};

	private:
		I32 m_width;									///< Screen width.
		I32 m_height;									///< Screen Height.
		I32 m_filteringMode;							///< Teture filtering mode.
		bool m_multisampling;							///< Use multisampling.
		bool m_shadows;									///< Use shadows.
		LuaPlus::LuaObject m_LuaEventData;				///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Sound_Config_Change
	// @author PJ O' Halloran
	//
	// Event fired when the user changes the sound parameters.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Sound_Config_Change : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param masterVolume Master volume.
		// @param music Use music.
		// @param fx Use sound fx.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Sound_Config_Change(const F32 masterVolume, const bool music, const bool fx)\
			: m_masterVolume(masterVolume), m_music(music), m_fx(fx)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Sound_Config_Change(std::istringstream &in)\
			: m_masterVolume(0.0f), m_music(false), m_fx(false)
		{
			in >> m_masterVolume;
			in >> m_music;
			in >> m_fx;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param srcData Lua script data describing the event.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Sound_Config_Change(LuaPlus::LuaObject &srcData)\
			: m_masterVolume(0.0f), m_music(false), m_fx(false)
		{
			GameHalloran::SetFloatFromLua(srcData["MasterVolume"], m_masterVolume);
			GameHalloran::SetBoolFromLua(srcData["Music"], m_music);
			GameHalloran::SetBoolFromLua(srcData["Fx"], m_fx);
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Sound_Config_Change(m_masterVolume, m_music, m_fx));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Sound_Config_Change() {}

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
			return m_LuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );
			
			// Serialize the data necessary.
			m_LuaEventData.SetNumber("MasterVolume", m_masterVolume);
			m_LuaEventData.SetBoolean("Music", m_music);
			m_LuaEventData.SetBoolean("Fx", m_fx);

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_masterVolume << " ";
			out << (m_music ? 1 : 0) << " ";
			out << (m_fx ? 1 : 0) << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the master volume.
		//
		// /////////////////////////////////////////////////////////////////
		F32 GetVolume() const
		{
			return (m_masterVolume);
		};

		// /////////////////////////////////////////////////////////////////
		// Play music?
		//
		// /////////////////////////////////////////////////////////////////
		bool PlayMusic() const
		{
			return (m_music);
		};

		// /////////////////////////////////////////////////////////////////
		// Play sound effects?
		//
		// /////////////////////////////////////////////////////////////////
		bool PlaySoundFx() const
		{
			return (m_fx);
		};

	private:
		F32 m_masterVolume;						///< Master volume.
		bool m_music;								///< Play music.
		bool m_fx;									///< Play sound effects.
		LuaPlus::LuaObject m_LuaEventData;			///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Save_Config_Change
	// @author PJ O' Halloran
	//
	// Event fired when the user changes the save configuration parameters.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Save_Config_Change : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param directoryName New save directory name.
		// @param saveType Save file type.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Save_Config_Change(const std::string &directoryName, const I32 saveType)\
			: m_directoryName(directoryName), m_saveType(saveType)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Save_Config_Change(std::istringstream &in)\
			: m_directoryName(), m_saveType()
		{
			in >> m_directoryName;
			in >> m_saveType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param srcData Lua script data describing the event.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Save_Config_Change(LuaPlus::LuaObject &srcData)\
			: m_directoryName(), m_saveType()
		{
			GameHalloran::SetStringFromLua(srcData["Directory"], m_directoryName);
			GameHalloran::SetIntFromLua(srcData["SaveType"], m_saveType);
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Save_Config_Change(m_directoryName, m_saveType));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Save_Config_Change() {}

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
			return m_LuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );
			
			// Serialize the data necessary.
			m_LuaEventData.SetString("Directory", m_directoryName.c_str());
			m_LuaEventData.SetInteger("SaveType", m_saveType);

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_directoryName << " ";
			out << m_saveType << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the new save directory.
		//
		// /////////////////////////////////////////////////////////////////
		std::string GetDirectory() const
		{
			return (m_directoryName);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the save file type.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetSaveType() const
		{
			return (m_saveType);
		};

	private:
		std::string m_directoryName;				///< New save directory name.
		I32 m_saveType;								///< Save file type.
		LuaPlus::LuaObject m_LuaEventData;			///< The LUA event data.
	};

	// TODO: Is this there a need for configurable controls for a demo game??? I dont know if it would be worth it....

	//// /////////////////////////////////////////////////////////////////
	//// @class EvtData_Control_Config_Change
	//// @author PJ O' Halloran
	////
	//// Event fired when the user changes the control parameters.
	////
	//// /////////////////////////////////////////////////////////////////
	//class EvtData_Control_Config_Change : public BaseEventData
	//{
	//public:
	//	static const EventType sk_EventType;

	//	// /////////////////////////////////////////////////////////////////
	//	// Get the event type.
	//	//
	//	// /////////////////////////////////////////////////////////////////
	//	virtual const EventType &VGetEventType(void) const
	//	{
	//		return sk_EventType;
	//	}

	//	// /////////////////////////////////////////////////////////////////
	//	// Constructor.
	//	//
	//	// @param x 
	//	//
	//	// /////////////////////////////////////////////////////////////////
	//	explicit EvtData_Control_Config_Change(const F32 x)\
	//		: m_x(x)
	//	{
	//	}

	//	// /////////////////////////////////////////////////////////////////
	//	// Constructor.
	//	//
	//	// @param in The string stream to create the event from.
	//	//
	//	// /////////////////////////////////////////////////////////////////
	//	EvtData_Control_Config_Change(std::istringstream &in)
	//	{
	//		in >> m_x;
	//	}

	//	// /////////////////////////////////////////////////////////////////
	//	// Make a copy of the event.
	//	//
	//	// /////////////////////////////////////////////////////////////////
	//	virtual IEventDataPtr VCopy() const
	//	{
	//		return IEventDataPtr(GCC_NEW EvtData_Control_Config_Change());
	//	}

	//	// /////////////////////////////////////////////////////////////////
	//	// Destructor.
	//	//
	//	// /////////////////////////////////////////////////////////////////
	//	virtual ~EvtData_Control_Config_Change() {}

	//	// /////////////////////////////////////////////////////////////////
	//	// Get the LUA event data.
	//	//
	//	// /////////////////////////////////////////////////////////////////
	//	virtual LuaPlus::LuaObject VGetLuaEventData(void) const
	//	{
	//		assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
	//		return m_LuaEventData;
	//	}

	//	// /////////////////////////////////////////////////////////////////
	//	// Build the LUA event data.
	//	//
	//	// /////////////////////////////////////////////////////////////////
	//	virtual void VBuildLuaEventData(void)
	//	{
	//		assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

	//		// Get the global state.
	//		LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
	//		m_LuaEventData.AssignNewTable( pState );
	//		
	//		// Serialize the data necessary.
	//		m_LuaEventData.SetInteger("Name", m_val);

	//		m_bHasLuaEventData = true;
	//	}

	//	// /////////////////////////////////////////////////////////////////
	//	// Serialize the event.
	//	//
	//	// @param The stream to serialize the event out to.
	//	//
	//	// /////////////////////////////////////////////////////////////////
	//	virtual void VSerialize(std::ostringstream &out) const
	//	{
	//		out << x << " ";
	//	}

	//private:
	//	I32 m_frLeft;
	//	I32 m_frRight;
	//	I32 m_frUp;
	//	I32 m_frDown;
	//	I32 m_frZoomIn;
	//	I32 m_frZoomOut;

	//	LuaPlus::LuaObject m_LuaEventData;			///< The LUA event data.
	//};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Game_Mode
	// @author PJ O' Halloran
	//
	// Event fired to inform the app what type of game is starting.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Game_Mode : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param mode The game mode to start.
		// @param humanPlayers The number of human players.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Game_Mode(const I32 mode, const I32 humanPlayers)\
			: m_mode(mode), m_humanPlayers(humanPlayers)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Game_Mode(std::istringstream &in)
		{
			in >> m_mode;
			in >> m_humanPlayers;
		}

		// /////////////////////////////////////////////////////////////////
		// Lua constructor.
		//
		// @param srcData The lua table to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Game_Mode(LuaPlus::LuaObject &srcData) : m_mode(0), m_humanPlayers(0), m_LuaEventData(srcData)
		{
			if(srcData["Mode"].IsInteger())
			{
				m_mode = srcData["Mode"].GetInteger();
			}
			if(srcData["HumanPlayers"].IsInteger())
			{
				m_humanPlayers = srcData["HumanPlayers"].GetInteger();
			}
		};

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Game_Mode(m_mode, m_humanPlayers));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Game_Mode() {}

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
			return m_LuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );
			
			// Serialize the data necessary.
			m_LuaEventData.SetInteger("Mode", m_mode);
			m_LuaEventData.SetInteger("HumanPlayers", m_humanPlayers);

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_mode << " ";
			out << m_humanPlayers << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the game mode Id.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetMode() const { return (m_mode); };

		// /////////////////////////////////////////////////////////////////
		// Get the number of human players
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetPlayers() const { return (m_humanPlayers); };

	private:
		I32 m_mode;									///< Game mode to start.
		I32 m_humanPlayers;							///< Number of human players.
		LuaPlus::LuaObject m_LuaEventData;			///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Camera_Aim_Mode_Update
	// @author PJ O' Halloran
	//
	// Event fired to inform the app of a change in the camera aim/move
	// mode.  The game logic controls the current camera mode.  It sends
	// out this message when the aim mode is changed.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Camera_Aim_Mode_Update : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param mode The current camera aim mode.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Camera_Aim_Mode_Update(const enum ViewMode mode)\
			: m_mode(mode), m_LuaEventData()
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Camera_Aim_Mode_Update(std::istringstream &in): m_mode(eShotAim), m_LuaEventData()
		{
			I32 tmp;
			in >> tmp;
			if(tmp < eNumberViewModes)
			{
				m_mode = ViewMode(tmp);
			}
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Camera_Aim_Mode_Update(m_mode));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Camera_Aim_Mode_Update() {}

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
			return m_LuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable(pState);
			
			// Serialize the data necessary.
			m_LuaEventData.SetInteger("ViewMode", int(m_mode));

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << I32(m_mode) << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the current camera mode.
		//
		// /////////////////////////////////////////////////////////////////
		ViewMode GetCameraMode() const { return (m_mode); };

	private:
		ViewMode m_mode;						///< ID of the player whose turn it is.
		LuaPlus::LuaObject m_LuaEventData;		///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Camera_Aim_Mode_Request
	// @author PJ O' Halloran
	//
	// Event fired to inform the app of a change request for the camera aim/move
	// mode.  The logic layer will be the only interested listener and
	// will decide if the aim mode should be changed.  If it does decide
	// to change the aim mode it will broadcast a EvtData_Camera_Aim_Mode_Update
	// event.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Camera_Aim_Mode_Request : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param mode The current camera aim mode.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Camera_Aim_Mode_Request(const enum ViewMode mode)\
			: m_mode(mode), m_LuaEventData()
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Camera_Aim_Mode_Request(std::istringstream &in): m_mode(eShotAim), m_LuaEventData()
		{
			I32 tmp;
			in >> tmp;
			if(tmp < eNumberViewModes)
			{
				m_mode = ViewMode(tmp);
			}
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Camera_Aim_Mode_Request(LuaPlus::LuaObject &srcData): m_mode(eShotAim), m_LuaEventData(srcData)
		{
			m_bHasLuaEventData = true;

			if(m_LuaEventData["ViewMode"].IsInteger())
			{
				I32 tmp(m_LuaEventData.GetInteger());
				if(tmp < eNumberViewModes)
				{
					m_mode = ViewMode(tmp);
				}
			}
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Camera_Aim_Mode_Request(m_mode));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Camera_Aim_Mode_Request() {}

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
			return m_LuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable(pState);
			
			// Serialize the data necessary.
			m_LuaEventData.SetInteger("ViewMode", I32(m_mode));

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << I32(m_mode) << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the current camera mode.
		//
		// /////////////////////////////////////////////////////////////////
		ViewMode GetCameraMode() const { return (m_mode); };

	private:
		ViewMode m_mode;						///< ID of the player whose turn it is.
		LuaPlus::LuaObject m_LuaEventData;		///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Shot_Start
	// @author PJ O' Halloran
	//
	// Event fired to inform the app that a shot was just taken.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Shot_Start : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param playerId The ID of the player.
		// @param direction Direction of shot.
		// @param spinDir Spin direction on cue ball.
		// @param powerForce Power of shot.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Shot_Start(const I32 playerId, const Vector3 &direction, const Vector3 &spinDir, const F32 shotPower)\
			: m_playerId(playerId), m_direction(direction), m_spinDir(spinDir), m_powerForce(shotPower)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Shot_Start(std::istringstream &in)
		{
			in >> m_playerId;
			F32 x, y, z;
			in >> x;
			in >> y;
			in >> z;
			m_direction.Set(x, y, z);
			in >> x;
			in >> y;
			in >> z;
			m_spinDir.Set(x, y, z);
			in >> m_powerForce;
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Shot_Start(m_playerId, m_direction, m_spinDir, m_powerForce));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Shot_Start() {}

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
			return m_LuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert((false == m_bHasLuaEventData) && "Already built lua event data!");

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable(pState);
			
			// Serialize the data necessary.
			m_LuaEventData.SetInteger("PlayerId", m_playerId);
			LuaPlus::LuaObject dirTable = m_LuaEventData.CreateTable("ShotDirection");
			dirTable.SetNumber("x", m_direction.GetX());
			dirTable.SetNumber("y", m_direction.GetY());
			dirTable.SetNumber("z", m_direction.GetZ());
			LuaPlus::LuaObject spinTable = m_LuaEventData.CreateTable("SpinDirection");
			spinTable.SetNumber("x", m_spinDir.GetX());
			spinTable.SetNumber("y", m_spinDir.GetY());
			spinTable.SetNumber("z", m_spinDir.GetZ());
			m_LuaEventData.SetNumber("ShotPower", m_powerForce);

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_playerId << " ";
			out << m_direction.GetX() << " " << m_direction.GetY() << " " << m_direction.GetZ() << " ";
			out << m_spinDir.GetX() << " " << m_spinDir.GetY() << " " << m_spinDir.GetZ() << " ";
			out << m_powerForce << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the player ID.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetPlayerId() const { return (m_playerId); };

		// /////////////////////////////////////////////////////////////////
		// Get the direction of the shot.
		//
		// /////////////////////////////////////////////////////////////////
		Vector3 GetShotDirection() const { return (m_direction); };

		// /////////////////////////////////////////////////////////////////
		// Get the spin direction of the shot.
		//
		// /////////////////////////////////////////////////////////////////
		Vector3 GetSpinDirection() const { return (m_spinDir); };

		// /////////////////////////////////////////////////////////////////
		// Get the power of the shot (in Newtons).
		//
		// /////////////////////////////////////////////////////////////////
		F32 GetShotPower() const { return (m_powerForce); };

	private:
		I32 m_playerId;								///< ID of player.
		Vector3 m_direction;						///< Direction of shot.
		Vector3 m_spinDir;							///< Direction of spin force on ball.
		F32 m_powerForce;							///< Power of shot.
		LuaPlus::LuaObject m_LuaEventData;			///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Shot_End
	// @author PJ O' Halloran
	//
	// Event fired to inform the app that a shot has completed (i.e. when
	// all balls come to a resting state on the table.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Shot_End : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param playerId The ID of the player who took the previous shot.
		// @param nextPlayerId The ID of the player who is next in line to take
		//					a shot.
		// @param cueBallPosition Position of the cue ball after the shot.
		// @param nearestBallPos Position of the nearest player ball.
		// @param playerScore The players score after the shot.
		// @param penalty Did the player incur a minor penalty?
		// @param gameEndPenalty Did the player incur a game ending penalty?
		// @param gameWon Did the player win the game?
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Shot_End(const I32 playerId, const I32 nextPlayerId, const boost::optional<Point3> &cueBallPosition, const boost::optional<Point3> nearestBallPos, const I32 playerScore, const bool penalty, const bool gameEndPenalty, const bool gameWon)\
			: m_playerId(playerId), m_nextPlayerId(nextPlayerId), m_cueBallPosition(cueBallPosition), m_nearestBallPos(nearestBallPos), m_playerScore(playerScore), m_penalty(penalty), m_gameEndPenalty(gameEndPenalty), m_gameWon(gameWon)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Shot_End(std::istringstream &in) : m_playerId(0), m_nextPlayerId(0), m_cueBallPosition(), m_nearestBallPos(), m_playerScore(0), m_penalty(false), m_gameEndPenalty(false), m_gameWon(false)
		{
			in >> m_playerId;
			in >> m_nextPlayerId;
			I32 tmp;
			in >> tmp;
			if(tmp > 0)
			{
				F32 x, y, z;
				in >> x;
				in >> y;
				in >> z;
				m_cueBallPosition = Point3(x, y, z);
			}
			in >> tmp;
			if(tmp > 0)
			{
				F32 x, y, z;
				in >> x;
				in >> y;
				in >> z;
				m_nearestBallPos = Point3(x, y, z);
			}
			in >> m_playerScore;
			in >> tmp;
			if(tmp > 0)
			{
				m_penalty = true;
			}
			in >> tmp;
			if(tmp > 0)
			{
				m_gameEndPenalty = true;
			}
			in >> tmp;
			if(tmp > 0)
			{
				m_gameWon = true;
			}
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Shot_End(m_playerId, m_nextPlayerId, m_cueBallPosition, m_nearestBallPos, m_playerScore, m_penalty, m_gameEndPenalty, m_gameWon));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Shot_End() {}

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
			return m_LuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert((false == m_bHasLuaEventData) && "Already built lua event data!");

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable(pState);

			// Serialize the data necessary.
			m_LuaEventData.SetInteger("PlayerId", m_playerId);
			m_LuaEventData.SetInteger("NextPlayerId", m_nextPlayerId);
			if(m_cueBallPosition.is_initialized())
			{
				LuaPlus::LuaObject posTable = m_LuaEventData.CreateTable("CueBallPosition");
				posTable.SetNumber("x", m_cueBallPosition->GetX());
				posTable.SetNumber("y", m_cueBallPosition->GetY());
				posTable.SetNumber("z", m_cueBallPosition->GetZ());
			}
			if(m_nearestBallPos.is_initialized())
			{
				LuaPlus::LuaObject posTable = m_LuaEventData.CreateTable("NearestBallPosition");
				posTable.SetNumber("x", m_nearestBallPos->GetX());
				posTable.SetNumber("y", m_nearestBallPos->GetY());
				posTable.SetNumber("z", m_nearestBallPos->GetZ());
			}
			m_LuaEventData.SetInteger("CurrentPlayerScore", m_playerScore);
			m_LuaEventData.SetBoolean("Penalty", m_penalty);
			m_LuaEventData.SetBoolean("GameEndPenalty", m_gameEndPenalty);
			m_LuaEventData.SetBoolean("GameWon", m_gameWon);

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_playerId << " ";
			out << m_nextPlayerId << " ";
			// Next value is 1 if a position follows in the stream and 0 if is no position.
			out << I32(m_cueBallPosition.is_initialized()) << " ";
			if(m_cueBallPosition.is_initialized())
			{
				out << m_cueBallPosition->GetX() << " " << m_cueBallPosition->GetY() << " " << m_cueBallPosition->GetZ() << " ";
			}
			out << I32(m_nearestBallPos.is_initialized()) << " ";
			if(m_nearestBallPos.is_initialized())
			{
				out << m_nearestBallPos->GetX() << " " << m_nearestBallPos->GetY() << " " << m_nearestBallPos->GetZ() << " ";
			}
			out << m_playerScore << " ";
			out << (m_penalty ? 1 : 0) << " ";
			out << (m_gameEndPenalty ? 1 : 0) << " ";
			out << (m_gameWon ? 1 : 0) << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the player ID of the player who took the previous shot.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetPlayerId() const { return (m_playerId); };

		// /////////////////////////////////////////////////////////////////
		// Get the player ID of the player who is next in line to take their
		// shot.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetNextPlayerId() const { return (m_nextPlayerId); };

		// /////////////////////////////////////////////////////////////////
		// Get the final cue ball position.  If the value is not initialized
		// it means that the cue ball was potted during the last shot.
		//
		// /////////////////////////////////////////////////////////////////
		boost::optional<Point3> GetFinalCueBallPosition() const { return (m_cueBallPosition); };

		// /////////////////////////////////////////////////////////////////
		// Get the position of the nearest player ball.
		//
		// /////////////////////////////////////////////////////////////////
		boost::optional<Point3> GetNearestBallPosition() const { return (m_nearestBallPos); };

		// /////////////////////////////////////////////////////////////////
		// Is the cue ball still on the table?
		//
		// /////////////////////////////////////////////////////////////////
		bool IsCueBallAlive() const { return (m_cueBallPosition.is_initialized()); };

		// /////////////////////////////////////////////////////////////////
		// Get the previous players score after the shot.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetPlayerScore() const { return (m_playerScore); };

		// /////////////////////////////////////////////////////////////////
		// Did the player incur a minor penalty?
		//
		// /////////////////////////////////////////////////////////////////
		bool IsPenalty() const { return (m_penalty); };

		// /////////////////////////////////////////////////////////////////
		// Did the player incur a game ending penalty?
		//
		// /////////////////////////////////////////////////////////////////
		bool IsGameEndPenalty() const { return (m_gameEndPenalty); };

		// /////////////////////////////////////////////////////////////////
		// Did the player win the game?
		//
		// /////////////////////////////////////////////////////////////////
		bool IsGameWon() const { return (m_gameWon); };

	private:
		I32 m_playerId;								///< ID of player who took last shot.
		I32 m_nextPlayerId;							///< ID of the player who is to take the next shot.
		boost::optional<Point3> m_cueBallPosition;	///< Final position of cue ball (Cue ball was potted during last shot if this is not initialized).
		boost::optional<Point3> m_nearestBallPos;	///< Position of the nearest player ball.
		I32 m_playerScore;							///< The players new score after the shot.
		bool m_penalty;								///< Did the player incur a penalty during the shot (pot invalid balls, miss cue ball, not hit any player balls)?
		bool m_gameEndPenalty;						///< Did the player incur a game ending penalty during the shot (pot black ball before time)?
		bool m_gameWon;								///< Did the player win the game during the last shot?
		LuaPlus::LuaObject m_LuaEventData;			///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Game_End
	// @author PJ O' Halloran
	//
	// Event fired to inform the app of a game over event.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Game_End : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param playerId The ID of the winning player.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Game_End(const I32 playerId)\
			: m_winnerPlayerId(playerId)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Game_End(std::istringstream &in)
		{
			in >> m_winnerPlayerId;
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Game_End(m_winnerPlayerId));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Game_End() {}

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
			return m_LuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );
			
			// Serialize the data necessary.
			m_LuaEventData.SetInteger("WinnerPlayerId", m_winnerPlayerId);

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_winnerPlayerId << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the player ID.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetWinnerPlayerId() const { return (m_winnerPlayerId); };

	private:
		I32 m_winnerPlayerId;						///< ID of the last player.
		LuaPlus::LuaObject m_LuaEventData;			///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Camera_Auto_Pan_Start
	// @author PJ O' Halloran
	//
	// Event fired to inform the app that the camera is beginning an
	// automatic panning operation over the scene.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Camera_Auto_Pan_Start : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param panId The ID of the panning operation.
		// @param startPos The starting position of the camera.
		// @param endPos The end position of the camera.
		// @param duration The time in seconds the camera has to pan.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Camera_Auto_Pan_Start(const I32 panId, const Point3 &startPos, const Point3 &endPos, const F32 duration)\
			: m_panId(panId), m_startPos(startPos), m_endPos(endPos), m_duration(duration)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Camera_Auto_Pan_Start(std::istringstream &in)
		{
			in >> m_panId;
			F32 x, y, z;
			in >> x;
			in >> y;
			in >> z;
			m_startPos.Set(x, y, z);
			in >> x;
			in >> y;
			in >> z;
			m_endPos.Set(x, y, z);
			in >> m_duration;
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Camera_Auto_Pan_Start(m_panId, m_startPos, m_endPos, m_duration));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Camera_Auto_Pan_Start() {}

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
			return m_LuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );
			
			// Serialize the data necessary.
			m_LuaEventData.SetInteger("PanId", m_panId);
			LuaPlus::LuaObject sPosTable = m_LuaEventData.CreateTable("StartPosition");
			sPosTable.SetNumber("x", m_startPos.GetX());
			sPosTable.SetNumber("y", m_startPos.GetY());
			sPosTable.SetNumber("z", m_startPos.GetZ());
			LuaPlus::LuaObject ePosTable = m_LuaEventData.CreateTable("EndPosition");
			ePosTable.SetNumber("x", m_endPos.GetX());
			ePosTable.SetNumber("y", m_endPos.GetY());
			ePosTable.SetNumber("z", m_endPos.GetZ());
			m_LuaEventData.SetNumber("Duration", m_duration);

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_panId << " ";
			out << m_startPos.GetX() << " " << m_startPos.GetY() << " " << m_startPos.GetZ() << " ";
			out << m_endPos.GetX() << " " << m_endPos.GetY() << " " << m_endPos.GetZ() << " ";
			out << m_duration << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the pan operation ID.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetPanId() const { return (m_panId); };

		// /////////////////////////////////////////////////////////////////
		// Get the starting position in world space.
		//
		// /////////////////////////////////////////////////////////////////
		Point3 GetStartingPosition() const { return (m_startPos); };

		// /////////////////////////////////////////////////////////////////
		// Get the ending position in world space.
		//
		// /////////////////////////////////////////////////////////////////
		Point3 GetEndingPosition() const { return (m_endPos); };

		// /////////////////////////////////////////////////////////////////
		// Get the time in seconds the camera has to pan to the new position.
		//
		// /////////////////////////////////////////////////////////////////
		F32 GetDuration() const { return (m_duration); };

	private:
		I32 m_panId;								///< ID of the panning operation.
		Point3 m_startPos;							///< Starting position.
		Point3 m_endPos;							///< Ending position.
		F32 m_duration;							///< Time to take to pan camera (seconds).
		LuaPlus::LuaObject m_LuaEventData;			///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Camera_Auto_Pan_Stop
	// @author PJ O' Halloran
	//
	// Event fired to inform the app that the camera is ending an
	// automatic panning operation over the scene.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Camera_Auto_Pan_Stop : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param panId The ID of the panning operation.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Camera_Auto_Pan_Stop(const I32 panId)\
			: m_panId(panId)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Camera_Auto_Pan_Stop(std::istringstream &in)
		{
			in >> m_panId;
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Camera_Auto_Pan_Stop(m_panId));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Camera_Auto_Pan_Stop() {}

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
			return m_LuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );
			
			// Serialize the data necessary.
			m_LuaEventData.SetInteger("PanId", m_panId);

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_panId << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the pan operation ID.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetPanId() const { return (m_panId); };

	private:
		I32 m_panId;								///< ID of the panning operation.
		LuaPlus::LuaObject m_LuaEventData;			///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_ResetCueBall
	// @author PJ O' Halloran
	//
	// Event fired to inform the app to reset the cue ball to a certain
	// position on the table and the the player cue and camera should be
	// reset behind it.  Called on game init and whenever the cue ball
	// is accidently or intentionally potted.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_ResetCueBall : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param panId The ID of the panning operation.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_ResetCueBall(const Point3 &pos)\
			: m_cueBallPos(pos)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param srcData Event data from a lua object.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_ResetCueBall(LuaPlus::LuaObject &srcData) : m_cueBallPos()
		{
			GameHalloran::SetPoint3FromLua(srcData["CueBallPosition"], m_cueBallPos);
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_ResetCueBall(std::istringstream &in) : m_cueBallPos()
		{
			F32 x, y, z;
			in >> x;
			in >> y;
			in >> z;
			m_cueBallPos.Set(x, y, z);
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_ResetCueBall(m_cueBallPos));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_ResetCueBall() {}

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
			return m_LuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert((false == m_bHasLuaEventData) && "Already built lua event data!");

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable(pState);
			
			// Serialize the data necessary.
			LuaPlus::LuaObject luaTable = m_LuaEventData.CreateTable("CueBallPosition");
			luaTable.SetNumber("x", m_cueBallPos.GetX());
			luaTable.SetNumber("y", m_cueBallPos.GetY());
			luaTable.SetNumber("z", m_cueBallPos.GetZ());

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_cueBallPos.GetX() << " ";
			out << m_cueBallPos.GetY() << " ";
			out << m_cueBallPos.GetZ() << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the cue ball position.
		//
		// /////////////////////////////////////////////////////////////////
		Point3 GetCueBallPosition() const { return (m_cueBallPos); };

	private:
		Point3 m_cueBallPos;						///< The position that the cue ball is to be reset to.
		LuaPlus::LuaObject m_LuaEventData;			///< The LUA event data.
	};

}

#endif
