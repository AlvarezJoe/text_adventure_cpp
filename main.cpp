#include <iostream>
#include <string>
#include <iomanip>

//JOEMICHAEL ALVAREZ && MIGUEL LORA
//EDITED TO CONTAIN HERO, MONSTERS, COMBAT, ITEMS, STORE, RANDOM EVENTS, WEATHER, AND A BOSS
//BALANCE IN THIS GAME WILL BE WONKY AT BEST

//GLOBAL CONSTANTS
const int ROW = 10;
const int COL = 10;

//STRUCTURES
struct TreasureChests {
    std::string name;
    int row{};
    int column{};
    double loot{};
    bool available = true;
};

struct Character {
    int health{};
    int attackPower{};
    int defense{};
    int torch{};
    bool longsword{};
    bool adamantitesword{};
    bool spikeBoots{};
    bool chainmail{};
    bool buckler{};
    bool alive = true;
};

//PROTOTYPES
void contents(TreasureChests&, TreasureChests&, TreasureChests&, TreasureChests&, TreasureChests&);
void pause();
void initialPause();
int randomNumberGenerator();
void characterInitialization(Character&, Character& ,Character&);
void combat(Character&, Character&);
void makeArr(char, char[ROW][COL]);
void intro(int);
int morningWeather();
void inventoryCheck(const Character&);
void outputArray(const char[ROW][COL], const Character&);
int inputAndValidate(int&);
double compareAndProcess (int, int, char, char, TreasureChests&, TreasureChests&, TreasureChests&,
                          TreasureChests&, TreasureChests&, Character&, Character&, char [ROW][COL]);
void daylightTracker(int);
void darknessEvent(Character&);
void storeAndRefresh(double&, Character&);
int eventGenerator(Character&);
void bossEvent(Character&, Character&);
void cheat(double, char, const TreasureChests&, const TreasureChests&, const TreasureChests&,
         const TreasureChests&, const TreasureChests&, char [ROW][COL]);

