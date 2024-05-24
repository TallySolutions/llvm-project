#include "FormatTestBase.h"


#define DEBUG_TYPE "braces-remover-test"

#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Format/Format.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/Inclusions/HeaderIncludes.h"
#include "llvm/ADT/StringRef.h"

namespace clang {
namespace format {
namespace test {
namespace {

    class TWClangFormatTest : public FormatTestBase {};

    TEST_F(TWClangFormatTest, WhiteSpaceAroundBinaryOperator) {
    
    FormatStyle Style = getTWStyle();

        verifyFormat("a = b + c;",
                    "a =b+  c;",
                    Style);

        verifyFormat("a = b - c;",
                    "a =b- c;",
                    Style);
    }

    TEST_F(TWClangFormatTest, NoSpaceBeforeSemicolon) {
    
    FormatStyle Style = getTWStyle();

        verifyFormat("int a;",
                    "int a  ;",
                    Style);
    }

    TEST_F(TWClangFormatTest, SpaceAfterFunctionAndFlowControlStatements) {
    
    FormatStyle Style = getTWStyle();

        verifyFormat("void\r\n"
                    "func ()\r\n"
                    "{\r\n"
                    "    ;\r\n"
                    "}",
                    "void func() {\r\n"
                    "    ;\r\n"
                    "}",
                    Style);

        verifyFormat("void    func ();",
                    "void   \r\n"
                    "func      ();",
                    Style);

        verifyFormat("if (true) {\r\n"
                    "\r\n"
                    "    ;\r\n"
                    "}",
                    "if(true)  \r\n"
                    "{\r\n"
                    "    ;\r\n"
                    "}",
                    Style);
    }

    TEST_F(TWClangFormatTest, SpaceInInitializerList) {
    
    FormatStyle Style = getTWStyle();

        verifyFormat("int arr[5] = {10, 20, 30, 40, 50};",
                    "int arr[5] = { 10,20,  30,40, 50  };",
                    Style);
    }
    TEST_F(TWClangFormatTest, SwitchCase) {
    
    FormatStyle Style = getTWStyle();

        verifyFormat("switch (x) {\r\n"
                    "    case 'A':\r\n"
                    "        a = b;\r\n"
                    "        break;\r\n"
                    "    case 'B':\r\n"
                    "        a = c;\r\n"
                    "        break;\r\n"
                    "    default:\r\n"
                    "        a = d;\r\n"
                    "        break;\r\n"
                    "}",
                                        "switch(x )\r\n"
                    "{\r\n"
                    "    case 'A'  :\r\n"
                    "        a = b;\r\n"
                    "        break;\r\n"
                    "   case     'B':\r\n"
                    "        a = c;\r\n"
                    "            break  ;\r\n"
                    " default   :\r\n"
                    "        a = d;\r\n"
                    "        break;\r\n"
                    "    }",
                    Style);

    }

    TEST_F(TWClangFormatTest, AsterixSpace) {
    
    FormatStyle Style = getTWStyle();

        verifyFormat("TInt32 *    x = a;",
                    "TInt32*x =   a;",
                    Style);

        verifyFormat("TInt32  DoMagic (const TChar8 * pParam);",
                    "TInt32 DoMagic (const TChar8*pParam);",
                    Style);

        verifyFormat("TChar8 *    GetTitle ();",
                    "TChar8* GetTitle ();",
                    Style);
    }

