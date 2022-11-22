#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define SIZE 52
#define STRING 10

//final.c allows player to keep drawing until move is able to be made

//Prototypes

void setDeck(int[], int[]);
//Set values and suits arrays to correct deck values
//Values and suits arrays

void printDeck(int[], int[]);
//Print entire deck of cards with format
//Values and suits arrays

void printCard(int[], int[], int);
//Print single card with format
//Values and suits arrays, with index of card to print
void shuffle(int[], int[]);
//Shuffle deck of cards
//Values and suits arrays

bool validMove(int[], int[], int, int);
//Determine whether a move is valid (suit or value has a match)
//Values and suits arrays, index of play card, index of move made
//Boolean value of whether move is valid or not

void removeCard(int[], int);
//Removes a card from a player's hand
//Player array with number of cards player has

int getSuit(int[], int);
//Get suit value of current card
//Suit array and card index
//Value of suit (1-4)

int getValue(int[], int);
//Get value of current card
//Value array with card index
//Value of card (2-14)

int main(void) {
    int i;

    //Values and suits arrays, used for deck
    int values[SIZE] = {0};
    int suits[SIZE] = {0};

    //Player and computer arrays, stores the hand
    int player[SIZE] = {0};
    int computer[SIZE] = {0};

    //youCards and compCards keeps track of # of cards in each player's hand
    //playIndex stores index of the card being played
    //drawIndex stores index of card being drawn
    int youCards, compCards, playIndex, drawIndex, move, suitSelect;

    bool youMove = false, cardFound = false;

    srand(time(NULL));

    //Set deck and shuffle deck
    setDeck(values, suits);
    shuffle(values, suits);

    //Deal 5 cards to each player
    youCards = 5;
    compCards = 5;

    //Set initial play and draw index. Since 10 cards were dealt (0-9) then index to play is 10
    playIndex = 10;

    //Draw index will keep be added to until it is = 52
    //First index of draw pile will be 11, and then 12, 13, and so on
    drawIndex = 11;

    //Set values for the cards that were dealt
    for (i = 0; i < 5; i++) {
        player[i] = i;
        computer[i] = i + 5;
    }

    //Loop as long as player has cards and there are cards left to be drawn
    while (youCards > 0 && compCards > 0 && drawIndex < SIZE) {

        //Message with how many cards each player has left
        printf("CRAZY 8! - YOU: %d - COMPUTER: %d\n", youCards, compCards);

        //Print your cards
        printf("\nYOUR CARDS:\n");
        for (i = 0; i < youCards; i++) {
            printf("Card: %d | ", i + 1);
            printCard(values, suits, player[i]);
        }

        //Print card to play
        printf("\nPLAY CARD: ");
        printCard(values, suits, playIndex);
        printf("\n");

        do {
            //Ask player to make move
            printf("MAKE MOVE (1-%d, 0 to draw):", youCards);
            scanf("%d", &move);

            //Set boolean of valid move
            if (move != 0) {
                youMove = validMove(values, suits, playIndex, player[move - 1]);
            }
            //If move is 0, then the loop must break to draw a card. Set youMove to true to break loop
            else
                youMove = true;

            //Loop as long as move is an invalid input, or move is invalid
        } while (move < 0 || move > youCards || !youMove);

        //If player chose to draw card
        if (move == 0) {
            //Logic to draw card
            player[youCards] = drawIndex;
            youCards++;
            drawIndex++;

            //Print card drawn
            printf("\nPLAYER CARD DRAWN:\n");
            printf("Card: %d | ", youCards);
            printCard(values, suits, player[youCards - 1]);
            printf("\n");

            //Ask player to make move or not based on new card drawn
            //Same loop logic as last do while loop
            do {
                printf("MAKE MOVE (1-%d, 0 for no move):", youCards);
                scanf("%d", &move);

                if (move != 0) {
                    youMove = validMove(values, suits, playIndex, player[move - 1]);
                }
                else
                    youMove = true;
            } while (move < 0 || move > youCards || !youMove);
        }

        //If player made a move, continue
        if (move != 0) {
            //Set new play card index to card that player chose
            playIndex = player[move - 1];

            //Logic for if new play card was an 8 card
            if (getValue(values, playIndex) == 8) {
                printf("\n8 CARD PLAYED\n");
                do {
                    //Name new suit based on value given
                    printf("NAME SUIT (1:C, 2:D, 3:H, 4:S):");
                    scanf("%d", &suitSelect);
                    //Loop while suitSelect is invalid
                } while (suitSelect < 1 || suitSelect > 4);
                //Set new suit value
                suits[playIndex] = suitSelect;
            }

            //Basically used as a sentinel value. If = -1, removes card from hand. if > 0, keeps card in hand
            //This logic can be seen in the removeCard function
            player[move - 1] = -1;
            //Remove 1 from number of cards player has
            youCards--;
            removeCard(player, youCards);
            //Remove card
            printf("\nCARD PLAYED: ");
            printCard(values, suits, playIndex);
        }
        else
            //Print if no move was made
            printf("\nNO MOVE MADE\n");

        //Algorithm to find out if computer has valid move in hand
        //Sets cardFound value to true if valid move is found
        //Loop until a valid move is found or all cards in hand have been scanned
        i = 0;
        cardFound = false;
        while (!cardFound && i < compCards) {
            if (validMove(values, suits, playIndex, computer[i]))
                cardFound = true;
            else {
                i++;
            }
        }

        //If player still has cards in hand (if not, game will end and computer move does not matter)
        if (youCards > 0) {
            //If computer does not have valid move
            if (!cardFound) {
                //Logic to draw card
                computer[compCards] = drawIndex;
                compCards++;
                drawIndex++;
                printf("\nCOMPUTER CARD DRAWN\n\n");
            } else {
                //Logic to remove computer card
                //Same as removing a player card
                playIndex = computer[i];
                if (getValue(values, playIndex) == 8) {
                    //If computer plays 8 card, a random value is selected for the new suit
                    suits[playIndex] = rand() % 4 + 1;
                    printf("\nCOMPUTER PLAYED 8 CARD AND CHOSE SUIT");
                }
                computer[i] = -1;
                compCards--;
                removeCard(computer, compCards);
                printf("\nCOMPUTER CARD PLAYED: ");
                printCard(values, suits, playIndex);
                printf("\n");
            }
        }
    }

    //Print message based on values. These values determine the outcome of the game
    if (youCards == 0)
        printf("\nYOU WIN - GAME OVER\n");
    else if (compCards == 0)
        printf("COMPUTER WINS - GAME OVER\n");
    else
        printf("TIE - GAME OVER\n");

    return 0;
}

