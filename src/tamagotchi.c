/** Programm: tamagotchi.c
  * Autor   : nico anders
  * Version : 1.2
  * Datum   : 28.03.2012
  * ToDo    : Resetfunktion debuggen
  *           in weiteren Versionen sollen Tamagotchis individuelle
  *           Verhaltensarten gemaeß ihres Wesens bekommen
  */

/** Praeprozessor
  */
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <time.h>
  #ifdef __linux__
  #include <termios.h>
  #include <unistd.h>
  #include <fcntl.h>
  #elif __WIN32__ || _MSC_VER__
  #include <conio.h>
  #include <windows.h>
  #endif
  #include "mylib.h"               /* eigene Libary mit Ein und Ausgabefunkt. */

/** Funktion : zufaellige Startwerte fuer Tama generieren
  * Status   : vorgelegt
  */
  int iZufall()
  {
	  FILE *FPLesen;
	  int i = 0;
	  char sStand[6][100];
	  double dWert = 0;
	  FPLesen = fopen("stand.txt", "r");
	  /* Ist noch kein Spielstand vorhanden werden zufaellige Werte generiert */
	  if(FPLesen == NULL) {
		  srand(time(NULL));
		  /* Staerke der Beduerfnisse werden zufaellig generiert,
		   * um das Spiel individuell zu machen */
		  stTamagotchi.dKoerpergewicht = rand() %100+1;
		  stTamagotchi.dHappyness = rand() %100+1;
		  stTamagotchi.dHunger = rand() %100+1;
		  /* Bei Ueber- oder Unterschreitung des erlaubten
		   * Bereiches, wird der zufaellige Wert auf ein Standardwert
		   * zurueck gesetzt, damit das Tama nicht schon vorher stirbt */
		  if(stTamagotchi.dKoerpergewicht <= 45.00 || stTamagotchi.dKoerpergewicht >= 100.00)
		  {
			  stTamagotchi.dKoerpergewicht = 47.00;
		  }
		  if(stTamagotchi.dHappyness <= 15.00 || stTamagotchi.dHappyness >= 50.00)
		  {
			  stTamagotchi.dHappyness = 16.00;
		  }
		  if(stTamagotchi.dHunger >= 75.00)
		  {
			  stTamagotchi.dHunger = 70.00;
		  }
		  /* Salzmenge die das Tama umbringt wird berechnet */
		  stTamagotchi.dTotmenge = stTamagotchi.dKoerpergewicht * 0.003;
	  }
	  else
	  {
		  for(i = 0; i < 6; i++)
		  {
			  fgets(sStand[i], sizeof(sStand), FPLesen);
			  sscanf(sStand[i], "%lf", &dWert);
			  if(i == 0)
			  {
				  stTamagotchi.dKoerpergewicht = dWert;
			  }
			  if(i == 1)
			  {
				  stTamagotchi.dHappyness = dWert;
			  }
			  if(i == 2)
			  {
				  stTamagotchi.dHunger = dWert;
			  }
			  if(i == 3)
			  {
				  stTamagotchi.dGegessenes_Salz = dWert;
			  }
			  if(i == 4)
			  {
				  iLeben = dWert;
			  }
			  if(i == 5)
			  {
				  iAuswahl = dWert;
			  }
		  }
		  fclose(FPLesen);
	  }
	  iAnzeigen();
	  return EXIT_SUCCESS;
  }
  
/** Funktion : Tama mit Salz behandeln
  * Status   : vorgelegt
  */
  int iVersalzen()
  {
	  Header();
	  printf("     Wieviel Salz soll dein Tamagotchi essen?\n");
	  printf("     Mit 0 Auswahl abbrechen: ");
	  scanf("%lf", &stTamagotchi.dSalzmenge);
	  flush();
	  /* Eingaben mit Null oder kleiner dienen zum Abbruch der Eingabe*/
	  if(stTamagotchi.dSalzmenge <= 0.00) 
	  {
	  }
	  else
	  {
		  stTamagotchi.dGegessenes_Salz = stTamagotchi.dGegessenes_Salz 
		  + stTamagotchi.dSalzmenge;
		  if(stTamagotchi.dGegessenes_Salz >= 0.500)
		  {
			  stTamagotchi.dHappyness = stTamagotchi.dHappyness - 10.00;
			  /* Bei grosser Menge einen Lebenspunkt abziehen */
			  if(stTamagotchi.dGegessenes_Salz >= 1.00)
			  {
				  iLeben--;
				  /* und setzt den Stand auf 1, wenn Null erreicht 
				   * wurde, damit das Spiel weiterlaeuft */
				  if(iLeben == 0)
				  {
					  iLeben = 1;
				  }
			  }
		  }
		  clr();
		  Header();
		  printf("     Dein Tamagotchi hat nun %.2lf Gramm Salz im Blut\n", 
		  stTamagotchi.dGegessenes_Salz);
		  /* Spielstand speichern */
		  iSpielstand_speichern();
	  }
	  return EXIT_SUCCESS;
  }
  
