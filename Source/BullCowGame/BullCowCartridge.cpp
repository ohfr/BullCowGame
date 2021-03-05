// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
 
void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWOrdList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);
    // FFileHelper::LoadFileToStringArrayWithPredicate(Words, *WordListPath, [](const FString& Word) { return Word.Len() < 10; }) using a lambda function to only pass in words that pass
    ValidWords = GetValidWords(Words);
    InitGame(); //setting up game

}

void UBullCowCartridge::OnInput(const FString &Input) // When the player hits enter  constant Input REFERENCE 
{
    // int32 a = 0;
    // int32& refa = a; int32 & refa = a; int32 &refa = a;  all are the same whitespace doesn't matter, first is standard, should use const with references const int32& refa = a;

    if (!gameOver) { // game running, check player guess
        if (lives == 0) {
            PrintLine(TEXT("You've ran out of lives"));
            PrintLine(TEXT("The word was: %s"), *HiddenWord);
            EndGame();
            // prompt to play again 
        }
        ProcessGuess(Input);
    } else if (Input == ""){
        ClearScreen();
        InitGame();
    }
}

void UBullCowCartridge::InitGame() {
    gameOver = false;
    HiddenWord =ValidWords[FMath::RandRange(0, ValidWords.Num()-1)];  //set HiddenWord to a random word from a valid word list
    lives = HiddenWord.Len();

    PrintLine(TEXT("Welcome to Bull Cows"));
    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());
    PrintLine(TEXT("Press enter to continue..."));

    // debug
    PrintLine(TEXT("The hidden word is: %s"), *HiddenWord);
}

void UBullCowCartridge::EndGame() {
    gameOver = true;
    PrintLine(TEXT("Press Enter to play again"));

}

void UBullCowCartridge::ProcessGuess(const FString& Guess) {
    if (Guess == "") {
        PrintLine(TEXT("Please enter a guess!"));
        return;
    }

    if (!IsIsogram(Guess)) {
        PrintLine("No repeatng letters, try again!");
        return;
    }

    if (Guess == HiddenWord) {
        PrintLine(TEXT("Correct !"));
        EndGame();
        return;
    }

    FBullCowCount Score = GetBullCows(Guess);
    PrintLine(TEXT("You have %i bulls, and %i cows"), Score.Bulls, Score.Cows);
    --lives;
    PrintLine(TEXT("Incorrect, try again! You have %i lives left."), lives);
    return;
    
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const {
    for (int32 i=0;i < Word.Len();i++) {
        for (int32 j=i+1; j < Word.Len();j++) {
            if (Word[i] == Word[j]) {
                return false;
            }
        }
    }   

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const {
    TArray<FString> ValidWordList;

    for (FString Word : WordList) {  // RANGE BASED FOR LOOP; works the same as a for in loop in python
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word)) {
            ValidWordList.Emplace(Word);
        }
    }
    return ValidWordList;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const {
   FBullCowCount Count; //new FBullCowCount structure

    // for every index Guess is same as index HiddenWord, BullCOunt ++
    // if not a bull was it a cow ? if yes CowCount ++

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++) {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex]) {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++) {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex]) {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}