int main() {
    //CONSTANTS, STRUCTURE ARRAY, VARIABLES, AND CHARACTERS
    const int ATTEMPTS = 4;
    const int DAYS = 3; //17% MAX MAP EXPLORATION / MAX 2 CHESTS PER DAY / REPLAYABLE?
    const char TREASURE = 'T';
    const char EMPTY = 'E';
    const char UNKNOWN = '-';
    TreasureChests chest1, chest2, chest3, chest4, chest5;
    Character hero, monster, boss;
    char playAgain = ' '; //TRIED VALIDATING PLAYAGAIN AND REST BUT MY BRAIN WAS MUSH BY THEN
    char rest = ' ';
    char chestAnswer = ' '; //CHEAT
    char map[ROW][COL];
    int userRow;
    int timeCount;
    int eventCond;
    int weatherCond;
    int dayCount = 0;
    int userCols = 0;
    double sum = 0;

    //INTRO
    intro(DAYS);

    do {
        //MAKE ARRAY AND INITIALIZE STRUCTURES
        characterInitialization(hero, monster, boss);
        makeArr(UNKNOWN, map);
        contents(chest1, chest2, chest3, chest4, chest5);

        do{
            timeCount = 0; //DAILY RESET
            weatherCond = morningWeather(); //WEATHER

            if (weatherCond == 2) { //WET WEATHER CHECK
                if (!hero.spikeBoots) {
                    std::cout << "The rain has made it easier to lose your footing."
                                 " You should be more careful today.";
                    hero.attackPower -= 2;
                } else {
                    std::cout << "The rain has made it easier to lose your footing,"
                                 " but your spiked boots prevent you from slipping.";
                }
                initialPause();
            }

            inventoryCheck(hero); //INVENTORY

            do{
                outputArray(map, hero); //MAP
                userRow = inputAndValidate(userCols); //INPUT AND VALIDATE
                sum += compareAndProcess(userRow, userCols, TREASURE, EMPTY,  chest1, chest2, chest3,
                                         chest4, chest5, hero, monster, map); //PROCESSING

                if (hero.alive) {
                    daylightTracker(timeCount); //FLAVOR TEXT SO PLAYER REALIZES TIME MATTERS
                    initialPause();

                    std::cout << "\nWould you like to head back to camp? (Y or N)";
                    std::cin >> rest;
                    std::cout << '\n';

                    if (++timeCount > ATTEMPTS) { //TIME TRACKER
                        if (hero.torch != 0) { //TORCH CHECK
                            std::cout << "Darkness encroaches, but you light a torch as to not lose your way\n";
                            pause();

                            //AGAIN
                            outputArray(map, hero);
                            userRow = inputAndValidate(userCols);
                            sum += compareAndProcess(userRow, userCols, TREASURE, EMPTY,  chest1,chest2,
                                                     chest3,chest4, chest5, hero, monster, map);

                            hero.torch -= 1;
                            rest = 'Y';
                        } else { //RIP
                            darknessEvent(hero);
                            rest = 'Y';
                        }
                    }
                } else {
                    rest = 'Y';
                }
            } while (toupper(rest) == 'N');

            if (hero.alive) {
                std::cout << "You head back to camp to rest.\n\n";
                pause();

                if (weatherCond == 2 && !hero.spikeBoots) { //WEATHER RESET
                    hero.attackPower +=2;
                }

                switch (eventCond) { //EVENT RESET
                    case 2:
                        hero.attackPower += 2;
                        break;
                    case 3:
                        hero.attackPower -= 2;
                        break;
                    default:
                        break;
                }

                if (++dayCount > DAYS) { //DAY TRACKER, STORE, AND RESET
                    storeAndRefresh(sum, hero);
                    bossEvent(hero, monster);
                    hero.alive = false;
                } else {
                    storeAndRefresh(sum, hero);
                    eventCond = eventGenerator(hero);
                }
                pause();
            }
        } while (hero.alive);

        /* CHEATING
        std::cout << "Would you like to see all the chest locations? (Y or N)";
        std::cin >> chestAnswer;
        std::cout << '\n';

        if (toupper(chestAnswer) == 'Y') {
            cheat(sum, TREASURE, chest1, chest2, chest3, chest4, chest5, map); //FINAL OUTPUT
            outputarray(map, hero);
            pause();
        }
        */

        std::cout << "Would you like to play again? (Y or N)";
        std::cin >> playAgain;
        std::cout << '\n';

    } while (toupper(playAgain) == 'Y');

    return 0;
}

//FUNCTION DEFINITION
//H = HERO / M = MONSTER / B = BOSS

//CHEST INITIALIZER
void contents(TreasureChests& A,TreasureChests& B,TreasureChests& C,TreasureChests& D,TreasureChests& E){
    std::cout << std::fixed << std::setprecision(2);

    A.name ="Pirate's Chest";
    B.name = "Dragon's Chest";
    C.name = "Troll's Chest";
    D.name = "Queen's Chest";
    E.name = "Thief's Chest";

    A.row = 2;
    B.row = 5;
    C.row = 3;
    D.row = 8;
    E.row = 1;

    A.column = 4;
    B.column = 8;
    C.column = 1;
    D.column = 0;
    E.column = 9;

    A.loot = 145.90;
    B.loot = 258.24;
    C.loot = 203.71;
    D.loot = 190.15;
    E.loot = 98.95;

}

void pause() {
    std::cin.ignore(1000, '\n');
    std::getchar();
    std::cout << '\n'; //Shouldn't have included these end lines messes too much with spacing
}

void initialPause() {
    std::getchar();
    std::cout << '\n'; //Shouldn't have included these end lines messes too much with spacing
}

int randomNumberGenerator() { //Could use <random> but I'm lazy
    srand(time(0));

    return rand();
}

void characterInitialization(Character& H, Character& M, Character& B) {
    H.health = 25;
    H.attackPower = 8;
    H.defense = 3;
    H.torch = 0;
    H.buckler = true;
    H.longsword = false;
    H.adamantitesword = false;
    H.chainmail = false;
    H.spikeBoots = false;
    H.alive = true;

    M.health = 10;
    M.attackPower = 8;
    M.defense = 0;

    B.health = 25;
    B.attackPower = 20;
    B.defense = 5;
}

