#include <iostream.h>
#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>
#include <dos.h>

int feld[9][9];
int moeglichkeiten[9][9][9];
int moeglichkeiten1[9][9][9];

class sudoku
{   private:
	public:
	sudoku(void);
	void grafikmodus(void);
	void grafik(void);
	void grafikausgabe(void);
	void eingabeauswahl(void);
	void eingabeauswahlgrafik(int,int,int);
	int eingabe(int,int);
	int spaltensucher(int,int);
	int reihensucher(int,int);
	int dreixdreisucher(int,int,int);
	int sucher(int,int,int);
	void moeglichkeitennullsetzer(void);
	void gesamtmoeglichkeiten(void);
	void single(int &reihe, int &spalte, int &ziffer);
	void hiddensinglespalte(int &reihe, int &spalte, int &ziffer);
	void hiddensinglereihe(int &reihe, int &spalte, int &ziffer);
	void hiddensinglefeld(int &reihe, int &spalte, int &ziffer);
	void hiddensingle(int &reihe, int &spalte, int &ziffer);
	int moeglichkeitenanzahl(int,int);
	int moeglichkeitenvergleich(int,int,int,int);
	void nakedpairsreihe(void);
	void nakedpairsreiheeleminieren(int,int);
	void nakedpairsspalte(void);
	void nakedpairsspalteeleminieren(int,int);
	void nakedpairsfeld(void);
	void nakedpairsfeldeleminieren(int,int);
	void nakedpairs(void);
	void feldstartwerte(int,int,int &xbeginn, int &ybeginn);
	void blockedcandidates(void);
	int blockedcandidatessucher(void);
	void hauptberechner(void);
	void bruteforcehaupt(void);
	int bruteforce(int,int);
	int loesungpruefen(void);
};

void sudoku::grafikmodus(void)
{
	int gdriver = DETECT, gmode, errorcode;
	initgraph(&gdriver, &gmode, "C:\\progra~1\\TC\\BGI\\");
	errorcode = graphresult();
	if (errorcode != grOk)
	{
		cout << "Graphics error: %s\n"<< grapherrormsg(errorcode) << endl;
		cout << "Press any key to halt:" << endl;
		getch();
		exit(1);
	}
}

sudoku::sudoku(void)
{
	int i=0,j=0;
	for(i=0;i<9;i++)
		for(j=0;j<9;j++)
		{
			feld[i][j]=0;
			for(int k=0;k<9;k++)
			{
				moeglichkeiten[i][j][k]=0;
				moeglichkeiten1[i][j][k]=0;
			}
		}
}

void sudoku::grafik(void)
{
	setbkcolor(7);
	setcolor(BLUE);
	settextstyle(1,HORIZ_DIR,4);
	outtextxy(250,10,"Sudoku");
	{
		int xstart=215, ystart=60,i;
		setfillstyle(1,BLUE);
		for(i=0;i<200;i+=20)
		{
			bar(xstart+i,ystart,xstart+i+1,ystart+184);
			bar(xstart+1,ystart+i,xstart+184,ystart+i+1);
			if(i==60)
			{
				i+=2;
				setfillstyle(1,YELLOW);
				bar(xstart+i,ystart,xstart+i+1,ystart+184);
				bar(xstart+1,ystart+i,xstart+184,ystart+i+1);
				setfillstyle(1,BLUE);
			}
			if(i==122)
			{
				i+=2;
				setfillstyle(1,YELLOW);
				bar(xstart+i,ystart,xstart+i+1,ystart+184);
				bar(xstart+1,ystart+i,xstart+184,ystart+i+1);
				setfillstyle(1,BLUE);
			}
		}
	}
	settextstyle(1,HORIZ_DIR,1);
	outtextxy(262,280,"Berechnen");
	rectangle(250,275,360,310);
}