    TEST_F(TWClangFormatTest, ArrowOperator) {
    
    FormatStyle Style = getTWStyle();

        verifyFormat("void\r\n"
                    "func ()\r\n"
                    "{\r\n"
                    "        TUInt32     nextid;\r\n"
                    "        TWSMustLock portelemslock (portelem->uSyncFlag[ipprotocol.IntVal ()][transportprotocol.IntVal ()], eTWSyncType::EXCLUSIVE);\r\n"
                    "\r\n"
                    "    sBaseClientEngine->GetSimpleDUID (pDUID, eTWBaseDCType::REPORT_DC, eTWAppDCType::UNKNOWN, nextid);\r\n"
                    "    (((ClassName *) this)->*pResolveQueryFunc) (parms);\r\n"
                    "    ((TWNWIOHandle *) eventqueue[idx].data.ptr)->OnSocketCanReadWithoutBlocking ();\r\n"
                    "    CHECKTRUE (pQty->PrivateIsRepeat ());\r\n"
                    "}",
                    "void\r\n"
                    "func ()\r\n"
                    "{\r\n"
                    "        TUInt32 nextid;\r\n"
                    "    TWSMustLock     portelemslock (portelem ->  uSyncFlag[ipprotocol.IntVal ()][transportprotocol.IntVal ()],       eTWSyncType::EXCLUSIVE);\r\n"
                    "\r\n"
                    "            sBaseClientEngine   ->      GetSimpleDUID (pDUID, eTWBaseDCType::REPORT_DC, eTWAppDCType::UNKNOWN, nextid);\r\n"
                    "        (((ClassName *  ) this    )-> *   pResolveQueryFunc) (parms);\r\n"
                    "    ((TWNWIOHandle      *) eventqueue[idx].data.ptr)->      OnSocketCanReadWithoutBlocking ();\r\n"
                    "            CHECKTRUE ( pQty->       PrivateIsRepeat ());\r\n"
                    "}",
                    Style);

        verifyFormat("DCType *\r\n"
                    "TWBaseClientEngine::InitializeTWReportDC (TWDUID & pDUID, TWSHeldElems & pHeldElems) noexcept\r\n"
                    "{\r\n"
                    "        TRACEFUNC;\r\n"
                    "\r\n"
                    "        TWSObject   dcdefinition;\r\n"
                    "\r\n"
                    "    vVersion   = parms->uPreviousStage.uRequestContext.GetContentValue<TUInt64> (iTWFieldID::RESOLVED_VERSION);\r\n"
                    "    rangetondx = rangedata->GetContentValue<TUInt32> (nullptr, iTWFieldID::RANGE_TO);\r\n"
                    "    rc         = TWDITPacket::RemoveSimpleDIT (pPacket->GetTWNPBasePacketHeader (), pPacket->uNumDITBlocks);\r\n"
                    "    parentref  = pInstructionSet->at (instruction->uParentIDx)->uElementRef;\r\n"
                    "\r\n"
                    "    if (TWUtils::IsMultipleOf64 (pPacket->uDataLen)) [[likely]] {\r\n"
                    "\r\n"
                    "        obj.GetDCObjectElemPtr ()->MoveToLRU (pLRUPriority);\r\n"
                    "    }\r\n"
                    "\r\n"
                    "    return sBaseClientEngine->InitializeSimpleDC<DCType> (pDUID, dcdefinition, pHeldElems);\r\n"
                    "}",
            "DCType *\r\n"
                    "TWBaseClientEngine::InitializeTWReportDC (TWDUID & pDUID, TWSHeldElems & pHeldElems) noexcept\r\n"
                    "{\r\n"
                    "        TRACEFUNC;\r\n"
                    "\r\n"
                    "        TWSObject   dcdefinition;\r\n"
                    "\r\n"
                    "    vVersion   =        parms   -> uPreviousStage.uRequestContext.GetContentValue<TUInt64> (iTWFieldID::RESOLVED_VERSION);\r\n"
                    "    rangetondx      =            rangedata       ->  GetContentValue<TUInt32> (nullptr, iTWFieldID::RANGE_TO);\r\n"
                    "        rc          =    TWDITPacket::RemoveSimpleDIT    (pPacket    ->GetTWNPBasePacketHeader (), pPacket   ->      uNumDITBlocks);\r\n"
                    "    parentref   = pInstructionSet    ->  at (instruction->     uParentIDx)->    uElementRef;\r\n"
                    "\r\n"
                    "    if (TWUtils::IsMultipleOf64 (   pPacket->   uDataLen)) [[likely]] {\r\n"
                    "\r\n"
                    "        obj.GetDCObjectElemPtr ()->   MoveToLRU (pLRUPriority);\r\n"
                    "    }\r\n"
                    "\r\n"
                    "    return sBaseClientEngine  ->   InitializeSimpleDC  <DCType> (pDUID,   dcdefinition, pHeldElems);\r\n"
                    "}",
                    
                    Style);
    }

    TEST_F(TWClangFormatTest, OpeningBraceForClassStructEnum) {
    
    FormatStyle Style = getTWStyle();

        verifyFormat("class TWQtEvent {\r\n"
                    "    ;\r\n"
                    "};",
                    "class TWQtEvent {;};",
                    Style);

        verifyFormat("while (true) {\r\n"
                    "\r\n"
                    "    ;\r\n"
                    "}",
                    "while (true ){\r\n"
                    ";}",
                    Style);

        verifyFormat("if (true) {\r\n"
                    "\r\n"
                    "    a = b;\r\n"
                    "} else {\r\n"
                    "\r\n"
                    "    a = c;\r\n"
                    "}",
                    "if (true) {\r\n"
                    "a=b;\r\n"
                    "}\r\n"
                    "else\r\n"
                    "{\r\n"
                    "a=c;\r\n"
                    "}",
                    Style);

    }