void combat(Character& H, Character& M) {
    while(M.health > 0 && H.health > 0) {
        if (H.adamantitesword) { //IN CASE M GETS DEFENSE
            H.health -= (M.attackPower - H.defense);
            M.health -= (H.attackPower);
        } else {
            H.health -= (M.attackPower - H.defense);
            M.health -= (H.attackPower - M.defense);
        }
        std::cout << "You exchange swings with the monster.\nHealth: " << H.health << "\n\n";
    }
    //COMBAT RESULT
    if (H.health <= 0 && M.health <= 0) {
        std::cout << "You and the monster wound each other mortally.\n\n";
        H.alive = false;
    } else if (H.health <= 0) {
        std::cout << "The monster wounds you mortally.\n\n";
        H.alive = false;
    } else {
        std::cout << "You vanquish the monster.\n\n";
        M.health = 11;
    }
}

void makeArr(const char u, char arr[ROW][COL]){
    for(int i=0;i < ROW; i++){
        for(int j=0;j <COL; j++){
            arr[i][j]= u;
        }
    }
}

void intro(const int a){
    std::cout<<"This is a treasure hunt game.\nYou have " << a <<
    " days to find the 5 treasure chests hidden in the grid below.\n\nPress Enter to continue in this game.";

    initialPause();
}

int morningWeather() {
    if (randomNumberGenerator() % 3 == 0) {
        if (randomNumberGenerator() % 2 == 0) {  //33% / 2
            std::cout << "\nThe clouds occlude the sun as you peer out your tent.";
            initialPause();
            return 1;
        } else { //33% / 2
            std::cout << "\nThe sound of rain beats down on your tent wakes you up.";
            initialPause();
            return 2;
        }
    } else { //~67%
        std::cout << "\nYou wake up as brightness begins seeping into your tent.";
        initialPause();
    }
    return 0;
}

void inventoryCheck(const Character& H) { //ADDS TO THE IMPORTANCE OF INVENTORY AND ENSURE EVERYTHING IS WORKING RIGHT
    std::cout << "\nYou make your way into the wilderness equipped with ";

    if (H.adamantitesword) {
        std::cout << "an adamantite sword, ";
    } else if (H.longsword) {
        std::cout << "a longsword, ";
    } else {
        std::cout << "a shortsword, ";
    }

    if (H.buckler) {
        std::cout << "a buckler, ";
    }

    if (H.chainmail) {
        std::cout << "chainmail, ";
    } else {
        std::cout << "a tunic, ";
    }

    if (H.spikeBoots) {
        std::cout << "and spiked boots.\n";
    } else {
        std::cout << "and flimsy boots.\n";
    }
    initialPause();
}

void outputArray(const char arr[ROW][COL],const Character& H){
    int count= 0;

    std::cout << "  0 1 2 3 4 5 6 7 8 9\n";

    for(int i=0;i < ROW; i++){
        std::cout<<count++<<" ";
        for(int j=0;j <COL; j++){
            std::cout<<arr[i][j]<<" ";
        }
        std::cout<<std::endl;
    }

    std::cout << "\nHealth: " << H.health << "\n\nLegend: - (Unknown); E (Empty); T (Treasure)\n\n";
}

int inputAndValidate(int& C ) {
    int row = 0;
    std::cout << "Enter the row #: ";
    std::cin >> row;
    std::cout << '\n';

    while (row < 0 || row > 9 || std::cin.fail()) {
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(100, '\n');
        }
        std::cout << "That entry is invalid. Please enter a valid integer based on the grid size: ";
        std::cin >> row;
        std::cout << '\n';
    }

    std::cout << "Enter the column #: ";
    std::cin >> C;
    std::cout << '\n';

    while (C < 0 || C > 9 || std::cin.fail()) {
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(100, '\n');
        }
        std::cout << "That entry is invalid. Please enter a valid integer based on the grid size: ";
        std::cin >> C;
        std::cout << '\n';
    }

    return row;
}

