//---------------------------------------------------------------------------

#ifndef EditCenterH
#define EditCenterH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TEditCenter : public TEdit
{
private:
protected:
void __fastcall CreateParams(TCreateParams & Params);
public:
	__fastcall TEditCenter(TComponent* Owner);
__published:
};
//---------------------------------------------------------------------------
#endif