/** Funktion : Tama fuettern
  * Status   : vorgelegt
  */
  int iFuettern()
  {
	  Header();
	  printf("     Wieviel Gramm soll dein Tamagotchi essen?\n");
	  printf("     Mit 0 Auswahl abbrechen: ");
	  scanf("%lf", &stTamagotchi.dFutter);
	  flush();
	  /* Tama nimmt nichts an, wenn es noch satt ist oder ueber dem
	   * Normalgewicht liegt */
	  if(stTamagotchi.dFutter <= 0.00 || stTamagotchi.dHunger <= 25.00
	  || stTamagotchi.dKoerpergewicht >= 860.00)
	  {
	  }
	  else
	  {
		  /* Berechnung fuer das neue Koerpergewicht */
		  stTamagotchi.dKoerpergewicht = stTamagotchi.dKoerpergewicht 
		  + stTamagotchi.dFutter;
		  /* Berechnung fuer den gestillten Hunger in Prozent */
		  stTamagotchi.dFutter = stTamagotchi.dHunger 
		  * stTamagotchi.dFutter / 100.00;
		  /* Tama kann mehr Salz pro zugefuehrtes Futter aufnehmen */    
		  stTamagotchi.dTotmenge = stTamagotchi.dKoerpergewicht * 0.003;
		  if(stTamagotchi.dFutter >= 900.00)
		  {
			  stTamagotchi.dKoerpergewicht = 545.00;
		  }
		  if(stTamagotchi.dKoerpergewicht >= 860.00)
		  {
			  iLeben--;
			  if(iLeben == 0)
			  {
				  iLeben = 1;
			  }
		  }
		  stTamagotchi.dHunger = stTamagotchi.dHunger 
		  - stTamagotchi.dFutter;
		  /* Hunger darf nicht unter Null sinken */
		  if(stTamagotchi.dHunger <= 0.00)
		  {
			  stTamagotchi.dHunger = 0.00;
			  stTamagotchi.dHappyness = stTamagotchi.dHappyness + 10.00;
			  /* Happyness soll nicht ueber 100% steigen */
			  if(stTamagotchi.dHappyness >= 100.00)
			  {
				  stTamagotchi.dHappyness = 100.00;
			  }
		  }
		  clr();
		  Header();
		  printf("     Dein Tamagotchi wiegt nun: %.2lf g\n", 
		  stTamagotchi.dKoerpergewicht);
		  /* Spielstand speichern */
		  iSpielstand_speichern();
	  }
	  return EXIT_SUCCESS;
  }
/** Funktion : Tama trainieren, um Gewicht zu reduzieren, mit Happynessfaktor
  * Status   : vorgelegt
  */
  int iTrainieren()
  {
	  Header();
	  printf("     Wie viele Runden soll dein Tamagotchi laufen?\n");
	  printf("     Mit 0 Auswahl abbrechen: ");
	  scanf("%lf", &stTamagotchi.dRunden);
	  flush();
	  /* Tama verweigert Training unter 150g, damit es nicht stirbt */
	  if(stTamagotchi.dRunden <= 0.00 || stTamagotchi.dKoerpergewicht 
	  <= 150.00)
	  {
	  }
	  else 
	  {
		  /* Ab 50 Runden steigt der Hunger und die Happyness */
		  if(stTamagotchi.dRunden >= 50)
		  {
			  stTamagotchi.dHunger = stTamagotchi.dHunger + 5.00;
			  stTamagotchi.dHappyness = stTamagotchi.dHappyness + 10.00;
			  if(stTamagotchi.dHunger >= 100.00)
			  {
				  stTamagotchi.dHunger = 100.00;
			  }
			  if(stTamagotchi.dHappyness >= 100.00)
			  {
				  stTamagotchi.dHappyness = 100.00;
			  }
		  }
		  /* ansonsten verliert das Tama nur an Gewicht */
		  stTamagotchi.dKoerpergewicht = stTamagotchi.dKoerpergewicht 
		  - stTamagotchi.dRunden;
		  stTamagotchi.dTotmenge = stTamagotchi.dKoerpergewicht * 0.003;
		  /* Gewicht wird bei 45g auf 47g gepusht, damit das Tamagotchi
		   * nicht ausversehen stirbt, wenn mehr als 50 Runden
		   * bei einem Gewicht von < 150g eingegeben wurden */
		  if(stTamagotchi.dKoerpergewicht <= 45.00)
		  {
			  stTamagotchi.dKoerpergewicht = 47.00;
		  }
		  /* wurden die Beduerfnisse befriedigt, gibts Punkte */
		  if(stTamagotchi.dHappyness >= 90.00 && stTamagotchi.dHunger <= 25.00)
		  {
			  iLeben++;
			  if(iLeben >= 15)          /* 15 Punkte sind das Maximum */
			  {
				  iLeben = 15;
			  }
		  }
		  clr();
		  Header();
		  printf("     Dein Tamagotchi hat nun: %i/15 Lebenspunkte\n", iLeben);
		  /* Spielstand speichern */
		  iSpielstand_speichern();
	  }
	  return EXIT_SUCCESS;
  }
  
