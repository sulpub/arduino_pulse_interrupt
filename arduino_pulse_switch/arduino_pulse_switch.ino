/*
   Code ouverture contact sur impulsion

   Le but est d'envoyer une impulsion à l'ouverture puis à la fermeture d'un contact
   L'interruption n'est pas utilisé
                  |-------------------------------------|
   contact   -----|                                     |-------------

                  |-----|                               |-----|
   inpulsion -----|     |-------------------------------|     |-----

   Date : 11/01/2020
   auteur : TBD

*/

/*
 * TODO :
 * - utiliser l'interruption
 * - ajouter des controles sur les variables 
 */
 
#define IN_PIN_RF   2       //17:D3 inter mini32 ou 2 config2
#define OUT_PIN_CMD 7       //16:D4 led   mini32 ou 7 config2
#define DUREE_PULSE 50      //ms
#define DUREE_DEBOUNCE 200  //ms


/*
____   ____            .__      ___.   .__                 
\   \ /   /____ _______|__|____ \_ |__ |  |   ____   ______
 \   Y   /\__  \\_  __ \  \__  \ | __ \|  | _/ __ \ /  ___/
  \     /  / __ \|  | \/  |/ __ \| \_\ \  |_\  ___/ \___ \ 
   \___/  (____  /__|  |__(____  /___  /____/\___  >____  >
               \/              \/    \/          \/     \/ 
 */
int flip  = 0;
int etatrf = 0;


/*
               __
  ______ _____/  |_ __ ________
  /  ___// __ \   __\  |  \____ \
  \___ \\  ___/|  | |  |  /  |_> >
  /____  >\___  >__| |____/|   __/
     \/     \/           |__|
*/
void setup() {
  //initialisation pin ENTREE
  pinMode(IN_PIN_RF, INPUT);
  pinMode(IN_PIN_RF,INPUT_PULLUP); //evite les etat bizarre si interrupteur dessoudé ou fil flottant
  digitalWrite(IN_PIN_RF, LOW);

  //initalisation pin SORTIE
  pinMode(OUT_PIN_CMD, OUTPUT);
  digitalWrite(OUT_PIN_CMD, LOW);

  //Initalisation port COM
  Serial.begin(115200);

  //Verbose
  Serial.printf("Demarrage programme.\n");

}  //end SETUP



/*
  .__
  |  |   ____   ____ ______
  |  |  /  _ \ /  _ \\____ \
  |  |_(  <_> |  <_> )  |_> >
  |____/\____/ \____/|   __/
                   |__|
*/
void loop() {

  //test reel
  etatrf = digitalRead(IN_PIN_RF);

  //simulation pin uart
  if (Serial.available()!=0) {
    etatrf = (char)Serial.read();
    if (etatrf == 0x30) etatrf=0;
    if (etatrf == 0x31) etatrf=1;    
  }

  //gestion pulse fermeture contact
  if (((etatrf == 1)  && (flip == 0)))
  {
    //debug port COM
    Serial.printf("Pulse contact ON : %d\n", etatrf);

    //bloque l'entree si le contact reste à 1
    flip = 1;

    //generation du pulse
    digitalWrite(OUT_PIN_CMD, HIGH);
    delay(DUREE_PULSE);  //en ms
    digitalWrite(OUT_PIN_CMD, LOW);
    delay(DUREE_DEBOUNCE);  //en ms
  }

  //gestion pulse ouverture contact
  if (((etatrf == 0)  && (flip == 1)))
  {
    //debug port COM
    Serial.printf("Pulse contact OFF : %d\n", etatrf);

    //bloque l'entree si le contact reste à 0
    flip = 0;

    //generation du pulse
    digitalWrite(OUT_PIN_CMD, HIGH);
    delay(DUREE_PULSE);  //en ms
    digitalWrite(OUT_PIN_CMD, LOW);
    delay(DUREE_DEBOUNCE);  //en ms
  }
}  //end LOOP
