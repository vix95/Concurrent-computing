Biblioteka funkcji graficznych XLIB

Zadanie 1. Obejrzeć, skompilować i wypróbować działanie dowolnego spośród programów graficznych przedstawionych na wykładzie. Zmieniając argument funkcji XOpenDisplay() wymusić wyświetlenie rysunku na monitorze jednego z sąsiadów. 

UWAGA: 
	1) sprawdzić przydzielone adresy IP i nazwy internetowe komputerów w laboratorium; 
	2) przeczytać opis polecenia xhost i przy jego użyciu udzielać uprawnień do wyświetlania innym uczestnikom zajęć.
	

2. Napisać program, który wyświetla na ekranie 10 rónokolorowych koncentrycznych kół o liniowo malejących promieniach ("pawie oko"). 


3. (PODLEGA ODBIOROWI - CZAS 2 TYGODNIE) Napisać program, który wyświetla kolorowe prostokąty na ekranach kilkorga sąsiadów (adresy/nazwy internetowe podawane jako argumenty w linii poleceń - zmienna liczba), a następnie (w oknie tekstowym) odnotowuje czasy (w sekundach), po jakich je zgaszą (naciskając dowolny klucz na klawiaturze). 

WSKAZ�WKA: utworzyć w programie tyle procesów, ile zostało podanych argumentów. 

====================== 1. (PODLEGA ODBIOROWI - CZAS 2 TYGODNIE) Na komputerze o adresie internetowym 153.19.1.202 uruchomiony jest serwer prostej funkcji arytmetycznej, który przez port UDP o numerze 5000 przyjmuje jedną liczbę całkowitą czterobajtową (int) i w odpowiedzi odsyła jedną liczbę tego samego typu. Napisać program klienta, który komunikuje się z tym serwerem i odgadnąć, jaką funkcję oblicza serwer.

UWAGA: pamiętać o przekształcaniu danych i adresu do postaci sieciowej i na odwrót (funkcje htonl() i ntohl()).
Treść programu serwera (z wyciętą realizowaną funkcją):
  
   // Ponizszy program serwera jest uruchomiony na komputerze
   // o adresie IP 153.19.1.202

   #include 
   #include 
   #include 
   #include 
   #include 

   main()
   {
      int sd,buff,clen;
      struct sockaddr_in sad,cad;

      signal(SIGHUP,SIG_IGN);
      sd=socket(AF_INET,SOCK_DGRAM,0);
      bzero((char *) &sad, sizeof(sad));
      sad.sin_family=AF_INET;
      sad.sin_addr.s_addr=htonl(INADDR_ANY);
      sad.sin_port=htons((ushort) 5000);
      bind(sd,(struct sockaddr *) &sad,sizeof(sad));
      clen=sizeof(cad);
      while (1)
      {
         recvfrom(sd,(char *) &buff,sizeof(int),0,(struct sockaddr *) &cad,&clen);
         buff=ntohl(buff);

         // Tutaj umieszczona jest funkcja przeksztalcajaca dana
         // odebrana w buff na wynik (tez w buff)

         buff=htonl(buff);
         sendto(sd,(char *) &buff,sizeof(int),0,(struct sockaddr *) &cad,clen);
      }
   }
  