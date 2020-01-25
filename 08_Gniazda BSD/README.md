Gniazda BSD

Zadanie 1. 1. (PODLEGA ODBIOROWI - CZAS 2 TYGODNIE) Na komputerze o adresie internetowym 10.10.10.230 uruchomiony jest serwer prostej funkcji arytmetycznej, który przez port UDP o numerze 5000 przyjmuje jedną liczbę całkowitą czterobajtową (int) i w odpowiedzi odsyła jedną liczbę tego samego typu. Napisać program klienta, który komunikuje się z tym serwerem i odgadnąć, jaką funkcję oblicza serwer.

UWAGA: pamiętać o przekształcaniu danych i adresu do postaci sieciowej i na odwrót (funkcje htonl() i ntohl()).
Treść programu serwera (z wyciętą realizowaną funkcją):
  
   // Ponizszy program serwera jest uruchomiony na komputerze
   // o adresie IP 10.10.10.230

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
  