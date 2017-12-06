/** Umlaute ASCII UTF8 Tabelle
  * \x81 \x84 \x94 \x8E \x99 \x9A \xE1"); ü ä ö Ä Ö Ü ß
  */ 
  #ifdef __linux__
  #define Ae "Ä"
  #define ae "ä"
  #define Ue "Ü"
  #define ue "ü"
  #define Oe "Ö"
  #define oe "ö"
  #define ss "ß"
  #elif __WIN32__ || _MSC_VER__
  #define Ae "\x8E"
  #define ae "\x84"
  #define Ue "\x9A"
  #define ue "\x81"
  #define Oe "\x99"
  #define oe "\x94"
  #define ss "\xE1"
  #endif


/** Funktion : Bidlschirm und Cursor zuruecksetzen
  * Status   : fertig
  */
  void clr()
  {
	  #ifdef __linux__
	  printf("\033[1J\033[1;1H");
	  #elif __WIN32__ || _MSC_VER__
      system("cls");
      #endif
  }

/** Funktion : Tastaturpuffer leeren
  * Status   : fertig
  */
  void flush()
  {
	  int c;
	  while( ((c = getchar()) != EOF) && (c != '\n') );
  }

/** Funktion : Alternative fuer Tastendruck getch()
  * Status   : fertig
  */
  int ctrl()
  {
	  #ifdef __linux__
	  static int ch = -1, fd = 0;
	  struct termios neu, alt;
	  fd = fileno(stdin);
	  tcgetattr(fd, &alt);
	  neu = alt;
	  neu.c_lflag &= ~(ICANON|ECHO);
	  tcsetattr(fd, TCSANOW, &neu);
	  ch = getchar();
	  tcsetattr(fd, TCSANOW, &alt);
	  return ch;
	  #elif __WIN32__ || _MSC_VER__
      getch();
      #endif
  }

/** Funktion : Threadfunktion fuer parallelen Ablauf
  * Status   : fertig
  */
  #ifdef __linux__
  int kbhit(void)
  {
	struct termios oldt, newt;
	int ch;
	int oldf;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}
	return 0;
}
#endif

/** Funktion : Cursor positionieren
  * Status   : fertig
  */
  void csr( unsigned int y , unsigned int x )
  {
      #ifdef __linux__
      printf("\033[%i;%iH" , ( y + 1 ) , ( x + 1 ) );
      #else
      COORD Position;
      Position.X = x;
      Position.Y = y;
      SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), Position );
      #endif
  }

/** Funktion : Prototypen
  */
  int iAnzeigen();
  int iSpielstand_speichern();
  int iController();
  int iLeben   = 3;
  int iAuswahl = 1;

/** Funktion : Struct zum Werte speichern
  * Status   : fertig
  */
  struct
  {
	  /*Eigenschaften des iTamagotchis*/
	  double dKoerpergewicht;
	  double dTotmenge;
	  double dGegessenes_Salz;
	  double dSalzmenge;
	  double dFutter;
	  double dRunden;
	  double dHunger;
	  double dHappyness;
	  double dReinigen;
}stTamagotchi; 
  
/** Funktion : 
  * Status   : fertig
  */
  void Header()
  {
       
      csr(0,0);
	  printf("\n\n     T A M A G O T C H I  S A L Z M O R D\n\n");
  }

/** Funktion : 
  * Status   : fertig
  */
  void Eingabefeld()
  {
	  clr();
	  printf("\n     Was m%schtest du mit deinem Tamagotchi machen?\n"
	  "     Mit 0 %sbersicht anzeigen lassen: ", oe, Ue);
  } 

/** Funktion : Threadfunktion fuer parallelen Ablauf
  * Status   : fertig
  */
  void Tot()
  {
	  if(iAuswahl == 1)
	  {
		  csr(7,55);
		  printf("()__()");
		  csr(8,55);
		  printf("= x_x)=");
		  csr(9,55);
		  printf("(\")(\")");
	}
	else if(iAuswahl == 2 && iLeben >= 8)
	{
		csr(6,55);
		printf("  ///");
		csr(7,55);
		printf("<(x )___");
		csr(8,55);
		printf(" ( ._> /");
		csr(9,55);
		printf("  `---'");		
	}
	else if(iAuswahl == 3 && iLeben >= 12)
	{
		csr(6,55);
		printf(" __");
		csr(7,55);
		printf("(_x\\.-.");
		csr(8,55);
		printf("  \\/___\\_");
		csr(9,55);
		printf("    u u");
	}
    getchar();
}

