// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

#include "Count.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
	Super::BeginPlay();
	const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordList/HiddenWordList.txt");
	FFileHelper::LoadFileToStringArray(Words, *WordListPath);
	
	InitGame();

}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
	ClearScreen();

	if (bGameOver)
	{
		ClearScreen();
		InitGame();
	}
	else
	{
		ProcessGuess(Input);
	}
}

void UBullCowCartridge::InitGame()
{
	const int8 Index = FMath::RandRange(0, (Words.Num()-1) );
	HiddenWord = Words[Index];
	Lives = 4;
	bGameOver = false;

	PrintLine(TEXT("Prepare for trouble and make it double!"));
	PrintLine(TEXT("It is Bull Cows Time."));
	PrintLine(FString::Printf(TEXT("You better guess the %i letter word!"), HiddenWord.Len()));
	PrintLine(TEXT("You have %i chances"), Lives);
}

void UBullCowCartridge::EndGame()
{
	bGameOver = true;
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
	PastGuesses.Emplace(Guess);
	
	if (HiddenWord == Guess)
	{
		PrintLine(TEXT("Correct! Here some milk."));
		EndGame();
		PastGuesses.Reset();
		return;
	}

	if (HiddenWord.Len() != Guess.Len())
	{
		PrintLine(TEXT("WRONG!! The word has %i letters"), HiddenWord.Len());
	}
	else
		if (!IsIsogram(Guess))
			PrintLine(TEXT("Bad guess, the word has to be a isogram. NO REPEATING LETTER."));
		else
		{
			const FBullCowCount Count = GetBullCows(Guess);
			
			PrintLine(TEXT("That's incorrect. Only %i chances before you get cowed."), --Lives);
			PrintLine(TEXT("You have %i Bulls and %i Cows"), Count.Bulls, Count.Cows);
		}
	PrintLine(TEXT("Your past guesses were: "));
	for (FString Guesses: PastGuesses)
	{
		PrintLine(FString::Printf(TEXT("%s"), *Guesses));
	}

	if (Lives == 0)
	{
		ClearScreen();
		PrintLine(TEXT("YOU LOST!!"));
		PrintLine(TEXT("The word was: %s"), *HiddenWord);
		EndGame();
		PastGuesses.Reset();
	}
}

bool UBullCowCartridge::IsIsogram(const FString& Word)
{
	for (int32 Index = 0; Index < Word.Len(); Index++)
	{
		for (int32 Jdex = Index + 1; Jdex < Word.Len(); Jdex++)
		{
			if (Word[Index] == Word[Jdex] && Index != Jdex)
			{
				return false;
			}
		}
	}
	return true;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
	FBullCowCount Count;
	
	for (int32 Index = 0; Index < Guess.Len(); Index++)
	{
		if (Guess[Index] == HiddenWord[Index])
		{
			Count.Bulls++;
			continue;
		}

		for (int32 WordIndex = 0; WordIndex < HiddenWord.Len(); WordIndex++)
		{
			if (Guess[Index] == HiddenWord[WordIndex])
			{
				Count.Cows++;
				break;
			}
		}
	}
	return Count;
}
