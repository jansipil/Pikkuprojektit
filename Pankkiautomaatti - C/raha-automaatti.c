#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tarkistaPin(char tilinumero[], int n);
int nostaRahaa(double saldo);
double talletaRahaa(void);
void tulostaRahat(int summa);
void tiedostonMuokkaus(double saldo, char tilinnumero[], int n);
int lueKokonaisluku(void);
double lueReaaliluku(void);
void lueRoskat(void);

int main(void){

	char tilinumero[30];
	char tilinsaldo[30];
	char lukitustestaus[10] = "_lukittu";
	int n=30;
	double saldo;
	int summa;
	int vaihtoehto = 0;
	double talletus;


	FILE *tili;

//Automaatti kysyy tilinumeroa ja muuttaa sen .tili muotoon jotta osaa avata tiedoston

while(1){
	printf("Tervetuloa raha-automaattiin!\n");
	printf("Anna tilinumero: ");
	fgets(tilinumero, 30, stdin);

	if(tilinumero[strlen(tilinumero) -1] == '\n')
		tilinumero[strlen(tilinumero) -1] = '\0';
	else
		lueRoskat();

	strcat(tilinumero, ".tili");

//Jos tili on olemassa, menee funktioon tarkistaPin
//Testaa myös yritetäänkö avata lukittua tiliä
//Ei päästä läpi jos tilinumerosta löytyy sana "_lukittu" eli ei voi yrittää huijata ohjelmaa kirjoittamalla "xxxxx_lukittu" tiliä kysyttäessä

	if((tili = fopen(tilinumero, "r" ))==NULL){
		printf("Vastaavaa tilia ei ole olemassa tai se on lukittu\n");
		fclose(tili);
	}
	else {
        fclose(tili);
        if(strstr(tilinumero, lukitustestaus) == NULL) {
        	tarkistaPin(tilinumero, n);
        }
        else{
        	printf("Tili on lukittu\n");
        }
	}

//Jos kaikki ok, irtaantuu loopista

	if((tili = fopen(tilinumero, "r" ))==NULL) {
		fclose(tili);
	}
	else if(strstr(tilinumero, lukitustestaus) == NULL) {
		fclose(tili);
		break;
	}

}

printf("\n");

//Katsoo tilin saldon

tili = fopen(tilinumero, "r");
fgets(tilinsaldo, 30, tili);
fgets(tilinsaldo, 30, tili);
sscanf(tilinsaldo, "%lf", &saldo);
fclose(tili);
while(vaihtoehto == 0){

    printf("Haluatko nostaa vai tallettaa rahaa?\n");
    printf("1. Nosta rahaa\n");
    printf("2. Talleta rahaa\n");
    printf("3. Katso tilin saldo\n");

	printf("Kirjoita vaihtoehtoa vastaava numero >");
	vaihtoehto = lueKokonaisluku();
	printf("\n");

//Valitaan vaihtoehto ja mennään vastaavaan nosto tai talletus funktioon + tiedostonMuokkaus tekee tarvittavat saldomuutokset tiedostoon
//Tarkistaa onko rahaa nostettava summa 10, jota ei normaalisti voi nostaa, vaan se toimii merkkinä loopata valikko uudelleen (katso tarkemmin funktiosta)
//tulostaRahat funktiosta löytyy rahanjako algoritmi
//Vaihtoehto 3 on vain saldon tarkistus joten ei tehdä mitään koska saldon printtaus tulee kaikissa tapauksissa seuraavaksi

	switch(vaihtoehto){
		case 1:
			summa = nostaRahaa(saldo);
			if(summa == 10){
				vaihtoehto = 0;
				break;
			}
			saldo = saldo - summa;
			tiedostonMuokkaus(saldo, tilinumero, n);
			tulostaRahat(summa);
			break;
		case 2:
			talletus = talletaRahaa();
			saldo = saldo + talletus;
			tiedostonMuokkaus(saldo, tilinumero, n);
			break;
		case 3:
			break;
		default:
			printf("Virheellinen vaihtoehto, yritä uudelleen\n");
			vaihtoehto = 0;
			break;

	}
}

printf("Tilisi saldo on %.2lf euroa\n", saldo);
printf("Kiitos kaynnista ja tervetuloa uudelleen!\n");


return(0);
}