void sudoku::grafikausgabe(void)
{
	int xstart=222,ystart=67,sonst1=0,sonst2=0;
	char wert[2];
	setcolor(8);
	settextstyle(0,HORIZ_DIR,1);
	for(int i=0;i<9;i++)
	{
		if(i==3 || i==6)
			sonst1+=2;
		sonst2=0;
		for(int j=0;j<9;j++)
		{
			if(j==3 || j==6)
				sonst2+=2;
			if(feld[i][j]>0)
			{
				sprintf(wert,"%d",feld[i][j]);
				outtextxy(xstart+(i*20)+sonst1,ystart+(j*20)+sonst2,wert);
			}
		}
	}
}

void sudoku::eingabeauswahl(void)
{
	int eingabebyte1=0, eingabebyte2=0, i=0,j=0;
	grafikmodus();
	grafik();
	eingabeauswahlgrafik(i,j,1);
	while(eingabebyte1!=13 || j!=9)
	{
		if(kbhit())
		{
			eingabebyte1=getch();
			if(eingabebyte1==0)
			{
				eingabebyte2=getch();
				if(eingabebyte2==72 && j>0)
					j--;
				else if(eingabebyte2==80 && j<9)
					j++;
				else if(eingabebyte2==75 && i>0 && j!=9)
					i--;
				else if(eingabebyte2==77 && i<8 && j!=9)
					i++;
				eingabeauswahlgrafik(i,j,1);
			}
			if(eingabebyte1==13)
			{
				if(j!=9)
				{
					eingabeauswahlgrafik(i,j,2);
					feld[i][j]=eingabe(i,j);
					eingabeauswahlgrafik(i,j,2);
				}
				else if(j==9)
				{
					hauptberechner();
					bruteforcehaupt();
				}
				eingabeauswahlgrafik(i,j,1);
				grafikausgabe();
			}
		}
	}
}

void sudoku::eingabeauswahlgrafik(int i,int j,int modus)
{
	int x,y,k,l,xstart=215,ystart=60,sonst1=0,sonst2=0;
	static int wert=0;
	x=i*20;
	y=j*20;
	if(i>2)
	{
		x+=2;
		if(i>5)
			x+=2;
	}
	if(j>2)
	{
		y+=2;
		if(j>5)
			y+=2;
	}
	x+=2;
	y+=2;
	if(modus==1)
	{
		setcolor(7);
		for(k=0;k<9;k++)
		{
			if(k==3 || k==6)
				sonst1+=2;
			sonst2=0;
			for(l=0;l<9;l++)
			{
				if(l==3 || l==6)
				sonst2+=2;
				rectangle(xstart+(k*20)+2+sonst1,ystart+(l*20)+2+sonst2,xstart+(k*20)+19+sonst1,ystart+(l*20)+19+sonst2);
			}
		}
		setcolor(BLUE);
		rectangle(250,275,360,310);
		setcolor(RED);
		if(j!=9)
			rectangle(x+xstart,y+ystart,x+xstart+17,y+ystart+17);
		else
			rectangle(250,275,360,310);
	}
	else if(modus==2)
	{
		if(wert==0)
		{
			setfillstyle(1,WHITE);
			wert=1;
		}
		else if(wert==1)
		{
			setfillstyle(1,7);
			wert=0;
		}
		bar(x+xstart,y+ystart,x+xstart+17,y+ystart+17);
	}
}