    TEST_F(TWClangFormatTest, StaticMemberVariableDefinition) {
    
    FormatStyle Style = getTWStyle();

        verifyFormat("FILE *  TWDevLogDestination<eTWDevLogDestType::METRICS_FILE>::vMetricsFile = nullptr;\r\n"
                    "int     MyClass::staticVar = 0;\r\n"
                    "int *   werbv::sdfertyw                                                                                         {};\r\n"
                    "char ** cvcv::scvb                                                                                              {};\r\n"
                    "TCh     TWDevLogDestination<eTWDevLogDestType::METRICS_FILE>::vMetricsLogFileName[TWDEVSUPPORT_FILEPATH_SIZE] = {};",
                    "FILE * TWDevLogDestination<eTWDevLogDestType::METRICS_FILE>::vMetricsFile   =  nullptr;\r\n"
                    "        int       MyClass::staticVar = 0;\r\n"
                    "int    *     werbv::sdfertyw  {};\r\n"
                    "    char  * * cvcv::scvb    {};\r\n"
                    "TCh  TWDevLogDestination<eTWDevLogDestType::METRICS_FILE>::vMetricsLogFileName[TWDEVSUPPORT_FILEPATH_SIZE]    =    {};",
                    Style);
    }

    TEST_F(TWClangFormatTest,VariableDeclarationAlignment) {
    
    FormatStyle Style = getTWStyle();

        verifyFormat("void\r\n"
                    "SetContentState ()\r\n"
                    "{\r\n"
                    "\r\n"
                    "        TWSObject           pathobject;\r\n"
                    "        iTWObjTypeID::Enum  vObjectType  : iTWObjTypeID::SizeBits;\r\n"
                    "        TUInt32             vIsFixedSize : eTWBool::SizeBits;\r\n"
                    "        const TUInt8        TW_CONTENT_VARIABLE_LENGTH_BITS {32};\r\n"
                    "        const TUInt8        TW_OBJ_LADDER_FACTOR_BITS       {1 * 2};\r\n"
                    "        const TUInt8        TW_OBJ_LADDER_FACTOR_BITS       {1 << TW_OBJ_LADDER_SHIFT_RESOLVE};\r\n"
                    "        TWSEvent            event (iTWObjTypeID::DISKIO_SCRATCHPAD);\r\n"
                    "        TWSEvent            event (iTWObjTypeID::UNKNOWN);\r\n"
                    "\r\n"
                    "    return;\r\n"
                    "}",
                                        "void\r\n"
                    "SetContentState ()\r\n"
                    "{\r\n"
                    "\r\n"
                    "   TWSObject           pathobject;\r\n"
                    "     iTWObjTypeID::Enum     vObjectType    :   iTWObjTypeID::SizeBits;\r\n"
                    "        TUInt32             vIsFixedSize  :    eTWBool::SizeBits;\r\n"
                    "      const   TUInt8        TW_CONTENT_VARIABLE_LENGTH_BITS    {32};\r\n"
                    "        const TUInt8        TW_OBJ_LADDER_FACTOR_BITS       {1 * 2};\r\n"
                    "    const        TUInt8    TW_OBJ_LADDER_FACTOR_BITS   {1 << TW_OBJ_LADDER_SHIFT_RESOLVE};\r\n"
                    "        TWSEvent                 event (iTWObjTypeID::DISKIO_SCRATCHPAD);\r\n"
                    "            TWSEvent      event (iTWObjTypeID::UNKNOWN);\r\n"
                    "\r\n"
                    "    return;\r\n"
                    "}",
                    Style);

        verifyFormat("void\r\n"
                    "f ()\r\n"
                    "{\r\n"
                    "        vector<int>                 dfewoir;\r\n"
                    "        vector<char *>              werio;\r\n"
                    "        vector<char *, long long>   werio;\r\n"
                    "        int *                       fgsdf;\r\n"
                    "        char **                     sdfe;\r\n"
                    "        float                       wer;\r\n"
                    "\r\n"
                    "    wer = 8;\r\n"
                    "}",
                    "void\r\n"
                    "f ()\r\n"
                    "{\r\n"
                    "        vector<int>         dfewoir;\r\n"
                    "     vector<char *>  werio;\r\n"
                    "        vector<char *, long long>  werio;\r\n"
                    "    int *           fgsdf;\r\n"
                    "            char **         sdfe;\r\n"
                    "    float           wer;\r\n"
                    "\r\n"
                    "        wer = 8;\r\n"
                    "}",
                    Style);

        verifyFormat("void\r\n"
                    "TWThreadMgr ()\r\n"
                    "{\r\n"
                    "\r\n"
                    "        a::b::c<int>        d;\r\n"
                    "        int                 g;\r\n"
                    "        int                 a ();\r\n"
                    "        const int           a;\r\n"
                    "        int *               a;\r\n"
                    "        int **              a;\r\n"
                    "        vector<vector<int>> a;\r\n"
                    "        a::b                c;\r\n"
                    "        a::b::c             d;\r\n"
                    "        int                 a[3];\r\n"
                    "        const int           a = 0;\r\n"
                    "        TWSSharedEventLock  lock (pDiskIOEvent, SHARED);\r\n"
                    "\r\n"
                    "    if (true) {\r\n"
                    "\r\n"
                    "        ;\r\n"
                    "    }\r\n"
                    "    while (true) {\r\n"
                    "\r\n"
                    "        ;\r\n"
                    "    }\r\n"
                    "    a ();\r\n"
                    "    cout << a ();\r\n"
                    "    return (0);\r\n"
                    "    a->b ();\r\n"
                    "    a<int, int>->b ();\r\n"
                    "    a            = b ();\r\n"
                    "    a::b<int>::c = 5;\r\n"
                    "    int a = (int) b;\r\n"
                    "}",
                    "void\r\n"
                    "TWThreadMgr ()\r\n"
                    "{\r\n"
                    "\r\n"
                    "    a::b::c<int>    d;\r\n"
                    "        int     g;\r\n"
                    "    int     a ();\r\n"
                    "            const   int a;\r\n"
                    "    int *   a;\r\n"
                    "        int *   * a;\r\n"
                    "            vector<vector<int>> a;\r\n"
                    "        a::b    c;\r\n"
                    "        a::b::c     d;\r\n"
                    "    int a[3];\r\n"
                    "            const   int a   =0;\r\n"
                    "        TWSSharedEventLock  lock (pDiskIOEvent, SHARED);\r\n"
                    "    if (true) {\r\n"
                    "    ;\r\n"
                    "    }\r\n"
                    "    while (true) {\r\n"
                    "    ;\r\n"
                    "    }\r\n"
                    "        a ();\r\n"
                    "        cout    << a ();\r\n"
                    "    return (0);\r\n"
                    "        a->b();\r\n"
                    "    a<int, int>->b      ();\r\n"
                    "         a   =   b();\r\n"
                    "          a::b<int>::c    =5  ;\r\n"
                    "      int a = (int)b;\r\n"
                    "}",
                    Style);
    }