/*
tarkistaPin
- kysyy pinkoodia ja vertaa sitä tiedostosta löytyvään pinkoodiin
- jos antaa pinkoodin 3 kertaa väärin, tili lukitaan
- Tili lukitaan muuttamalla tiedoston nimi muotoon xxxxx_lukittu.tili

Parametrit: 
- char tilinumero[] 	tilinumero merkkijono joka kertoo tiedoston nimen
- int n 				merkkijonon pituus
*/

void tarkistaPin(char tilinumero[], int n){

	char pin[10];
	char tarkpin[10];
	int pin_laskuri = 0;
	char lukitus[30];

FILE *tili;

tili = fopen(tilinumero, "r");

fgets(tarkpin, 10, tili);
if(tarkpin[strlen(tarkpin) -1] == '\n')
	tarkpin[strlen(tarkpin) -1] = '\0';

fclose(tili);



while(strcmp(pin,tarkpin)!=0){
	printf("Anna pinkoodi: ");
	fgets(pin, 10, stdin);
	if(pin[strlen(pin) -1] == '\n')
    	pin[strlen(pin) -1] = '\0';
    if(pin[strlen(pin) -1] == '\r')
		pin[strlen(pin) -1] = '\0';

	if(strcmp(pin,tarkpin)==0){
    	printf("Pin oikein\n");
	}
	else if (strcmp(pin,tarkpin)!=0 && pin_laskuri < 2){
		printf("Virheellinen pin, yrita uudelleen\n");
		pin_laskuri++;
	}
	else {
		strncpy(lukitus, tilinumero, (strlen(tilinumero)-5));
		strcat(lukitus, "_lukittu.tili");
		rename(tilinumero, lukitus);
		memset(tilinumero, '\0', sizeof(tilinumero));
		strcpy(tilinumero, lukitus);
		printf("Tilisi on lukittu\n");
		break;
	}
}

}

/*
nostaRahaa
- Antaa kättöliitymän, josta voi valita vaihtoehdoista nostettavia summia tai paluun päävalikkoon
- Muu summassa voi itse kirjoittaa summan (20e pienin seteli ja 1000e maksimiraja tavallisissa automaateissa)
- loop pyörii kunnes annetaan hyväksytty summa
- Paluu päävalikkoon toimii antamalla summaksi 10 jota ei muuten pystyisi nostamaan, jolloin päävalikko tietää looppautua uudelleen

Parametrit:
- double saldo 		tilin saldo, joka on haettu mainissa tiedostosta

Paluuarvo:
- int summa 		palauttaa nostettavan summan (jos summa on 10 menee takaisin päävalikkoon)
*/

int nostaRahaa(double saldo){

	int nosto;
	int summa = 0;

while(summa > saldo || summa == 0){

printf("\nTilisi saldo on %.2lfe\n", saldo);
printf("Nostettavat summat:\n");
printf("0.  Palaa paavalikkoon\n");
printf("1.  20e\n");
printf("2.  40e\n");
printf("3.  60e\n");
printf("4.  90e\n");
printf("5.  140e\n");
printf("6.  240e\n");
printf("7.  Muu summa\n");

	printf("Kirjoita vaihtoehtoa vastaava numero >");
	nosto = lueKokonaisluku();


switch(nosto){
	case 1:
		summa = 20;
		break;
	case 2:
		summa = 40;
		break;
	case 3:
		summa = 60;
		break;
	case 4:
		summa = 90;
		break;
	case 5:
		summa = 140;
		break;
	case 6:
		summa = 240;
		break;
	case 7:
		printf("Anna nostettava summa (vahintaan 20e ja enintaan 1000e): ");
		summa = lueKokonaisluku();
		printf("\n");
		if(summa < 20 || summa > 1000){
			printf("\nViallinen nosto, yrita uudelleen\n");
			summa = 0;
		}
		break;
	case 0:
		summa = 10;
		break;
	default:
		printf("\nViallinen nosto, yrita uudelleen\n");
		break;
}

	if (summa > saldo){
		printf("Tilillasi ei ole tarpeeksi rahaa tahan nostoon\n");
		summa = 0;
	}
	else if (summa != 0 && summa % 10 != 0){
		printf("Automaatti voi antaa vain 20e ja 50e seteleita, yrita uudelleen\n");
		summa = 0;
	}

}
printf("\n");

return(summa);
}

