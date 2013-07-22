#pragma once
#ifndef __GCC3_RES_CACHE_2_H
#define __GCC3_RES_CACHE_2_H

//========================================================================
// ResCache.h : Defines a simple resource cache.
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
// I have added/modified the following functionality:
// - Added under the GameHalloran namespace.
// - Using boost::optional instead of M. McShaffrys' version.
//
// Resource:
// - Added a GetName() method.
//
// IResourceFile:
// - Changed the definition of VGetResourceSize(Resource) from
//      I32 VGetResourceSize()
//      to
//      optional<I32> VGetResourceSize(Resource)
// - Changed the definition of VGetResourceSize(Resource) from
//      I32 VGetResource(...)
//      to
//      bool VGetResource(...)
// - Function definitions were changed to give clearer return values.
//
// IResourceFile:
// - Same as base class IResourceFile.
//
// ResHandle
// - I removed the friend class ResCache reference.
//
// ResCache
// - I removed the friend class ResHandle reference.
// - I made the MemoryHasBeenFreed() member public so that ResHandles'
//      destructor may call it.
// - I added a NULL check to the input pointer for GetHandle().
// - I made note of a possible bug in Flush() in the comments.
// - Added logging into the ResourceCache to record when cache misses
//      occur and in general when and if errors occur.
//
// /////////////////////////////////////////////////////////////////

#include <string>
#include <list>
#include <map>
#include <vector>

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>

#include "ZipFile.h"
#include "GameLog.h"

namespace GameHalloran {
    class ResHandle;
    class ResCache;

    // /////////////////////////////////////////////////////////////////
    // @class Resource
    // @author Michael L. McShaffry.
    //
    // A game resource.
    //
    // /////////////////////////////////////////////////////////////////
    class Resource {
    private:

        std::string m_name;                             ///< The filename of the resource.

    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param nameRef The name of the resource.
        //
        // /////////////////////////////////////////////////////////////////
        Resource(const std::string &nameRef);

        // /////////////////////////////////////////////////////////////////
        // Get the resource name.
        //
        // /////////////////////////////////////////////////////////////////
        std::string GetName() const;

        // /////////////////////////////////////////////////////////////////
        // Create a handle to the resource.
        //
        // @param buffer Pointer to the resource data.
        // @param size The file size.
        // @param pResCache Pointer to the reaource cache.
        //
        // @return ResHandle* A pointer to the handle of the resource.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ResHandle *VCreateHandle(const char *buffer, U32 size, ResCache *pResCache);
    };

    // /////////////////////////////////////////////////////////////////
    // @class IResourceFile
    // @author Michael L. McShaffry.
    //
    // Interface for the resource cache.
    //
    // /////////////////////////////////////////////////////////////////
    class IResourceFile {
    public:

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~IResourceFile() { };

        // /////////////////////////////////////////////////////////////////
        // Open the file.
        //
        // @return bool True on success or false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOpen() = 0;

        // /////////////////////////////////////////////////////////////////
        // Get the resource file size.
        //
        // @return boost::optional<I32> The size of the resource.  The return
        //                              value will not be valid if the resource
        //                              did not exist.
        //
        // /////////////////////////////////////////////////////////////////
        virtual boost::optional<I32> VGetResourceSize(const Resource &r) = 0;

        // /////////////////////////////////////////////////////////////////
        // Get the resource data.
        //
        // @param r The resource.
        // @param buffer The buffer to store the file data inside.
        //
        // @return bool True if we got the resource and false otherwise
        //              (if, for example if the resource did not exist).
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VGetResource(const Resource &r, char *buffer) = 0;

        // /////////////////////////////////////////////////////////////////
        // Get all resources in the cache file matching the pattern.
        //
        // @param std::string Regular expression file pattern.
        // @param ResourceListing Array of Resources matching the pattern.
        //
        // @return bool True|False if there were any files fond|not found.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VGetResourceListing(const std::string &regex, ResourceListing &listings) = 0;
    };

    // /////////////////////////////////////////////////////////////////
    // @class ResourceZipFile
    // @author Michael L. McShaffry.
    //
    // ZIP file implementation of the resource cache.
    //
    // /////////////////////////////////////////////////////////////////
    class ResourceZipFile : public IResourceFile {
    private:

        boost::scoped_ptr<ZipFile> m_pZipFile;                          ///< Pointer to the ZIP file interface.
        boost::filesystem::path m_resFileName;                          ///< The filename of the resource.

    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param resFileNameRef The filename of the zip directory.
        //
        // /////////////////////////////////////////////////////////////////
        ResourceZipFile(const boost::filesystem::path &resFileNameRef);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~ResourceZipFile();