int sudoku::eingabe(int a, int j)
{
	int i=0, wert=0,erg=0,xposition=0,yposition=0,xstart=215,ystart=60;
	char zwi[2];
	zwi[0]=0;
	zwi[1]=0;
	xposition=a*20;
	yposition=j*20;
	if(a>2)
	{
		xposition+=2;
		if(a>5)
			xposition+=2;
	}
	if(j>2)
	{
		yposition+=2;
		if(j>5)
			yposition+=2;
	}
	xposition+=7;
	yposition+=7;
	settextstyle(0, HORIZ_DIR, 1);
	setcolor(8);
	do
	{
		if(kbhit())
		{
			wert=getch();
			if(wert==0)
				getch();
			else
			{
				if(wert==8 && i>0)//Falls Backspace gedrÅckt wird letztes Zeichen lîschen
				{
					sprintf(zwi,"%c", erg+48);
					setcolor(WHITE);
					outtextxy(xposition-8+xstart, yposition+ystart, zwi); //Das letzte Zeichen mit der Hintergrundfarbe Åberschreiben
					setcolor(8);
					erg=0;
					xposition-=8;
					i--;
				}
				if(wert!=8 && wert!=13 && i<1 && wert>48 && wert<58)//Neues Zeichen in den String einlesen
				{
					erg=wert-48;
					sprintf(zwi,"%c", erg+48);
					setcolor(8);
					outtextxy(xposition+xstart, yposition+ystart, zwi); //Das neue Zeichen auf dem Bildschirm ausgeben
					xposition+=8;
					i++;
				}
			}
		}
	}
	while(wert!=13);
	return erg;
}

int sudoku::spaltensucher(int spalte,int ziffer)
{
	int i=0, ret=0;
	for(i=0;i<9 && ret==0;i++)
		if(feld[spalte][i]==ziffer)
			ret=1;
	return ret;
}

int sudoku::reihensucher(int reihe,int ziffer)
{
	int i=0, ret=0;
	for(i=0;i<9 && ret==0;i++)
		if(feld[i][reihe]==ziffer)
			ret=1;
	return ret;
}

int sudoku::dreixdreisucher(int reihe, int spalte, int ziffer)
{
	int ret=0, xbeginn=0, ybeginn=0,i=0,j=0;
	feldstartwerte(spalte,reihe,xbeginn,ybeginn);
	for(i=ybeginn;i<ybeginn+3 && ret==0;i++)
		for(j=xbeginn;j<xbeginn+3 && ret==0;j++)
			if(feld[j][i]==ziffer)
				ret=1;
	return ret;
}

int sudoku::sucher(int reihe,int spalte, int ziffer)
{
	int ret=0;
	if(spaltensucher(spalte,ziffer))
		ret=1;
	if(reihensucher(reihe,ziffer))
		ret=1;
	if(dreixdreisucher(reihe,spalte,ziffer))
		ret=1;
	return ret;
}

void sudoku::moeglichkeitennullsetzer(void)
{
	int i,j,k;
	for(i=0;i<9;i++)
		for(j=0;j<9;j++)
			for(k=0;k<9;k++)
				moeglichkeiten[i][j][k]=0;
}

void sudoku::gesamtmoeglichkeiten(void)
{
	int i=0,j=0,k=0;
	moeglichkeitennullsetzer();
	for(i=0;i<9;i++)
		for(j=0;j<9;j++)
			for(k=1;k<10;k++)
				if(feld[j][i]==0)
					if(sucher(i,j,k)==0 && moeglichkeiten1[j][i][k-1]!=-1)
						moeglichkeiten[j][i][k-1]=k;
}

