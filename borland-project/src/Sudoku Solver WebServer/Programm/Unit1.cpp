//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "EditCenter"
#pragma link "Trayicon"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	/*Im Konstruktor werden alle Elemente des Arrays feld[x][y] und feldtemp[x][y]auf 0 gesetzt,
    auch alle Elemente des Arrays moeglichkeiten[x][y][z] werden auf 0 gesetzt!*/
	int i, j,k;
    for(i=0;i<9;i++)
    	for(j=0;j<9;j++)
        {
        	feld[i][j]=0;
            feldtemp[i][j]=0;
            for(k=0;k<9;k++)
            	moeglichkeiten[i][j][k]=0;
        }
}

//----------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
	/*Beim Ereignis OnFormCreate werden die 9 Panels und die 81 Editfelder erzeugt!!!
    Die Panels und die Editfelder werden in Form von Arrays zur Laufzeit erstellt,
    wobei die Edit-Felder zweidimensional sind (Edit[x][y]). Zu beachten ist, dass
    die Edit-Felder eine abgeleitete Klasse(TEditCenter) von TEdit sind.*/
    int i=0, k=0, j=0, abstand_links_panel=50, abstand_panel_oben =40,panel_breite=120, panel_hoehe=120;
    int abstand_edit_links=5, abstand_edit_oben=5,edit_breite=32, edit_hoehe=32, panel_nr;
    for(i=0;i<3;i++)
    {
    	for(j=0;j<3;j++)
        {
    		Panel[k]= new TPanel(Form1);
        	Panel[k]->Left = abstand_links_panel + (i*panel_breite)+(i);
        	Panel[k]->Top = abstand_panel_oben + (j*panel_hoehe)+(j);
        	Panel[k]->Width = panel_breite;
        	Panel[k]->Height = panel_hoehe;
        	Panel[k]->Visible = true;
        	Panel[k]->Enabled = true;
        	Panel[k]->ParentBiDiMode = true;
        	Panel[k]->Parent = Form1;
            k++;
        }
    }
    for(j=0;j<9;j++)
    {
    	for(i=0;i<9;i++)
        {
        	panel_nr=((i/3)%3)*3+((j/3)%3);
        	Edit[i][j] = new TEditCenter(Panel[panel_nr]);
            Edit[i][j]->Left = abstand_edit_links*(i%3)+abstand_edit_links+2 + (i%3)*edit_breite;
      		Edit[i][j]->Top = abstand_edit_oben*(j%3)+abstand_edit_oben+2 + (j%3)*edit_hoehe;
            Edit[i][j]->Visible = true;
      		Edit[i][j]->Enabled = true;
      		Edit[i][j]->ParentBiDiMode=true;
      		Edit[i][j]->Parent = Panel[panel_nr];
            Edit[i][j]->Font->Size = 19;
            Edit[i][j]->Font->Style = Edit[i][j]->Font->Style << fsBold;
            Edit[i][j]->Font->Name = "Verdana";
            Edit[i][j]->Width = edit_breite;
        	Edit[i][j]->Height = edit_hoehe;
            Edit[i][j]->MaxLength=1;
            Edit[i][j]->BorderStyle=bsNone;
            Edit[i][j]->OnChange = FontBlack;
        }
    }
    //Datei1->InsertNewLineAfter(Datei1->Items[3]); //Trennlinien einfügen
    //Datei1->InsertNewLineAfter(Datei1->Items[8]);
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void __fastcall TForm1::Neu1Click(TObject *Sender)
{
	/*Wenn ein neues Sudoku gestartet werden soll, dann wird dies erst mit einer
    Messagebox überprüft. Falls der User bestätigt, werden alle EditFelder gelöscht*/
	int result,i,j;
    result=MessageDlgPos("Möchten Sie ein neues Sudoku starten?",mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel,0, Form1->Left+90, Form1->Top+200);
    if(result==mrNo || result==mrCancel)
    	return;
    else
    {
    	for(i=0;i<9;i++)
        	for(j=0;j<9;j++)
            	Edit[i][j]->Text="";
        SaveDialog1->FileName="";
        OpenDialog1->FileName="";
        Form1->Caption="Sudoku Solver 1.0";
    }
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void __fastcall TForm1::Beenden1Click(TObject *Sender)
{
	/*Es wird die Methode Close ausgeführt. Dadurch wird das Programm geschlossen.
    Es wird aber noch das Ereignis OnCloseQuery ausgelöst, dass eine Bestätigunf abfragt!!!*/
	Form1->Close();
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void __fastcall TForm1::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	/*Wird beim Ereignis OnCloseQuery ausgeführt. Es wird vor dem Schließen des Programms
    noch abgefragt, ob das Programm auch wirklich beendet werden soll. Falls das Programm
    nich beendet werden soll, wird CanClose = false geseztzt und das Schließen wird somit verhindert*/
	int result;
    result=MessageDlgPos("Soll das Programm beendet werden?",mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel,0, Form1->Left+90, Form1->Top+200);
    if(result==mrCancel || result==mrNo)
    	CanClose = false;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

Boolean TForm1::EditPruefen(void)
{
	/*Es wird überprüft, ob in den Edit-Feldern gültige Werte(1-9) stehen. Wenn alles
    in Ordnung ist, dann wird ein True zurückgegeben. Falls sich in einem der Felder
    ein falsches Zeichen befindet, wird ein false zurückgegeben!!! Es wird auch geprüft,
    ob genügend Ziffern eingegeben wurden(min. 17), ist dies nicht der Fall, dann wird auch
    false zurückgegeben!!!*/
	int i, j, count=0;
    Boolean ret=true;
    for(i=0;i<9;i++)
    	for(j=0;j<9;j++)
        	if(Edit[i][j]->Text!="")
            {
            	count++;
            	if(Edit[i][j]->Text[1]<49 || Edit[i][j]->Text[1]>57)
                	ret=false;
            }
    if(count<17)
    	ret=false;
    return ret;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

Boolean TForm1::AnordnungPruefenReihe(void)
{
	/*Es wird geprüft, ob eine Zahl öfters in einer Reihe vorkommt. Ist dies der Fall,
    dann werden die betroffenen Ziffern rot markiert und ein false zurückgegeben*/
	int i,j,k,l,m,count,temp[18],p;
	for(i=0;i<9;i++)
		for(k=1;k<10;k++)
		{
        	for(p=0;p<18;p++)
    			temp[p]=-1;
            p=-1;
			count=0;
			for(j=0;j<9;j++)
				if(feld[j][i]==k)
                {
                	temp[++p]=j;
                    temp[++p]=i;
					count++;
                }
			if(count>1)
            {
            	for(p=0;p<18;p+=2)
                	if(temp[p]>=0)
                    	Edit[temp[p]][temp[p+1]]->Font->Color=clRed;
                    else
                    	break;
				return false;
            }
		}
        return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

Boolean TForm1::AnordnungPruefenSpalte(void)
{
	/*Es wird geprüft, ob eine Zahl öfters in einer Spalten vorkommt. Ist dies der Fall,
    dann werden die betroffenen Ziffern rot markiert und ein false zurückgegeben*/
	int i,j,k,l,m,count,temp[18],p;
	for(i=0;i<9;i++)
		for(k=1;k<10;k++)
		{
        	for(p=0;p<18;p++)
    			temp[p]=-1;
            p=-1;
			count=0;
			for(j=0;j<9;j++)
				if(feld[i][j]==k)
                {
                	temp[++p]=i;
                    temp[++p]=j;
					count++;
                }
			if(count>1)
			{
            	for(p=0;p<18;p+=2)
                	if(temp[p]>=0)
                    	Edit[temp[p]][temp[p+1]]->Font->Color=clRed;
                    else
                    	break;
				return false;
            }
		}
        return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

Boolean TForm1::AnordnungPruefenFeld(void)
{
	/*Es wird geprüft, ob eine Zahl öfters in einem 3x3-Feld vorkommt. Ist dies der Fall,
    dann werden die betroffenen Ziffern rot markiert und ein false zurückgegeben*/
     int i,j,k,l,m,count,temp[18],p;
     for(i=0;i<9;i=i+3)
		for(j=0;j<9;j=j+3)
			for(m=1;m<10;m++)
			{
            	for(p=0;p<18;p++)
    				temp[p]=-1;
                p=-1;
				count=0;
				for(k=i;k<i+3;k++)
					for(l=j;l<j+3;l++)
						if(feld[l][k]==m)
                        {
                        	temp[++p]=l;
                    		temp[++p]=k;
							count++;
                        }
				if(count>1)
				{
            		for(p=0;p<18;p+=2)
                		if(temp[p]>=0)
                    		Edit[temp[p]][temp[p+1]]->Font->Color=clRed;
                    	else
                    		break;
					return false;
            	}
			}
     return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

Boolean TForm1::AnordnungPruefen(void)
{
	/*Es wird geprüft, ob eine Ziffer mehrmals in einer Reihe, einer Spalte, oder einem 3x3-Feld vorkommt. Ist dies der Fall,
	dann wird ein false zurückgegebenm ansonsten ein true.*/

    if(!AnordnungPruefenReihe())
    	return false;
    if(!AnordnungPruefenSpalte())
    	return false;
    if(!AnordnungPruefenFeld())
    	return false;
	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void __fastcall TForm1::FontBlack(TObject *Sender)
{
	int i, j;
    for(i=0;i<9;i++)
    	for(j=0;j<9;j++)
        	Edit[i][j]->Font->Color=clBlack;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void __fastcall TForm1::Berechnen1Click(TObject *Sender)
{
	/*Es wird zunächst überprüft, ob gültige und genügend Werte eingegeben wurden.
    Ist dies nicht der Fall, dann wird keine Berechnung ausgeführt. Ansonsten wird
    die Berechnung gestartet. Falls die Berechnung nicht erfolgreich war, wird der Benutzer
    gefragt, ob die Ausgangswerte wieder in die Edit-Felder eingetragen werden sollen. Dadurch
    kann der Nuzer mögliche Fehleingaben korrigieren. Zusätzlich wird auch noch die Dauer der
    Berechnung ermittelt. Wenn bereits alle Felder gefüllt sind, dann wird die Berechnung
    nicht gestartet!!!*/
	int i,j,result;
	if(!EditPruefen())
    {
    	MessageDlgPos("Sie haben keine gültigen oder nicht genügend Werte eingegeben!!!",mtInformation, TMsgDlgButtons() << mbOK,0, Form1->Left+30, Form1->Top+200);
        return;
    }
    else
    {
    	if(!FeldVoll())
        {
    		Startzeit=Time();
        	EditTofeld();
        	if(!AnordnungPruefen())
    		{
    			MessageDlgPos("Es wurde eine Ziffer mehrfach in einem Bereich gefunden. Bitte überprüfen Sie Ihre Eingabe!",mtInformation, TMsgDlgButtons() << mbOK,0, Form1->Left-20, Form1->Top+200);
        		return;
    		}
        	for(j=0;j<9;j++)
	        	for(i=0;i<9;i++)
            		feldtemp[i][j]=feld[i][j];
    		HauptBerechner();
    		Ausgabe();
        	ZeitmessungStop();
        	if(!LoesungPruefen())
        	{
    			result=MessageDlgPos("Das Sudoku konnte nicht erfolgreich berechnet werden.\n Wahrscheinlich wurden falsche Werte eingegeben. \nMöchten Sie das Sudoku bearbeiten?",mtInformation, TMsgDlgButtons() << mbYes << mbNo,0, Form1->Left+50, Form1->Top+200);
            	if(result==mrYes)
            	{
                	Ausgangssudoku1Click(Berechnen1);
            		/*for(j=0;j<9;j++)
                		for(i=0;i<9;i++)
                    		feld[i][j]=feldtemp[i][j];
                	Ausgabe(); */
            	}
        	}
    	}
    }
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void TForm1::ZeitmessungStop(void)
{
	/*Nach der Berechnung wird die Zeit gestoppt. Bei der Berechnung wurde die Startzeit
    ermittelt. Es wird die Startzeit von der Stop-Zeit abgezogen und das Ergebnis in MSec
    ausgegeben!!!*/
	Word h, min, sec, msec;
    TDateTime Stopzeit,Differenz;
	Stopzeit=Time();
    Differenz=Stopzeit-Startzeit;
    DecodeTime(Differenz, h, min, sec, msec);
    Gesamtzeit=(sec*1000+msec);
    Label2->Caption= "Zeit: "+(AnsiString)Gesamtzeit+" MSec";
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void TForm1::EditTofeld(void)
{
	/*Schreibt die Werte der Edit-Felder in das Array feld[x][y], dass später für
    alle Berechnungen genutzt wird. Wenn ein Edit-Feld leer ist, dann wird in das
    Array feld[x][y] "0" eingetragen!!!*/
	int i, j;
    for(j=0;j<9;j++)
    	for(i=0;i<9;i++)
        {
        	if(Edit[i][j]->Text=="")
            	feld[i][j]=0;
            else
        		feld[i][j]=StrToInt(Edit[i][j]->Text);
        }
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void TForm1::Ausgabe(void)
{
	/*Gibt das Array feld[x][y] in die Edit-Felder aus. Wenn feld[x][y]=0 dann wird
    das Edit-Feld auf "" gesetzt.*/
	int i, j;
    for(j=0;j<9;j++)
    	for(i=0;i<9;i++)
        {
        	if(feld[i][j]!=0)
        		Edit[i][j]->Text=(AnsiString)feld[i][j];
            else
            	Edit[i][j]->Text="";
        }
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

Boolean TForm1::FeldVoll(void)
{
	/*Überprüft, ob das Array feld[x][y] voll ist(also kein Element=0 ist).
    Falls alle Felder gesetzt sind, wird ein true zurückgegeben, ansonsten false*/
	int i, j;
    for(i=0;i<9;i++)
    	for(j=0;j<9;j++)
        	if(feld[i][j]==0)
            	return false;
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void TForm1::FeldStartwerte(int spalte, int reihe,int &xbeginn, int &ybeginn)
{
	/*Es wird eine X- und Y-Koordinate übergeben. Diese Methode ermittelt dann
    die X- und Y-Koordinate, des ersten Feldes, das im 3x3-Quadrat der übergebenen
    Werte liegt*/
	if(spalte>=0 && spalte<=2)
	{
		xbeginn=0;
		if(reihe>=0 && reihe<=2)
			ybeginn=0;
		if(reihe>=3 && reihe<=5)
			ybeginn=3;
		if(reihe>=6 && reihe<=8)
			ybeginn=6;
        return;
	}
	if(spalte>=3 && spalte<=5)
	{
		xbeginn=3;
		if(reihe>=0 && reihe<=2)
			ybeginn=0;
		if(reihe>=3 && reihe<=5)
			ybeginn=3;
		if(reihe>=6 && reihe<=8)
			ybeginn=6;
        return;
	}
	if(spalte>=6 && spalte<=8)
	{
		xbeginn=6;
		if(reihe>=0 && reihe<=2)
			ybeginn=0;
		if(reihe>=3 && reihe<=5)
			ybeginn=3;
		if(reihe>=6 && reihe<=8)
			ybeginn=6;
        return;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

Boolean TForm1::SpaltenSucher(int spalte,int ziffer)
{
	/*Sucht in der Spalte vom feld[x][y], die übergeben wird, nach der Ziffer, die
    ebenfalls übergeben wird. Wird die Ziffer in der Spalte gefunden, dann
    wird ein True zurückgegeben, ansonsten ein False!*/
	int i;
    Boolean ret=false;
	for(i=0;i<9 && ret==0;i++)
		if(feld[spalte][i]==ziffer)
        {
			ret=true;
            break;
        }
	return ret;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

Boolean TForm1::ReihenSucher(int reihe,int ziffer)
{
	/*Sucht in der Reihe vom feld[x][y], die übergeben wird, nach der Ziffer, die
    ebenfalls übergeben wird. Wird die Ziffer in der Reihe gefunden, dann
    wird ein True zurückgegeben, ansonsten ein False!*/
	int i;
    Boolean ret=false;
	for(i=0;i<9 && ret==0;i++)
		if(feld[i][reihe]==ziffer)
        {
			ret=true;
            break;
        }
	return ret;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

Boolean TForm1::DreixDreiSucher(int reihe, int spalte, int ziffer)
{
	/*Sucht in dem 3x3-Quadrat von dem übergebenen X-Wert und übergebenem Y-Wert
    nach einer Ziffer, die ebenfalls übergeben wird. Wird die Ziffer gefunden, dann
    wird ein True zurückgegeben, ansonsten ein True!*/
	int xbeginn, ybeginn,i,j;
    Boolean ret=false;
	FeldStartwerte(spalte,reihe,xbeginn,ybeginn);
	for(i=ybeginn;i<ybeginn+3 && ret==false;i++)
		for(j=xbeginn;j<xbeginn+3 && ret==false;j++)
			if(feld[j][i]==ziffer)
            {
				ret=true;
                break;
            }
	return ret;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

Boolean TForm1::Sucher(int reihe,int spalte, int ziffer)
{
	/*Fasst die Methoden Spaltensucher, Reihensucher und DreixDreiSucher zusammen.
    Dieser Methode wird eine X- und eine Y-Koordinate übergeben und eine Ziffer.
    Es wird dann in der betreffenden Spalte, in der betreffenden Reihe und in dem betreffenden
    3x3-Feld nach dieser Ziffer gesucht. Wird die Ziffer gefunden, dann wird ein True
    zurückgegeben, ansonsten ein False!*/
	if(SpaltenSucher(spalte,ziffer))
		return true;
	if(ReihenSucher(reihe,ziffer))
		return true;
	if(DreixDreiSucher(reihe,spalte,ziffer))
		return true;
    return false;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void TForm1::Gesamtmoeglichkeiten(void)
{
	/*Sucht nach allen Möglichkeiten, die ein leeres Feld haben kann und speichert
    diese Möglichkeiten im Array moeglichkeiten[x][y][z] ab!!!*/
	int i,j,k;
	for(i=0;i<9;i++)
		for(j=0;j<9;j++)
			for(k=1;k<10;k++)
				if(feld[j][i]==0)
                {
					if(Sucher(i,j,k)==0)
						moeglichkeiten[j][i][k-1]=k;
                    else
                    	moeglichkeiten[j][i][k-1]=0;
                }
                else
                	moeglichkeiten[j][i][k-1]=0;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

Boolean TForm1::Analytisch(void)
{
	/*Es wird versucht das Sudoku analytisch zu lösen, also mithilfe der der Methoden
    Nakedpairs, Single und HiddenSingle. Kann das Sudoku nicht mehr weiter analytisch
    gelöst werden, dann wird ein False zurückgegeben und es wird in die Methode Hauptberechner
    zurückgekehrt*/
	int x, y, z;
	Boolean gemacht=false;
        do //Single() wird in einer Schleife ausgeführt, da in der Regel direkt mehrere Singles hintereinander gefunden werden
        {
        	Gesamtmoeglichkeiten();
            Nakedpairs();
        	Single(y,x,z);
        	if(y>-1 && x>-1 && z>-1)
			{
				feld[x][y]=z;
                gemacht=true;
			}
        }
        while(y>-1 && x>-1 && z>-1);
        Gesamtmoeglichkeiten();
        Nakedpairs();
        HiddenSingle(y, x, z);
		if(y>-1 && x>-1 && z>-1)
		{
			feld[x][y]=z;
            gemacht=true;
		}
    return gemacht;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

Boolean TForm1::HauptBerechner(void)
{
	/*Als erstes wird die Methode Analytisch aufgerufen, die versucht das Sudoku
    analytisch zu lösen. Klappt dies nicht, dann wird eine Ziffer für das
    Feld geraten und dann wieder versucht das Feld analytisch zu lösen. Klappt
    dies auch nicht, dann wird für das Feld eine andere Ziffer geraten*/
    Boolean beenden=false, gemacht;
    do
    {
    	gemacht=Analytisch();
        if(FeldVoll()) //Wenn alle Felder besetzt sind
        {
        	if(LoesungPruefen())//Stimmt denn die Loesung
            	beenden=true; // Wenn ja, dann kann die Methode beendet werden
            else
            	return false; //Wenn nicht, dann wird ein false an die höhere Ebene zurückgegeben
        }
        if(!gemacht) //Wenn das Sudoku nicht weiter analytisch gelöst werden kann
        {
        	int i,j,k,min,anzahl,x,y;
            int tempfeld[9][9];
            Boolean weitermachen=true;
            for(min=1;min<9;min++)
            	for(i=0;i<9;i++)
            		for(j=0;j<9;j++)
                    {
                		if(feld[j][i]==0)
                        {
                        	anzahl=MoeglichkeitenAnzahl(j,i);
                            if(anzahl==0)
                            	return false;
                    		if(anzahl==min)
                            	for(k=0;k<9;k++)
                                	if(moeglichkeiten[j][i][k]>0)
                                    {
                                    	weitermachen=false;
                                        for(y=0;y<9;y++)
                                        	for(x=0;x<9;x++)
                                            	tempfeld[x][y]=feld[x][y];
                                    	feld[j][i]=moeglichkeiten[j][i][k];
                                        if(HauptBerechner())
                                        	return true;
                                        for(y=0;y<9;y++)
                                        	for(x=0;x<9;x++)
                                            	feld[x][y]=tempfeld[x][y];
                                        Gesamtmoeglichkeiten();
                                    }
                    	}
                        if(!weitermachen)
                        	return false;
                    }
    	}
    }
    while(!beenden);
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void TForm1::Single(int &reihe, int &spalte, int &ziffer)
{
	/*Sucht nach einem leeren Feld, das nur eine Möglichkeit hat. Wenn ein Feld
    nur eine Möglichkeit hat(z.B. 5) dann wird der Wert(Ziffer) und die X- und
    die Y-Koordinate des Feldes zurückgegeben!*/
	int i=0,j=0,k=0,anzahl=0;
	reihe=-1;
	spalte=-1;
	ziffer=-1;
	for(i=0;i<9 && anzahl!=1;i++)
		for(j=0;j<9 && anzahl!=1;j++)
			for(k=0, anzahl=0;k<9;k++)
				if(feld[j][i]==0 && moeglichkeiten[j][i][k]>0)
				{
					anzahl++;
					reihe=i;
					spalte=j;
					ziffer=moeglichkeiten[j][i][k];
					if(anzahl>1)
					{
						reihe=-1;
						spalte=-1;
						ziffer=-1;
					}
				}
	if(anzahl>1)
	{
		reihe=-1;
		spalte=-1;
		ziffer=-1;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void TForm1::HiddenSingleSpalte(int &reihe, int &spalte, int &ziffer)
{
	/* Es wird nach einem Hidden-Single-Element in den Spalten gesucht. Wird eins
    gefunden, dann wird die Ziffer, die X- und die Y-Koordinate zurückgegeben!!!*/
	int i,j,k,anzahl;
	for(i=0;i<9 && anzahl!=1;i++)
		for(j=0;j<9 && anzahl!=1;j++)
			for(k=0,anzahl=0;k<9;k++)
				if(moeglichkeiten[i][k][j]>0)
				{
					anzahl++;
					reihe=k;
					spalte=i;
					ziffer=moeglichkeiten[i][k][j];
					if(anzahl>1)
					{
						reihe=-1;
						spalte=-1;
						ziffer=-1;
					}
				}
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void TForm1::HiddenSingleReihe(int &reihe, int &spalte, int &ziffer)
{
	/* Es wird nach einem Hidden-Single-Element in den Reihen gesucht. Wird eins
    gefunden, dann wird die Ziffer, die X- und die Y-Koordinate zurückgegeben!!!*/
	int i,j,k,anzahl;
	for(i=0;i<9 && anzahl!=1;i++)
		for(j=0;j<9 && anzahl!=1;j++)
			for(k=0, anzahl=0;k<9;k++)
				if(moeglichkeiten[k][i][j]>0)
				{
					anzahl++;
					reihe=i;
					spalte=k;
					ziffer=moeglichkeiten[k][i][j];
					if(anzahl>1)
					{
						reihe=-1;
						spalte=-1;
						ziffer=-1;
					}
				}
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void TForm1::HiddenSingleFeld(int &reihe, int &spalte, int &ziffer)
{
	/* Es wird nach einem Hidden-Single-Element in den 3x3-Feldern gesucht. Wird eins
    gefunden, dann wird die Ziffer, die X- und die Y-Koordinate zurückgegeben!!!*/
	int i,j,k,l,anzahl,zahl;
	for(i=0;i<9 && anzahl!=1;i+=3)
		for(j=0;j<9 && anzahl!=1;j+=3)
			for(zahl=0;zahl<9 && anzahl!=1;zahl++)
				for(k=i, anzahl=0;k<i+3;k++)
					for(l=j;l<j+3;l++)
						if(moeglichkeiten[l][k][zahl]>0)
						{
							anzahl++;
							reihe=k;
							spalte=l;
							ziffer=moeglichkeiten[l][k][zahl];
							if(anzahl>1)
							{
								reihe=-1;
								spalte=-1;
								ziffer=-1;
							}
						}
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void TForm1::HiddenSingle(int &reihe, int &spalte, int &ziffer)
{
	/*Fasst die Methoden HiddenSingleReihe, HiddenSingleSpalte und HiddenSingleFeld
    zusammen. Es wird daher im gesamten Sudoku-Feld nach einem Hidden-Single-Element
    gesucht und das erste gefundene wird zurückgegeben!*/
	int x=-1,y=-1,zahl=-1;
	HiddenSingleSpalte(y, x, zahl);
	if(y!=-1)
	{
		reihe=y;
		spalte=x;
		ziffer=zahl;
	}
	HiddenSingleReihe(y, x, zahl);
	if(y!=-1)
	{
		reihe=y;
		spalte=x;
		ziffer=zahl;
	}
	HiddenSingleFeld(y, x, zahl);
	if(y!=-1)
	{
		reihe=y;
		spalte=x;
		ziffer=zahl;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

int TForm1::MoeglichkeitenAnzahl(int x,int y)
{
	/*Ermittelt, wie viele Möglichkeiten ein leeres Feld mit einer X- und einer
    Y-Koordinate hat. Die Anzahl der Moeglichkeiten für ein Feld ist auch der
    Rückgabewert*/
	int i, anzahl=0;
	for(i=0;i<9;i++)
		if(moeglichkeiten[x][y][i]>0 && feld[x][y]==0)
			anzahl++;
	return anzahl;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

Boolean TForm1::MoeglichkeitenVergleich(int x1,int y1,int x2,int y2)
{
	/*Vergleicht die Möglichkeiten von zwei leeren Feldern. Die Koordinaten der
    leeren Felder werden als Parameter übergeben. Wenn beide Felder genau dieselben
    Möglichkeiten haben, dann wird ein True zurückgegeben, ansonsten ein False!*/
	int i;
	for(i=0;i<9;i++)
		if(moeglichkeiten[x1][y1][i]!=moeglichkeiten[x2][y2][i])
			return false;
	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void TForm1::NakedpairsReihe(void)
{
	/*Sucht nach Naked-Pairs in den Reihen. Wird ein sogenanntes Naked-Pair gefunden,
    dann werden die beiden Ziffern als Möglichkeiten in den anderen leeren Feldern der
    Reihe entfernt!*/
	int i,j,k;
	for(i=0;i<9;i++)
		for(j=0;j<9;j++)
			if(MoeglichkeitenAnzahl(i,j)==2)
				for(k=i+1;k<9;k++)
					if(MoeglichkeitenVergleich(i,j,k,j))
						NakedpairsReiheEliminieren(i,j);
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void TForm1::NakedpairsSpalte(void)
{
	/*Sucht nach Naked-Pairs in den Spalten. Wird ein sogenanntes Naked-Pair gefunden,
    dann werden die beiden Ziffern als Möglichkeiten in den anderen leeren Feldern der
    Reihe entfernt!*/
	int i,j,k;
	for(i=0;i<9;i++)
		for(j=0;j<9;j++)
			if(MoeglichkeitenAnzahl(i,j)==2)
				for(k=j+1;k<9;k++)
					if(MoeglichkeitenVergleich(i,j,i,k))
						NakedpairsSpalteEliminieren(i,j);
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void TForm1::NakedpairsFeld(void)
{
	/*Sucht nach Naked-Pairs in den 3x3-Feldern. Wird ein sogenanntes Naked-Pair gefunden,
    dann werden die beiden Ziffern als Möglichkeiten in den anderen leeren Feldern des
    3x3-Feldes entfernt!*/
	int i=0,j=0,k=0,l=0,xbeginn=0,ybeginn=0;
	for(i=0;i<9;i++)
		for(j=0;j<9;j++)
			if(MoeglichkeitenAnzahl(i,j)==2)
			{
				FeldStartwerte(i,j,xbeginn,ybeginn);
				if((i+1)%3==0)
				{
					l=j+1;
					k=xbeginn;
				}
				else
				{
					k=i+1;
					l=j;
				}
				if(l<9)
				{
					for(l;l<ybeginn+3;l++)
						for(k;k<xbeginn+3;k++)
							if(MoeglichkeitenVergleich(i,j,k,l))
								NakedpairsFeldEliminieren(i,j);
				}
			}
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void TForm1::NakedpairsReiheEliminieren(int x,int y)
{
	/*Die Moeglcihkeiten des Naked-Pairs werden in den anderen Feldern der Reihe
    entfernt*/
	int i=0,j=0,k=0;
	for(i=0;i<9;i++)
		if(!MoeglichkeitenVergleich(x,y,i,y))
			for(j=0;j<9;j++)
				for(k=0;k<9;k++)
					if(moeglichkeiten[i][y][k]==moeglichkeiten[x][y][j])
						moeglichkeiten[i][y][k]=0;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void TForm1::NakedpairsSpalteEliminieren(int x,int y)
{
	/*Die Moeglichkeiten des Naked-Pairs werden in den anderen Feldern der Spalte
    entfernt*/
	int i=0,j=0,k=0;
	for(i=0;i<9;i++)
		if(!MoeglichkeitenVergleich(x,y,x,i))
			for(j=0;j<9;j++)
				for(k=0;k<9;k++)
					if(moeglichkeiten[x][i][k]==moeglichkeiten[x][y][j])
						moeglichkeiten[x][i][k]=0;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void TForm1::NakedpairsFeldEliminieren(int x, int y)
{
	/*Die Moeglichkeiten des Naked-Pairs werden in den anderen Feldern des
    3x3-Feldes entfernt*/
	int i=0,j=0,k=0,l=0,xbeginn=0,ybeginn=0;
	FeldStartwerte(x,y,xbeginn,ybeginn);
	for(j=ybeginn;j<ybeginn+3;j++)
		for(i=xbeginn;i<xbeginn+3;i++)
			if(!MoeglichkeitenVergleich(x,y,i,j))
				for(k=0;k<9;k++)
					for(l=0;l<9;l++)
						if(moeglichkeiten[x][y][k]==moeglichkeiten[i][j][l])
							moeglichkeiten[i][j][l]=0;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void TForm1::Nakedpairs(void)
{
	/*Es wurden die Methoden NakedpairsReihe, NakedpairsSpalte und
	NakedpairsFeld zusammengefasst!*/
	NakedpairsReihe();
	NakedpairsSpalte();
	NakedpairsFeld();
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

Boolean TForm1::LoesungPruefen(void)
{
	/*Es wird geprüft, ob das gelöste Sudoku gültig ist. Es wird geprüft, ob die
    Ziffern 1-9 in allen Spalten, in allen Reihen und in allen 3x3-Feldern vorkommen.
    Ist dies der Fall, dann wird ein True zurückgegeben, ansonsten ein False.*/
	int i,j,k,l,m,n,ziffern[9];
	for(i=0;i<9;i++)
	{
		for(n=0;n<9;n++)
			ziffern[n]=0;
		for(k=1;k<10;k++)
			for(j=0;j<9;j++)
				if(feld[i][j]==k)
					ziffern[k-1]=1;
		for(n=0;n<9;n++)
			if(ziffern[n]==0)
				return false;
	}
	for(i=0;i<9;i++)
	{
		for(n=0;n<9;n++)
			ziffern[n]=0;
		for(k=1;k<10;k++)
			for(j=0;j<9;j++)
				if(feld[j][i]==k)
					ziffern[k-1]=1;
		for(n=0;n<9;n++)
			if(ziffern[n]==0)
				return false;
	}
	for(i=0;i<9;i+=3)
		for(j=0;j<9;j+=3)
		{
			for(n=0;n<9;n++)
				ziffern[n]=0;
			for(m=1;m<10;m++)
				for(k=i;k<i+3;k++)
					for(l=j;l<j+3;l++)
						if(feld[k][l]==m)
							ziffern[m-1]=1;
			for(n=0;n<9;n++)
				if(ziffern[n]==0)
					return false;
		}
	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void __fastcall TForm1::ffnen1Click(TObject *Sender)
{
	/*Es wird als erstes der OpenDialog ausgeführt. Wird eine Datei ausgewählt, dann
    wird diese Datei geöffnet, die Werte ausgelesen und dann ins Array feld[x][y] geschrieben!!
    Anschließend erfolgt eine Ausgabe!!!*/
	int i=0,x=0,y=0;
	ifstream file;
    char zeile[20];
	if(OpenDialog1->Execute())
    {
    	SaveDialog1->FileName=OpenDialog1->FileName;
        Form1->Caption="Sudoku Solver 1.0  "+SaveDialog1->FileName;
    	file.open(OpenDialog1->FileName.c_str(), ios::in);
        x=0; y=0;
        while(!file.eof())
		{
        	file >> zeile;
            for(i=0;i<13;i++)
            {
            	if(zeile[i]=='.')
                	feld[x++][y]=0;
                if(zeile[i]>48 && zeile[i]<58)
                	feld[x++][y]=zeile[i]-48;
                if(x>8)
                {
                	x=0; y++;
                }
            }
        }
        Ausgabe();
    }
    file.close();
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void __fastcall TForm1::Speichern1Click(TObject *Sender)
{
	/*Wenn die Datei noch nicht gespeichert wurde, dann wird der SaveDialog ausgeführt.
    Anschließend wird die Datei geöffnet und die Werte des Arrays feld[x][y] im festgelegten Format
    in die Datei geschrieben!! Zum Schluss wird die Datei geschlossen!!*/
	int i, j,k;
	char temp[20];
	ofstream file;
    if(SaveDialog1->FileName=="")
    	if(!SaveDialog1->Execute())
        	return;
        else
        	Form1->Caption="Sudoku Solver 1.0  "+SaveDialog1->FileName;
    file.open(SaveDialog1->FileName.c_str(), ios::out);
    file<<"*-----------*\n";
    k=0;
    EditTofeld();
    for(i=0;i<9;i++)
    {
    	for(j=0;j<9;j++)
        {
        	if(j%3==0)
            	temp[k++]='|';
        	if(feld[j][i]>0)
        		temp[k++]=feld[j][i]+48;
            else
                temp[k++]='.';
        }
        temp[k++]='|';
        temp[k++]='\n';
        temp[k]=0;
        file<<temp;
        k=0;
        if(i==2 || i==5)
        	file<<"|---+---+---|\n";
    }
    file<<"*-----------*\n";
    file.close();
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void __fastcall TForm1::SpeichernUnter1Click(TObject *Sender)
{
	/*Wenn im Menü Speichern Unter angelickt, dann wird der SaveDialog ausgeführt,
    damit eine neue Datei ausgewählt werden kann und anschließend wird die Methode
    Speichern1Click() ausgeführt, die die Datei speichert*/
	if(SaveDialog1->Execute())
    {
    	Form1->Caption="Sudoku Solver 1.0  "+SaveDialog1->FileName;
    	Speichern1Click(SpeichernUnter1);
    }
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void TForm1::Bruteforce(int intx,int inty)
{
	/*Diese Methode zaehlt mithilfe der Bruteforce-Methode(Es werden alle Möglichkeiten
    durchgegangen) wieviele Loesungen ein Sudoku hat. Dabei wird die Methode von sich
    selber aufgerufen(rekursiv). Damit der Rechenvorgang nicht zu lange dauert wird zusätzlich
    die MEthode Analytisch() aufgerufen, die bei der Loesung hilft!!!*/
	int x,y,k, tempfeld[9][9],i,j;
    x=intx; y=inty;
    for(k=0;k<9 && anzahl<2000 && feld[x][y]==0;k++)  //&&feld[x][y]==0 verringert die Berechnungszeit um bis zu 10 Sek
    {
    	Gesamtmoeglichkeiten();
    	if(moeglichkeiten[x][y][k]>0)
        {
        	feld[x][y]=moeglichkeiten[x][y][k];
            for(i=0;i<9;i++)
            	for(j=0;j<9;j++)
                	tempfeld[j][i]=feld[j][i];
            while(Analytisch()); //verringert die Berechnungzeit drastisch
            if(x<8)
            	Bruteforce(x+1,y);
            else
            {
            	if(y<8)
            		Bruteforce(0,y+1);
            }
            if(LoesungPruefen())
            {
                anzahl++;
                ProgressBar1->StepIt();
            }
            for(i=0;i<9;i++)
            	for(j=0;j<9;j++)
                	feld[j][i]=tempfeld[j][i];
            feld[x][y]=0;
        }
    }
    if(feld[x][y]>0)
    {
    	if(x<8)
        	Bruteforce(x+1,y);
        else
        	if(y<8)
            	Bruteforce(0,y+1);
    }
    return;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void __fastcall TForm1::Lsungprfen1Click(TObject *Sender)
{
	/*Wenn im Menü auf "Anzahl der Loesungen zählen" geklickt wird, dann wird diese
    Methode aufgerufen. Es wird zunächst geprüft, ob gültige Werte eingegeben wurden, anschließend
    wird die Methode Bruteforce() aufgerufen, die die Anzahl der Loesungen ermittelt. Die Anzahl wird
    anschließend zurückgegeben!!*/
	if(!EditPruefen())
    {
    	MessageDlgPos("Sie haben keine gültigen oder nicht genügend Werte eingegeben!!!",mtInformation, TMsgDlgButtons() << mbOK,0, Form1->Left+30, Form1->Top+200);
        return;
    }
    else
    {
    	EditTofeld();
    	if(!AnordnungPruefen())
    	{
    		MessageDlgPos("Es wurde eine Ziffer mehrfach in einem Bereich gefunden. Bitte überprüfen Sie Ihre Eingabe!",mtInformation, TMsgDlgButtons() << mbOK,0, Form1->Left-20, Form1->Top+200);
        	return;
    	}
    	anzahl=0;
		Gesamtmoeglichkeiten();
    	ProgressBar1->Position=0;
    	ProgressBar1->Visible=true;
        Startzeit=Time();
		Bruteforce(0,0);
        ZeitmessungStop();
    	ProgressBar1->Visible=false;
    	if(anzahl==1)
    		MessageDlgPos("Ihr Sudoku hat eine Loesung!",mtInformation, TMsgDlgButtons() << mbOK,0, Form1->Left+100, Form1->Top+200);
    	else if(anzahl<2000)
    		MessageDlgPos("Ihr Sudoku hat "+(AnsiString)anzahl+" Loesungen!",mtInformation, TMsgDlgButtons() << mbOK,0, Form1->Left+100, Form1->Top+200);
    	else
    		MessageDlgPos("Ihr Sudoku hat über 2000 Loesungen!",mtInformation, TMsgDlgButtons() << mbOK,0, Form1->Left+100, Form1->Top+200);
    }
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void __fastcall TForm1::Ausgangssudoku1Click(TObject *Sender)
{
	/*Wenn Ausgangssudoku im Menü angeklickt wird, dann wird das Array feld[x][y]
    auf die Startwerte zurückgesetzt und ausgegeben!!!*/
	int i,j;
	for(j=0;j<9;j++)
		for(i=0;i<9;i++)
			feld[i][j]=feldtemp[i][j];
    Ausgabe();
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void __fastcall TForm1::FertigeLsungberprfen1Click(TObject *Sender)
{
	/*Falls ein Benutzer eine fertige Loesung von einem Sudoku hat und überprüfen möchte,
    ob es korrekt ist, dann kann er die vollständige Loesung eingeben und das Programm
    überprüft anschließend, ob die Lösung korrekt ist!!!*/
	int i,j,count=0;
    for(i=0;i<9;i++)
    	for(j=0;j<9;j++)
        {
        	if(Edit[i][j]->Text!="")
            {
            	if(Edit[i][j]->Text[1]<49 || Edit[i][j]->Text[1]>57)
                {
                	MessageDlgPos("Sie haben keine gültigen Werte eingegeben!!!",mtInformation, TMsgDlgButtons() << mbOK,0, Form1->Left+80, Form1->Top+200);
                	return;
                }
                else
                	count++;
            }
        }
        if(count<81)
        {
        	MessageDlgPos("Sie haben nicht genügend Werte eingegeben!!!",mtInformation, TMsgDlgButtons() << mbOK,0, Form1->Left+80, Form1->Top+200);
            return;
        }
    EditTofeld();
    if(LoesungPruefen())
    	MessageDlgPos("Ihre Loesung ist korrekt!",mtInformation, TMsgDlgButtons() << mbOK,0, Form1->Left+160, Form1->Top+200);
    else
    	MessageDlgPos("Ihre Loesung ist NICHT korrekt!",mtInformation, TMsgDlgButtons() << mbOK,0, Form1->Left+140, Form1->Top+200);
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------


/*------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------*/
/*Die nachfolgenden Methoden sind allesamt für die Loesung mit einer Datenbank gedacht*/
/*------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------*/


void __fastcall TForm1::Button1Click(TObject *Sender)
{
	int i;
	/*Wenn auf den Button "Datenbank Start gedrückt wird, dann wird diese Methode ausgeführt.
    Dadurch wird ein Timer gestartet wodurch die Methode DatenbankBerechner jede Sekunde prüft, ob ein
    noch nicht gelöstes Sudoku in der Datenbank vorhanden ist!!! Wenn nochmals auf den Button gedrückt
    wird, wird der Timer wieder deaktiviert. Läuft der Timer kann auch kein Sudoku gelöst werden, dass
    in diesem Programm von Handeingegeben wurde!!!*/
	if(Timer1->Enabled)
    {
    	Timer1->Enabled=false;
        Button1->Caption="Datenbank Start";
        ButtonBerechnen->Enabled=true;//Man darf auf den Button Berechnen drücken
        ButtonNeu->Enabled=true;
        for(i=0;i<9;i++)//Im Menü alles aktivieren
        	Datei1->Items[i]->Enabled=true;
    }
    else
    {
    	Timer1->Enabled=true;
        Button1->Caption="Datenbank Stop";
        ButtonBerechnen->Enabled=false;//Man darf NICHT auf den Button Berechnen drücken
        ButtonNeu->Enabled=false;
        for(i=0;i<9;i++)//Im Menü alles bis auf beenden deaktivieren
        	Datei1->Items[i]->Enabled=false;
    }
	
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	/*Wenn der Timer nach jeder Sekunde abgelaufen ist, dann wird die Methode
    DatenbankBerechner gestartet und anschließend der Timer neu gestartet!*/
	DatenbankBerechner();
    Timer1->Interval=1000;
    Timer1->Enabled=true;

}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void TForm1::DatenbankBerechner(void)
{
	int i=0, j=0,k=1;
	AnsiString Start, Loesung, Zeit, query;
    Timer1->Enabled=false;
    Query1->Close();
    Query1->SQL->Clear();
	Query1->SQL->Add("SELECT startsudoku FROM sudoku WHERE ID = (SELECT MIN(ID) FROM sudoku WHERE loesungsudoku IS NULL)"); //Die Unterabfrage ist dafür da, dass nur eine Lösung zurückgeliefert wird!
	Query1->Open();
    Query1->First();
    Start="";
    Start = Query1->FieldByName("startsudoku")->AsString;
    if(Start!="")//Wenn ein noch nicht gelöstes Sudoku gefunden wird dann...
    {
    	for(i=0;i<9;i++)        //String aus der Datenbank in Array umwandeln
    		for(j=0;j<9;j++)
        		feld[j][i]= StrToInt(Start[k++]);

    	Startzeit=Time(); //Loesung des Sudokus
    	HauptBerechner();
    	Ausgabe();
    	ZeitmessungStop();

    	k=1;
    	Loesung=Start;//Damit keine Zugriffsverletzung auftritt
    	for(i=0;i<9;i++)//Loesung des Sudokus von Array in String umwandeln
    		for(j=0;j<9;j++)
        		Loesung[k++]=(char)feld[j][i]+48;
        if(!FeldVoll())
        	Loesung="error";
    	Zeit=(AnsiString)Gesamtzeit;
    	Query1->Close();
		Query1->SQL->Clear();
    	query="UPDATE sudoku SET loesungsudoku='"+Loesung+"', "+"zeit="+Zeit+" WHERE startsudoku='"+Start+"'";
		Query1->SQL->Add(query); //Die Loesung des Sudokus und die Loesungszeit in die Datenbank schreiben
		Query1->ExecSQL();
    }
    Timer1->Enabled=true;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

/*Die nachfolgenden Methoden, sind dafür, dass ins Tray minmiert werden kann*/
void __fastcall TForm1::InsTrayminimieren1Click(TObject *Sender)
{
	/*Es wird die Möglichkeit ins Tray zu minimieren entweder aktiviert, oder deaktiviert*/
	if(!InsTrayminimieren1->Checked)
    {
		InsTrayminimieren1->Checked=true;
    	TrayIcon1->Visible=true;
    }
    else
    {
		InsTrayminimieren1->Checked=false;
    	TrayIcon1->Visible=false;
    }
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void __fastcall TForm1::Wiederherstellen1Click(TObject *Sender)
{
	/*Falls im Pop-Up-Menü für das Icon im Tray auf wiederherstellen gedrückt wird, dann
    wird das Programm wieder angzeigt*/
	TrayIcon1->Restore();	
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

void __fastcall TForm1::Beenden2Click(TObject *Sender)
{
	/*Falls im Pop-Up-Menü für das Icon im Tray auf Beenden gedrückt wird, dann
    wird das Programm beendet*/
	Form1->Close();	
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

