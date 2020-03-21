// ------------------------------------------------------------------------------------------------
#include "Library/System/Dir.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(TypenameD, _SC("SqSysDir"))
SQMODE_DECL_TYPENAME(TypenameF, _SC("SqSysFile"))

// ------------------------------------------------------------------------------------------------
LightObj SysDir::ReadFile() const
{
    Validate("read current file");
    // Create a file instance
    std::unique_ptr< SysFile > mem = std::make_unique< SysFile >();
    // Turn it into a script object
    LightObj obj(mem.get());
    // Will hold the raw object pointer
    SysFile * ptr = nullptr;
    // Release it if it was taken over by the script engine
  	if (obj.IsNull())
  	{
  		STHROWF("Failed to create a SqSysFile object.");
  	}
  	else
  	{
  		ptr = mem.release();
  	}
  	// The file handle where it will be opened
  	tinydir_file * handle = ptr->GetOrMake();
  	// Attempt to read the current file
  	if (tinydir_readfile(mHandle.get(), handle) == -1)
  	{
  		STHROWF("Failed to read current file.");
  	}
  	// Return the resulted object
  	return obj;
}
// ------------------------------------------------------------------------------------------------
LightObj SysDir::ReadFileAt(SQInteger i) const
{
    Validate("read scanned file");
    // Make sure the specified directory index is valid
    if (i < 0)
    {
        STHROWF("File index (" PRINT_INT_FMT " < 0) our of bounds.", i);
    }
    if (static_cast< size_t >(i) >= mHandle->n_files)
    {
        STHROWF("File index (" PRINT_INT_FMT " >= " PRINT_SZ_FMT ") our of bounds.", i, mHandle->n_files);
    }
    // Create a file instance
    std::unique_ptr< SysFile > mem = std::make_unique< SysFile >();
    // Turn it into a script object
    LightObj obj(mem.get());
    // Will hold the raw object pointer
    SysFile * ptr = nullptr;
    // Release it if it was taken over by the script engine
  	if (obj.IsNull())
  	{
  		STHROWF("Failed to create a SqSysFile object.");
  	}
  	else
  	{
  		ptr = mem.release();
  	}
  	// The file handle where it will be opened
  	tinydir_file * handle = ptr->GetOrMake();
  	// Attempt to read the current file
  	if (tinydir_readfile_n(mHandle.get(), handle, i) == -1)
  	{
  		STHROWF("Failed to read file at index (" PRINT_INT_FMT ")", i);
  	}
  	// Return the resulted object
  	return obj;
}

// ================================================================================================
void Register_SysDir(HSQUIRRELVM vm)
{
	RootTable(vm).Bind(TypenameD::Str,
	    Class< SysDir, NoCopy< SysDir > >(vm, TypenameD::Str)
		// Constructors
        .Ctor()
        .Ctor< StackStrF & >()
        .Ctor< bool, StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &TypenameD::Fn)
        .Func(_SC("_tostring"), &SysDir::ToString)
        // Member Properties
        .Prop(_SC("IsValid"), &SysDir::IsValid)
        .Prop(_SC("Path"), &SysDir::GetPath)
        .Prop(_SC("HasNext"), &SysDir::HasNext)
        .Prop(_SC("FileCount"), &SysDir::FileCount)
        // Member Methods
        .FmtFunc(_SC("Open"), &SysDir::Open)
        .FmtFunc(_SC("OpenSorted"), &SysDir::OpenSorted)
        .Func(_SC("OpenSubDir"), &SysDir::OpenSubDir)
        .Func(_SC("Next"), &SysDir::Next)
        .Func(_SC("Close"), &SysDir::Close)
        .Func(_SC("ReadFile"), &SysDir::ReadFile)
        .Func(_SC("ReadFileAt"), &SysDir::ReadFileAt)
	);
	RootTable(vm).Bind(TypenameF::Str,
	    Class< SysFile, NoCopy< SysFile > >(vm, TypenameF::Str)
		// Constructors
        .Ctor()
        .Ctor< StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &TypenameF::Fn)
        .Func(_SC("_tostring"), &SysFile::ToString)
        // Member Properties
        .Prop(_SC("IsValid"), &SysFile::IsValid)
        .Prop(_SC("IsDir"), &SysFile::IsDir)
        .Prop(_SC("IsReg"), &SysFile::IsReg)
        .Prop(_SC("Path"), &SysFile::GetPath)
        .Prop(_SC("Name"), &SysFile::GetName)
        .Prop(_SC("Extension"), &SysFile::GetExtension)
        // Member Methods
        .FmtFunc(_SC("Open"), &SysFile::Open)
	);

}

} // Namespace:: SqMod