double compareAndProcess(const int R, const int Co, const char t, const char e, TreasureChests& A, TreasureChests& B,
                         TreasureChests& C, TreasureChests& D, TreasureChests& E, Character& H, Character& M,
                         char arr[ROW][COL]) { //THAT'S A LOT OF PARAMETERS
    std::string stupid = "You revisit the site where you have been previously.";
    //REPEATS A LOT
    if (R == A.row && Co == A.column) {
        if (A.available) {
            std::cout << "You find a monster guarding a treasure chest.";
            pause();
            combat(H, M);

            if (H.alive) {
                std::cout << "You have found the " << A.name << "!\nInside you find $" << A.loot
                << " worth of valuables.";

                arr[R][Co] = t;
                A.available = false;
                initialPause();
                return A.loot;
            } else {
                return 0;
            }

        } else {
            std::cout << stupid << '\n';
            pause();
            return 0;
        }
    }

    if (R == B.row && Co == B.column) {
        if (B.available) {
            std::cout << "You find a monster guarding a treasure chest.";
            pause();
            combat(H, M);

            if (H.alive) {
                std::cout << "You have found the " << B.name << "!\nInside you find $" << B.loot
                << " worth of valuables.";

                arr[R][Co] = t;
                B.available = false;
                initialPause();
                return B.loot;
            } else {
                return 0;
            }
        } else {
            std::cout << stupid << '\n';
            pause();
            return 0;
        }
    }

    if (R == C.row && Co == C.column) {
        if (C.available) {
            std::cout << "You find a monster guarding a treasure chest.";
            pause();
            combat(H, M);

            if (H.alive) {
                std::cout << "You have found the " << C.name << "!\nInside you find $" << C.loot
                << " worth of valuables.";

                arr[R][Co] = t;
                C.available = false;
                initialPause();
                return C.loot;
            } else {
                return 0;
            }
        } else {
            std::cout << stupid << '\n';
            pause();
            return 0;
        }
    }

    if (R == D.row && Co == D.column) {
        if (D.available) {
            std::cout << "You find a monster guarding a treasure chest.";
            pause();
            combat(H, M);

            if (H.alive) {
                std::cout << "You have found the " << D.name << "!\nInside you find $" << D.loot
                << " worth of valuables.";

                arr[R][Co] = t;
                D.available = false;
                initialPause();
                return D.loot;
            } else {
                return 0;
            }
        } else {
            std::cout << stupid << '\n';
            pause();
            return 0;
        }
    }

    if (R == E.row && Co == E.column) {
        if (E.available) {
            std::cout << "You find a monster guarding a treasure chest.";
            pause();
            combat(H, M);

            if (H.alive) {
                std::cout << "You have found the " << E.name << "!\nInside you find $" << E.loot
                << " worth of valuables.";

                arr[R][Co] = t;
                E.available = false;
                initialPause();
                return E.loot;
            } else {
                return 0;
            }
        } else {
            std::cout << stupid << '\n';
            pause();
            return 0;
        }
    }

    std::cout << "Sorry, there is no treasure at those coordinates.\n";
    pause();

    arr[R][Co] = e;

    return 0;
}
void daylightTracker(const int T) {
    if (T == 0) {
        std::cout << "The sun begins peaking over the tree line";
    } else if (T == 1) {
        std::cout << "The sun hangs high in the sky casting distorted shadows in the forest";
    } else if (T == 2) {
        std::cout << "An afternoon breeze begins blowing, providing respite from the heat of the day";
    } else if (T == 3) {
        std::cout << "Light begins its retreat as an airy coolness begins encroaching upon the wilderness.";
    } else {
        std::cout << "The light of the moon looms over you as glowing eyes can be seen in the thickets.";
    }
}

void darknessEvent(Character& H) { //GOTTA BRING THOSE TORCHES
    std::cout << "In your search for treasure darkness encompasses you and you become lost.";
    pause();
    std::cout << "As yellow eyes begin surrounding you, you dart deeper into the unknown. Never to be seen again.\n";
    pause();

    H.alive = false;
}