        // /////////////////////////////////////////////////////////////////
        // Open the file.
        //
        // @return bool True on success or false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOpen();

        // /////////////////////////////////////////////////////////////////
        // Get the resource file size.
        //
        // @return boost::optional<I32> The size of the resource.  The return
        //                              value will not be valid if the resource
        //                              did not exist.
        //
        // /////////////////////////////////////////////////////////////////
        virtual boost::optional<I32> VGetResourceSize(const Resource &r);

        // /////////////////////////////////////////////////////////////////
        // Get the resource data.
        //
        // The buffer to store the data retrieved from the resource must be
        // pre allocted before you call this method to be large enough to
        // hold the data.
        //
        // e.g.
        // ResourceZipFile zf("temp.zip");
        // zf.VOpen();
        // ...
        // ...
        // ...
        // const I32 size = zf.GetResourceSize(resource);
        // char buf = new char[size];
        // zf.VGetResource(resource, buf);
        //
        // @param r The resource.
        // @param buffer The buffer to store the file data inside.
        //
        // @return bool True if we got the resource and false otherwise
        //              (if, for example if the resource did not exist).
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VGetResource(const Resource &r, char *buffer);

        // /////////////////////////////////////////////////////////////////
        // Get all resources in the cache file matching the pattern.
        //
        // @param std::string Regular expression file pattern.
        // @param ResourceListing Array of Resources matching the pattern.
        //
        // @return bool True|False if there were any files fond|not found.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VGetResourceListing(const std::string &regex, ResourceListing &listings);
    };

    // /////////////////////////////////////////////////////////////////
    // @class ResHandle
    // @author Michael L. McShaffry.
    //
    // A handle for a resource.
    //
    // /////////////////////////////////////////////////////////////////
    class ResHandle {
    private:

        Resource m_resource;                    ///< The resource.
        char *m_buffer;                         ///< The resource data.
        U32 m_size;                             ///< The size of the resource.
        ResCache *m_pResCache;                  ///< Pointer to the resource cache manager.

    protected:

    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param resource The resource.
        // @param buffer The resource data.
        // @param size The size of the resource.
        // @param pResCache Pointer to the resource cache manager.
        //
        // /////////////////////////////////////////////////////////////////
        ResHandle(Resource &resource, char *buffer, U32 size, ResCache *pResCache);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~ResHandle();

        // /////////////////////////////////////////////////////////////////
        // Load the resource using the custom resource loader.
        //
        // @param resLoaderPtr The pointer to the resource loader.
        //
        // @return bool True on success and false otherwise.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VLoad(IResourceFile *resLoaderPtr);

        // /////////////////////////////////////////////////////////////////
        // Gets the size of the resource.
        //
        // /////////////////////////////////////////////////////////////////
        U32 Size() const;

        // /////////////////////////////////////////////////////////////////
        // Gets the resource data.
        //
        // /////////////////////////////////////////////////////////////////
        char *Buffer() const;

        // /////////////////////////////////////////////////////////////////
        // Gets the resource name.
        //
        // /////////////////////////////////////////////////////////////////
        std::string GetResourceName() const;
    };

    // List of recently used resources.  Least recently used resources are located at the back of the list.
    typedef std::list<boost::shared_ptr<ResHandle> > ResHandleList;

    // Resource data stored in a map and retrieved by the resource name.
    typedef std::map<std::string, boost::shared_ptr<ResHandle> > ResHandleMap;      // maps indentifiers to resource data

    // /////////////////////////////////////////////////////////////////
    // @class ResCache
    // @author Michael L. McShaffry.
    //
    // The class which manages caching our game resources and loading
    // resources into memory when required.
    //
    // /////////////////////////////////////////////////////////////////
    class ResCache {
    private:

        ResHandleList m_lru;                        ///< Least recently used list.
        ResHandleMap m_resources;                   ///< Fast resource retrieval map.
        IResourceFile *m_file;                      ///< Pointer to the interface for loading in resources from container.
        U32 m_cacheSize;                            ///< Total cache size.
        U32 m_allocated;                            ///< Total memory allocated.
        boost::shared_ptr<GameLog> m_loggerPtr;     ///< Pointer to the game logger.

    protected:

        // /////////////////////////////////////////////////////////////////
        // Make space available in the cache for a new resource of size N
        // by freeing up resource files not recently used (if there is not
        // space available).
        //
        // @param size The number of bytes that needs to be available in the
        //              cache to create the new resource.
        //
        // @return bool True if we created enough room and false otherwise.
        //
        // /////////////////////////////////////////////////////////////////
        bool MakeRoom(const U32 size);