/*
talletaRahaa
- Kysyy talletettavan määrän rahaa reaalilukumuotoisena
- talletettava summa voi olla mikä tahansa ja mainissa tapahtuu sitten itse saldon muokkaus
- Rahan talletus jäi aika turhaksi funktioksi koska tiedostonmuokkaus oli loppujen lopuksi järkevämpää tehdä omana funktionaan

Paluuarvo:
- double talletus		talletettava summa
*/

double talletaRahaa(void){

	double talletus;
 

printf("Syota talletettava summa: ");
talletus = lueReaaliluku();


return(talletus);
}

/*
tulostaRahat
- omakehitteinen algoritmi nostettavan summan jakamiseen sopiviin seteleihin
- jakaa nostettavan summan 50:llä
- tarkistaa jakojäännöksien perustella montako mitäkin seteliä tulostetaan
- algoritmi tajuaa jos nostettava summan on alle 50e ja näyttää silloin tulostuksessa vain 20e setelit

Parametrit:
- int summa 		nostettava summa, joka jaetaan seteleiksi
*/

void tulostaRahat(int summa){

	div_t jako;

jako = div(summa,50);


printf("Rahat tulevat pian\n");

if(jako.rem == 0){
	printf("%d x 50e\n", jako.quot);
}
else if(jako.rem == 10){
	if(jako.quot > 1){
		printf("%d x 50e\n", jako.quot-1);
	}
	printf("3 x 20e\n");
}
else if(jako.rem == 20){
	if(jako.quot >= 1){
		printf("%d x 50e\n", jako.quot);
	}
	printf("1 x 20e\n");
}
else if(jako.rem == 30){
	if(jako.quot > 1){
		printf("%d x 50e\n", jako.quot-1);
	}
	printf("4 x 20e\n");
}
else if(jako.rem == 40){
	if(jako.quot >= 1){
		printf("%d x 50e\n", jako.quot);
	}
	printf("2 x 20e\n");
}

}

/*
tiedostonMuokkaus
- mainissa tapahtuu ensin muutos saldo muuttujaan, joka ei muuta vielä itse tiedostoa mitenkään
- tämän takia tiedoston muokkauksesta pystyi tekemään oman funktionsa, joka helpottaa varsinkin jos tiedostonmuokkauskohtia olisi vielä enemmän
- käytännössä toimii kirjoittamalla vanhan tiedoston päälle uuden tiedoston entisen nimellä, samalla pinkoodilla ja uudella saldolla

Parametrit:
- double saldo 				uusi saldo joka muokataan tiedostoon
- char tilinumero[] 			tilinumero jotta tietää mitä tiliä muokataan
- int n 					tilinumero merkkijonon pituus
*/

void tiedostonMuokkaus(double saldo, char tilinumero[], int n){

	char temp_pin[30];

    FILE *tili;

	tili = fopen(tilinumero, "r");
	fgets(temp_pin, 30, tili);
	fclose(tili);
	tili = fopen(tilinumero, "w");
	fprintf(tili, "%s", temp_pin);
	fprintf(tili, "%.2lf\n", saldo);
	fclose(tili);

}

/*
lueKokonaisluku (luentodemo)
- lukee kokonaisluvun
- muutettu viallisen syötön printtiä sopivammaksi

Paluuarvo: 
- int luku 		kokonaisluku
*/

int lueKokonaisluku(void){

   int luku;
   char mki;
   int status;

   while((status = scanf("%d%c", &luku, &mki))==0  || (2 == status && mki != '\n') ){
      lueRoskat();
      printf("Viallinen syotto, yrita uudelleen\n");
   }

   return luku;
}

/*
lueReaaliluku (luentodemo)
- lukee reaaliluvun
- muutettu viallisen syötön printtiä sopivammaksi

Paluuarvo: 
- double luku 		reaaliluku
*/

double lueReaaliluku(void){

   double luku;
   char mki;
   int status;

   while((status = scanf("%lf%c", &luku, &mki))==0  || (2 == status && mki != '\n') ){
      lueRoskat();
      printf("Viallinen syotto, yrita uudelleen\n");
   }

   return luku;
}

/*
lueRoskat (luentodemo)
- tyhjentää puskurin
*/

void lueRoskat(void){

   while( fgetc(stdin) != '\n');

}
