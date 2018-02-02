//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EditCenter.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck wird benutzt, um sicherzustellen, daﬂ die erzeugten Komponenten
// keine rein virtuellen Funktionen besitzen.
//

static inline void ValidCtrCheck(TEditCenter *)
{
	new TEditCenter(NULL);
}
//---------------------------------------------------------------------------
__fastcall TEditCenter::TEditCenter(TComponent* Owner)
	: TEdit(Owner)
{
}
//---------------------------------------------------------------------------
namespace Editcenter
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TEditCenter)};
		RegisterComponents("Standard", classes, 0);
	}
}
//---------------------------------------------------------------------------

void __fastcall TEditCenter::CreateParams(TCreateParams & Params)
{
TEdit::CreateParams(Params);
Params.Style |= ES_CENTER;
}