/** Funktion : verschied. iTamagotchi Charaktere je Zustand anzeigen
  * Status   : vorgelegt
  */
  void ASCII()
  {
	  /* hier werden Gewicht, Hunger und Happyness ueberprueft und der
	   * jeweilige Charakter angezeigt */
	  if(iAuswahl == 1 || iAuswahl == 0)
	  {
		  if(stTamagotchi.dKoerpergewicht <= 100.00 ||
		  stTamagotchi.dHappyness <= 50.00 )
		  {
			  csr(7,55);
			  printf("()__()");
			  csr(8,55);
			  printf("= -,-)=");
			  csr(9,55);
			  printf("(\")(\")");		
		  }
		  if(stTamagotchi.dKoerpergewicht >= 100.00 && 
		  stTamagotchi.dKoerpergewicht <= 900.00)
		  {
			  csr(7,55);
			  printf("()__()");
			  csr(8,55);
			  printf("= 0.0)=");
			  csr(9,55);
			  printf("(\")(\")");
		  }	
		  if(stTamagotchi.dHappyness <= 15.00)
		  {
			  csr(7,55);
			  printf("()__()");
			  csr(8,55);
			  printf("= ;.;)=");
			  csr(9,55);
			  printf("(\")(\")");
			  if(stTamagotchi.dHappyness <= 10.00)
			  {
				  csr(7,62);
				  printf("  (");
				  csr(8,62);
				  printf(" (_)");
				  csr(9,62);
				  printf("(___)");
				  csr(8,55);
				  printf("= >=_<=)=");
			  }	
		  }
		  else if(stTamagotchi.dHappyness >= 50.00)
		  {
			  csr(7,55);
			  printf("()__()");
			  csr(8,55);
			  printf("= ^.^)=");
			  csr(9,55);
			  printf("(\")(\")");	
		  }
		  else if(stTamagotchi.dHunger >= 75.00)
		  {
			  csr(7,55);
			  printf("()__()");
			  csr(8,55);
			  printf("= ';')=");
			  csr(9,55);
			  printf("(\")(\")");	
		  }
	  }
	  /* wird erst aktiviert, wenn Lebenspunkte erreicht wurden */
	  if(iLeben >= 8 && iLeben <= 15)
	  {                                
		  if(iAuswahl == 2 || iAuswahl == 0) /* steht nun zur Auswahl */
		  {
			  if(stTamagotchi.dKoerpergewicht <= 100.00 || 
			  stTamagotchi.dHappyness <= 50.00 )
			  {
				  csr(6,55);
				  printf("  ///");
				  csr(7,55);
				  printf("<(' )___");
				  csr(8,55);
				  printf(" ( ._> /");
				  csr(9,55);
				  printf("  `---'");	
			  }
			  if(stTamagotchi.dKoerpergewicht >= 100.00 && 
			  stTamagotchi.dKoerpergewicht <= 900.00)
			  {
				  csr(6,55);
				  printf("  ///");
				  csr(7,55);
				  printf("<(0 )___");
				  csr(8,55);
				  printf(" ( ._> /");
				  csr(9,55);
				  printf("  `---'");		
			  }	
			  if(stTamagotchi.dHappyness <= 15.00)
			  {
				  csr(6,55);
				  printf("  ///");
				  csr(7,55);
				  printf("=(; )___");
				  csr(8,55);
				  printf(" ( ._> /");
				  csr(9,55);
				  printf("  `---'");	
				  if(stTamagotchi.dHappyness <= 10.00)
				  {
					  csr(6,62);
					  printf("  (");
					  csr(7,62);
					  printf(" (_)");
					  csr(8,62);
					  printf("(___)");
					  csr(7,55);
					  printf("=(- )___");
				  }		
			  }
			  else if(stTamagotchi.dHappyness >= 50.00)
			  {
				  csr(6,55);
				  printf("  ///");
				  csr(7,55);
				  printf(">(^ )___");
				  csr(8,55);
				  printf(" ( ._> /");
				  csr(9,55);
				  printf("  `---'");	
			  }
			  else if(stTamagotchi.dHunger >= 75.00)
			  {
				  csr(6,55);
				  printf("  ///");
				  csr(7,55);
				  printf(">(0 )___");
				  csr(8,55);
				  printf(" ( ._> /");
				  csr(9,55);
				  printf("  `---'");	
			  }
		  }
	  }
	  if(iLeben >= 12)
	  {
		  if(iAuswahl == 3 || iAuswahl == 0)
		  {
			  if(stTamagotchi.dKoerpergewicht <= 100.00 || 
			  stTamagotchi.dHappyness <= 50.00 )
			  {
				  csr(6,55);
				  printf(" __");
				  csr(7,55);
				  printf("(_'\\.-.");
				  csr(8,55);
				  printf("  \\/___\\_");
				  csr(9,55);
				  printf("    u u");		
			  }
			  if(stTamagotchi.dKoerpergewicht >= 100.00 && 
			  stTamagotchi.dKoerpergewicht <= 900.00)
			  {
				  csr(6,55);
				  printf(" __");
				  csr(7,55);
				  printf("(_*\\.-.");
				  csr(8,55);
				  printf("  \\/___\\_");
				  csr(9,55);
				  printf("    u u");			
			  }	
			  if(stTamagotchi.dHappyness <= 15.00)
			  {
				  csr(6,55);
				  printf(" __");
				  csr(7,55);
				  printf("(_;\\.-.");
				  csr(8,55);
				  printf("  \\/___\\_");
				  csr(9,55);
				  printf("    u u");		
				  if(stTamagotchi.dHappyness <= 10.00)
				  {
					  csr(6,62);
					  printf("  (");
					  csr(7,62);
					  printf(" (_)");
					  csr(8,62);
					  printf("(___)");
					  csr(7,55);
					  printf("(_-\\.-.");
				  }		
			  }
			  else if(stTamagotchi.dHappyness >= 50.00)
			  {
				  csr(6,55);
				  printf(" __");
				  csr(7,55);
				  printf("(_^\\.-.");
				  csr(8,55);
				  printf("  \\/___\\_");
				  csr(9,55);
				  printf("    u u");		
			  }
			  else if(stTamagotchi.dHunger >= 75.00)
			  {
				  csr(6,55);
				  printf(" __");
				  csr(7,55);
				  printf(")_0\\.-.");
				  csr(8,55);
				  printf("  \\/___\\_");
				  csr(9,55);
				  printf("    u u");	
			  }
		  }
	  }
  }

