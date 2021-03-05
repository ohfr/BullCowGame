// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    InitGame(); //setting up game

}

void UBullCowCartridge::OnInput(const FString &Input) // When the player hits enter
{
    if (!gameOver) { // game running, check player guess
        if (lives == 0) {
            PrintLine(TEXT("You've ran out of lives"));
            PrintLine(TEXT("The word was: %s"), *HiddenWord);
            EndGame();
            // prompt to play again 
        }

        if (Input == HiddenWord) {
            PrintLine(TEXT("Correct !"));
            EndGame();
        } else if (Input == ""){
            PrintLine(TEXT("Please enter a guess!"));
        } else {
            PrintLine(TEXT("Incorrect, try again!"));
            lives--;
        }
    } else if (Input == ""){
        ClearScreen();
        InitGame();
    }
}

void UBullCowCartridge::InitGame() {
    gameOver = false;
    HiddenWord = TEXT("cake");
    lives = HiddenWord.Len();

    PrintLine(TEXT("Welcome to Bull Cows"));
    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());
    PrintLine(TEXT("Press enter to continue..."));
}

void UBullCowCartridge::EndGame() {
    gameOver = true;
    PrintLine(TEXT("Press Enter to play again"));

}