void setDeck(int values[], int suits[]) {
    int i, cardCounter = 2, suitCounter = 1;

    //Set values from 2-14, set suits from 1-4
    for (i = 0; i < SIZE; i++) {
        values[i] = cardCounter;
        suits[i] = suitCounter;

        if (cardCounter % 14 == 0) {
            cardCounter = 2;
            suitCounter++;
        }
        else
            cardCounter++;
    }
}

//Print entire deck
void printDeck(int values[], int suits[]) {
    char suit[STRING];
    char value[STRING];
    int i;

    //Set suit string based on value
    for (i = 0; i < SIZE; i++) {
        switch (suits[i]) {
            case 1:
                strcpy(suit, "Clubs");
                break;
            case 2:
                strcpy(suit, "Diamonds");
                break;
            case 3:
                strcpy(suit, "Hearts");
                break;
            case 4:
                strcpy(suit, "Spades");
                break;
        }

        //Set value string if necessary, and print single card
        //If value string is not needed (value is not Jack,Queen,King,Ace), will default to printing value integer
        switch (values[i]) {
            case 11:
                strcpy(value, "Jack");
                printf("Suit: %s | Value: %s\n", suit, value);
                break;
            case 12:
                strcpy(value, "King");
                printf("Suit: %s | Value: %s\n", suit, value);
                break;
            case 13:
                strcpy(value, "Queen");
                printf("Suit: %s | Value: %s\n", suit, value);
                break;
            case 14:
                strcpy(value, "Ace");
                printf("Suit: %s | Value: %s\n", suit, value);
                break;
            default:
                printf("Suit: %s | Value: %d\n", suit, values[i]);
                break;
        }
    }
}

//Print single card, same logic as printDeck function
void printCard(int values[], int suits[], int c) {
    char suit[STRING];
    char value[STRING];

    switch (suits[c]) {
        case 1:
            strcpy(suit, "Clubs");
            break;
        case 2:
            strcpy(suit, "Diamonds");
            break;
        case 3:
            strcpy(suit, "Hearts");
            break;
        case 4:
            strcpy(suit, "Spades");
            break;
    }

    switch (values[c]) {
        case 11:
            strcpy(value, "Jack");
            printf("Suit: %s | Value: %s\n", suit, value);
            break;
        case 12:
            strcpy(value, "King");
            printf("Suit: %s | Value: %s\n", suit, value);
            break;
        case 13:
            strcpy(value, "Queen");
            printf("Suit: %s | Value: %s\n", suit, value);
            break;
        case 14:
            strcpy(value, "Ace");
            printf("Suit: %s | Value: %s\n", suit, value);
            break;
        default:
            printf("Suit: %s | Value: %d\n", suit, values[c]);
            break;
    }
}

//Shuffle deck. Will swap random values from 0-51 1000 times
void shuffle(int values[], int suits[]) {
    int i, r1, r2, t1, t2;

    for (i = 0; i < 1000; i++) {
        r1 = rand() % 52;
        r2 = rand() % 52;

        if (r1 != r2) {
            t1 = values[r1];
            t2 = suits[r1];
            values[r1] = values[r2];
            suits[r1] = suits[r2];
            values[r2] = t1;
            suits[r2] = t2;
        }
    }
}

//Get suit value (1-4)
int getSuit(int suits[], int c) {
    return suits[c];
}

//Get card value (2-14)
int getValue(int values[], int c) {
    return values[c];
}

//Determines if a particular move is valid
bool validMove(int values[], int suits[], int c, int m) {
    //If suit values of play card and move card are same
    //If value of play card and move card are same
    //If value of move card is 8
    //All return true
    if (getSuit(suits, c) == getSuit(suits, m) ||
        getValue(values, c) == getValue(values, m) ||
        getValue(values, m) == 8)
        return true;
    else
        return false;
}

//Remove card from player deck
void removeCard(int player[], int youCards) {
    int i = 0, j = 0;
    //Algorithm to remove card and remove any gaps from player array
    //Main will set value of card to remove to -1
    //So if value is >= 0, algorithm will remove card and correct the array so there is no gaps
    while (i < (youCards + 1) && j < youCards) {
        if (player[i] >= 0) {
            player[j] = player[i];
            j++;
        }
        i++;
    }
}