// CompilerNode.h - a node that manages a compiler executable
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "FileNode.h"
#include "Tools/FBuild/FBuildCore/Helpers/ToolManifest.h"

// Forward Declarations
//------------------------------------------------------------------------------
class BFFIterator;
class Function;

// CompilerNode
//------------------------------------------------------------------------------
class CompilerNode : public Node
{
    REFLECT_NODE_DECLARE( CompilerNode )
public:
    explicit CompilerNode();
    virtual bool Initialize( NodeGraph & nodeGraph, const BFFIterator & iter, const Function * function ) override;
    virtual ~CompilerNode() override;

    virtual bool IsAFile() const override;

    static inline Node::Type GetTypeS() { return Node::COMPILER_NODE; }

    inline const ToolManifest & GetManifest() const { return m_Manifest; }

    inline bool SimpleDistributionMode() const { return m_SimpleDistributionMode; }
    inline bool GetUseLightCache() const { return m_UseLightCache; }
    inline bool CanBeDistributed() const { return m_AllowDistribution; }
    #if defined( __WINDOWS__ )
        inline bool IsVS2012EnumBugFixEnabled() const { return m_VS2012EnumBugFix; }
    #endif
    inline bool IsClangRewriteIncludesEnabled() const { return m_ClangRewriteIncludes; }

    enum CompilerFamily : uint8_t
    {
        CUSTOM          = 0,
        MSVC            = 1,
        CLANG           = 2,
        GCC             = 3,
        SNC             = 4,
        CODEWARRIOR_WII = 5,
        GREENHILLS_WIIU = 6,
        CUDA_NVCC       = 7,
        QT_RCC          = 8,
        VBCC            = 9,
        ORBIS_WAVE_PSSLC= 10,
    };
    CompilerFamily GetCompilerFamily() const { return static_cast<CompilerFamily>( m_CompilerFamilyEnum ); }

    const AString & GetExecutable() const { return m_StaticDependencies[ 0 ].GetNode()->GetName(); }
    const AString & GetExtraFile( size_t index ) const { return m_StaticDependencies[ index + 1 ].GetNode()->GetName(); }
    const char * GetEnvironmentString() const;

private:
    bool InitializeCompilerFamily( const BFFIterator & iter, const Function * function );

    virtual BuildResult DoBuild( Job * job ) override;
    virtual void Migrate( const Node & oldNode ) override;

    // Exposed params
    AString                 m_Executable;
    Array< AString >        m_ExtraFiles;
    Array< AString >        m_CustomEnvironmentVariables;
    bool                    m_AllowDistribution;
    bool                    m_VS2012EnumBugFix;
    bool                    m_ClangRewriteIncludes;
    AString                 m_ExecutableRootPath;
    AString                 m_CompilerFamilyString;
    uint8_t                 m_CompilerFamilyEnum;
    bool                    m_SimpleDistributionMode;
    bool                    m_UseLightCache;
    ToolManifest            m_Manifest;
    Array< AString >        m_Environment;

    // Internal state
    mutable const char *    m_EnvironmentString;
};

//------------------------------------------------------------------------------
