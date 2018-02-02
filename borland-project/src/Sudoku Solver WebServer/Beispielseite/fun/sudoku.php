<?php
$user="markus";
$password="markus1";
$db="test";
$host="localhost";

function nullsetzer()
{	/*Setzt alle Elemente des Arrays auf 0*/
	global $feld;
	for($i=0;$i<9;$i++)
		for($j=0;$j<9;$j++)
			$feld[$j][$i]=0;
}

function nullentfernen()
{	
	/*Vor der Ausgabe werden die Nullen in dem Array feld[][] entfernt und durch "" ersetzt, da eine Null
	in einem Feld nicht so besonders aussieht. Es wird aber nicht das Array feld[][] bearbeitet, sondern es wird
	ein Array als Rückgabewert geliefert*/
	global $feld;
	for($i=0;$i<9;$i++)
		for($j=0;$j<9;$j++)
			if($feld[$j][$i]==0)
				$a[$j][$i]="";
			else
				$a[$j][$i]=$feld[$j][$i];
	return $a;
}

function feldsetzer()
{
	/*Es wird eine Array[][] mit den Daten aus dem POST gesetzt und als Rückgabewert zurückgegeben*/
	for($i=0;$i<9;$i++)
		for($j=0;$j<9;$j++)
		{
			if($_POST['feld'][$j][$i]>0)
				$array[$j][$i]=$_POST['feld'][$j][$i];
			else
				$array[$j][$i]=0;
		}
	return $array;
}

function pruefen()
{
	/*Es wird geprüft, ob falsche Werte, nicht genügend Werte eingegeben wurden. Ist dies der Fall, dann
	wird eine Fehlermeldung zurückgegeben. Zusätzlich wird die Funktion anordnungpruefen aufgerufen, die überprüft, ob
	eine Ziffer mehrmals in einem Bereich vorkommt.*/
	global $feld;
	$count=0;
	if($_POST['senden'])
	{
		for($i=0;$i<9;$i++)
			for($j=0;$j<9;$j++)
				if(is_numeric($feld[$i][$j])==false)
					return "Sie haben falsche Werte eingegeben, bitte ändern!!!<br>";
				else
				{
					if($feld[$i][$j]>0)	
						$count++;
				}
		if($count<17)
			return "Sie haben nicht genügend Werte eingegeben, es müssen mindestens 17 Ziffern eingetragen werden!!!";
		if(!anordnungpruefen())
			return "Es wurde eine Ziffer mehrfach in einem Bereich gefunden. Bitte &uuml;berpr&uuml;fen Sie Ihre Eingabe!!!";
	}
	return "";
}

function anordnungpruefen()
{
	/*Es wird geprüft, ob eine Ziffer mehrmals in einer Reihe, einer Spalte, oder einem 3x3-Feld vorkommt. Ist dies der Fall,
	dann wird ein false zurückgegebenm ansonsten ein true.*/
	global $feld;
	for($i=0;$i<9;$i++)
		for($k=1;$k<10;$k++)
		{
			$count=0;
			for($j=0;$j<9;$j++)
				if($feld[$j][$i]==$k)
					$count++;
			if($count>1)
				return false;
		}
	for($i=0;$i<9;$i++)
		for($k=1;$k<10;$k++)
		{
			$count=0;
			for($j=0;$j<9;$j++)
				if($feld[$i][$j]==$k)
					$count++;
			if($count>1)
				return false; 
		}
	for($i=0;$i<9;$i=$i+3)
		for($j=0;$j<9;$j=$j+3)
			for($m=1;$m<10;$m++)
			{
				$count=0;
				for($k=$i;$k<$i+3;$k++)
					for($l=$j;$l<$j+3;$l++)
						if($feld[$l][$k]==$m)
							$count++;
				if($count>1)
					return false;
			}
					
	return true;
}
			