void sudoku::single(int &reihe, int &spalte, int &ziffer)
{
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

void sudoku::hiddensinglespalte(int &reihe, int &spalte, int &ziffer)
{
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

void sudoku::hiddensinglereihe(int &reihe, int &spalte, int &ziffer)
{
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

void sudoku::hiddensinglefeld(int &reihe, int &spalte, int &ziffer)
{
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

void sudoku::hiddensingle(int &reihe, int &spalte, int &ziffer)
{
	int x,y,zahl;
	hiddensinglespalte(y, x, zahl);
	if(y!=-1)
	{
		reihe=y;
		spalte=x;
		ziffer=zahl;
	}
	hiddensinglereihe(y, x, zahl);
	if(y!=-1)
	{
		reihe=y;
		spalte=x;
		ziffer=zahl;
	}
	hiddensinglefeld(y, x, zahl);
	if(y!=-1)
	{
		reihe=y;
		spalte=x;
		ziffer=zahl;
	}
}

int sudoku::moeglichkeitenanzahl(int x,int y)
{
	int i=0, anzahl=0;
	for(i=0;i<9;i++)
		if(moeglichkeiten[x][y][i]>0 && feld[x][y]==0)
			anzahl++;
	return anzahl;
}

int sudoku::moeglichkeitenvergleich(int x1,int y1,int x2,int y2)
{
	int i=0,ret=1;
	for(i=0;i<9;i++)
		if(moeglichkeiten[x1][y1][i]!=moeglichkeiten[x2][y2][i])
			ret=0;
	return ret;
}

void sudoku::nakedpairsreihe(void)
{
	int i,j,k;
	for(i=0;i<9;i++)
		for(j=0;j<9;j++)
			if(moeglichkeitenanzahl(i,j)==2)
				for(k=i+1;k<9;k++)
					if(moeglichkeitenvergleich(i,j,k,j)==1)
						nakedpairsreiheeleminieren(i,j);
}

void sudoku::nakedpairsspalte(void)
{
	int i,j,k;
	for(i=0;i<9;i++)
		for(j=0;j<9;j++)
			if(moeglichkeitenanzahl(i,j)==2)
				for(k=j+1;k<9;k++)
					if(moeglichkeitenvergleich(i,j,i,k)==1)
						nakedpairsspalteeleminieren(i,j);
}

void sudoku::nakedpairsfeld(void)
{
	int i=0,j=0,k=0,l=0,xbeginn=0,ybeginn=0;
	for(i=0;i<9;i++)
		for(j=0;j<9;j++)
			if(moeglichkeitenanzahl(i,j)==2)
			{
				feldstartwerte(i,j,xbeginn,ybeginn);
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
							if(moeglichkeitenvergleich(i,j,k,l)==1)
								nakedpairsfeldeleminieren(i,j);
				}
			}
}

void sudoku::nakedpairsreiheeleminieren(int x,int y)
{
	int i=0,j=0,k=0;
	for(i=0;i<9;i++)
		if(moeglichkeitenvergleich(x,y,i,y)==0)
			for(j=0;j<9;j++)
				for(k=0;k<9;k++)
					if(moeglichkeiten[i][y][k]==moeglichkeiten[x][y][j])
						moeglichkeiten[i][y][k]=0;
}

void sudoku::nakedpairsspalteeleminieren(int x,int y)
{
	int i=0,j=0,k=0;
	for(i=0;i<9;i++)
		if(moeglichkeitenvergleich(x,y,x,i)==0)
			for(j=0;j<9;j++)
				for(k=0;k<9;k++)
					if(moeglichkeiten[x][i][k]==moeglichkeiten[x][y][j])
						moeglichkeiten[x][i][k]=0;
}

void sudoku::nakedpairsfeldeleminieren(int x, int y)
{
	int i=0,j=0,k=0,l=0,xbeginn=0,ybeginn=0;
	feldstartwerte(x,y,xbeginn,ybeginn);
	for(j=ybeginn;j<ybeginn+3;j++)
		for(i=xbeginn;i<xbeginn+3;i++)
			if(moeglichkeitenvergleich(x,y,i,j)==0)
				for(k=0;k<9;k++)
					for(l=0;l<9;l++)
						if(moeglichkeiten[x][y][k]==moeglichkeiten[i][j][l])
							moeglichkeiten[i][j][l]=0;
}

void sudoku::nakedpairs(void)
{
	nakedpairsreihe();
	nakedpairsspalte();
	nakedpairsfeld();
}

void sudoku::feldstartwerte(int spalte, int reihe,int &xbeginn, int &ybeginn)
{
	if(spalte>=0 && spalte<=2)
	{
		xbeginn=0;
		if(reihe>=0 && reihe<=2)
			ybeginn=0;
		if(reihe>=3 && reihe<=5)
			ybeginn=3;
		if(reihe>=6 && reihe<=8)
			ybeginn=6;
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
	}
}

void sudoku::blockedcandidates(void)
{
	int i,j,k;
	for(i=0;i<9;i++)
		for(j=0;j<9;j++)
			if(feld[i][j]==0)
				for(k=0;k<9;k++)
					if(moeglichkeiten[i][j][k]>0)
					{
						feld[i][j]=moeglichkeiten[i][j][k];
						if(blockedcandidatessucher()==1)
						{
							feld[i][j]=0;
							moeglichkeiten1[i][j][k]=-1;
							gesamtmoeglichkeiten();
						}
						feld[i][j]=0;
						gesamtmoeglichkeiten();
					}
	gesamtmoeglichkeiten();
}

int sudoku::blockedcandidatessucher(void)
{
	int i,j,k,l,m,n,ret=0,ziffern[9];
	gesamtmoeglichkeiten();
	nakedpairs();
	for(j=0;j<9;j+=3)
		for(i=0;i<9;i+=3)
		{
			for(n=0;n<9;n++)
				ziffern[n]=0;
			n=0;
			for(m=1;m<10;m++)
			{
				for(l=j;l<j+3;l++)
					for(k=i;k<i+3;k++)
					{
						if(feld[k][l]==m || moeglichkeiten[k][l][m-1])
							ziffern[n]=1;
					}
				n++;
			}
			for(n=0;n<9;n++)
				if(ziffern[n]==0)
					ret=1;
		}
	return ret;
}

void sudoku::hauptberechner(void)
{
	int i=0,x=0,y=0,z=0,gemacht=0,anzahl=0;
	setcolor(BLUE);
	settextstyle(0,HORIZ_DIR,1);
	outtextxy(20,250,"Analyse");
	for(i=0;i<75 && anzahl<2;i++)
	{
		gesamtmoeglichkeiten();
		nakedpairs();
		single(y, x, z);
		gemacht=0;
		if(y>-1 && x>-1 && z>-1)
		{
			feld[x][y]=z;
			grafikausgabe();
			gemacht=1;
			anzahl=0;
		}
		gesamtmoeglichkeiten();
		nakedpairs();
		hiddensingle(y, x, z);
		if(y>-1 && x>-1 && z>-1)
		{
			feld[x][y]=z;
			grafikausgabe();
			gemacht=1;
			anzahl=0;
		}
		if(gemacht==0)
		{
			blockedcandidates();
			anzahl++;
		}
	}
}

void sudoku::bruteforcehaupt(void)
{
	gesamtmoeglichkeiten();
	bruteforce(0,0);
	while(getch()!=13);
}

int sudoku::bruteforce(int inti,int intj)
{
	int i,j,k,ret=0;
	setcolor(BLUE);
	settextstyle(0,HORIZ_DIR,1);
	outtextxy(20,260,"Brute Force");
	i=inti;
	j=intj;
	for(k=0;k<9 && ret==0;k++)
		if(moeglichkeiten[i][j][k]>0)
		{
			feld[i][j]=moeglichkeiten[i][j][k];
			 gesamtmoeglichkeiten();
			if(i<8)
				ret=bruteforce(i+1,j);
			if(i==8)
				ret=bruteforce(0,j+1);
			feld[i][j]=0;
			gesamtmoeglichkeiten();
		}
		if(feld[i][j]>0)
		{
			if(i<8)
				ret=bruteforce(i+1,j);
			if(i==8)
				ret=bruteforce(0,j+1);
		}
		if(feld[8][8]>0)
			if(loesungpruefen()==1)
			{
				grafikausgabe();
				ret=1;
			}
	return ret;
}

int sudoku::loesungpruefen(void)
{
	int i,j,k,l,m,n,ziffern[9],ret=1;
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
				ret=0;
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
				ret=0;
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
					ret=0;
		}
	return ret;
}

int main(void)
{
	sudoku eins;
	eins.eingabeauswahl();
	return 0;
}