        // /////////////////////////////////////////////////////////////////
        // Allocate a buffer of the required size in the cache.
        //
        // The memory must be freed at a later time.
        //
        // @param size The size of the resource.
        //
        // @return char* A pointer to a buffer of size N or NULL if we cannot
        //                  create a buffer of the required size in the cache.
        //
        // /////////////////////////////////////////////////////////////////
        char *Allocate(const U32 size);

        // /////////////////////////////////////////////////////////////////
        // Free the resource from the cache.
        //
        // @param gonner A pointer to the resource we want to remove out of
        //                  the cache.
        //
        // /////////////////////////////////////////////////////////////////
        void Free(boost::shared_ptr<ResHandle> gonner);

        // /////////////////////////////////////////////////////////////////
        // Load a resource into the cache.
        //
        // @param r Pointer to a Resource.
        //
        // @return boost::shared_ptr<ResHandle> A resource handle or NULL if
        //                                      we fail to allocate memory.
        //
        // /////////////////////////////////////////////////////////////////
        boost::shared_ptr<ResHandle> Load(Resource *r);

        // /////////////////////////////////////////////////////////////////
        // Finds a resource in the map container.  Searches for the resource
        // by the resource name.
        //
        // @param r A pointer to the resource.
        //
        // @return shared_ptr<ResHandle> A pointer to the resource handle
        //                                  or NULL if the resource was not
        //                                  found.
        //
        // /////////////////////////////////////////////////////////////////
        boost::shared_ptr<ResHandle> Find(Resource *r);

        // /////////////////////////////////////////////////////////////////
        // Updates the least recently used list moving the resource to the
        // front of the queue.
        //
        // @param handle A pointer to the Resource Handle.
        //
        // /////////////////////////////////////////////////////////////////
        void Update(boost::shared_ptr<ResHandle> handle);

        // /////////////////////////////////////////////////////////////////
        // Free the memory allocated in the cache for the least recently used
        // resource currently in the cache.
        //
        // /////////////////////////////////////////////////////////////////
        void FreeOneResource();

    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param sizeInMb The size of the cache in MB.
        // @param file Pointer to the resource file interface to be used to
        //              load caches into memory.
        //
        // /////////////////////////////////////////////////////////////////
        ResCache(const U32 sizeInMb, IResourceFile *file, boost::shared_ptr<GameLog> loggerPtr);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~ResCache();

        // /////////////////////////////////////////////////////////////////
        // Initialize the Resource Cache by opening the resource container.
        //
        // @return bool True on success or false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        bool Init();

        // /////////////////////////////////////////////////////////////////
        // Get a handle to a resource.  This will have the effect of loading
        // the resource into memory if it does not already exist in the
        // cache.
        //
        // @param r Pointer to the resource to get/load.
        //
        // @return boost::shared_ptr<ResHandle> A pointer to the resource
        //                                      handle or an empty shared
        //                                      pointer if the Resource pointer
        //                                      supplied was NULL or if the
        //                                      resource cache is out of memory.
        //
        // /////////////////////////////////////////////////////////////////
        boost::shared_ptr<ResHandle> GetHandle(Resource *r);

        // /////////////////////////////////////////////////////////////////
        // Remove all resources currently loaded into memory from the cache.
        //
        // NOTE: This does not seem to be working as intended by Mike McShaffry
        // as Flush calls Free() which removes a resource from the list,
        // however Flush() calls pop_front() straight after which indicates to
        // me that its not removing caches accurretly.  Must debug this before
        // I use it....
        //
        // /////////////////////////////////////////////////////////////////
        void Flush(void);

        // /////////////////////////////////////////////////////////////////
        // Tell the resource cache manager that memory has recently been freed
        // so it may reclaim some memory in the near future.
        //
        // Only call this after the memory has been successfully freed!
        //  I must replace this as it could be a source of bugs....
        //
        // @param size The number of bytes that has been freed.
        // @param resourceName The name of the resource that is telling the
        //                      cache that it has just marked its memory as
        //                      free.
        //
        // /////////////////////////////////////////////////////////////////
        void MemoryHasBeenFreed(const U32 size, const std::string &resourceName);

        // /////////////////////////////////////////////////////////////////
        // Get all resources in the cache file matching the pattern.
        //
        // @param std::string Regular expression file pattern.
        // @param ResourceListing Array of Resources matching the pattern.
        //
        // @return bool True|False if there were any files fond|not found.
        //
        // /////////////////////////////////////////////////////////////////
        bool GetResourceListing(const std::string &regex, ResourceListing &listings);
    };

}

#endif