/** Funktion : freigeschaltete Tamas auswaehlen
  * Status   : vorgelegt
  */
  void iAvatar()
  {
	  do
	  {
		  Header();
		  csr(5,8);
		  printf("()__()");
		  csr(6,8);
		  printf("= '.')=");
		  csr(7,5);
		  printf(",;,(\")(\"),,,;,,,,;,,,,;,,,,,,,;,,,,;,");
		  if(iLeben >= 8 && iLeben <= 15)
		  {
			  csr(4,30);
			  printf("  ///");
			  csr(5,30);
			  printf("<(' )___");
			  csr(6,31);
			  printf("( ._> /");
			  csr(7,32);
			  printf("`---'");
		  }
		  if(iLeben >= 12)
		  {
			  csr(4,19);
			  printf("__");
			  csr(5,18);
			  printf("(_'\\.-.");
			  csr(6,20);
			  printf("\\/___\\_");
			  csr(7,22);
			  printf("u u");
		  }
		  /* Auswahl wird anhand der Lebenspunkte freigeschaltet
		   * sinkt das Leben, wird die Auswahl wieder eingegrenzt */
		  printf("\n\n     Welches Tamagotchi m%schtest du w%shlen?\n", oe, ae);
		  printf("\n     -1- Maus");
		  if(iLeben >= 8 && iLeben <= 15)
		  {
			  printf("\n     -2- Ente");
		  }
		  if(iLeben >= 12)
		  {
			  printf("\n     -3- Schildkr%ste", oe);
		  }
		  printf("\n\n     Mit 0 Auswahl abbrechen: ");
		  scanf("%i", &iAuswahl);
		  flush();
		  if(iAuswahl == 1)
		  {
			  break;
		  }
		  if(iLeben >= 8 && iLeben <= 15)
		  {
			  if(iAuswahl == 2)
			  {
				  break;
			  }
		  }
		  if(iLeben >= 12)
		  {
			  if(iAuswahl == 3)
			  {
				  break;
			  }
		  }
	  }while(iAuswahl != 0);
	  clr();
	  printf("\n\n     T A M A G O T C H I  S A L Z M O R D\n\n");
	  csr(5,8);
	  printf("()__()");
	  csr(6,8);
	  printf("= '.')=");
	  csr(7,5);
	  printf(",;,(\")(\"),,,;,,,,;,,,,;,,,,,,,;,,,,;,");
	  if(iLeben >= 8 && iLeben <= 15)
	  {
		  csr(4,30);
		  printf("  ///");
		  csr(5,30);
		  printf("<(' )___");
		  csr(6,31);
		  printf("( ._> /");
		  csr(7,32);
		  printf("`---'");
	  }
	  if(iLeben >= 12)
	  {
		  csr(4,19);
		  printf("__");
		  csr(5,18);
		  printf("(_'\\.-.");
		  csr(6,20);
		  printf("\\/___\\_");
		  csr(7,22);
		  printf("u u");
	  }
	  printf("\n");
	  /* Spielstand speichern */
	  iSpielstand_speichern();
  }

