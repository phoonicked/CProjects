#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PLAYER_CARDS 11

typedef struct {
    char *rank;
    char *suit;
    int value;
} Card;

//Function prototypes
void initialiseDeck(Card deck[]);
void shuffleDeck(Card deck[]);
Card dealCard(Card deck[]);
void addCardToHand(Card playerHand[], int *cardCount, Card drawCard);
void displayHand(Card playerHand[], int cardCount);
int calculateHandValue(Card playerHand[], int cardCount);
void playerTurn(Card deck[], Card playerHand[], int *cardCount);


const char *suits[] = {"Hearts", "Spades", "Diamonds", "Clubs"};
const char *ranks[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "King", "Queen"};

int currentCard = 0;

int main(){
    Card deck[52];
    Card playerHand[MAX_PLAYER_CARDS];
    int playerCardCount = 0;

    initialiseDeck(deck);
    shuffleDeck(deck);

    addCardToHand(playerHand, &playerCardCount, dealCard(deck));
    addCardToHand(playerHand, &playerCardCount, dealCard(deck));
    displayHand(playerHand, playerCardCount);

    playerTurn(deck, playerHand, &playerCardCount);

    return 0;
}

/*void bettingSystem(int amountOfBet){
    // If player wins
    if(){
        
    }
}

void gameSystem(){

}
*/

void initialiseDeck(Card deck[]){
    int index = 0;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 13; j++){
            deck[index].suit = (char *)suits[i];
            deck[index].rank = (char *)ranks[j];

            // Assign value based on rank for Blackjack
            if(j >= 10){
                deck[index].value = 10;
            }
            else if(j == 0){
                deck[index].value = 11;
            }
            else if (j > 0 && j <= 9) {
                deck[index].value = j + 1;
            }
            index++;
        }
    }
}

void shuffleDeck(Card deck[]){
    srand(time(NULL));
    for(int i = 0; i < 52; i++){
        int randomIndex = rand() % 52;
        //Swap card with random index
        Card temp = deck[i];
        deck[i] = deck[randomIndex];
        deck[randomIndex] = temp;
    }
}



Card dealCard(Card deck[]){
    if(currentCard >= 52){
        printf("All cards have been dealt. Reshuffling the deck...\n");
        shuffleDeck(deck);
        currentCard = 0;
    }
    return deck[currentCard++];
}

void addCardToHand(Card playerHand[], int *cardCount, Card drawCard){
    if(*cardCount < MAX_PLAYER_CARDS){
        playerHand[*cardCount] = drawCard;
        (*cardCount)++;
    } else {
        printf("Unable to draw. Your hand is full!\n");
    }
}

void displayHand(Card playerHand[], int cardCount){
    printf("Your hand: \n");
    for(int i = 0; i < cardCount; i++){
       printf(" - %s of %s (Value: %d)\n", playerHand[i].rank, playerHand[i].suit, playerHand[i].value);
    }
}

int calculateHandValue(Card playerHand[], int cardCount){
    int sum = 0;
    int aceCount = 0;
    for(int i = 0; i < cardCount; i++){
        sum += playerHand[i].value;
        if(sum == 11){
            aceCount++;
        }
    }
    while(sum > 21 && aceCount > 0){
        sum -= 10;
        aceCount--;
    }
    return sum;
}

void playerTurn(Card deck[], Card playerHand[], int *cardCount){
    char choice;
    while(1){
        int handValue = calculateHandValue(playerHand, *cardCount);
        if(handValue == 21){
            printf("BLACKJACK!\n");
            break;
        }
        printf("Your hand value is %d\n", handValue);
        printf("Hit (h) or Stand (s)?\n");
        scanf(" %c", &choice);
        if(choice == 'h'){
            addCardToHand(playerHand, cardCount, dealCard(deck));
            displayHand(playerHand, *cardCount);
            if(calculateHandValue(playerHand, *cardCount) > 21){
                printf("BUST!\n");
                break;
            }
        }
        else if(choice == 's'){
            printf("You chose to stand.\n");
            break;
        }
        else{
            printf("Invalid choice. Please only choose 'h' to hit or 's' to stand.\n");
        }
    }
}
    
