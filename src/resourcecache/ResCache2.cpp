//========================================================================
// ResCache.cpp : Defines a simple resource cache
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
// The source code is managed and maintained through Google Code: http://gamecode3.googlecode.com/svn/trunk/
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
// @file ResCache.h
// @author Michael L. McShaffry.
// @date 12/07/2010
//
// File contains the headers for the ResourceCache family of classes.  
// This class was extracted from the Game Coding Complete 3 code.
// 
// It contains functionality for loading and caching game resource
// files in memory.  A resource is defined as any file which contains
// game assets such as a png, mp3, 3d model, etc.
//
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.
//
// Please see the associated header for details on what changes I have
// made.
//
// /////////////////////////////////////////////////////////////////

#include "GameBase.h"
#include "ResCache2.h"
#include "GameMain.h"

using std::string;

using boost::filesystem::path;
using boost::optional;
using boost::scoped_ptr;
using boost::shared_ptr;

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Resource::Resource(const string &nameRef) : m_name(nameRef)
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	string Resource::GetName() const
	{
		return (m_name);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ResHandle *Resource::VCreateHandle(const char *buffer, U32 size, ResCache *pResCache)
	{ 
		return GCC_NEW ResHandle(*this, const_cast<char *>(buffer), size, pResCache);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ResourceZipFile::ResourceZipFile(const path &resFileNameRef) : m_pZipFile(), m_resFileName(resFileNameRef)
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ResourceZipFile::~ResourceZipFile() 
	{
		try
		{
		}
		catch(...)
		{
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ResourceZipFile::VOpen()
	{
		m_pZipFile.reset(GCC_NEW ZipFile());
		if(m_pZipFile)
		{
			return (m_pZipFile->Init(m_resFileName));
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	optional<I32> ResourceZipFile::VGetResourceSize(const Resource &r)
	{
		optional<I32> size;
		optional<I32> resourceNum = m_pZipFile->Find(r.GetName().c_str());
		if (resourceNum)
		{
			U64 val(0);
            m_pZipFile->GetFileLen(*resourceNum, val);

			if(m_pZipFile->GetFileLen(*resourceNum, val))
			{
				size = val;
			}
		}

		return (size);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ResourceZipFile::VGetResource(const Resource &r, char *buffer)
	{
		optional<I32> resourceNum = m_pZipFile->Find(r.GetName().c_str());
		if (resourceNum)
		{
			//I32 size = m_pZipFile->GetFileLen(*resourceNum);
			m_pZipFile->ReadFile(*resourceNum, buffer);
		}

		// Return true if the resource was in the ZIP directory and false otherwise.
		return (resourceNum);
	}
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ResourceZipFile::VGetResourceListing(const std::string &regex, ResourceListing &listings) {
        return m_pZipFile->Find(regex, listings);
    }

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ResHandle::ResHandle(Resource & resource, char *buffer, U32 size, ResCache *pResCache)
		: m_resource(resource), m_buffer(buffer), m_size(size), m_pResCache(pResCache)
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ResHandle::~ResHandle()
	{
		try
		{
			// NB. We delete the buffer allocated by the resource cache here (inside ResCache::Allocate()).
			if (m_buffer)
			{
				delete [] m_buffer;
				m_buffer = NULL;
			}

			// We now tell the ResCache manager that the memory has been freed.
			m_pResCache->MemoryHasBeenFreed(m_size, m_resource.GetName());
		}
		catch(...)
		{
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ResHandle::VLoad(IResourceFile *resLoaderPtr)
	{
		if(resLoaderPtr)
		{
			return (resLoaderPtr->VGetResource(m_resource, m_buffer));
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	U32 ResHandle::Size() const
	{
		return (m_size);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	char *ResHandle::Buffer() const
	{
		return (m_buffer);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	string ResHandle::GetResourceName() const
	{
		return (m_resource.GetName());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ResCache::ResCache(const U32 sizeInMb, IResourceFile *resFile, shared_ptr<GameLog> loggerPtr) : m_lru(), m_resources(), m_file(resFile),\
		m_cacheSize(sizeInMb * 1024 * 1024), m_allocated(0), m_loggerPtr(loggerPtr)
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ResCache::~ResCache()
	{
		try
		{
			while (!m_lru.empty())
			{
				FreeOneResource();
			}
			Delete(m_file);
		}
		catch(...)
		{
		}
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool ResCache::Init()
	{
		bool result = true;				// Result of method.

		if(m_file == NULL)
		{
            GF_LOG_ERR("Failed to initialize the resource cache.  The resource file is NULL");
			result = false;
		}
		else
		{
			result = m_file->VOpen();
		}

		if(!result)
		{
            GF_LOG_ERR("Failed to initialize the resource cache.  Failed to open the resource container");
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	shared_ptr<ResHandle> ResCache::GetHandle(Resource *r)
	{
		// Check the input parameter.
		if(r == NULL)
		{
            GF_LOG_ERR("Cannot get the resource as the resource pointer is NULL");
			return (shared_ptr<ResHandle>());
		}

		shared_ptr<ResHandle> handle(Find(r));
		if (handle == NULL)
		{
			// Load the resource into memory.
            GF_LOG_INF(string("The resource ") + r->GetName() + string(" is not currently in the cache so it will be loaded."));
			handle = Load(r);
		}
		else
		{
			// The resource is already in memory so update the LRU list.
            GF_LOG_DEB(string("The resource ") + r->GetName() + string(" is currently in the cache."));
			Update(handle);
		}

		return (handle);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	shared_ptr<ResHandle> ResCache::Load(Resource *r)
	{
		// No need to check for r == NULL as we did this in the public method
		//  GetHandle() already...

		optional<I32> size = m_file->VGetResourceSize(*r);
		if(!size)
		{
            GF_LOG_INF(string("Failed to get the resource size: ") + r->GetName());
			return shared_ptr<ResHandle>();		// Could not find resource in container!
		}
		
		char *buffer = Allocate(*size);
		if (buffer == NULL)
		{
            GF_LOG_INF(string("Failed to allocate cache memory for the resource from the ResCache: ") + r->GetName());
			return shared_ptr<ResHandle>();		// ResCache is out of memory!
		}

		// Create a new resource.
		bool error = false;
		shared_ptr<ResHandle> handle (r->VCreateHandle(buffer, *size, this));
		if(!handle)
		{
            GF_LOG_INF(string("Failed to allocate dynamic memory for the resource handle: ") + r->GetName());
			// Reverse the changes made in Allocate (its not stored in a valid handle yet so we must do this manually).
			DeleteArray(buffer);
			m_allocated -= *size;
			error = true;
		}

		// Load in resource data from the resource container.
		if(!error && !handle->VLoad(m_file))
		{
            GF_LOG_INF(string("Failed to load in the resource from the resource container: ") + r->GetName());
			// Reset the handle as we should not pass back a valid handle in the event that we failed to load the resource from the file.
			handle.reset();
			error = true;
		}

		// Add it to the lru list and map
		if(!error)
		{
			m_lru.push_front(handle);
			m_resources[r->GetName()] = handle;
            GF_LOG_DEB(string("Resource loaded: ") + r->GetName());
		}

		return (handle);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	shared_ptr<ResHandle> ResCache::Find(Resource *r)
	{
		// No need to check for r == NULL as we did this in the public method
		//  GetHandle() already...

		ResHandleMap::iterator i = m_resources.find(r->GetName());
		if (i==m_resources.end())
		{
			return (shared_ptr<ResHandle>());
		}

		return ((*i).second);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ResCache::Update(shared_ptr<ResHandle> handle)
	{
		m_lru.remove(handle);
		m_lru.push_front(handle);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	char *ResCache::Allocate(const U32 size)
	{
		// Ensure there is enough memory for the resource.
		if (!MakeRoom(size))
		{
			return NULL;
		}

		// Allocate the memory.
		char *mem = GCC_NEW char[size];
		if (mem)
		{
			m_allocated += size;
		}

		return (mem);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ResCache::FreeOneResource()
	{
		// Get the least recently used resource handle.
		ResHandleList::iterator gonner = m_lru.end();
		gonner--;
		shared_ptr<ResHandle> handle = *gonner;

		// Remove the least recently used resource from the list and the queue.
        GF_LOG_DEB(string("Freeing the least recently used resource (") + handle->GetResourceName() + string(") from the cache now"));
		m_lru.pop_back();							
		m_resources.erase(handle->GetResourceName());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ResCache::Flush()
	{
        GF_LOG_DEB("Flushing the entire cache now");

		while (!m_lru.empty())
		{
			shared_ptr<ResHandle> handle = *(m_lru.begin());
			Free(handle);
			m_lru.pop_front();
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ResCache::MakeRoom(const U32 size)
	{
		// Check if the size requested is greater than the cache itself.
		if (size > m_cacheSize)
		{
            GF_LOG_ERR("Could not make room for the resource as its bigger than the cache");
			return (false);
		}

		// return null if there's no possible way to allocate the memory
		while (size > (m_cacheSize - m_allocated))
		{
			// The cache is empty, and there's still not enough room.
			if (m_lru.empty())
			{
                GF_LOG_ERR("Could not make room for the resource as we cleared the entire cache but there still is not enough room");
				return (false);
			}

			// Free the least recently used resource.
			FreeOneResource();
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ResCache::Free(shared_ptr<ResHandle> gonner)
	{
		m_lru.remove(gonner);
		m_resources.erase(gonner->GetResourceName());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ResCache::MemoryHasBeenFreed(const U32 size, const string &resourceName)
	{
        GF_LOG_DEB(string("The resource ") + resourceName + string(" is reporting that it has freed itself."));
		m_allocated -= size;
	}

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ResCache::GetResourceListing(const std::string &regex, ResourceListing &listings) {
        return m_file->VGetResourceListing(regex, listings);
    }
}