function datenbank()
{
	/*Es wird eine Verbindung zur Datenbank hergestellt. Dann wird das Startsudoku dort eingetragen. Es wird anschließend
	solange gewartet, bis das C++-Programm die Loesung dort eingetragen hat. Diese Loesung wird dann ausgegeben!!!*/
	global $feld;
	global $user;
	global $password;
	global $db;
	$string="";
	for($i=0;$i<9;$i++)//Das Array $feld[x][y] in einen String umwandeln, damit es in die Datenbank eingetragen werden kann
		for($j=0;$j<9;$j++)
			$string=$string.$feld[$j][$i];
	$link=mysql_connect($host,$user,$password); //Zu der MySQL Datenbank verbinden
	if(!$link)
		die("Es konnte keine Verbindung hergestellt werden".mysql_error());
	mysql_select_db($db)
  		or die("Konnte $db nicht öffnen".mysql_error());
	$query = "INSERT INTO sudoku (startsudoku) values('$string')"; //Das Startsudoku in die Datenbak eintragen.
	mysql_query($query, $link)
    	or die("INSERT ERROR: ".mysql_error());
	$abfrage = "SELECT loesungsudoku, zeit from sudoku where startsudoku='$string'"; //die Loesung aus der Datenbank lesen
	do
	{
		sleep(1);
		$erg = mysql_query($abfrage, $link);
		list($loesung, $zeit) = mysql_fetch_row($erg);
	}
	while($loesung=="");//Es wird solange die Abfrage $abfrage ausgeführt, bis eine Loesung in der Datenbank steht!!!
	if($loesung!="error")
	{
		$l=0;
		for($i=0;$i<9;$i++)//Den String der Loesung aus der Datenbank in das Array $feld[x][y] umwandeln!!!
			for($j=0;$j<9;$j++)
    			$feld[$j][$i]=substr($loesung,$l,++$l)+0;
	}
	else
		$zeit="error";
	//$zeit=$zeit+0;
	print $zeit;
	ausgabe($zeit);	//Die Loesung ausgeben!!!	
	mysql_close($link);//Verbindung zur Datenbank beenden!!!
}
				
