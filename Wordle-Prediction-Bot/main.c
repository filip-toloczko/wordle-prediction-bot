/* ---------------------------------------------
Author: Filip Toloczko and Vlad Symonenko
---------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>   // for exit( -1)
#include <string.h>

// Declaration of struct used to store words and their scores
struct wordCountStruct{
  char word[6];
  int score;
};

//Get number of words in answers file
void getNumberInAnswers(char answersFileName[], int *wordsInAnswers){
  FILE *inFileAnswersPtr  = fopen(answersFileName, "r");  // Connect logical name to filename
  char inputString[ 81];

  // Sanity check: ensure file open worked correctly
  if( inFileAnswersPtr == NULL ) {
    printf("Error: could not open %s for reading\n", answersFileName);
    exit(-1);    // must include stdlib.h
  }

    // Read each word from file and print it.  You could do other things along the
    // way, like counting how many words there are.
  while( fscanf( inFileAnswersPtr, "%s", inputString) != EOF) {
    *wordsInAnswers+=1;
  }

  // Close the file
  fclose( inFileAnswersPtr);
}

//Get number of words in guesses file
void getNumberInGuesses(char guessesFileName[], int *wordsInGuesses){
  FILE *inFileGuessesPtr  = fopen(guessesFileName, "r");  // Connect logical name to filename
  char inputString[ 81];

  // Sanity check: ensure file open worked correctly
  if( inFileGuessesPtr == NULL ) {
    printf("Error: could not open %s for reading\n", guessesFileName);
    exit(-1);    // must include stdlib.h
  }

  // Read each word from file and print it.  You could do other things along the
  // way, like counting how many words there are.
  while( fscanf( inFileGuessesPtr, "%s", inputString) != EOF) {
    *wordsInGuesses+=1;
  }

  // Close the file
  fclose( inFileGuessesPtr);
}

//Add words to answers file
void populateAnswers(char answersFileName[], char **answersList){
  FILE *inFileAnswersPtr  = fopen(answersFileName, "r");  // Connect logical name to filename
  char inputString[ 81];
  int index = 0;
  // Sanity check: ensure file open worked correctly
  if( inFileAnswersPtr == NULL ) {
    printf("Error: could not open %s for reading\n", answersFileName);
    exit(-1);    // must include stdlib.h
  }

  // Read each word from file and print it.  You could do other things along the
  // way, like counting how many words there are.
  while( fscanf( inFileAnswersPtr, "%s", inputString) != EOF) {
    for(int i = 0; i<6; i++){
      answersList[index][i] = inputString[i];
    }

    index++;
  }

  // Close the file
  fclose( inFileAnswersPtr);
}

//Add words to guesses file
void populateGuesses(char guessesFileName[], char **guessesList){
  FILE *inFileGuessesPtr  = fopen(guessesFileName, "r");  // Connect logical name to filename
  char inputString[ 81];
  int index = 0;
  // Sanity check: ensure file open worked correctly
  if( inFileGuessesPtr == NULL ) {
    printf("Error: could not open %s for reading\n", guessesFileName);
    exit(-1);    // must include stdlib.h
  }

  // Read each word from file and print it.  You could do other things along the
  // way, like counting how many words there are.
  while( fscanf( inFileGuessesPtr, "%s", inputString ) != EOF) {
    
    for(int i = 0; i<6; i++){
      guessesList[index][i] = inputString[i];
    }
    
    index++;
  }

  // Close the file
  fclose( inFileGuessesPtr);
}

// Iterates over answers array and gets the score for each word
int GetScoreForWord(int numLetters, // number of letters in each word
                    char answerWord[], // word to check the guess against
                    char guessWord[]) { // the word to get score for

  int points = 0;
  
  // check if the letter matches and is in the correct position
  for(int letterIndex = 0; letterIndex < numLetters; letterIndex++){

    //Check if the letters are in the identical spot 
    if(answerWord[letterIndex] == guessWord[letterIndex]){
      points += 3;
      
      // blank out the letter so that we cannot use it again
      answerWord[letterIndex] = ' ';
      guessWord[letterIndex] = ' ';
    }
  }

  // check if the letter matches but is not in the correct position
  for (int guessWordIndex = 0; guessWordIndex < numLetters; guessWordIndex++) {
  
    // skip the letter already verified to be in the correct position
    if (guessWord[guessWordIndex] == ' ') {
      continue;
    }

    // Increment the counter variable for the index of the answer word
    for(int ansWordIndex = 0; ansWordIndex < numLetters; ansWordIndex++) {
      
      // find the first occurence of the letter and skip the rest of the letters in answers
      if (answerWord[ansWordIndex] == guessWord[guessWordIndex]) {

        //Add a point if the letter is in the word but not the right spot
        points++;
        // blanked out so that we do not read in this position again. instead we need to
        // search for another occurence of the letter
        answerWord[ansWordIndex] = ' '; 
        break;
      }
    }
  }
  return points;
}

// Function used in order to sort the words in the array based on score and alphabetical order
void sortInDescending(struct wordCountStruct wordCountArr[], // array of structs
                      int size) { // size of the array

  int i = 0;
  int j = 0;

  // Temporary variable for swap
  struct wordCountStruct temp;  

  //For loop that runs the sort
  for (i = 1; i < size; ++i) {
    j = i;
    while (j > 0 && (wordCountArr[j].score > wordCountArr[j - 1].score || (wordCountArr[j].score == wordCountArr[j - 1].score) && strcmp(wordCountArr[j].word, wordCountArr[j-1].word) < 0)) {
       temp = wordCountArr[j];
       wordCountArr[j] = wordCountArr[j - 1];
       wordCountArr[j - 1] = temp;
       --j;
    }
  }
}

//Main function that runs the program
int main() {
  char answersFileName[81] = "answersTiny.txt";  // Stores the answers file name
  char guessesFileName[81] = "guessesTiny.txt";;  // Stores the guesses file name

  printf("Default file names are %s and %s\n", answersFileName, guessesFileName);
  
  int wordsInAnswers = 0; //Stores number of words in answer file
  int wordsInGuesses = 0; //Stores number of words in guesses file

  //Functions that get number of words in answer and guess files
  getNumberInAnswers(answersFileName, &wordsInAnswers);
  getNumberInGuesses(guessesFileName, &wordsInGuesses);

  //Allocate memory for arrays
  char **answersList = malloc((wordsInAnswers)*sizeof(char*));
  for(int i = 0; i<wordsInAnswers; i++){
    answersList[i] = malloc(6*sizeof(char));
  }
  
  char **guessesList = malloc((wordsInGuesses)*sizeof(char*));
  for(int i = 0; i<wordsInGuesses; i++){
    guessesList[i] = malloc(6*sizeof(char));
  } 
  
  char **allWords = malloc((wordsInAnswers + wordsInGuesses)*sizeof(char*));
  
  //Functions that populates separate arrays
  populateAnswers(answersFileName, answersList);
  populateGuesses(guessesFileName, guessesList);

  //Combine the elements of wordsInAnswers and wordsInGuesses into one array
  for (int i = 0; i < wordsInAnswers; i++) {
    allWords[i] = answersList[i];
  }
  for (int j = 0; j < wordsInGuesses; j++) {
    allWords[wordsInAnswers + j] = guessesList[j];
  }
  
  //Default menu option set to 0
  int menuOption = 0;

  // Do while loop displays menu and asks for user input for menu option
  do {
    printf("\n");
    printf("Menu Options:\n");
    printf("  1. Display best first words only\n");
    printf("  2. Display best first and best second words\n");
    printf("  3. Change answers and guesses filenames\n");
    printf("  4. Exit\n");
    printf("Your choice: ");
    scanf(" %d", &menuOption);

    // Handle menu option 3 to exit the program
    if (menuOption == 4) {
      exit(1); // Exit the program
    }
    else if (menuOption == 3) {
      
      // Change file names.  Menu will then be redisplayed.   
      printf("Enter new answers and guesses filenames: ");
      scanf(" %s %s", answersFileName, guessesFileName);
      wordsInAnswers = 0;
      wordsInGuesses = 0;
      //Functions that get number of words in answer and guess files
      getNumberInAnswers(answersFileName, &wordsInAnswers);
      getNumberInGuesses(guessesFileName, &wordsInGuesses);
          
      free(answersList);
      free(guessesList);
      free(allWords);
      
      answersList = malloc((wordsInAnswers)*sizeof(char*));
      for(int i = 0; i<wordsInAnswers; i++){
        answersList[i] = malloc(6*sizeof(char));
      }
  
      guessesList = malloc((wordsInGuesses)*sizeof(char*));
      
      for(int i = 0; i<wordsInGuesses; i++){
        guessesList[i] = malloc(6*sizeof(char));
      } 
    
      //Functions that populate arrays
      populateAnswers(answersFileName, answersList);
      populateGuesses(guessesFileName, guessesList);
    
      allWords = malloc((wordsInAnswers + wordsInGuesses)*sizeof(char*));
    
      for (int i = 0; i < wordsInAnswers; i++) {
        allWords[i] = answersList[i];
      }
      for (int j = 0; j < wordsInGuesses; j++) {
        allWords[wordsInAnswers + j] = guessesList[j];
      }
    }
        // Handle menu option 1 of the program
        else if (menuOption == 1){
          
          //Declare an array of wordCountStruct objects
          struct wordCountStruct structArray[wordsInAnswers + wordsInGuesses];
    
          //For loop that loops through all of the words 
          for(int allWordsIndex = 0; allWordsIndex < wordsInAnswers + wordsInGuesses; allWordsIndex++){
            
            struct wordCountStruct item;
            int points = 0;
            char answerWord[6];
            char anyWord[6];
    
            //For loop that loops through all the answer words
            
            for(int answerIndex = 0; answerIndex<wordsInAnswers; answerIndex++){
              strcpy(anyWord, allWords[allWordsIndex]);
              strcpy(answerWord, answersList[answerIndex]);
              points += GetScoreForWord(5, answerWord, anyWord);
            }
            
            item.score = points;
            strcpy(item.word, allWords[allWordsIndex]);
            
            structArray[allWordsIndex] = item;
          }
    
          //Print the amount of words in each file
          printf("%s has %d words\n", answersFileName, wordsInAnswers);
          printf("%s has %d words\n", guessesFileName, wordsInGuesses);
    
          //Sort the words in descending order
          sortInDescending(structArray, wordsInAnswers + wordsInGuesses);
    
          //Print the word(s) with the highest score(s)
          printf("\nWords and scores for top first words:\n");
          int j = 0;
          while(structArray[j].score == structArray[0].score){
            printf("%s %d\n", structArray[j].word, structArray[j].score);
            j++;
          }
        }
    
    // Handle menu option 2
    if(menuOption == 2){
      
      // Declare an array of type wordCountStruct
      struct wordCountStruct structArray[wordsInAnswers + wordsInGuesses];
      
      //Print file names and how many words are in each file
      printf("%s has %d words\n", answersFileName, wordsInAnswers);
      printf("%s has %d words\n", guessesFileName, wordsInGuesses);

      // Loop through the words in both files
      for(int allWordsIndex = 0; allWordsIndex < wordsInAnswers + wordsInGuesses; allWordsIndex++){
        
        struct wordCountStruct item;
        int points = 0;
        char answerWord[6];
        char anyWord[6];

        // Loop through the answer words and remove matching letters
        for(int answerIndex = 0; answerIndex<wordsInAnswers; answerIndex++){
          strcpy(anyWord, allWords[allWordsIndex]);
          strcpy(answerWord, answersList[answerIndex]);
          points += GetScoreForWord(5, answerWord, anyWord);
        }

        // Set the score of the struct equal to points
        item.score = points;
        strcpy(item.word, allWords[allWordsIndex]);

        // Add the struct to the array of structs
        structArray[allWordsIndex] = item;
      }

      //Sort all of the words in descending order
      sortInDescending(structArray, wordsInAnswers + wordsInGuesses);

      // Print out prompt to display first and second words and their scores
      printf("\nWords and scores for top first words and second words:\n");
      int i = 0;

      // Loop while the score of the struct item is equal to the score of the highest struct item
      while (structArray[i].score == structArray[0].score){

        //Create an array that copies the answerList array
        char **copyAnswersList = malloc((wordsInAnswers)*sizeof(char*));

        //Loop through words in answers and populate the copy array
        for(int ptrIndex = 0; ptrIndex < wordsInAnswers; ptrIndex++){
          copyAnswersList[ptrIndex] = malloc(6*sizeof(char));
          
          for (int letterIndex = 0; letterIndex < 6; letterIndex++){
            copyAnswersList[ptrIndex][letterIndex] = answersList[ptrIndex][letterIndex];
          }
        }

        // compare answers to best scoring word
        for(int j = 0; j<wordsInAnswers; j++){
          char word[6];
          strcpy(word, structArray[i].word);
          GetScoreForWord(5, copyAnswersList[j], word);
        }

        // Create a struct to find the second best words
        struct wordCountStruct structArrayForBestWord[wordsInAnswers + wordsInGuesses];

        // Loop through all of the words
        for(int allWordsIndex = 0; allWordsIndex < wordsInAnswers + wordsInGuesses; allWordsIndex++){
          
          struct wordCountStruct item;
          int points = 0;
          char answerWord[6];
          char anyWord[6];

          // Loop through all of the answer words to calculate points
          for(int answerIndex = 0; answerIndex<wordsInAnswers; answerIndex++){
            strcpy(anyWord, allWords[allWordsIndex]);
            strcpy(answerWord, copyAnswersList[answerIndex]);
            points += GetScoreForWord(5, answerWord, anyWord);
          }

          // Set the struct score value equal to the number of points
          item.score = points;
          strcpy(item.word, allWords[allWordsIndex]);

          // Add the structs to the array of structs
          structArrayForBestWord[allWordsIndex] = item;
        }

        // Sort the items in the array of structs in a descending order
        sortInDescending(structArrayForBestWord, wordsInAnswers + wordsInGuesses);

        // Print the best word and the second best word with their scores
        printf("%s %d\n", structArray[i].word, structArray[i].score);

        // Loop through and display all of the best 2nd words
        int y = 0;
        while(structArrayForBestWord[y].score == structArrayForBestWord[0].score){
          printf("   %s %d", structArrayForBestWord[y].word, structArrayForBestWord[y].score);
          y++;
        }
        printf("\n");

        // Increment for the while loop
        i++;

        // Free the copy array of answer words
        free(copyAnswersList);
      }
      
      int arrayPosition = 0;
    }   
  } while (menuOption == 3);

  printf("Done\n");
  return 0;
} // end main()
