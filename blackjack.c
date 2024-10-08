#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CARDS 11

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
void displayHand(Card playerHand[], int cardCount, int hideFirstCard);
int calculateHandValue(Card playerHand[], int cardCount);
void playerTurn(Card deck[], Card playerHand[], int *cardCount);
void dealerTurn(Card deck[], Card dealerHand[], int *cardCount);
void checkWinner(Card playerHand[], int playerCardCount, Card dealerHand[], int dealerCardCount, int *playerBalance, int *bet);
void checkBetValidity(int *playerBalance, int *bet);
void saveBalance(int playerBalance);
int loadBalance();

const char *suits[] = {"Hearts", "Spades", "Diamonds", "Clubs"};
const char *ranks[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "King", "Queen"};

int currentCard = 0;

int main(){
    Card deck[52];
    Card playerHand[MAX_CARDS];
    Card dealerHand[MAX_CARDS];
    int dealerCardCount = 0;
    int playerCardCount = 0;
    int bet = 0;
    int playerBalance = loadBalance();

    printf("Enter your bet: \n");
    scanf("%d", &bet);
    checkBetValidity(&playerBalance, &bet);

    initialiseDeck(deck);
    shuffleDeck(deck);


    addCardToHand(dealerHand, &dealerCardCount, dealCard(deck));
    addCardToHand(dealerHand, &dealerCardCount, dealCard(deck));
    addCardToHand(playerHand, &playerCardCount, dealCard(deck));
    addCardToHand(playerHand, &playerCardCount, dealCard(deck));

    printf("Dealer's hand: \n");
    displayHand(dealerHand, dealerCardCount, 1);

    printf("Your hand: \n");
    displayHand(playerHand, playerCardCount, 0);

    playerTurn(deck, playerHand, &playerCardCount);
    dealerTurn(deck, dealerHand, &dealerCardCount);

    checkWinner(playerHand, playerCardCount, dealerHand, dealerCardCount, &playerBalance, &bet);

    return 0;
}

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

void addCardToHand(Card hand[], int *cardCount, Card drawCard){
    if(*cardCount < MAX_CARDS){
        hand[*cardCount] = drawCard;
        (*cardCount)++;
    } else {
        printf("Unable to draw. Hand is full!\n");
    }
}

void displayHand(Card playerHand[], int cardCount, int hideFirstCard){
    for(int i = 0; i < cardCount; i++){
        if(i == 0 && hideFirstCard) {
          printf(" - Hidden Card\n");
        } 
        else {
            printf(" - %s of %s (Value: %d)\n", playerHand[i].rank, playerHand[i].suit, playerHand[i].value);
        }
    }
}

int calculateHandValue(Card hand[], int cardCount){
    int sum = 0;
    int aceCount = 0;
    for(int i = 0; i < cardCount; i++){
        sum += hand[i].value;
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

void playerTurn(Card deck[], Card playerHand[], int *playerCardCount){
    char choice;
    while(1){
        int handValue = calculateHandValue(playerHand, *playerCardCount);
        if(handValue == 21){
            printf("BLACKJACK!\n");
            break;
        }
        printf("Your hand value is %d\n", handValue);
        printf("Hit (h) or Stand (s)?\n");
        scanf(" %c", &choice);
        if(choice == 'h'){
            addCardToHand(playerHand, playerCardCount, dealCard(deck));
            displayHand(playerHand, *playerCardCount, 0);
            if(calculateHandValue(playerHand, *playerCardCount) > 21){
                printf("%d. BUST!\n", handValue);
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

void dealerTurn(Card deck[], Card dealerHand[], int *dealerCardCount){
    printf("\nDealer's turn...\n");
    displayHand(dealerHand, *dealerCardCount, 0);

    int handValue = calculateHandValue(dealerHand, *dealerCardCount);
    while(handValue < 17){
        printf("Dealer hits.\n");
        addCardToHand(dealerHand, dealerCardCount, dealCard(deck));
        displayHand(dealerHand, *dealerCardCount, 0);
        handValue = calculateHandValue(dealerHand, *dealerCardCount);
    }
    if(handValue > 21){
        printf("Dealer busts.\n");
    }
    else {
        printf("Dealer stands with a hand value of %d\n", handValue);
    }
}

void checkWinner(Card playerHand[], int playerCardCount, Card dealerHand[], int dealerCardCount, int *playerBalance, int *bet){
    int playerHandValue = calculateHandValue(playerHand, playerCardCount);
    int dealerHandValue = calculateHandValue(dealerHand, dealerCardCount);

    if (playerHandValue > 21) {
        printf("You lose! You busted.\n");
        *playerBalance -= *bet;
        saveBalance(*playerBalance);
    }
    else if (dealerHandValue > 21) {
        printf("You win! The dealer busted.\n");
        *playerBalance += *bet;
        saveBalance(*playerBalance);
    } 
    else if (playerHandValue > dealerHandValue) {
        printf("You win! Your hand value is higher.\n");
        *playerBalance += *bet;
        saveBalance(*playerBalance);
    } 
    else if (playerHandValue < dealerHandValue) {
        printf("You lose! The dealer's hand value is higher.\n");
        *playerBalance -= *bet;
        saveBalance(*playerBalance);
    }
     else {
        printf("It's a tie!\n");
    }
    printf("Your new balance is: $%d\n", *playerBalance);
}

void checkBetValidity(int *playerBalance, int *bet){
    while(*bet <= 0 || *bet > *playerBalance) {
        if (*bet <= 0) {
            printf("Error. You can't bet nothing or negative money!\n");
        } else if (*bet > *playerBalance) {
            printf("Error. You can't bet more than your current balance of $%d!\n", *playerBalance);
        }
        printf("Please enter a valid bet: ");
        scanf("%d", bet);
    }
    printf("You have placed $%d\n", *bet);
}

void saveBalance(int playerBalance){
    FILE *file = fopen("balance.txt", "w");
    if(file == NULL){
        printf("Error opening file to save balance.\n");
        return;
    }
    fprintf(file, "%d", playerBalance);
    fclose(file);
}

int loadBalance(){
    FILE *file = fopen("balance.txt", "r");
    if(file == NULL){
        printf("Error. No such file is found. Your balance is set to $500.\n");
        return 500;
    }

    int balance;
    if(fscanf(file, "%d", &balance) != 1){
        printf("Error. File is empty. Your balance is set to $500.\n");
        fclose(file);
        return 500;
    }
    
    fclose(file);
    return balance;
}