void storeAndRefresh(double& S, Character& H) { //LONGEST PART TO MAKE STORE OP
    const int tPrice = 40;
    const int lPrice = 160;
    const int aPrice = 230;
    const int sPrice = 80;
    const int cPrice = 100;
    const int bPrice = 50;
    int menu;
    char answer = ' ';

    std::cout << "On your way back to camp, you stumble upon a traveling merchant.\n";

    do {
        std::cout << "(1)Torch: " << tPrice << "gp\n(2)Longsword: " << lPrice << "gp\n(3)Adamantite Sword: " << aPrice
        << "gp\n(4)Spike Boots: " << sPrice << "gp\n(5)Chainmail: " << lPrice << "gp\n(6)Buckler: "
        << bPrice << "gp\n(0)Exit\n\n";

        menu = -1;

        while (menu < 0 || menu > 6 || std::cin.fail()) {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(100, '\n');
            }

            std::cout << "You currently have " << S << "gp.\nWhat would you like to buy? ";
            std::cin >> menu;
            std::cout << "\n\n";
        }
        switch (menu) {
            case 1:
                if (S < tPrice) {
                    std::cout << "You unfortunately don't have enough gold pieces.\n";
                    break;
                } else {
                    std::cout << "You have " << H.torch << " torches currently.\n\n"
                    << "Light your path in the dark.\nWould you like to buy a torch for " << tPrice
                    << "gp? Answer (Y or N) ";
                    std::cin >> answer;
                    std::cout << '\n';

                    if (toupper(answer) == 'Y') {
                        S -= tPrice;
                        H.torch++;
                    }
                    break;
                }

            case 2:
                    if (S < lPrice) {
                        std::cout << "You unfortunately don't have enough gold pieces.\n";
                        break;
                    } else {
                        if (H.longsword) {
                            std::cout << "You already own this.\n";
                        } else if (H.adamantitesword) {
                            std::cout << "You already have a better sword.\n";
                        } else {
                            std::cout << "Strong and light.\nWould you like to buy a longsword for " << lPrice
                            << "gp? Answer (Y or N) ";
                            std::cin >> answer;
                            std::cout << '\n';
                        }

                        if (toupper(answer) == 'Y') {
                            S -= lPrice;
                            H.longsword = true;
                            H.attackPower += 3;
                        }
                        break;
                    }

            case 3:
                if (S < aPrice) {
                    std::cout << "You unfortunately don't have enough gold pieces.\n";
                    break;
                } else {
                    if (H.adamantitesword) {
                        std::cout << "You already own this.\n";
                    } else {
                        std::cout << "A green shimmer with a sharp tip.\nWould you like to buy an"
                                     " adamantite sword for " << aPrice << "gp? Answer (Y or N) ";
                        std::cin >> answer;
                        std::cout << '\n';
                    }

                    if (toupper(answer) == 'Y') {
                        S -= aPrice;
                        H.adamantitesword = true;

                        if (H.longsword) {
                            H.attackPower += 2;
                        } else {
                            H.attackPower += 5;
                        }
                    }
                    break;
                }

            case 4:
                if (S < sPrice) {
                    std::cout << "You unfortunately don't have enough gold pieces.\n";
                    break;
                } else {
                    if (H.spikeBoots) {
                        std::cout << "You already own this.\n";
                    } else {
                        std::cout << "Sturdy boots with sharp ridges on the sole.\nWould you like to buy"
                                     " spiked boots for " << sPrice << "gp? Answer (Y or N) ";
                        std::cin >> answer;
                        std::cout << '\n';
                    }

                    if (toupper(answer) == 'Y') {
                        S -= sPrice;
                        H.spikeBoots = true;
                        H.attackPower += 1;
                        H.defense += 1;
                    }
                    break;
                }

            case 5:
                if (S < cPrice) {
                    std::cout << "You unfortunately don't have enough gold pieces.\n";
                    break;
                } else {
                    if (H.chainmail) {
                        std::cout << "You already own this.\n";
                    } else {
                        std::cout << "Lightweight yet very protective.\nWould you like to buy "
                                     "chainmail for " << cPrice << "gp? Answer (Y or N) ";
                        std::cin >> answer;
                        std::cout << '\n';
                    }

                    if (toupper(answer) == 'Y') {
                        S -= cPrice;
                        H.chainmail = true;
                        H.defense += 3;
                    }
                    break;
                }

            case 6:
                if (S < bPrice) {
                    std::cout << "You unfortunately don't have enough gold pieces.\n";
                    break;
                } else {
                    if (H.buckler) {
                        std::cout << "You already own this.\n";
                    } else {
                        std::cout << "Small but affordable defense.\nWould you like to buy"
                                     " a buckler for " << bPrice << "gp? Answer (Y or N) ";
                        std::cin >> answer;
                        std::cout << '\n';
                    }

                    if (toupper(answer) == 'Y') {
                        S -= bPrice;
                        H.buckler = true;
                        H.defense += 2;
                    }
                    break;
                }

            default:
                break;
        }
    } while (menu != 0);

    std::cout << "You find your way into your bedroll.\n";
    H.health = 25; //HEALTH REFRESH
}