/** Funktion : Tamagotchi sauber halten
  * Status   : vorgelegt
  */
  int iPflegen()
  {
	  Header();
	  printf("     Wie oft soll dein Tamagotchi gepflegt werden?\n");
	  printf("     Mit 0 Auswahl abbrechen: ");
	  scanf("%lf", &stTamagotchi.dReinigen);
	  flush();
	  /* ab 50% Happyness gibt's nicht's mehr zu pflegen */
	  if(stTamagotchi.dReinigen <= 0.00 || stTamagotchi.dHappyness >= 50.00)
	  {
	  }
	  else
	  {
		  /* werden die 100% ueberschritten, wird die Happyness auf 75%
		   * gedrosselt, die restlichen 25% muessen vom User bzw. Tamagotchi
		   * erarbeitet werden */
		  if(stTamagotchi.dReinigen >= 100.00)
		  {
			  stTamagotchi.dReinigen = 75.00;
		  }
		  /* Berechnung fuer die geleistete Pflegeeinheit in Prozent */
		  stTamagotchi.dReinigen = stTamagotchi.dHappyness * 
		  stTamagotchi.dReinigen / 100.00;
		  /* Berechnung fuer die Happyness */
		  stTamagotchi.dHappyness = stTamagotchi.dHappyness + 
		  stTamagotchi.dReinigen;
		  if(stTamagotchi.dHappyness >= 100.00)
		  {
			  stTamagotchi.dHappyness = 100.00;
		  }
		  clr();
		  Header();
		  printf("     Dein Tamagotchi ist nun zu %.2lf%% gl%scklich\n", 
		  stTamagotchi.dHappyness, ue);
		  /* Spielstand speichern */
		  iSpielstand_speichern();
	  }
	  return EXIT_SUCCESS;
  }
  
/** Funktion : Statistik des Tama anzeigen
  * Status   : vorgelegt
  */
  int iAnzeigen()
  {
	  FILE *FPDatei;
	  while(1)
	  {
		  Status();
		  /* hier werden die natuerlichen
		   * Beduerfnisse des Tamas simuliert*/
		  stTamagotchi.dHunger = stTamagotchi.dHunger + 0.00005;
		  stTamagotchi.dHappyness = stTamagotchi.dHappyness - 0.00002;
		  if(stTamagotchi.dHappyness <= 0.00)
		  {
			  stTamagotchi.dHappyness = 0.00;
		  }
		  if(stTamagotchi.dHunger >= 75.00 || stTamagotchi.dHappyness <= 15.00)
		  {
			  /* hat das Hungergefuehl die 75% erreicht,
			   * setzt die Verdauung ein */
			  stTamagotchi.dKoerpergewicht = stTamagotchi.dKoerpergewicht 
			  - 0.00006;
			  if(stTamagotchi.dHunger >= 75.00)
			  {
				  printf("\n\n     Dein Tamagotchi hat Hunger\n");
			  }
			  else if(stTamagotchi.dHappyness <= 15.00)
			  {
				  printf("\n\n     Dein Tamagotchi f%shlt sich unwohl\n", ue);
			  }
			  printf("\a");
			  csr(12,38);
		  }
		  if(stTamagotchi.dHappyness <= 5.00 || stTamagotchi.dHunger 
		  >= 95.00 || stTamagotchi.dKoerpergewicht <= 45.00)
		  {
			  /* Bei Verwarlosung, faellt das Leben auf 1 zurueck */
			  iLeben = 1;
		  }
		  if(stTamagotchi.dKoerpergewicht <= 46.00)
		  {
			  printf("\n\n     Dein Tamagotchi wird bald sterben\n");
			  printf("\a");
			  csr(12,38);
		  }
		  /* werden die Beduerfnisse nicht gedeckt, ist das Tama tot */
		  if(stTamagotchi.dHunger >= 100.00 || stTamagotchi.dKoerpergewicht
		  <= 44.00 || stTamagotchi.dHappyness <= 00.00)
		  {
			  clr();
			  Header();
			  printf("     Dein Tamagotchi ist gestorben\n");
			  /* totes Tamagotchi anzeigen */
			  Tot();
			  /* Jedes Tama hat ein einmaliges Leben.
			   * File blockiert weitere Inkarnation */
			  FPDatei = fopen("lock", "w");
			  remove("stand.txt");           /* Spielstand loeschen */
			  fclose(FPDatei);
			  exit(0);
		  }
		  /* Beduerfnisse des Tama werden weiter simuliert,
		   * waehrend eine andere Funktion aufgerufen werden kann */
		  if(kbhit())
		  {
			  break;
		  }
		  #ifdef __linux__
		  usleep(5000);   /* Verzoegert das Ansteigen der Beduerfnisse */
		  #elif __WIN32__ || _MSC_VER__
		  sleep(50);
		  #endif
		  /* Spielstand speichern */
		  iSpielstand_speichern();
	  }
	  return EXIT_SUCCESS;
  }