/** Funktion : Spielstand speichern
  * Status   : vorgelegt
  */
  int iSpielstand_speichern()
  {
	  FILE *FpSpeichern;
	  FpSpeichern = fopen("stand.txt", "w");
	  fprintf(FpSpeichern,"%.2lf\n", stTamagotchi.dKoerpergewicht);
	  fprintf(FpSpeichern,"%.2lf\n", stTamagotchi.dHappyness);
	  fprintf(FpSpeichern,"%.2lf\n", stTamagotchi.dHunger);
	  fprintf(FpSpeichern,"%.2lf\n", stTamagotchi.dGegessenes_Salz);
	  fprintf(FpSpeichern,"%i\n", iLeben);
	  fprintf(FpSpeichern,"%i", iAuswahl);
	  fclose(FpSpeichern);
	  return EXIT_SUCCESS;  
  }
  
/** Funktion : Statistiken anzeigen
  * Status   : vorgelegt
  */
  void Status()
  {
      Header();
	  ASCII();
	  csr(4,0);
	  printf("     Dein Tamagotchi hat..: %i/15 Lebenspunkte\n\n", iLeben );
	  printf("     wiegt zur Zeit.......: %.2lf g\n", 
	  stTamagotchi.dKoerpergewicht);
	  printf("     mit einem Hunger von.: %.2lf %%\n", 
	  stTamagotchi.dHunger);
	  printf("     Hat zur Zeit.........: %.2lf g Salz im Blut\n", 
	  stTamagotchi.dGegessenes_Salz);
	  printf("     und ist zur Zeit zu..: %.2lf %% gl%scklich\n",
	  stTamagotchi.dHappyness, ue);
	  printf("\n     Was m%schtest du mit deinem Tamagotchi machen?\n"
	  "     Mit 0 %sbersicht anzeigen lassen: ", oe, Ue);
	  csr(23,0);
	  printf("     (c)2012 Nico Anders v.1.2");
	  csr(12,38);
  }
  
/** Funktion : Menue anzeigen
  * Status   : vorgelegt
  */
  void Menue()
  {  
	  Header();
	  printf("     -0- %sbersicht\n", Ue);
	  printf("     -1- Versalzen\n");
	  printf("     -2- Fuettern\n");
	  printf("     -3- Trainieren\n");
	  printf("     -4- Pflegen\n");
	  printf("     -5- Ansehen\n");
	  printf("     -6- Ausw%shlen\n", ae);
  }

/** Funktion : Fehlermeldung bei falschem Tastendruck
  * Status   : vorgelegt
  */
  void Fehler()
  {
	  Header();
	  printf("     Diese Funktion kenne ich leider nicht.\n");
  }
