//---------------------------------------------------------- -*- Mode: C++ -*-
// $Id: //depot/SOURCE/OPENSOURCE/kfs/src/cc/chunk/ChunkServer.h#2 $
//
// Created 2006/03/16
// Author: Sriram Rao (Kosmix Corp.) 
//
// Copyright 2006 Kosmix Corp.
//
// This file is part of Kosmos File System (KFS).
//
// Licensed under the Apache License, Version 2.0
// (the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied. See the License for the specific language governing
// permissions and limitations under the License.
//
// 
//----------------------------------------------------------------------------

#ifndef _CHUNKSERVER_H
#define _CHUNKSERVER_H

#include "libkfsIO/Acceptor.h"
#include "libkfsIO/DiskManager.h"
#include "libkfsIO/EventManager.h"
#include "libkfsIO/NetManager.h"
#include "libkfsIO/Globals.h"

#include "ChunkManager.h"
#include "ClientManager.h"
#include "ClientSM.h"
#include "MetaServerSM.h"
#include "RemoteSyncSM.h"

class ChunkServer {
public:
    ChunkServer() { };
    
    void Init();

    void MainLoop(int clientAcceptPort);

    bool IsLocalServer(const ServerLocation &location) const {
        return mLocation == location;
    }
    RemoteSyncSMPtr FindServer(const ServerLocation &location,
                               bool connect = true);
    void RemoveServer(RemoteSyncSM *target);

private:
    int mClientAcceptPort;
    
    ServerLocation mLocation;
    list<RemoteSyncSMPtr> mRemoteSyncers;

};

extern ChunkServer gChunkServer;

#endif // _CHUNKSERVER_H