    TEST_F(TWClangFormatTest, AlignmentInClassScope) {
    
    FormatStyle Style = getTWStyle();

        verifyFormat("class TWSString {\r\n"
                    "\r\n"
                    "    public:\r\n"
                    "\r\n"
                    "                            TWSString                       () noexcept;\r\n"
                    "                           ~TWSString                       () noexcept;\r\n"
                    "                bool        Initialize                      (TUInt32 pInitialLen = TWSTR_DEFAULT_ALLOC_SIZE_NEW) noexcept;\r\n"
                    "                bool        SetString                       (const jstring pString) noexcept;\r\n"
                    "\r\n"
                    "    private:\r\n"
                    "\r\n"
                    "                TWString *  InternalInitializeScratch       (TUInt32 pInitialLength) noexcept;\r\n"
                    "                TWString *  InternalResizeScratchContent    (TUInt32 pRequiredLength) noexcept;\r\n"
                    "};",
                    "class TWSString {\r\n"
                    "\r\n"
                    "  public:\r\n"
                    "\r\n"
                    "                        TWSString                       () noexcept;\r\n"
                    "                                ~TWSString                       () noexcept;\r\n"
                    "                 bool                Initialize                      (TUInt32 pInitialLen = TWSTR_DEFAULT_ALLOC_SIZE_NEW) noexcept;\r\n"
                    "                        bool                SetString                       (const jstring pString) noexcept;\r\n"
                    " private:\r\n"
                    "\r\n"
                    "        TWString *          InternalInitializeScratch       (TUInt32 pInitialLength) noexcept;\r\n"
                    "                    TWString *          InternalResizeScratchContent    (TUInt32 pRequiredLength) noexcept;\r\n"
                    "\r\n"
                    "};",
                    Style);

        verifyFormat("class TWRangeDesignation {\r\n"
                    "\r\n"
                    "    public:\r\n"
                    "\r\n"
                    "                TUInt64         uStartNdx;\r\n"
                    "                TUInt64         uEndNdx;\r\n"
                    "                TWDataBuffer *  vScratchContent;\r\n"
                    "\r\n"
                    "    private:\r\n"
                    "\r\n"
                    "                TWFieldID       vSchemaID;\r\n"
                    "                QString         uWindowQml;\r\n"
                    "};",
                    "class TWRangeDesignation {\r\n"
                    "\r\n"
                    "    public:\r\n"
                    "\r\n"
                    "    TUInt64                 uStartNdx;\r\n"
                    "        TUInt64         uEndNdx;\r\n"
                    "    TWDataBuffer *               vScratchContent;\r\n"
                    "    private:\r\n"
                    "        TWFieldID                       vSchemaID;\r\n"
                    "    QString             uWindowQml;\r\n"
                    "};",
                    Style);

        verifyFormat("class TWSString {\r\n"
                    "\r\n"
                    "    public:\r\n"
                    "\r\n"
                    "                                TWSString                       () noexcept;\r\n"
                    "                               ~TWSString                       () noexcept;\r\n"
                    "                bool            Initialize                      (TUInt32 pInitialLen = TWSTR_DEFAULT_ALLOC_SIZE_NEW) noexcept;\r\n"
                    "                bool            SetString                       (const jstring pString) noexcept;\r\n"
                    "                TUInt64         uStartNdx;\r\n"
                    "                TUInt64         uEndNdx;\r\n"
                    "                TWDataBuffer *  vScratchContent;\r\n"
                    "\r\n"
                    "    private:\r\n"
                    "\r\n"
                    "                TWString *      InternalInitializeScratch       (TUInt32 pInitialLength) noexcept;\r\n"
                    "                TWString *      InternalResizeScratchContent    (TUInt32 pRequiredLength) noexcept;\r\n"
                    "\r\n"
                    "                TWFieldID       vSchemaID;\r\n"
                    "                QString         uWindowQml;\r\n"
                    "};",
                    "class TWSString {\r\n"
                    "\r\n"
                    "   public:\r\n"
                    "\r\n"
                    "\r\n"
                    "\r\n"
                    "                TWSString                       () noexcept;\r\n"
                    "                                ~TWSString                       () noexcept;\r\n"
                    "                 bool                Initialize                      (TUInt32 pInitialLen = TWSTR_DEFAULT_ALLOC_SIZE_NEW) noexcept;\r\n"
                    "                        bool                SetString                       (const jstring pString) noexcept;\r\n"
                    "    TUInt64                 uStartNdx;\r\n"
                    "        TUInt64         uEndNdx;\r\n"
                    "    TWDataBuffer *               vScratchContent;\r\n"
                    " private:\r\n"
                    "\r\n"
                    "        TWString *          InternalInitializeScratch       (TUInt32 pInitialLength) noexcept;\r\n"
                    "                    TWString *          InternalResizeScratchContent    (TUInt32 pRequiredLength) noexcept;\r\n"
                    "\r\n"
                    "        TWFieldID                       vSchemaID;\r\n"
                    "    QString             uWindowQml;\r\n"
                    "};",
                    Style);
        

        verifyFormat("class TWSString {\r\n"
                    "\r\n"
                    "    public:\r\n"
                    "\r\n"
                    "                            TWSString                       () noexcept;\r\n"
                    "                           ~TWSString                       () noexcept;\r\n"
                    "                bool        Initialize                      (TUInt32 pInitialLen = TWSTR_DEFAULT_ALLOC_SIZE_NEW) noexcept;\r\n"
                    "                bool        SetString                       (const jstring pString) noexcept;\r\n"
                    "static          void        Destroy                         (Type ** pContent) noexcept;\r\n"
                    "\r\n"
                    "    private:\r\n"
                    "\r\n"
                    "                TWString *  InternalInitializeScratch       (TUInt32 pInitialLength) noexcept;\r\n"
                    "                TWString *  InternalResizeScratchContent    (TUInt32 pRequiredLength) noexcept;\r\n"
                    "                CVPtr       PrivateGetConstValuePtr         () const noexcept;\r\n"
                    "static          TWContent * PrivateCreateHybridFrom         (const TWContent * pSource, const TWContent * pOwner, bool pIsReconstructCopy, TUInt32 pMyIndexInOwner) noexcept;\r\n"
                    "static          TWContent * CreateFromBuffer                (const TWDataBuffer * pBuffer);\r\n"
                    "};", 
                    "class TWSString {\r\n"
                    "\r\n"
                    "    public:\r\n"
                    "\r\n"
                    "                            TWSString                       () noexcept;\r\n"
                    "                                ~TWSString                       () noexcept;\r\n"
                    "                    bool        Initialize          (TUInt32 pInitialLen = TWSTR_DEFAULT_ALLOC_SIZE_NEW) noexcept;\r\n"
                    "            bool        SetString                       (const jstring pString) noexcept;\r\n"
                    "                static          void                            Destroy                             (Type ** pContent) noexcept;\r\n"
                    "\r\n"
                    "    private:\r\n"
                    "\r\n"
                    "            TWString *  InternalInitializeScratch       (TUInt32 pInitialLength) noexcept;\r\n"
                    "        TWString *  InternalResizeScratchContent    (TUInt32 pRequiredLength) noexcept;\r\n"
                    "                    CVPtr                           PrivateGetConstValuePtr             () const noexcept;\r\n"
                    "        static          TWContent *                     PrivateCreateHybridFrom             (const TWContent * pSource, const TWContent * pOwner, bool pIsReconstructCopy, TUInt32 pMyIndexInOwner) noexcept;\r\n"
                    "                    static          TWContent *                     CreateFromBuffer                    (const TWDataBuffer * pBuffer);\r\n"
                    "\r\n"
                    "};",
                    Style);

        verifyFormat("class TWQtEvent {\r\n"
                    "\r\n"
                    "    public:\r\n"
                    "\r\n"
                    "                QString     uWindowQml;\r\n"
                    "\r\n"
                    "                TWFieldID   vSchemaID;\r\n"
                    "                            TWQtEvent   ()\r\n"
                    "                            {\r\n"
                    "                                    int  f  : e;\r\n"
                    "                                    char gg : t;\r\n"
                    "\r\n"
                    "                                f  = 4;\r\n"
                    "                                gg = 'c';\r\n"
                    "\r\n"
                    "                                {\r\n"
                    "                                        int  f;\r\n"
                    "                                        char gg;\r\n"
                    "\r\n"
                    "                                    f  = 4;\r\n"
                    "                                    gg = 'c';\r\n"
                    "                                }\r\n"
                    "                            }\r\n"
                    "                TWFieldID   vSchemaID;\r\n"
                    "\r\n"
                    "    private:\r\n"
                    "\r\n"
                    "                TWFieldID   vSchemaID;\r\n"
                    "                TUInt64     vEmbeddedPrimitve;\r\n"
                    "};",
            "class TWQtEvent {\r\n"
                    "\r\n"
                    "    public:\r\n"
                    "\r\n"
                    "           QString     uWindowQml;\r\n"
                    "\r\n"
                    "       TWFieldID              vSchemaID;\r\n"
                    "                      TWQtEvent       ()\r\n"
                    "            {\r\n"
                    "                    int  f          : e;\r\n"
                    "                    char gg     : t;\r\n"
                    "\r\n"
                    "                            f  = 4;\r\n"
                    "                        gg = 'c';\r\n"
                    "\r\n"
                    "                {\r\n"
                    "                                int  f;\r\n"
                    "                                    char gg;\r\n"
                    "\r\n"
                    "                            f  = 4;\r\n"
                    "                        gg = 'c';\r\n"
                    "                }\r\n"
                    "            }\r\n"
                    "                    TWFieldID           vSchemaID;\r\n"
                    "\r\n"
                    "    private:\r\n"
                    "\r\n"
                    "                    TWFieldID   vSchemaID;\r\n"
                    "        TUInt64             vEmbeddedPrimitve;\r\n"
                    "};",
                    
                    Style);

        verifyFormat("class TWSString {\r\n"
                    "\r\n"
                    "    public:\r\n"
                    "\r\n"
                    "                            TWSString                       () noexcept;\r\n"
                    "                           ~TWSString                       () noexcept;\r\n"
                    "static          TWContent * PrivateMakeEmptyNotPossible     (const TWContent * pContent) noexcept;\r\n"
                    "\r\n"
                    "                [[nodiscard]]\r\n"
                    "static          TUInt32     PrivateGetFixedContentLength    (const TWContent * pContent) noexcept;\r\n"
                    "                [[nodiscard]]\r\n"
                    "static          TUInt32     PrivateGetVarContentLength      (const TWContent * pContent) noexcept;\r\n"
                    "\r\n"
                    "static          void        PrivateSetFixedContentLength    (TWContent * pContent, TUInt32 pLengthInBytes) noexcept;\r\n"
                    "                [[nodiscard]]\r\n"
                    "                VPtr        PrivateGetValuePtr              () noexcept;\r\n"
                    "\r\n"
                    "    private:\r\n"
                    "\r\n"
                    "                TWString *  InternalInitializeScratch       (TUInt32 pInitialLength) noexcept;\r\n"
                    "static          TWContent * CreateFromBuffer                (const TWDataBuffer * pBuffer);\r\n"
                    "};",
            "class TWSString {\r\n"
                    "\r\n"
                    "    public:\r\n"
                    "\r\n"
                    "                            TWSString                       () noexcept;\r\n"
                    "                           ~TWSString               () noexcept;\r\n"
                    "                static          TWContent *                     PrivateMakeEmptyNotPossible         (const TWContent * pContent) noexcept;\r\n"
                    "\r\n"
                    "            [[nodiscard]]\r\n"
                    "    static          TUInt32                         PrivateGetFixedContentLength        (   const TWContent * pContent) noexcept;\r\n"
                    "                            [[nodiscard]]\r\n"
                    "        static      TUInt32                         PrivateGetVarContentLength          (const TWContent * pContent) noexcept;\r\n"
                    "\r\n"
                    "static              void                                PrivateSetFixedContentLength        (TWContent * pContent, TUInt32 pLengthInBytes) noexcept;\r\n"
                    "                    [[nodiscard]]\r\n"
                    "                    VPtr                            PrivateGetValuePtr                  ()  noexcept;\r\n"
                    "\r\n"
                    "    private:\r\n"
                    "\r\n"
                    "                TWString *          InternalInitializeScratch   (TUInt32 pInitialLength)    noexcept;\r\n"
                    "static                  TWContent *     CreateFromBuffer                (const TWDataBuffer * pBuffer);\r\n"
                    "};",
                    
                    Style);
        }

