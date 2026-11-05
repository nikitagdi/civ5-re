/// origin guess
#pragma once
#include "Civ5/Src/App/CvReplayHeader.h"
#include "Civ5/Src/App/CvSavedGameHeader.h"
#include "Civ5/Src/CvGameDatabase/Database.h"
#include "Civ5/Src/CvGameDatabase/DatabaseResults.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include <Windows.h>
#include <list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class FNetSessionIFace;
class FXmlDocument;
class FXmlElement;
class cvContentPackageIDList;
class cvPath;

/// size ?
/// vtable 0x0a303f80
class FNetMessage {
public:
    /// 0x08d4b5c6
    FNetMessage(int a1);
    /// 0x0871a594
    /// deleting 0x08d4c4b4
    virtual ~FNetMessage();
    /// pure
    virtual UNK_RET Debug(char* a1, unsigned int a2) const = 0;
    /// 0x08d4c200
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d4c294
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// pure
    virtual UNK_RET Execute(FNetSessionIFace* a1) const = 0;
    /// 0x08d4b5ac
    UNK_RET SetMessageDebugging(bool a1);
    /// 0x08d4b62e
    UNK_RET Process(FNetSessionIFace* a1) const;
    /// 0x08d4b668
    UNK_RET CreateAndReadMessage(unsigned char const* a1, int a2, long a3, int a4);
    /// 0x08d4b6d6
    UNK_RET CreateAndReadMessage(FDataStream& a1, long a2, int a3);
    /// 0x08d4bdfe
    UNK_RET PeekBuffer(FDataStream& a1, int* a2, int* a3, unsigned int* a4, int* a5, int* a6, unsigned int* a7, unsigned int* a8);
    /// 0x08d4bf82
    UNK_RET Serialize(FDataStream& a1);
    /// 0x08d4c02a
    UNK_RET GetSize() const;
    /// 0x08d4c094
    UNK_RET GetMessageFragmentationSize();
    /// 0x08d4c0aa
    UNK_RET PokeBuffer(FDataStream& a1, int* a2, int* a3, unsigned int* a4, int* a5, int* a6, unsigned int* a7, unsigned int* a8);
    /// 0x08d4c310
    UNK_RET ConnectionClosed(long a1);
    /// 0x08d4c378
    UNK_RET ResetConnections();
    /// 0x08d4c39c
    UNK_RET SetMessageFragmentationSize(unsigned int a1);
    /// 0x08d4c3d4
    UNK_RET ResetMessageFragmentationSize();
    /// 0x0ff87b28
    static UNK_DATA ms_mpkpbyMessageChunks[0x1c];
    /// 0x0a312ea8
    static UNK_DATA ms_uiMessageFragmentationSize[0x4];
};

/// 0x089f10fd SerializeToSequenceContainer<CvReplayHeader::DlcInfo, std::vector<CvReplayHeader::DlcInfo, std::allocator<CvReplayHeader::DlcInfo> > >
/// 0x089f0e38 SerializeToSequenceContainer<CvReplayHeader::ModInfo, std::vector<CvReplayHeader::ModInfo, std::allocator<CvReplayHeader::ModInfo> > >
/// 0x089973e3 SerializeToSequenceContainer<CvSavedGameHeader::DlcInfo, std::vector<CvSavedGameHeader::DlcInfo, std::allocator<CvSavedGameHeader::DlcInfo> > >
/// 0x0899711e SerializeToSequenceContainer<CvSavedGameHeader::ModInfo, std::vector<CvSavedGameHeader::ModInfo, std::allocator<CvSavedGameHeader::ModInfo> > >
/// 0x0871c13c SerializeToSequenceContainer<_GUID, std::list<_GUID, std::allocator<_GUID> > >
/// 0x0871b459 SerializeToSequenceContainer<int, std::vector<int, std::allocator<int> > >
/// 0x0871ba99 SerializeToSequenceContainer<std::pair<std::string, std::string>, std::vector<std::pair<std::string, std::string>, std::allocator<std::pair<std::string, std::string> > > >
/// 0x0871c40c SerializeToSequenceContainer<unsigned char, std::vector<unsigned char, std::allocator<unsigned char> > >
template <class T0, class T1>
void SerializeToSequenceContainer(FDataStream& a1, T1& a2);
/// 0x08720ba0
UNK_RET InsertLandmarkTypeEntries(Database::Results& a1, FXmlElement& a2, char const* a3, unsigned int a4);
/// 0x08724e55
UNK_RET AppendIfFound(std::vector<cvPath, std::allocator<cvPath> >& a1, char const* a2);
extern "C" {
    /// 0x0a312eb0
    extern UNK_DATA s_workaroundBrokenWindowsNetworkImplementation[0x1];
}

namespace CvFolders {
/// 0x08720538
UNK_RET Init();
/// 0x0872054c
UNK_RET AddFolders();
/// 0x08720b62
UNK_RET Update();
/// 0x08720b64
UNK_RET GetReplayExtension();
/// 0x08720b78
UNK_RET GetSaveExtension();
/// 0x08720b8c
UNK_RET GetMapExtension();
}

namespace CvGameDatabaseAppSide {
/// 0x08720d4c
UNK_RET LoadLandmarksXMLtoDB(Database::Connection& a1, FXmlDocument const& a2);
/// 0x0872119c
UNK_RET LoadLandmarkTypesXMLtoDB(Database::Connection& a1, FXmlDocument const& a2);
/// 0x08721318
UNK_RET LoadLandmarkDefinitions(std::vector<std::string, std::allocator<std::string> > const& a1, Database::Connection& a2);
/// 0x087213bc
UNK_RET LoadStrategicViewXMLtoDB(Database::Connection& a1, char const* a2, char const* a3);
/// 0x08721638
UNK_RET CreateCoreDatabase(Database::Connection& a1);
/// 0x08722310
UNK_RET AddScannedFile(Database::Results& a1, cvPath const& a2);
/// 0x087223ee
UNK_RET GetGameplayXmlFiles(std::vector<cvPath, std::allocator<cvPath> >& a1);
/// 0x08722bf4
UNK_RET InitializeCoreDatabase(Database::Connection& a1);
/// 0x087234fa
UNK_RET DatabaseUpToDate(cvPath const& a1, std::vector<cvPath, std::allocator<cvPath> > const& a2, bool a3);
/// 0x08723768
UNK_RET LoadCoreDatabase(Database::Connection& a1, bool a2);
/// 0x08723fbe
UNK_RET ResetCoreDatabase(Database::Connection& a1);
/// 0x087240b8
UNK_RET IsCoreDatabaseDirty();
/// 0x087240cc
UNK_RET LoadDLC(Database::Connection& a1, cvContentPackageIDList const* a2, bool a3);
/// 0x08724bf6
UNK_RET SaveDebugDatabase();
/// 0x08724da0
UNK_RET FileWasAlreadyScanned(Database::Results& a1, cvPath const& a2);
/// 0x08724f34
UNK_RET SetCoreDatabaseDirty();
}

namespace FNetMessageDebug {
/// 0x0ff87b44
extern UNK_DATA c_isMessageDebuggingOn[0x1];
}

namespace Protocol {
/// 0x0a312eac
extern UNK_DATA s_checkTimeouts[0x1];
}
