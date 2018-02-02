//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "EditCenter.h"
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include "Trayicon.h"
#include <ImgList.hpp>
#include <fstream>
using namespace std;
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TLabel *Label1;
	TButton *ButtonNeu;
	TButton *ButtonBerechnen;
	TMainMenu *MainMenu1;
	TMenuItem *Datei1;
	TMenuItem *Neu1;
	TMenuItem *Berechnen1;
	TMenuItem *Lsungprfen1;
	TMenuItem *Beenden1;
	TLabel *Label2;
	TMenuItem *ffnen1;
	TMenuItem *Speichern1;
	TSaveDialog *SaveDialog1;
	TOpenDialog *OpenDialog1;
	TMenuItem *SpeichernUnter1;
	TProgressBar *ProgressBar1;
	TMenuItem *Ausgangssudoku1;
	TMenuItem *FertigeLsungberprfen1;
	TTimer *Timer1;
	TDataSource *DataSource1;
	TQuery *Query1;
	TButton *Button1;
	TMenuItem *N1;
	TMenuItem *N2;
	TTrayIcon *TrayIcon1;
	TPopupMenu *PopupMenu1;
	TImageList *ImageList1;
	TMenuItem *Wiederherstellen1;
	TMenuItem *N3;
	TMenuItem *Beenden2;
	TMenuItem *InsTrayminimieren1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Neu1Click(TObject *Sender);
	void __fastcall Beenden1Click(TObject *Sender);
	void __fastcall Berechnen1Click(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall ffnen1Click(TObject *Sender);
	void __fastcall Speichern1Click(TObject *Sender);
	void __fastcall SpeichernUnter1Click(TObject *Sender);
	void __fastcall Lsungprfen1Click(TObject *Sender);
	void __fastcall Ausgangssudoku1Click(TObject *Sender);
	void __fastcall FertigeLsungberprfen1Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall InsTrayminimieren1Click(TObject *Sender);
	void __fastcall Wiederherstellen1Click(TObject *Sender);
	void __fastcall Beenden2Click(TObject *Sender);
private:
	TEditCenter *Edit[9][9]; //Array der Eingabefelder
    TPanel *Panel[9]; //Array von Panels, auf denen die Edit-Felder posiotioniert werden
    int feld[9][9];   //Haupt-Array, in dem sich die einzelnen Ziffern des Sudokus befinden
    int feldtemp[9][9];//Das Ausgabgssudoku wird hier gespeichert, sodass es nach einer Berechnung ernuet geladen werden kann
    int moeglichkeiten[9][9][9]; //Array in dem die Möglichkeiten eines Feldes gespeichert werden
    int anzahl;//Für die Methode Bruteforce, die die Anzahl der Loesungen zählt
    TDateTime Startzeit;
    int Gesamtzeit;
public:
    void ZeitmessungStop(void);
	Boolean EditPruefen(void);
    Boolean AnordnungPruefenReihe(void);
    Boolean AnordnungPruefenSpalte(void);
    Boolean AnordnungPruefenFeld(void);
    Boolean AnordnungPruefen(void);
    void __fastcall FontBlack(TObject *Sender);
    void EditTofeld(void);
    void Ausgabe(void);
    Boolean Analytisch(void);
    Boolean HauptBerechner(void);
    Boolean FeldVoll(void);
    void FeldStartwerte(int spalte, int reihe,int &xbeginn, int &ybeginn);
    Boolean SpaltenSucher(int spalte,int ziffer);
    Boolean ReihenSucher(int reihe,int ziffer);
    Boolean DreixDreiSucher(int reihe, int spalte, int ziffer);
    Boolean Sucher(int reihe,int spalte, int ziffer);
    void Gesamtmoeglichkeiten(void);
    void Single(int &reihe, int &spalte, int &ziffer);
    void HiddenSingleSpalte(int &reihe, int &spalte, int &ziffer);
    void HiddenSingleReihe(int &reihe, int &spalte, int &ziffer);
    void HiddenSingleFeld(int &reihe, int &spalte, int &ziffer);
    void HiddenSingle(int &reihe, int &spalte, int &ziffer);
    int MoeglichkeitenAnzahl(int x,int y);
    Boolean MoeglichkeitenVergleich(int x1,int y1,int x2,int y2);
    void NakedpairsReihe(void);
    void NakedpairsSpalte(void);
    void NakedpairsFeld(void);
    void NakedpairsReiheEliminieren(int x,int y);
    void NakedpairsSpalteEliminieren(int x,int y);
    void NakedpairsFeldEliminieren(int x, int y);
    void Nakedpairs(void);
    Boolean LoesungPruefen(void);
    void Bruteforce(int inti,int intj);
	__fastcall TForm1(TComponent* Owner);
    void DatenbankBerechner(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