    TEST_F(TWClangFormatTest, MaybeUnused) {
    
    FormatStyle Style = getTWStyle();

        verifyFormat("bool\r\n"
                    "TWDevConfigLexer::PrintError ([[maybe_unused]] const CharType * pFmt, [[maybe_unused]] TArgs... pArgs) noexcept\r\n"
                    "{\r\n"
                    "        TRACEFUNC;\r\n"
                    "\r\n"
                    "    INTERNAL_TRACE_LOG_FILE_LOC (vFileName, vLineNumber, pFmt, pArgs...);\r\n"
                    "    return false;        // error always return false\r\n"
                    "}",
                    "bool\r\n"
                    "TWDevConfigLexer::PrintError ([[maybe_unused]]  const   CharType * pFmt,    [[maybe_unused]]    TArgs...    pArgs) noexcept\r\n"
                    "{\r\n"
                    "            TRACEFUNC;\r\n"
                    "\r\n"
                    "INTERNAL_TRACE_LOG_FILE_LOC (vFileName, vLineNumber, pFmt, pArgs...);\r\n"
                    "        return false;        // error always return false\r\n"
                    "}",
                    Style);

        verifyFormat("bool\r\n"
                    "TWOSInternalSocketFDWatcher::StartWatchingNWIOHandle (SOCKET_FD pSocketFD, eTWSockFDWatchOptions::Enum pMode) noexcept\r\n"
                    "{\r\n"
                    "        TRACEFUNC;\r\n"
                    "\r\n"
                    "        [[maybe_unused]]\r\n"
                    "        TInt64  mode;\r\n"
                    "        [[maybe_unused]]\r\n"
                    "        TInt64  error_code;\r\n"
                    "        TInt64  flags;\r\n"
                    "\r\n"
                    "    flags = EV_ENABLE | EV_ADD;\r\n"
                    "}",
                    "bool\r\n"
                    "TWOSInternalSocketFDWatcher::StartWatchingNWIOHandle (SOCKET_FD pSocketFD, eTWSockFDWatchOptions::Enum pMode) noexcept\r\n"
                    "{\r\n"
                    "        TRACEFUNC;\r\n"
                    "\r\n"
                    "                [[maybe_unused]]\r\n"
                    "    TInt64              mode;\r\n"
                    "            [[maybe_unused]]\r\n"
                    "TInt64                  error_code;\r\n"
                    "    TInt64  flags;\r\n"
                    "\r\n"
                    "            flags = EV_ENABLE | EV_ADD;\r\n"
                    "\r\n"
                    "}",
                    Style);

        verifyFormat("class TWThreadMgr TWCLASS_MAKE_FINAL {\r\n"
                    "\r\n"
                    "    public:\r\n"
                    "\r\n"
                    "                [[nodiscard]]\r\n"
                    "static          TWThreadExitFunc    SetExitThreadFunc           (TWThreadExitFunc pExitThreadFunc) noexcept;\r\n"
                    "\r\n"
                    "    private:\r\n"
                    "\r\n"
                    "                [[noreturn]]\r\n"
                    "static          void                PrivateExitThread           () noexcept;\r\n"
                    "\r\n"
                    "                [[noreturn]]\r\n"
                    "static          void                PrivateDefaultExitProcess   () noexcept;\r\n"
                    "\r\n"
                    "    private:\r\n"
                    "\r\n"
                    "                [[maybe_unused]]\r\n"
                    "static          TInt32              sExitValue;        // this is only used in Linux Kernel, unused in Windows\r\n"
                    "\r\n"
                    "static          TWThreadExitFunc    sExitThreadFunc;\r\n"
                    "};",
                    "class TWThreadMgr TWCLASS_MAKE_FINAL {\r\n"
                    "\r\n"
                    "    public:\r\n"
                    "\r\n"
                    "            [[nodiscard]]\r\n"
                    "        static          TWThreadExitFunc    SetExitThreadFunc           (TWThreadExitFunc pExitThreadFunc) noexcept;\r\n"
                    "\r\n"
                    "private:\r\n"
                    "\r\n"
                    "                    [[noreturn]]\r\n"
                    "    static                  void                    PrivateExitThread           () noexcept;\r\n"
                    "\r\n"
                    "                        [[noreturn]]\r\n"
                    "            static  void                PrivateDefaultExitProcess   () noexcept;\r\n"
                    "\r\n"
                    "    private:\r\n"
                    "\r\n"
                    "                        [[maybe_unused]]\r\n"
                    "        static  TInt32      sExitValue;        // this is only used in Linux Kernel, unused in Windows\r\n"
                    "\r\n"
                    "static          TWThreadExitFunc            sExitThreadFunc;\r\n"
                    "};",
                    Style);
    }