int eventGenerator(Character& H) {
    if (randomNumberGenerator() % 5 == 0) { //20% OF ALL EVENTS
        if (randomNumberGenerator() % 3 == 0) { //~5% of 20% CHANCE
            std::cout << "You awaken after hearing a rustling in the dark.";
            pause();
            std::cout << "You see a small impish creature with your buckler in hand. It darts out into the darkness.\n"
                         "You give chase, but lose it in the darkness.";

            H.buckler = false; //VERY UNFORTUNATE
            H.defense -= 2;
            return 1;
        } else if (randomNumberGenerator() % 2 == 0) { //~45% of 20% CHANCE
            std::cout << "You awaken after hearing a rustling in the dark.";
            pause();
            std::cout << "As you peer outside your tent a beady pair of yellow eyes stares back.";
            pause();
            std::cout << "You quickly arm yourself and exit the tent, but the eyes have disappeared into the night\n"
                         "You spend the rest of the night on edge and lose sleep in the process.";

            H.attackPower -= 2;
            return 2;
        } else { //~45% of 20% CHANCE
            std::cout << "The moon is bright tonight and cuts into the dark like a knife.";
            pause();
            std::cout << "With the renewed confidence of your safety, you sleep soundly.";

            H.attackPower += 2;
            return 3;
        }
    }
    return 0;
}

void bossEvent(Character& H, Character& B) {
    std::cout << "A immense gnarled monster lumbers out of a thicket.\n";
    pause();
    std::cout << "You draw your ";

    if (H.adamantitesword) {
        std::cout << "adamantite sword ";
    } else if (H.longsword) {
        std::cout << "longsword ";
    } else {
        std::cout << "shortsword ";
    }

    std::cout << "in preparation for battle.";
    initialPause();

    while(B.health > 0 && H.health > 0) {
        if (H.adamantitesword) {
            H.health -= (B.attackPower - H.defense);
            B.health -= (H.attackPower);
        } else {
            H.health -= (B.attackPower - H.defense);
            B.health -= (H.attackPower - B.defense);
        }
        std::cout << "You exchange swings with the behemoth.\nHealth: " << H.health << "\n\n";
    }

    if (H.health <= 0 && B.health <= 0) {
        std::cout << "You and the behemoth wound each other mortally.\n"
                     "If only you were better prepared.\n";
        H.alive = false;
    } else if (H.health <= 0) {
        std::cout << "You were not strong enough to defeat the behemoth.\n"
                     "If only you were better prepared.\n";
        H.alive = false;
    } else {
        std::cout << "Once you defeat the behemoth, you spot a series of eyes surrounding you.\n"
                     "They quickly scatter in what sounds like thunderous applause.\n"
                     "A silence and stillness once again grips the wilderness.\n";
    }
}

void cheat(double s, const char t, const TreasureChests& A, const TreasureChests& B, const TreasureChests& C,
         const TreasureChests& D, const TreasureChests& E, char arr[ROW][COL]) {

    arr[A.row][A.column] = t;
    arr[B.row][B.column] = t;
    arr[C.row][C.column] = t;
    arr[D.row][D.column] = t;
    arr[E.row][E.column] = t;

    std::cout << "The game is now over. You looted $" << s << " worth of valuables from chests.\n"
    << "The grid below shows the position of all treasure chests:\n\n";
}