function ausgabe($zeit)
{
	/*Ausgabe der HTML-Tags*/
global $feld, $feldtemp;
$feld1=nullentfernen();
if($_POST['senden'])//Wenn der Lösung Berechnen Button gedrückt wird, dann wird erst die Eingabe geprüft!!!
	$pruef=pruefen();
if($zeit=="error")
	$zeitausgabe="<b>Das Sudoku konnte nicht gelöst werden!!! Bitte &uuml;berpr&uuml;fen Sie ihre Eingabe!!!</b>";
else if($zeit!="")//Falls die Loesung angezeigt werden soll, soll zusätzlich die Loesungszeit angezeigt werden!!!
	$zeitausgabe="<b>Das Sudoku wurde in ".$zeit." MSec vom Programm gel&ouml;st!</b>";

print "
<html>
<head>
<meta http-equiv=\"Content-Type\"
content=\"text/html; charset=iso-8859-1\">
<meta name=\"robots\" content=\"INDEX,FOLLOW\">
<meta name=\"keywords\"
content=\"homepage,dokument,webpage,page,web,netz,homepage dokument webpage page web net\">
<meta name=\"description\"
content=\"homepage, dokument, webpage, page, web, netz\" >
<title>Eine Homepage von Markus Thill</title>
<!-- Der Copyright-Hinweis ist nicht zu löschen. Ein Entfernen des Copyright/Urheberrecht-Vermerks kann rechtliche Schritte nach sich ziehen -->
<!-- (c)Copyright by S.I.S.Papenburg / www.on-mouseover.de/templates/ -->
<script>
<!--
function BlurLinks(){
lnks=document.getElementsByTagName('a');
for(i=0;i<lnks.length;i++){
lnks[i].onfocus=new Function(\"if(this.blur)this.blur()\");
}
}

onload=BlurLinks;
-->
</script>
<link rel=\"stylesheet\" href=\"../css/format.css\" type=\"text/css\">

<script language=\"JavaScript\">
<!--

/* Hier alle am Mouseover beteiligten Grafiken vorladen*/

bild01= new Image();
bild01.src = \"../images/back.gif\";
bild02= new Image();
bild02.src = \"../images/back_over.gif\";
bild03= new Image();
bild03.src = \"../images/top.gif\";
bild04= new Image();
bild04.src = \"../images/top_over.gif\";


/* usw. für alle weiteren Buttons */

//-->
</script>



<link rel=\"stylesheet\" href=\"../css/datumstyle01.css\" type=\"text/css\">
<script src=\"../datumuhr.js\" type=\"text/javascript\"></script>
<style type=\"text/css\">
<!--
.Stil1 {
	font-size: 14pt;
	font-weight: bold;
}
-->
</style>
</head>
<body  onload=\"window.setTimeout ('datumuhr()',1000);\"  >
<!-- aussen-->
<table border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\"  width=\"100%\" >
<tr>
<td style=\"border:solid 1px black\"><!-- banner-->
<table bgcolor=\"#ffffff\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\"  width=\"100%\" >
<tr>
<td ><table border=\"0\" cellpadding=\"0\" cellspacing=\"1\"  width=\"100%\">
<tr>
<td height=\"22\" background=\"images/a1.gif\" class=\"rub\" bgcolor=\"#eeeeee\"  >&nbsp;diedackel.de</td>
</tr>

<tr>
<td  bgcolor=\"#ffffff\" height=\"82\" style=\"border-top:solid 1px #BFBFBF;border-bottom:solid 1px #BFBFBF\" > <center><a href=\"../index.html\"><img src=\"../images/banner.jpg\" width=\"468\" height=\"60\" border=\"0\"></a>
</center></td>
</tr>

<tr>
<td height=\"22\" width=\"100%\" background=\"../images/a2.gif\"    bgcolor=\"#eeeeee\"align=\"right\" ><table  cellspacing=\"0\" cellpadding=\"0\" border=\"0\" width=\"100%\" align=\"right\" ><tr>
<td align=\"right\" width=\"100%\" ><form  name=\"anzeige\"><input size=10 name=\"date\" class=\"datum\" ></form></td><td>&nbsp;</td><td><form name=\"anzeige2\" ><input type=\"text\" name=\"time\" size=\"10\" class=\"zeit\" ></form></td><td>&nbsp;</td></tr>
</table></td>
</tr>
</table></td>
</tr>
</table>
<!-- ende banner--></td>
</tr>


<tr>
<td align=\"left\">
<table width=\"100%\"  border=\"0\" cellpadding=\"0\" cellspacing=\"0\"  id=\"menu\">
<tr>
<td width=\"213\"><img src=\"../images/nav_back_213px.jpg\" width=\"213\" height=\"30\" border=\"0\" alt=\"\"></td>
<td width=\"3\"><img src=\"../images/seperator.jpg\" width=\"3\" height=\"30\" border=\"0\" alt=\"\"></td>
<td align=\"center\" background=\"../images/nav_back.jpg\" width=\"136\" height=\"30\"><a href=\"../index.html\">Home</a></td>
<td width=\"3\"><img src=\"../images/seperator.jpg\" width=\"3\" height=\"30\" border=\"0\" alt=\"\"></td>
<td align=\"center\" background=\"../images/nav_back.jpg\" width=\"136\" height=\"30\"><a href=\"../schule.html\">Schule</a></td>
<td width=\"3\"><img src=\"../images/seperator.jpg\" width=\"3\" height=\"30\" border=\"0\" alt=\"\"></td>
<td align=\"center\" background=\"../images/nav_back.jpg\" width=\"136\" height=\"30\"><a href=\"../privat.html\">Privat</a></td>
<td width=\"3\"><img src=\"../images/seperator.jpg\" width=\"3\" height=\"30\" border=\"0\" alt=\"\"></td>
<td align=\"center\" background=\"../images/nav_back.jpg\" width=\"136\" height=\"30\"><a href=\"../fun.html\">Fun</a></td>
<td width=\"3\"><img src=\"../images/seperator.jpg\" width=\"3\" height=\"30\" border=\"0\" alt=\"\"></td>
<td align=\"center\" background=\"../images/nav_back.jpg\" width=\"136\" height=\"30\"><a href=\"../impressum.html\">Impressum</a></td>
<td width=\"3\"><img src=\"../images/seperator.jpg\" width=\"3\" height=\"30\" border=\"0\" alt=\"\"></td>
<td width=\"*\" background=\"../images/nav_back.jpg\" >&nbsp;</td>
</tr>
</table>
</td>
</tr>

<tr>
<td  valign=\"top\" style=\"border:solid 1px black\"  ><!-- hauptbereich menü und inhalt-->
<table bgcolor=\"#ffffff\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\"  width=\"100%\" >
<tr>
<td  >
<table border=\"0\" cellpadding=\"0\" cellspacing=\"1\"   width=\"100%\">
<tr>
<td  valign=\"top\" height=\"870\" bgcolor=\"#AFAFAF\"><!-- menüs-->
<table bgcolor=\"black\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\"   >
<tr>
<td  valign=\"top\" style=\"border-left:solid 0px black;border-right:solid 0px black;border-bottom:solid 1px white\"  >
<table   border=\"0\" cellpadding=\"0\" cellspacing=\"0\" id=\"menu1\" >
<tr>
<td  bgcolor=\"#4E6F81\" class=\"rub\"  height=\"21\"   background=\"../images/a1.gif\"  >&nbsp;&nbsp;Fun</td>
</tr>
<tr>
<td    ><a href=\"streichholzspiel.html\"  >&raquo;&nbsp;Streichholzspiel</a></td>
</tr>
<tr>
<td    ><a href=\"sudoku.php\"  >&raquo;&nbsp;Sudoku Solver</a></td>
</tr>
<!--
<tr>
<td ><a href=\"schule.html\" >&raquo;&nbsp;Schule</a></td>
</tr>
<tr>
<td   ><a href=\"privat.html\" >&raquo;&nbsp;Privat</a></td>
</tr>
<tr>
<td  ><a href=\"fun.html\"  >&raquo;&nbsp;Fun</a></td>
</tr>
<tr>
<td   ><a href=\"gaestebuch.html\"  >&raquo;&nbsp;G&auml;stebuch</a></td>
</tr>
<tr>
<td   ><a href=\"kontakt.html\"  >&raquo;&nbsp;Kontakt</a></td>
</tr>
<tr>
<td   ><a href=\"impressum.html\"  >&raquo;&nbsp;Impressum</a></td>
</tr>
-->
</table>
</td>
</tr></table><!-- ende menü 1-->
<br><br>
<center>
<div id=\"links1\">
<!-- X-STAT START : CODE NICHT VERÄNDERN -->
<script language=\"JavaScript\" type=\"text/javascript\" src=\"http://www.x-stat.de/stat.php?id=diedackel\"></script> <a href=\"http://www.x-stat.de/\" target=\"_blank\"><font size=\"1\">X-Stat.de</font></a>
<!-- X-STAT ENDE -->
</div>
</center>
</td>

<td  width=\"100%\" valign=\"top\"  style=\"border-left:solid 1px black;\">
<!-- inhalt -->
<table bgcolor=\"#000000\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\"  width=\"100%\" >
<tr>

<td valign=\"top\"  height=\"100%\">
<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"  width=\"100%\">
<tr>
<td colspan=\"3\"  bgcolor=\"#eeeeee\" class=\"rub\"  height=\"22\"   style=\"border-bottom:solid 1px #bfbfbf;\" background=\"../images/a1.gif\"  >&nbsp;&nbsp;Sudoku</td>
</tr>
<tr>
<td colspan=\"3\" bgcolor=\"#ffffff\"><img src=\"../images/space_trans.gif\" width=\"1\" height=\"9\" border=\"0\" alt=\"\"></td>
</tr>
<tr>
<td bgcolor=\"#ffffff\"><img src=\"../images/space_trans.gif\" width=\"15\" height=\"1\" border=\"0\" alt=\"\"></td>
<td valign=\"top\"  bgcolor=\"#ffffff\" height=\"870\">
<p align=\"center\">
  <!--  inhalt-->
  <span class=\"Stil1\">Sudoku Solver</span></p>
<p>Falls Sie bis jetzt keine Sudokus gesehen haben sollten, hier eine kleine Anleitung:<br>
Ein Sudoku hat 9 Reihen und 9 Spalten und ist zus&auml;tzlich noch in 9 3x3-Felder unterteilt. In der Regel sind knapp 22 bis 36 Felder vorgegeben. Die leeren Felder m&uuml;ssen so ausgef&uuml;llt werden, dass in jeder waagerechten Zeile und in jeder senkrechten Spalte alle Ziffern von 1 bis 9 stehen. Dabei darf auch jedes 3x3-Qudrat nur je einmal die Ziffern 1 bis 9 enthalten. </p>
<p>Dieser Sudoku-Solver sollte alle l&ouml;sbaren Sudokus l&ouml;sen k&ouml;nnen. Bitte geben Sie die bereits vorhanden Ziffern in die Felder ein und klicken Sie anschlie&szlig;end auf L&ouml;sen. Es kann einige Zeit in Anspruch nehmen, bis alle leeren Felder berechnet wurden, je nach Schwierigkeit des Sudokus. <br>
  <br>
  
    <div>
<center>
<p><p>
<form method=\"post\" action=\"$PHP_SELF\">
<TABLE style=\"BORDER-RIGHT: #6d747c 2px solid; BORDER-TOP: #6d747c 2px solid; BORDER-LEFT: #6d747c 2px solid; BORDER-BOTTOM: #6d747c 2px solid\" cellSpacing=\"0\" cellPadding=\"0\" width=\"374\" border=\"0\">
 <tr>
  <TD class=border_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[0][0]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[0][0]."\">
  </TD>
  <TD class=border_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[1][0]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[1][0]."\">
  </TD>
  <TD class=border_right_strong vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[2][0]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[2][0]."\">
  </TD>
  <TD class=border_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[3][0]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[3][0]."\">
  </TD>
  <TD class=border_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[4][0]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[4][0]."\">
  </TD>
  <TD class=border_right_strong vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[5][0]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[5][0]."\">
  </TD>
  <TD class=border_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[6][0]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[6][0]."\">
  </TD>
  <TD class=border_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[7][0]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[7][0]."\">
  </TD>
  <TD class=border_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[8][0]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[8][0]."\">
  </TD>
 </tr>

 <tr>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[0][1]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[0][1]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[1][1]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[1][1]."\">
  </TD>
  <TD class=border_top_right_strong vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[2][1]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[2][1]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[3][1]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[3][1]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[4][1]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[4][1]."\">
  </TD>
  <TD class=border_top_right_strong vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[5][1]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[5][1]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[6][1]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[6][1]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[7][1]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[7][1]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[8][1]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[8][1]."\">
  </TD>
 </tr>

 <tr>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[0][2]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[0][2]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[1][2]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[1][2]."\">
  </TD>
  <TD class=border_top_right_strong vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[2][2]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[2][2]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[3][2]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[3][2]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[4][2]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[4][2]."\">
  </TD>
  <TD class=border_top_right_strong vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[5][2]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[5][2]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[6][2]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[6][2]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[7][2]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[7][2]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[8][2]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[8][2]."\">
  </TD>
 </tr>

 <tr>
  <TD class=border_top_strong_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[0][3]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[0][3]."\">
  </TD>
  <TD class=border_top_strong_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[1][3]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[1][3]."\">
  </TD>
  <TD class=border_top_strong_right_strong vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[2][3]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[2][3]."\">
  </TD>
  <TD class=border_top_strong_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[3][3]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[3][3]."\">
  </TD>
  <TD class=border_top_strong_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[4][3]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[4][3]."\">
  </TD>
  <TD class=border_top_strong_right_strong vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[5][3]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[5][3]."\">
  </TD>
  <TD class=border_top_strong_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[6][3]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[6][3]."\">
  </TD>
  <TD class=border_top_strong_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[7][3]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[7][3]."\">
  </TD>
  <TD class=border_top_strong_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[8][3]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[8][3]."\">
  </TD>
 </tr>

<tr>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[0][4]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[0][4]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[1][4]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[1][4]."\">
  </TD>
  <TD class=border_top_right_strong vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[2][4]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[2][4]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[3][4]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[3][4]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[4][4]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[4][4]."\">
  </TD>
  <TD class=border_top_right_strong vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[5][4]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[5][4]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[6][4]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[6][4]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[7][4]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[7][4]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[8][4]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[8][4]."\">
  </TD>
 </tr>

 <tr>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[0][5]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[0][5]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[1][5]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[1][5]."\">
  </TD>
  <TD class=border_top_right_strong vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[2][5]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[2][5]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[3][5]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[3][5]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[4][5]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[4][5]."\">
  </TD>
  <TD class=border_top_right_strong vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[5][5]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[5][5]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[6][5]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[6][5]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[7][5]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[7][5]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[8][5]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[8][5]."\">
  </TD>
 </tr>

 <tr>
  <TD class=border_top_strong_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[0][6]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[0][6]."\">
  </TD>
  <TD class=border_top_strong_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[1][6]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[1][6]."\">
  </TD>
  <TD class=border_top_strong_right_strong vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[2][6]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[2][6]."\">
  </TD>
  <TD class=border_top_strong_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[3][6]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[3][6]."\">
  </TD>
  <TD class=border_top_strong_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[4][6]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[4][6]."\">
  </TD>
  <TD class=border_top_strong_right_strong vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[5][6]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[5][6]."\">
  </TD>
  <TD class=border_top_strong_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[6][6]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[6][6]."\">
  </TD>
  <TD class=border_top_strong_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[7][6]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[7][6]."\">
  </TD>
  <TD class=border_top_strong_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[8][6]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[8][6]."\">
  </TD>
 </tr>

 <tr>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[0][7]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[0][7]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[1][7]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[1][7]."\">
  </TD>
  <TD class=border_top_right_strong vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[2][7]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[2][7]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[3][7]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[3][7]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[4][7]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[4][7]."\">
  </TD>
  <TD class=border_top_right_strong vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[5][7]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[5][7]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[6][7]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[6][7]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[7][7]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[7][7]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[8][7]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[8][7]."\">
  </TD>
 </tr>

<tr>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[0][8]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[0][8]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[1][8]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[1][8]."\">
  </TD>
  <TD class=border_top_right_strong vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[2][8]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[2][8]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[3][8]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[3][8]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[4][8]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[4][8]."\">
  </TD>
  <TD class=border_top_right_strong vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[5][8]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[5][8]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[6][8]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[6][8]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[7][8]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[7][8]."\">
  </TD>
  <TD class=border_top_right vAlign=\"center\" align=\"middle\" width=\"40\" height=\"40\">
   <input class=\"sudoku_black\" type=\"text\" name=\"feld[8][8]\" size=\"1\" maxlength=\"1\" value=\"".$feld1[8][8]."\">
  </TD>
 </tr>
</table>
<p><p><p>";
for($i=0;$i<9;$i++)
	for($j=0;$j<9;$j++)
		print "<input type=\"hidden\" name=\"feldtemp[$j][$i]\" value=\"".$feldtemp[$j][$i]."\">\n";
print "
<input type=\"submit\" value=\"Sudoku L&ouml;sen\" name=\"senden\">
<input type=\"submit\" value=\"Neues Sudoku\" name=\"neu\">
<input type=\"submit\" value=\"Ausgangssudoku\" name=\"start\">
</form><br><br><br>
<b>".$pruef."</b>
".$zeitausgabe."

  <!--<p><strong>&raquo; 13.01.2007:
</strong></p>-->
</p></td><td bgcolor=\"#ffffff\"><img src=\"../images/space_trans.gif\" width=\"20\" height=\"1\" border=\"0\" alt=\"\"></td>
</tr>
<tr>
<td align=\"right\" colspan=\"3\"  bgcolor=\"#eeeeee\" class=\"rub\"  height=\"22\"   style=\"border-top:solid 1px #bfbfbf;\" background=\"../images/a2.gif\"  ><table  border=\"0\" cellpadding=\"0\" cellspacing=\"0\"  width=\"100%\">
<tr>
<!--pfeile-->
<td align=\"right\"   ><a href=\"javascript:history.go(-1);\" onMouseOver=\"b.src='../images/back_over.gif';window.status='&nbsp;.: Zur&uuml;ck'; return true\"  onMouseOut=\"b.src='../images/back.gif';window.status=''\"><img src=\"../images/back.gif\" name=\"b\" width=\"20\" height=\"20\" border=\"0\" alt=\"back\" title=\"back\"style=\"border-left:solid 1px #bfbfbf;\"></a><a href=\"#\"  onMouseOver=\"t.src='../images/top_over.gif';window.status='&nbsp;.: Nach oben'; return true\"  onMouseOut=\"t.src='../images/top.gif';window.status=''\"><img src=\"../images/top.gif\" width=\"20\" height=\"20\" border=\"0\" name=\"t\" alt=\"top\" title=\"top\" style=\"border-left:solid 1px #bfbfbf;border-right:solid 1px #bfbfbf;\"></a></td>
<!--ende pfeile-->
</tr>
</table></td>
</tr>
</table></td>
</tr>
</table>
<!--end inhalt-->
</td>
</tr>
</table>
</td>
</tr>
</table>
<!-- ende haupt--></td>
</tr>
<tr>
<td style=\"border:solid 1px black\">


<!-- fuss -->
<table bgcolor=\"#ffffff\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\"  width=\"100%\" >
<tr>
<td ><table border=\"0\" cellpadding=\"0\" cellspacing=\"1\"  width=\"100%\">
<tr>
<td valign=\"bottom\" align=\"center\" height=\"22\" background=\"../images/a1.gif\" class=\"rub\" bgcolor=\"#eeeeee\"  ><span style=\"font-size: 8pt;color:#808080\">.:&nbsp;&nbsp;&copy; Markus Thill&nbsp;&nbsp;</span><span style=\"font-size: 8pt;color:#808080\">&nbsp;&nbsp;:.</span></td>
</tr>

<tr>
<td  bgcolor=\"#ffffff\"  style=\"border-top:solid 1px #BFBFBF;border-bottom:solid 1px #BFBFBF\" ><img src=\"../images/space_trans.gif\" width=\"1\" height=\"1\" border=\"0\" alt=\"\"></td>
</tr>

<tr>
<td height=\"22\" width=\"100%\" background=\"../images/a2.gif\"    bgcolor=\"#eeeeee\"align=\"center\" class=\"rub\" ><span style=\"font-size: 7pt;color:#5F5F5F\">&nbsp;&nbsp;www.diedackel.de&nbsp;&nbsp;</span></td>
</tr>
</table></td>
</tr>
</table>

<!-- ende fuss--></td>
</tr>
</table>
</td>
</tr>
</table>
<</body>
</html>";

if($pruef!="")
	return false;
return true;
}//Ende Ausgabe

if(!$_POST['neu'] && !$_POST['senden'] && !$_POST['start'])//Wenn die Seite das erste mal aufgerufen wird
	ausgabe("");
if($_POST['neu'])//Wenn der Button Neues Sudoku gedrückt wird!!!
{
	nullsetzer();
	ausgabe("");
}
else if($_POST['senden'])//Wenn der Button Sudoku lösen gedrückt wird!!!
{
	$feld=feldsetzer();
	if(pruefen()=="")
	{
		$feldtemp=$feld;
		datenbank();
	}
	else //Wenn der Benutzer falsche Angaben gemacht hat
		ausgabe("");
}
else if($_POST['start'])//Wenn der Button Ausgangssudoku gedrükt wird!!!
{
	for($i=0;$i<9;$i++)
		for($j=0;$j<9;$j++)
			$feld[$j][$i]=$_POST['feldtemp'][$j][$i];
	ausgabe("");
}

?>