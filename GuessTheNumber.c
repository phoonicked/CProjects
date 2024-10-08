#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void guessNumber(int range);

int main(){
    int range = 0;
    printf("Enter a positive number: \n");
    scanf("%d", &range);
    guessNumber(range);
    return 0;
}

void guessNumber(int range){
    int generatedNumber, guess = 0;
    srand(time(NULL));
    generatedNumber = rand() % range;
    printf("Guess a number between 1 and %d\n", range);
    do {
        printf("Guess the number: \n");
        scanf("%d", &guess);
        if(guess > generatedNumber){
            printf("Lower!\n");
        }
        else if(guess < generatedNumber){
            printf("Higher!\n");
        }
        else{
            printf("You win!\n");
        }
    } while (guess != generatedNumber);
}