    TEST_F(TWClangFormatTest, Comments) {
    
    FormatStyle Style = getTWStyle();

        verifyFormat("// Callback func to receive network notifications\r\n"
                    "\r\n"
                    "int a;        // jkl\r\n"
                    "\r\n"
                    "/*\r\n"
                    " * sdfle\r\n"
                    " * sdf\r\n"
                    " */\r\n"
                    "\r\n"
                    "bool\r\n"
                    "TWInternalEventsKernelWindows::CallRegister () noexcept\r\n"
                    "{\r\n"
                    "\r\n"
                    "    // Foreground and background events doesn't require any Registration\r\n"
                    "    // Just need to add WM_ACTIVATEAPP switch case and check value\r\n"
                    "\r\n"
                    "        int a;\r\n"
                    "\r\n"
                    "    /*\r\n"
                    "     * sdfle\r\n"
                    "     * sdf\r\n"
                    "     */\r\n"
                    "\r\n"
                    "    a = 8;\r\n"
                    "\r\n"
                    "    // sdf\r\n"
                    "\r\n"
                    "    return true;\r\n"
                    "}",
                    "// Callback func to receive network notifications\r\n"
                    "int a;        // jkl\r\n"
                    "\r\n"
                    "/*\r\n"
                    "* sdfle\r\n"
                    "* sdf\r\n"
                    "*/\r\n"
                    "bool\r\n"
                    "TWInternalEventsKernelWindows::CallRegister () noexcept\r\n"
                    "{\r\n"
                    "\r\n"
                    "    // Foreground and background events doesn't require any Registration\r\n"
                    "    // Just need to add WM_ACTIVATEAPP switch case and check value\r\n"
                    "    int a;\r\n"
                    "\r\n"
                    "    /*\r\n"
                    "     * sdfle\r\n"
                    "     * sdf\r\n"
                    "     */\r\n"
                    "    a = 8;\r\n"
                    "\r\n"
                    "    // sdf\r\n"
                    "    return true;\r\n"
                    "}",
                    Style);
        }

} // namespace
} // namespace test
} // namespace format
} // namespace clang