/** Funktion : Steuerfunktion fuer Tamagotchi
  * Status   : fertig
  */
  int iTama()
  {
	  char cAktion;
	  FILE *FPDatei;
	  FPDatei = fopen("lock", "r");
	  /* Wenn Tamagotchi gestorben ist, dient das File als Sperre */
	  if(FPDatei != NULL)
	  {
		  Header();
		  printf("     Dein Tamagotchi ist bereits gestorben\n"
		         "     Mit 0 Reseten: ");
		  scanf("%c", &cAktion);
		  flush();
		  if(cAktion == '0')
		  {
			  remove("lock");                   /* Tamagotchi reseten */
			  clr();
			  iZufall();                      /* und von neuem beginnen */
		  }
		  else
		  {
			  return EXIT_FAILURE;
		  }
	  }
	  Header();
	  iZufall(); /* Zufallswerte fuer die Statisitk werden uebergeben */
	  do
	  {
		  printf("\n     Was m%schtest du mit deinem Tamagotchi machen?\n"
		  "     Mit 0 %sbersicht anzeigen lassen: ", oe, Ue);
		  cAktion = ctrl();
		  /*Hier sieht man, welche Aktivitaeten zur Verfuegung stehen */
		  clr();
		  if(cAktion == '0')
		  {
			  /* die Uebersicht aufklappen */
			  Menue();
			  /* wiederholt die Schleife, um die Uebersicht von
			   * allen Funktionen aus aufrufen zu koennen */
			  continue;
		  }
		  else if(cAktion == '1')
		  {
			  iVersalzen();
			  /* Hier laeuft die Abfrage, ob die Salzmenge im Blut des
			   * Tamagotchis mehr ist, als die Grenze die es umbringt */
			   if(stTamagotchi.dGegessenes_Salz >= stTamagotchi.dTotmenge)
			   {
				   printf("     Du bist ein M%srder!\n", oe);
				   Tot();
				   /* Wenn der Fall eintritt, endet die Schleife
				    * und ein File wird als Sperre angelegt */
				    FPDatei = fopen("lock", "w");
				    fclose(FPDatei);
				    break;
				}
			}
			else if(cAktion == '2')
			{
				iFuettern();
			}
			else if(cAktion == '3')
			{
				iTrainieren();	 
			}
			else if(cAktion == '4')
			{
				iPflegen();
			}
			else if(cAktion == '5')
			{
				iAnzeigen();
			}
			else if(cAktion == '6')
			{
				iAvatar();
			}
			else if(cAktion != '0'||cAktion != '1'||cAktion != '2'
			||cAktion != '3'||cAktion != '4'||cAktion != '5'
			||cAktion != '6')
			{
				Fehler();
			}
	  /* Es wird geprueft, ob die Eingabe keine 99 war */
	  }while(cAktion != 99);
	  /*Ende der Schleife*/
	  return EXIT_SUCCESS; 
  } 

/** Funktion : Steuerfunktion
  * Status   : fertig
  */
  int iController()
  {
	  iTama();
	  return EXIT_SUCCESS;
  }

/** Funktion : Hauptprogramm
  * Status   : fertig
  */
  int main()
  {
	  iController();
	  return EXIT_SUCCESS;
  }
