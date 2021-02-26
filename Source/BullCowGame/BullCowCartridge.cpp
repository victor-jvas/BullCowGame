// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    InitGame();
    
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    ClearScreen();

    if (!bGameOver)
    {
        if (HiddenWord.Len() == Input.Len())
        {
            if (HiddenWord == Input)
            {
                PrintLine(TEXT("Correct! Here some milk."));
                EndGame();
            }
            else
            {
                PrintLine(TEXT("WRONG!! You gonna get cowed"));
                --Lives;
                if (Lives == 0)
                {
                    EndGame();
                }
            }
        }
        else
        {
            PrintLine(TEXT("That's not even the right letter count..."));
        }
    }
    else
    {
        ClearScreen();
        InitGame();
    }
    
    
}

void UBullCowCartridge::InitGame() 
{
    HiddenWord = TEXT("birds");
    Lives = 4;
    bGameOver = false;

    PrintLine(TEXT("Prepare for trouble and make it double!"));
    PrintLine(TEXT("It is Bull Cows Time."));
    PrintLine(FString::Printf(TEXT("You better guess the %i letter word!"), HiddenWord.Len()));

}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
}