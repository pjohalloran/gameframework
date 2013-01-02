// /////////////////////////////////////////////////////////////////
// @file Pool3dActors.cpp
// @author PJ O Halloran
// @date 01/11/2010
//
// File contains the impl for the Pool3d actors classes.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <strstream>

#include <string>

#include <cstring>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

// Project Headers
#include "Pool3dActors.h"
#include "Pool3dSceneNodes.h"
#include "Pool3dLogic.h"

#include "GameBase.h"
#include "GameColors.h"
#include "LuaStateManager.h"
#include "GameMain.h"
#include "BulletPhysics.h"


using boost::shared_ptr;

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// ********************** Pool3dActorParams ************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Pool3dActorParams::Pool3dActorParams(const std::string &textureName, const std::string &shaderName, const std::string &meshName, const Material &material, const PhysicsActorInfo &physicsInfo)\
		: ActorParams(), m_material(material), m_physicsInfo(physicsInfo)
	{
		ActorParams::VSetSize(sizeof(Pool3dActorParams));
		ActorParams::VSetType(AT_Unknown);

		// Copy the strings into their places.
		SetStringMember(m_textureName, textureName);
		SetStringMember(m_shaderName, shaderName);
		SetStringMember(m_meshName, meshName);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Pool3dActorParams::~Pool3dActorParams()
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dActorParams::SetStringMember(char *memberArr, const std::string &str)
	{
		memset(memberArr, 0, sizeof(char) * MAX_NAME_SIZE);
		if(!str.empty() && str.size() < std::string::size_type(MAX_NAME_SIZE))
		{
			strcpy(memberArr, str.c_str());
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dActorParams::VInit(std::istringstream &in)
	{
		if (ActorParams::VInit(in))
		{
			in >> m_textureName;
			in >> m_shaderName;
			in >> m_meshName;

			// Actor Material Info.
			F32 r, g, b, a;
			// ambient
			in >> r;
			in >> g;
			in >> b;
			in >> a;
			m_material.SetAmbient(GameColor(r, g, b, a));
			// diffuse
			in >> r;
			in >> g;
			in >> b;
			in >> a;
			m_material.SetDiffuse(GameColor(r, g, b, a));
			// specular
			in >> r;
			in >> g;
			in >> b;
			in >> a;
			m_material.SetSpecular(GameColor(r, g, b, a));
			// emissive
			in >> r;
			in >> g;
			in >> b;
			in >> a;
			m_material.SetEmissive(GameColor(r, g, b, a));
			// shininess
			in >> r;
			m_material.SetSpecularPower(r);

			// Actor physics info.
			in >> m_physicsInfo.m_restitution;
			in >> m_physicsInfo.m_friction;
			in >> m_physicsInfo.m_density;
			in >> m_physicsInfo.m_linearDamping;
			in >> m_physicsInfo.m_angularDamping;

			return (true);
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dActorParams::VSerialize(std::ostringstream &out) const
	{
		ActorParams::VSerialize(out);
		out << m_textureName << " ";
		out << m_shaderName << " ";
		out << m_meshName << " ";

		GameColor curr = m_material.GetAmbient();
		out << curr.GetX() << " ";
		out << curr.GetY() << " ";
		out << curr.GetZ() << " ";
		out << curr.GetW() << " ";
		curr = m_material.GetDiffuse();
		out << curr.GetX() << " ";
		out << curr.GetY() << " ";
		out << curr.GetZ() << " ";
		out << curr.GetW() << " ";
		curr = m_material.GetSpecular();
		out << curr.GetX() << " ";
		out << curr.GetY() << " ";
		out << curr.GetZ() << " ";
		out << curr.GetW() << " ";
		curr = m_material.GetEmissive();
		out << curr.GetX() << " ";
		out << curr.GetY() << " ";
		out << curr.GetZ() << " ";
		out << curr.GetW() << " ";
		out << m_material.GetSpecularPower();

		out << m_physicsInfo.m_restitution << " ";
		out << m_physicsInfo.m_friction << " ";
		out << m_physicsInfo.m_density << " ";
		out << m_physicsInfo.m_linearDamping << " ";
		out << m_physicsInfo.m_angularDamping << " ";
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool Pool3dActorParams::VInit(LuaPlus::LuaObject &srcData, TErrorMessageList &errorMessages)
	{
		if (!ActorParams::VInit(srcData, errorMessages))
		{
			return (false);
		}

		std::string tempStr;

		if(srcData["TextureName"].IsString())
		{
			if(!SetStringFromLua(srcData["TextureName"], tempStr))
			{
				return (false);
			}
			SetTextureName(tempStr);
		}

		if(srcData["ShaderName"].IsString())
		{
			if(!SetStringFromLua(srcData["ShaderName"], tempStr))
			{
				return (false);
			}
			SetShaderName(tempStr);
		}

		if(srcData["MeshName"].IsString())
		{
			if(!SetStringFromLua(srcData["MeshName"], tempStr))
			{
				return (false);
			}
			SetMeshName(tempStr);
		}

		LuaPlus::LuaObject materialTable = srcData["Material"];
		if(materialTable.IsTable())
		{
			GameColor tmpColor;

			if(materialTable["Ambient"].IsTable())
			{
				if(!SetColorFromLua(materialTable["Ambient"], tmpColor))
				{
					return (false);
				}
				m_material.SetAmbient(tmpColor);
			}

			if(materialTable["Diffuse"].IsTable())
			{
				if(!SetColorFromLua(materialTable["Diffuse"], tmpColor))
				{
					return (false);
				}
				m_material.SetDiffuse(tmpColor);
				// Don't forget to set the actors color to the same value as its diffuse color!
				VSetColor(tmpColor);
			}

			if(materialTable["Specular"].IsTable())
			{
				if(!SetColorFromLua(materialTable["Specular"], tmpColor))
				{
					return (false);
				}
				m_material.SetSpecular(tmpColor);
			}

			if(materialTable["Emissive"].IsTable())
			{
				if(!SetColorFromLua(materialTable["Emissive"], tmpColor))
				{
					return (false);
				}
				m_material.SetEmissive(tmpColor);
			}

			F32 number = 0.0f;
			if(materialTable["Shininess"].IsNumber())
			{
				if(!SetFloatFromLua(materialTable["Shininess"], number))
				{
					return (false);
				}
				m_material.SetSpecularPower(number);
			}
		}

		LuaPlus::LuaObject physicsTable = srcData["PhysicsInformation"];
		if(physicsTable.IsTable())
		{
			if(physicsTable["Restitution"].IsNumber() && !SetFloatFromLua(physicsTable["Restitution"], m_physicsInfo.m_restitution))
			{
				return (false);
			}
			if(physicsTable["Friction"].IsNumber() && !SetFloatFromLua(physicsTable["Friction"], m_physicsInfo.m_friction))
			{
				return (false);
			}
			if(physicsTable["Density"].IsNumber() && !SetFloatFromLua(physicsTable["Density"], m_physicsInfo.m_density))
			{
				return (false);
			}
			if(physicsTable["LinearDamping"].IsNumber() && !SetFloatFromLua(physicsTable["LinearDamping"], m_physicsInfo.m_linearDamping))
			{
				return (false);
			}
			if(physicsTable["AngularDamping"].IsNumber() && !SetFloatFromLua(physicsTable["AngularDamping"], m_physicsInfo.m_angularDamping))
			{
				return (false);
			}
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dActorParams::SetMaterial(const Material &material)
	{
		m_material = material;
		// Don't forget to set the actors color to the same value as its diffuse color!
		VSetColor(material.GetDiffuse());
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	boost::shared_ptr<IActor> Pool3dActorParams::VCreate()
	{
		shared_ptr<IActor> actorPtr;

		// Create a copy of this Parameter object using the default copy constructor.
		shared_ptr<Pool3dActorParams> paramsPtr(GCC_NEW Pool3dActorParams(*this));
		if(!paramsPtr)
		{
			return (shared_ptr<IActor>());
		}

		// Create the sphere actor.
		Matrix4 mat;
		Point3 pos(ActorParams::VGetPos());
		GameHalloran::BuildTranslationMatrix4(mat, pos.GetX(), pos.GetY(), pos.GetZ());
		actorPtr.reset(GCC_NEW BaseActor(mat, ActorParams::VGetType(), paramsPtr));
		if(!actorPtr)
		{
			return (shared_ptr<IActor>());
		}

		// Set the ID if the actor has an ID.
		if(VGetId().is_initialized())
		{
			actorPtr->VSetID(*(VGetId()));
		}

		return (actorPtr);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	boost::shared_ptr<ISceneNode> Pool3dActorParams::VCreateSceneNode()
	{
		// Initial matrix of the ball.
		Matrix4 mat;
		Point3 pos(ActorParams::VGetPos());
		GameHalloran::BuildTranslationMatrix4(mat, pos.GetX(), pos.GetY(), pos.GetZ());

		ObjModelFileLoader objLoader;						// 3D mesh loading object.
		ModelLoadingProgress loadProgressObj(ePoolCue);		// Proress update interface.
		BoundingCube bb;									// BoundingBox for mesh.

		boost::shared_ptr<GLTriangleBatch> batchPtr = GameHalloran::LoadBatchFromResourceCache(std::string(m_meshName), &objLoader, loadProgressObj, bb);

		// Create the appropriate scene node for the actor.
		boost::shared_ptr<CommonBatchSceneNode> generalNode(GCC_NEW CommonBatchSceneNode(NULL, VGetId(),\
																		std::string(""),\
																		RenderPassStatic,\
																		GetMaterial(),\
																		mat,\
																		std::string(GetTextureName()),\
																		std::string(GetShaderName()),\
																		batchPtr));

		if(!generalNode)
		{
			// error
			return (boost::shared_ptr<ISceneNode>());
		}

		return (generalNode);
	}

	// /////////////////////////////////////////////////////////////////
	// *********************** BallActorParams *************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	BallActorParams::BallActorParams(const std::string &textureName, const std::string &shaderName, const Material &material, const PhysicsActorInfo &physicsInfo, const F32 radius, const I32 slices, const I32 stacks, const BallGroup group)\
		: Pool3dActorParams(textureName, shaderName, std::string(""), material, physicsInfo), m_radius(radius), m_slices(slices), m_stacks(stacks), m_group(group)
	{
		// Set the size of the parameter.
		ActorParams::VSetSize(sizeof(BallActorParams));
		// Set the type of actor
		ActorParams::VSetType(AT_Ball);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	BallActorParams::~BallActorParams()
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool BallActorParams::VInit(std::istringstream &in)
	{
		if (Pool3dActorParams::VInit(in))
		{
			VSetType(AT_Ball);
			in >> m_radius;
			in >> m_slices;
			in >> m_stacks;
			I32 tmp;
			in >> tmp;
			m_group = static_cast<BallGroup>(tmp);
			return (true);
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BallActorParams::VSerialize(std::ostringstream &out) const
	{
		Pool3dActorParams::VSerialize(out);
		out << m_radius << " ";
		out << m_slices << " ";
		out << m_stacks << " ";
		out << static_cast<I32>(m_group) << " ";
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool BallActorParams::VInit(LuaPlus::LuaObject &srcData, TErrorMessageList &errorMessages)
	{
		if (!Pool3dActorParams::VInit(srcData, errorMessages))
		{
			return (false);
		}

		VSetType(AT_Ball);

		LuaPlus::LuaObject rObj = srcData["Radius"];
		if(rObj.IsNumber())
		{
			m_radius = static_cast<F32>(rObj.GetNumber());
		}
		LuaPlus::LuaObject slObj = srcData["Slices"];
		if(slObj.IsInteger())
		{
			m_slices = slObj.GetInteger();
		}
		LuaPlus::LuaObject stObj = srcData["Stacks"];
		if(stObj.IsInteger())
		{
			m_stacks = stObj.GetInteger();
		}
		LuaPlus::LuaObject groupObj = srcData["Group"];
		if(groupObj.IsString())
		{
			const char *groupStr = groupObj.GetString();
			if(strcmp(groupStr, "cue") == 0)
			{
				m_group = eCue;
			}
			else if(strcmp(groupStr, "stripes") == 0)
			{
				m_group = eStripes;
			}
			else if(strcmp(groupStr, "plain") == 0)
			{
				m_group = ePlain;
			}
			else if(strcmp(groupStr, "black") == 0)
			{
				m_group = eBlack;
			}
			else
			{
				m_group = eCue;
			}
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	boost::shared_ptr<IActor> BallActorParams::VCreate()
	{
		shared_ptr<IActor> ballActorPtr;

		// Create a copy of this Parameter object using the default copy constructor.
		shared_ptr<BallActorParams> paramsPtr(GCC_NEW BallActorParams(*this));
		if(!paramsPtr)
		{
			return (shared_ptr<IActor>());
		}

		// Create the sphere actor.
		Matrix4 mat;
		Point3 pos(ActorParams::VGetPos());
		GameHalloran::BuildTranslationMatrix4(mat, pos.GetX(), pos.GetY(), pos.GetZ());
		ballActorPtr.reset(GCC_NEW BaseActor(mat, ActorParams::VGetType(), paramsPtr));
		if(!ballActorPtr)
		{
			return (shared_ptr<IActor>());
		}

		// Set the ID if the actor has an ID.
		if(VGetId().is_initialized())
		{
			ballActorPtr->VSetID(*(VGetId()));
		}

		// Add an appropriate actor to the physics engine in the logic layer for the scene.
		if(g_appPtr->GetLogicPtr()->VGetGamePhysics())
		{
			PhysicsObjectAttributes physicsAtt;
			PhysicsActorInfo actorPhyInfo = GetPhysicsInfo();
			physicsAtt.m_restitution = actorPhyInfo.m_restitution;
			physicsAtt.m_friction = actorPhyInfo.m_friction;
			physicsAtt.m_linearDamping = actorPhyInfo.m_linearDamping;
			physicsAtt.m_angularDamping = actorPhyInfo.m_angularDamping;
			physicsAtt.m_actorId = VGetId();
			physicsAtt.m_bodyType = eRigidBody;
			physicsAtt.m_id = 0;
			// Balls are dynamic objects so we add them to the default worlds filter.
			physicsAtt.m_collisionGroup = I32(btBroadphaseProxy::DefaultFilter);
			// Balls (in general) are affected by all other physics objects
			physicsAtt.m_collisionMask = I32(btBroadphaseProxy::DefaultFilter | btBroadphaseProxy::StaticFilter |\
				btBroadphaseProxy::KinematicFilter | btBroadphaseProxy::DebrisFilter | btBroadphaseProxy::SensorTrigger |\
					btBroadphaseProxy::CharacterFilter);
			physicsAtt.m_actorId = VGetId();
			physicsAtt.m_objMatrix = ballActorPtr->VGetMat();

			// The cue ball is special, it is also affected by the Pool3d cue group.
			if(m_group == eCue)
			{
				physicsAtt.m_collisionMask |= PLAYER_POOL_PHYSICS_GROUP;
			}

			g_appPtr->GetLogicPtr()->VGetGamePhysics()->VAddSphere(m_radius, actorPhyInfo.m_density, physicsAtt);
		}

		return (ballActorPtr);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	boost::shared_ptr<ISceneNode> BallActorParams::VCreateSceneNode()
	{
		// Initial matrix of the ball.
		Matrix4 mat;
		Point3 pos(ActorParams::VGetPos());
		GameHalloran::BuildTranslationMatrix4(mat, pos.GetX(), pos.GetY(), pos.GetZ());

		// Create the appropriate scene node for the ball.
		boost::shared_ptr<BallSceneNode> ballNode(GCC_NEW BallSceneNode(NULL, VGetId(),\
																		GetMaterial(),\
																		mat,\
																		std::string(GetTextureName()),\
																		std::string(GetShaderName()),\
																		*this));

		if(!ballNode)
		{
			// error
			return (boost::shared_ptr<ISceneNode>());
		}

		return (ballNode);
	}

	// /////////////////////////////////////////////////////////////////
	// ********************** TableActorParams *************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	TableActorParams::TableActorParams(const std::string &textureName,\
										const std::string &shaderName,\
										const std::string &meshName,\
										const Material &material,\
										const PhysicsActorInfo &physicsInfo,\
										const std::string &frontPanelMeshName,\
										const std::string &sidePanelMeshName,\
										const std::string &panelTextureName,\
										const Material &panelMaterial,\
										const std::string &middlePocketMeshName,\
										const std::string &cornerPocketMeshName,\
										const Material &pocketMaterial,
										const std::string &pocketTextureName)\
										: Pool3dActorParams(textureName, shaderName, meshName, material, physicsInfo),\
											m_panelMaterial(panelMaterial), m_pocketsMaterial(pocketMaterial), m_width(0.0f), m_height(0.0f), m_depth(0.0f), m_pocketRadius(0.0f),\
												m_tlpTriggerPos(), m_tlPocketId(0), m_trpTriggerPos(), m_trPocketId(1), m_blpTriggerPos(), m_blPocketId(2), m_brpTriggerPos(), m_brPocketId(3),\
													m_mlpTriggerPos(), m_mlPocketId(4), m_mrpTriggerPos(), m_mrPocketId(5)
	{
		ActorParams::VSetSize(sizeof(TableActorParams));
		ActorParams::VSetType(AT_Table);

		SetStringMember(m_frontPanelMeshName, frontPanelMeshName);
		SetStringMember(m_sidePanelMeshName, sidePanelMeshName);
		SetStringMember(m_panelTextureName, panelTextureName);
		SetStringMember(m_middlePocketMeshName, middlePocketMeshName);
		SetStringMember(m_cornerPocketMeshName, cornerPocketMeshName);
		SetStringMember(m_pocketTextureName, pocketTextureName);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	TableActorParams::~TableActorParams()
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool TableActorParams::VInit(std::istringstream &in)
	{
		if (Pool3dActorParams::VInit(in))
		{
			VSetType(AT_Table);

			// Table panels.
			in >> m_frontPanelMeshName;
			in >> m_sidePanelMeshName;
			in >> m_panelTextureName;
			F32 r, g, b, a;
			in >> r;
			in >> g;
			in >> b;
			in >> a;
			m_panelMaterial.SetAmbient(GameColor(r, g, b, a));
			in >> r;
			in >> g;
			in >> b;
			in >> a;
			m_panelMaterial.SetDiffuse(GameColor(r, g, b, a));
			in >> r;
			in >> g;
			in >> b;
			in >> a;
			m_panelMaterial.SetSpecular(GameColor(r, g, b, a));
			in >> r;
			in >> g;
			in >> b;
			in >> a;
			m_panelMaterial.SetEmissive(GameColor(r, g, b, a));
			in >> r;
			m_panelMaterial.SetSpecularPower(r);

			// Table pockets.
			in >> m_middlePocketMeshName;
			in >> m_cornerPocketMeshName;
			in >> r;
			in >> g;
			in >> b;
			in >> a;
			m_pocketsMaterial.SetAmbient(GameColor(r, g, b, a));
			in >> r;
			in >> g;
			in >> b;
			in >> a;
			m_pocketsMaterial.SetDiffuse(GameColor(r, g, b, a));
			in >> r;
			in >> g;
			in >> b;
			in >> a;
			m_pocketsMaterial.SetSpecular(GameColor(r, g, b, a));
			in >> r;
			in >> g;
			in >> b;
			in >> a;
			m_pocketsMaterial.SetEmissive(GameColor(r, g, b, a));
			in >> r;
			m_pocketsMaterial.SetSpecularPower(r);
			in >> m_pocketTextureName;

			in >> m_width;
			in >> m_height;
			in >> m_depth;
			in >> m_pocketRadius;
			
			F32 x, y, z;
			in >> x;
			in >> y;
			in >> z;
			m_tlpTriggerPos.Set(x, y, z);
			in >> m_tlPocketId;
			in >> x;
			in >> y;
			in >> z;
			m_trpTriggerPos.Set(x, y, z);
			in >> m_trPocketId;
			in >> x;
			in >> y;
			in >> z;
			m_blpTriggerPos.Set(x, y, z);
			in >> m_blPocketId;
			in >> x;
			in >> y;
			in >> z;
			m_brpTriggerPos.Set(x, y, z);
			in >> m_brPocketId;
			in >> x;
			in >> y;
			in >> z;
			m_mlpTriggerPos.Set(x, y, z);
			in >> m_mlPocketId;
			in >> x;
			in >> y;
			in >> z;
			m_mrpTriggerPos.Set(x, y, z);
			in >> m_mrPocketId;

			return (true);
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void TableActorParams::VSerialize(std::ostringstream &out) const
	{
		Pool3dActorParams::VSerialize(out);

		// Table Panels.
		out << m_frontPanelMeshName << " ";
		out << m_sidePanelMeshName << " ";
		out << m_panelTextureName << " ";
		GameColor currColor(m_panelMaterial.GetAmbient());
		out << currColor.GetX() << " ";
		out << currColor.GetY() << " ";
		out << currColor.GetZ() << " ";
		out << currColor.GetW() << " ";
		currColor = m_panelMaterial.GetDiffuse();
		out << currColor.GetX() << " ";
		out << currColor.GetY() << " ";
		out << currColor.GetZ() << " ";
		out << currColor.GetW() << " ";
		currColor = m_panelMaterial.GetSpecular();
		out << currColor.GetX() << " ";
		out << currColor.GetY() << " ";
		out << currColor.GetZ() << " ";
		out << currColor.GetW() << " ";
		currColor = m_panelMaterial.GetEmissive();
		out << currColor.GetX() << " ";
		out << currColor.GetY() << " ";
		out << currColor.GetZ() << " ";
		out << currColor.GetW() << " ";
		out << m_panelMaterial.GetSpecularPower() << " ";

		// Table pockets.
		out << m_middlePocketMeshName << " ";
		out << m_cornerPocketMeshName << " ";
		currColor = m_pocketsMaterial.GetAmbient();
		out << currColor.GetX() << " ";
		out << currColor.GetY() << " ";
		out << currColor.GetZ() << " ";
		out << currColor.GetW() << " ";
		currColor = m_pocketsMaterial.GetDiffuse();
		out << currColor.GetX() << " ";
		out << currColor.GetY() << " ";
		out << currColor.GetZ() << " ";
		out << currColor.GetW() << " ";
		currColor = m_pocketsMaterial.GetSpecular();
		out << currColor.GetX() << " ";
		out << currColor.GetY() << " ";
		out << currColor.GetZ() << " ";
		out << currColor.GetW() << " ";
		currColor = m_pocketsMaterial.GetEmissive();
		out << currColor.GetX() << " ";
		out << currColor.GetY() << " ";
		out << currColor.GetZ() << " ";
		out << currColor.GetW() << " ";
		out << m_pocketsMaterial.GetSpecularPower() << " ";
		out << m_pocketTextureName << " ";

		out << m_width << " ";
		out << m_height << " ";
		out << m_depth << " ";
		out << m_pocketRadius << " ";

		out << m_tlpTriggerPos.GetX() << " " << m_tlpTriggerPos.GetY() << " " << m_tlpTriggerPos.GetZ() << " ";
		out << m_tlPocketId << " ";
		out << m_trpTriggerPos.GetX() << " " << m_trpTriggerPos.GetY() << " " << m_trpTriggerPos.GetZ() << " ";
		out << m_trPocketId << " ";
		out << m_blpTriggerPos.GetX() << " " << m_blpTriggerPos.GetY() << " " << m_blpTriggerPos.GetZ() << " ";
		out << m_blPocketId << " ";
		out << m_brpTriggerPos.GetX() << " " << m_brpTriggerPos.GetY() << " " << m_brpTriggerPos.GetZ() << " ";
		out << m_brPocketId << " ";
		out << m_mlpTriggerPos.GetX() << " " << m_mlpTriggerPos.GetY() << " " << m_mlpTriggerPos.GetZ() << " ";
		out << m_mlPocketId << " ";
		out << m_mrpTriggerPos.GetX() << " " << m_mrpTriggerPos.GetY() << " " << m_mrpTriggerPos.GetZ() << " ";
		out << m_mrPocketId << " ";
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool TableActorParams::VInit(LuaPlus::LuaObject &srcData, TErrorMessageList &errorMessages)
	{
		if (!Pool3dActorParams::VInit(srcData, errorMessages))
		{
			return (false);
		}

		VSetType(AT_Table);

		// Table Panels data.
		LuaPlus::LuaObject obj = srcData["FrontPanelMeshName"];
		if(obj.IsString())
		{
			SetFrontPanelMeshName(obj.GetString());
		}
		obj = srcData["SidePanelMeshName"];
		if(obj.IsString())
		{
			SetSidePanelMeshName(obj.GetString());
		}
		obj = srcData["PanelTextureName"];
		if(obj.IsString())
		{
			SetPanelTextureName(obj.GetString());
		}
		obj = srcData["PanelMaterial"];
		if(obj.IsTable())
		{
			GameColor tmp;
			if(SetColorFromLua(obj["Ambient"], tmp))
			{
				m_panelMaterial.SetAmbient(tmp);
			}
			if(SetColorFromLua(obj["Diffuse"], tmp))
			{
				m_panelMaterial.SetDiffuse(tmp);
			}
			if(SetColorFromLua(obj["Specular"], tmp))
			{
				m_panelMaterial.SetSpecular(tmp);
			}
			if(SetColorFromLua(obj["Emissive"], tmp))
			{
				m_panelMaterial.SetEmissive(tmp);
			}
			F32 tmp2;
			if(SetFloatFromLua(obj["Shininess"], tmp2))
			{
				m_panelMaterial.SetSpecularPower(tmp2);
			}
		}

		// Table pockets data.
		obj = srcData["MiddlePocketMeshName"];
		if(obj.IsString())
		{
			SetMiddlePocketMeshName(obj.GetString());
		}
		obj = srcData["CornerPocketMeshName"];
		if(obj.IsString())
		{
			SetCornerPocketMeshName(obj.GetString());
		}
		obj = srcData["PocketMaterial"];
		if(obj.IsTable())
		{
			GameColor tmp;
			if(SetColorFromLua(obj["Ambient"], tmp))
			{
				m_pocketsMaterial.SetAmbient(tmp);
			}
			if(SetColorFromLua(obj["Diffuse"], tmp))
			{
				m_pocketsMaterial.SetDiffuse(tmp);
			}
			if(SetColorFromLua(obj["Specular"], tmp))
			{
				m_pocketsMaterial.SetSpecular(tmp);
			}
			if(SetColorFromLua(obj["Emissive"], tmp))
			{
				m_pocketsMaterial.SetEmissive(tmp);
			}
			F32 tmp2;
			if(SetFloatFromLua(obj["Shininess"], tmp2))
			{
				m_pocketsMaterial.SetSpecularPower(tmp2);
			}
		}
		obj = srcData["PocketTexture"];
		if(obj.IsString())
		{
			SetPocketTextureName(obj.GetString());
		}

		obj = srcData["Width"];
		if(obj.IsNumber())
		{
			SetFloatFromLua(obj, m_width);
		}
		obj = srcData["Height"];
		if(obj.IsNumber())
		{
			SetFloatFromLua(obj, m_height);
		}
		obj = srcData["Depth"];
		if(obj.IsNumber())
		{
			SetFloatFromLua(obj, m_depth);
		}
		obj = srcData["PocketRadius"];
		if(obj.IsNumber())
		{
			SetFloatFromLua(obj, m_pocketRadius);
		}

		obj = srcData["TopLeftPocketTriggerPos"];
		if(obj.IsTable())
		{
			SetPoint3FromLua(obj, m_tlpTriggerPos);
		}
		obj = srcData["TopLeftPocketTriggerId"];
		if(obj.IsInteger())
		{
			SetIntFromLua(obj, m_tlPocketId);
		}
		obj = srcData["TopRightPocketTriggerPos"];
		if(obj.IsTable())
		{
			SetPoint3FromLua(obj, m_trpTriggerPos);
		}
		obj = srcData["TopRightPocketTriggerId"];
		if(obj.IsInteger())
		{
			SetIntFromLua(obj, m_trPocketId);
		}
		obj = srcData["BottomLeftPocketTriggerPos"];
		if(obj.IsTable())
		{
			SetPoint3FromLua(obj, m_blpTriggerPos);
		}
		obj = srcData["BottomLeftPocketTriggerId"];
		if(obj.IsInteger())
		{
			SetIntFromLua(obj, m_blPocketId);
		}
		obj = srcData["BottomRightPocketTriggerPos"];
		if(obj.IsTable())
		{
			SetPoint3FromLua(obj, m_brpTriggerPos);
		}
		obj = srcData["BottomRightPocketTriggerId"];
		if(obj.IsInteger())
		{
			SetIntFromLua(obj, m_brPocketId);
		}
		obj = srcData["MiddleLeftPocketTriggerPos"];
		if(obj.IsTable())
		{
			SetPoint3FromLua(obj, m_mlpTriggerPos);
		}
		obj = srcData["MiddleLeftPocketTriggerId"];
		if(obj.IsInteger())
		{
			SetIntFromLua(obj, m_mlPocketId);
		}
		obj = srcData["MiddleRightPocketTriggerPos"];
		if(obj.IsTable())
		{
			SetPoint3FromLua(obj, m_mrpTriggerPos);
		}
		obj = srcData["MiddleRightPocketTriggerId"];
		if(obj.IsInteger())
		{
			SetIntFromLua(obj, m_mrPocketId);
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	boost::shared_ptr<IActor> TableActorParams::VCreate()
	{
		shared_ptr<IActor> tableActorPtr;

		// Create a copy of this Parameter object using the default copy constructor.
		shared_ptr<TableActorParams> paramsPtr(GCC_NEW TableActorParams(*this));
		if(!paramsPtr)
		{
			return (shared_ptr<IActor>());
		}

		// Create the sphere actor.
		Matrix4 mat;
		Point3 pos(ActorParams::VGetPos());
		GameHalloran::BuildTranslationMatrix4(mat, pos.GetX(), pos.GetY(), pos.GetZ());
		tableActorPtr.reset(GCC_NEW BaseActor(mat, ActorParams::VGetType(), paramsPtr));
		if(!tableActorPtr)
		{
			return (shared_ptr<IActor>());
		}

		// Set the ID if the actor has an ID.
		if(VGetId().is_initialized())
		{
			tableActorPtr->VSetID(*(VGetId()));
		}

		// Add an appropriate actor to the physics engine in the logic layer for the scene.
		if(tableActorPtr && g_appPtr->GetLogicPtr()->VGetGamePhysics())
		{
			TableActorParams *actorCopyParamsPtr = dynamic_cast<TableActorParams *>(tableActorPtr->VGetParams().get());
			boost::shared_ptr<BulletPhysics> bp = boost::static_pointer_cast<BulletPhysics>(g_appPtr->GetLogicPtr()->VGetGamePhysics());
			boost::shared_ptr<Pool3dLogic> pgl = boost::static_pointer_cast<Pool3dLogic>(g_appPtr->GetLogicPtr());

			if(bp && actorCopyParamsPtr)
			{
				PhysicsObjectAttributes physicsAtt;
				PhysicsActorInfo actorPhyInfo = GetPhysicsInfo();
				physicsAtt.m_friction = actorPhyInfo.m_friction;
				physicsAtt.m_restitution = actorPhyInfo.m_restitution;
				physicsAtt.m_linearDamping = actorPhyInfo.m_linearDamping;
				physicsAtt.m_angularDamping = actorPhyInfo.m_angularDamping;
				physicsAtt.m_actorId = VGetId();
				physicsAtt.m_bodyType = eRigidBody;
				physicsAtt.m_id = 0;

				// Table is a static physics object with 0 mass so we set these attributes here.
				physicsAtt.m_mass = 0.0f;
				physicsAtt.m_objectType = eStatic;
				physicsAtt.m_collisionGroup = I32(btBroadphaseProxy::StaticFilter);
				physicsAtt.m_collisionMask = I32(btBroadphaseProxy::AllFilter ^ btBroadphaseProxy::StaticFilter);
				physicsAtt.m_actorId = VGetId();
				physicsAtt.m_objMatrix = tableActorPtr->VGetMat();

				// unused const F32 hh = m_height / 2.0f;
				const F32 hw = m_width / 2.0f;
				const F32 hd = m_depth / 2.0f;
				// unused const F32 prh = m_pocketRadius / 2.0f;
				const F32 pc = m_pocketRadius * 2.0f;
				const F32 min = 0.2f;
				const F32 padding = 0.02f;

				// Note: Child collision shapes new'd here are deleted in ~Pool3dLogic().

				// x = (m_width - (2.0f * (pc))) / 2.0f, y = min, z = hd
				btCollisionShape *m_floorMiddleShapePtr = new btBoxShape(btVector3((m_width - (pc) + (m_width / 20.0f)) / 2.0f, min*2.0f, hd));
				pgl->m_collisionShapeLookupTable[pgl->PGL_POOL_TABLE_FLOOR_MIDDLE_COLLISION_SHAPE] = m_floorMiddleShapePtr;

				// x = hw, y = hh, z = min
				btCollisionShape *m_frontWallShapePtr = new btBoxShape(btVector3(hw - pc + padding, m_height, min));
				pgl->m_collisionShapeLookupTable[pgl->PGL_POOL_TABLE_FRONT_COLLISION_SHAPE] = m_frontWallShapePtr;

				// x = min, y = hh, z = hd
				btCollisionShape *m_sideWallShapePtr = new btBoxShape(btVector3(min, m_height, (hd - pc - m_pocketRadius) / 2.0f));
				pgl->m_collisionShapeLookupTable[pgl->PGL_POOL_TABLE_SIDE_COLLISION_SHAPE] = m_sideWallShapePtr;

				btCollisionShape *m_sideBoundaryShapePtr = new btBoxShape(btVector3(min, m_height, hd + (hd / 2.0f)));
				pgl->m_collisionShapeLookupTable[pgl->PGL_POOL_TABLE_SIDE_BOUNDARY_COLLISION_SHAPE] = m_sideBoundaryShapePtr;

				btCollisionShape *m_frontBoundaryShapePtr = new btBoxShape(btVector3(hw + (hw / 2.0f), m_height, min));
				pgl->m_collisionShapeLookupTable[pgl->PGL_POOL_TABLE_FRONT_BOUNDARY_COLLISION_SHAPE] = m_frontBoundaryShapePtr;

				// Note: btCompoundShape new'd here is deleted in ~BulletPhysics() or BulletPhysics::VRemoveActor().
				btCompoundShape* tableShapePtr = new btCompoundShape();

				btTransform	startTransform;
				startTransform.setIdentity();

				// Add floor
				startTransform.setOrigin(btVector3(0.0f, -(m_height/2.0f) - CONVEX_DISTANCE_MARGIN - min - (min / 2.0f) - 0.01f, 0.0f));
				//startTransform.setOrigin(btVector3(0.0f, -CONVEX_DISTANCE_MARGIN - min - (min / 2.0f) - 0.01f, 0.0f));
				tableShapePtr->addChildShape(startTransform, m_floorMiddleShapePtr);

				// Add the 4 side floor panels ...

				// Add Front and Back boundaries.
				startTransform.setOrigin(btVector3(0.0f, 0.0f, hd + m_pocketRadius - padding + (min / 2.0f)));
				tableShapePtr->addChildShape(startTransform, m_frontWallShapePtr);

				//startTransform.setOrigin(btVector3(0.0f, -hh, hd + pc + (min / 2.0f)));
				startTransform.setOrigin(btVector3(0.0f, 0.0f, hd + pc + (min / 2.0f)));
				tableShapePtr->addChildShape(startTransform, m_frontBoundaryShapePtr);

				//startTransform.setOrigin(btVector3(0.0f , -hh, -hd - m_pocketRadius + padding - (min / 2.0f)));
				startTransform.setOrigin(btVector3(0.0f , 0.0f, -hd - m_pocketRadius + padding - (min / 2.0f)));
				tableShapePtr->addChildShape(startTransform, m_frontWallShapePtr);

				//startTransform.setOrigin(btVector3(0.0f, -hh, -hd - pc - (min / 2.0f)));
				startTransform.setOrigin(btVector3(0.0f, 0.0f, -hd - pc - (min / 2.0f)));
				tableShapePtr->addChildShape(startTransform, m_frontBoundaryShapePtr);

				// Add Left boundaries.
				// top left side
				startTransform.setOrigin(btVector3(-hw - m_pocketRadius + CONVEX_DISTANCE_MARGIN - (min / 2.0f), 0.0f, (-hd / 2.0f) + m_pocketRadius - CONVEX_DISTANCE_MARGIN));
				tableShapePtr->addChildShape(startTransform, m_sideWallShapePtr);

				// bottom left side
				startTransform.setOrigin(btVector3(-hw - m_pocketRadius + CONVEX_DISTANCE_MARGIN - (min / 2.0f), 0.0f, (hd / 2.0f) - m_pocketRadius + CONVEX_DISTANCE_MARGIN + padding));
				tableShapePtr->addChildShape(startTransform, m_sideWallShapePtr);

				// middle left side outside pockets.
				startTransform.setOrigin(btVector3(-hw - pc - (min / 2.0f), 0.0f, 0.0f));
				tableShapePtr->addChildShape(startTransform, m_sideBoundaryShapePtr);

				// Add right boundaries.
				// top right side
				startTransform.setOrigin(btVector3(hw + m_pocketRadius - CONVEX_DISTANCE_MARGIN + (min / 2.0f), 0.0f, (-hd / 2.0f) + m_pocketRadius - CONVEX_DISTANCE_MARGIN));
				tableShapePtr->addChildShape(startTransform, m_sideWallShapePtr);

				// bottom right side.
				//startTransform.setOrigin(btVector3(hw + m_pocketRadius - CONVEX_DISTANCE_MARGIN + (min / 2.0f), -hh, (hd / 2.0f) - m_pocketRadius + CONVEX_DISTANCE_MARGIN + padding));
				startTransform.setOrigin(btVector3(hw + m_pocketRadius - CONVEX_DISTANCE_MARGIN + (min / 2.0f), 0.0f, (hd / 2.0f) - m_pocketRadius + CONVEX_DISTANCE_MARGIN + padding));
				tableShapePtr->addChildShape(startTransform, m_sideWallShapePtr);

				// middle right side outside pockets.
				startTransform.setOrigin(btVector3(hw + pc + (min / 2.0f), 0.0f, 0.0f));
				tableShapePtr->addChildShape(startTransform, m_sideBoundaryShapePtr);

				bp->AddGameActorRigidBody(tableShapePtr, physicsAtt);

				// Create the trigger areas for the 6 pockets so the game logic may be informed when a ball drops into a pocket
				// Top left and right
				m_tlpTriggerPos.Set(-hw + padding, -m_height * 2.0f, -hd + padding);
				actorCopyParamsPtr->m_tlpTriggerPos = m_tlpTriggerPos;
				bp->VCreateTrigger(m_tlpTriggerPos, m_pocketRadius * 2.0f, m_tlPocketId);
				m_trpTriggerPos.Set(hw - padding, -m_height * 2.0f, -hd + padding);
				actorCopyParamsPtr->m_trpTriggerPos = m_trpTriggerPos;
				bp->VCreateTrigger(m_trpTriggerPos, m_pocketRadius * 2.0f, m_trPocketId);

				// Bottom left and right
				m_blpTriggerPos.Set(-hw + padding, -m_height * 2.0f, hd - padding);
				actorCopyParamsPtr->m_blpTriggerPos = m_blpTriggerPos;
				bp->VCreateTrigger(m_blpTriggerPos, m_pocketRadius * 2.0f, m_blPocketId);
				m_brpTriggerPos.Set(hw - padding, -m_height * 2.0f, hd - padding);
				actorCopyParamsPtr->m_brpTriggerPos = m_brpTriggerPos;
				bp->VCreateTrigger(m_brpTriggerPos, m_pocketRadius * 2.0f, m_brPocketId);

				// Middle left and right
				m_mlpTriggerPos.Set(-hw + padding, -m_height * 2.0f, 0.0f);
				actorCopyParamsPtr->m_mlpTriggerPos = m_mlpTriggerPos;
				bp->VCreateTrigger(m_mlpTriggerPos, m_pocketRadius * 2.0f, m_mlPocketId);
				m_mrpTriggerPos.Set(hw - padding, -m_height * 2.0f, 0.0f);
				actorCopyParamsPtr->m_mrpTriggerPos = m_mrpTriggerPos;
				bp->VCreateTrigger(m_mrpTriggerPos, m_pocketRadius * 2.0f, m_mrPocketId);
			}
		}

		return (tableActorPtr);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	boost::shared_ptr<ISceneNode> TableActorParams::VCreateSceneNode()
	{
		// Initial matrix of the object.
		Matrix4 mat;
		Point3 pos(ActorParams::VGetPos());
		GameHalloran::BuildTranslationMatrix4(mat, pos.GetX(), pos.GetY(), pos.GetZ());

		// Create the appropriate scene node for the object.
		boost::shared_ptr<TableSceneNode> tableNode(GCC_NEW TableSceneNode(NULL, VGetId(),\
																			GetMaterial(),\
																			mat,\
																			std::string(GetTextureName()),\
																			std::string(GetShaderName()),\
																			*this));
		if(!tableNode)
		{
			// error
			return (boost::shared_ptr<ISceneNode>());
		}

		return (tableNode);
	}

	// /////////////////////////////////////////////////////////////////
	// *********************** CueActorParams **************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	CueActorParams::CueActorParams(const std::string &textureName,\
									const std::string &shaderName,\
									const std::string &meshName,\
									const Material &material,\
									const PhysicsActorInfo &physicsInfo) : Pool3dActorParams(textureName, shaderName, meshName, material, physicsInfo)
	{
		ActorParams::VSetSize(sizeof(CueActorParams));
		ActorParams::VSetType(AT_Cue);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	CueActorParams::~CueActorParams()
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CueActorParams::VInit(std::istringstream &in)
	{
		if(Pool3dActorParams::VInit(in))
		{
			VSetType(AT_Cue);
			in >> m_width;
			in >> m_height;
			in >> m_depth;
			return (true);
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CueActorParams::VSerialize(std::ostringstream &out) const
	{
		Pool3dActorParams::VSerialize(out);
		out << m_width << " ";
		out << m_height << " ";
		out << m_depth << " ";
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool CueActorParams::VInit(LuaPlus::LuaObject &srcData, TErrorMessageList &errorMessages)
	{
		if (!Pool3dActorParams::VInit(srcData, errorMessages))
		{
			return (false);
		}

		VSetType(AT_Cue);

		if(srcData["Width"].IsNumber())
		{
			SetFloatFromLua(srcData["Width"], m_width);
		}
		if(srcData["Height"].IsNumber())
		{
			SetFloatFromLua(srcData["Height"], m_height);
		}
		if(srcData["Depth"].IsNumber())
		{
			SetFloatFromLua(srcData["Depth"], m_depth);
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	boost::shared_ptr<IActor> CueActorParams::VCreate()
	{
		shared_ptr<IActor> cueActorPtr;

		// Create a copy of this Parameter object using the default copy constructor.
		shared_ptr<CueActorParams> paramsPtr(GCC_NEW CueActorParams(*this));
		if(!paramsPtr)
		{
			return (shared_ptr<IActor>());
		}

		// Create the sphere actor.
		Matrix4 mat;
		Point3 pos(ActorParams::VGetPos());
		GameHalloran::BuildTranslationMatrix4(mat, pos.GetX(), pos.GetY(), pos.GetZ());
		cueActorPtr.reset(GCC_NEW BaseActor(mat, ActorParams::VGetType(), paramsPtr));
		if(!cueActorPtr)
		{
			return (shared_ptr<IActor>());
		}

		// Set the ID if the actor has an ID.
		if(VGetId().is_initialized())
		{
			cueActorPtr->VSetID(*(VGetId()));
		}

		if(g_appPtr->GetLogicPtr()->VGetGamePhysics())
		{
			PhysicsActorInfo actorPhyInfo = GetPhysicsInfo();
			PhysicsObjectAttributes physicsAtt;

			physicsAtt.m_actorId = VGetId();
			physicsAtt.m_id = 0;
			physicsAtt.m_angularDamping = actorPhyInfo.m_angularDamping;
			physicsAtt.m_bodyType = eRigidBody;
			physicsAtt.m_friction = actorPhyInfo.m_friction;
			physicsAtt.m_linearDamping = actorPhyInfo.m_linearDamping;
			physicsAtt.m_mass = 0.0f;
			physicsAtt.m_objectType = eKinematic;
			physicsAtt.m_restitution = actorPhyInfo.m_restitution;
			// The cue is in the player control filter group only, it only affects the cue ball.
			physicsAtt.m_collisionGroup = PLAYER_POOL_PHYSICS_GROUP;
			physicsAtt.m_collisionMask = I32(btBroadphaseProxy::AllFilter ^ btBroadphaseProxy::StaticFilter);
			physicsAtt.m_actorId = VGetId();
			physicsAtt.m_objMatrix = cueActorPtr->VGetMat();

			g_appPtr->GetLogicPtr()->VGetGamePhysics()->VAddCylinder(Vector3(m_width / 2.0f, m_height / 2.0f, m_depth / 2.0f), 0.0f, physicsAtt);
		}

		return (cueActorPtr);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	boost::shared_ptr<ISceneNode> CueActorParams::VCreateSceneNode()
	{
		// Initial matrix of the object.
		Matrix4 mat;
		Point3 pos(ActorParams::VGetPos());
		GameHalloran::BuildTranslationMatrix4(mat, pos.GetX(), pos.GetY(), pos.GetZ());

		// Create the appropriate scene node for the object.
		boost::shared_ptr<CueSceneNode> cueNode(GCC_NEW CueSceneNode(NULL, VGetId(),\
																		GetMaterial(),\
																		mat,\
																		std::string(GetTextureName()),\
																		std::string(GetShaderName()),\
																		*this));
		if(!cueNode)
		{
			// error
			return (boost::shared_ptr<ISceneNode>());
		}

		return (cueNode);
	}

}
