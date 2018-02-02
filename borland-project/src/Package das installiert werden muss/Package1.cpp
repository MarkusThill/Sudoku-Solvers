//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("Package1.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("EditCenter.cpp");
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Package-Quelltext.
//---------------------------------------------------------------------------

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
	return 1;
}
//---------------------------------------------------------------------------
