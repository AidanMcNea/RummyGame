

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>





//The Rank enum defines the possible ranks for a card
enum Rank
{
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	JACK,
	QUEEN,
	KING
};

/*
The Suit enum defines the possible suits that a card can have
*/
enum Suit
{
	SPADES,
	HEARTS,
	DIAMONDS,
	CLUBS
};


/*
The CardPile struct keeps an array of card pointers to represent a deck
The max size of the card array is 52 because there can be a maximum of 52 cards at any time
*/
struct CardPile {
	struct Card* cards[52];
	int currentSize;
};


/*
The Table struct holds the values for a meld table as an arrays of CardPile pointers
*/
struct Table {
	struct CardPile* table[10];
	int currentSize;
};


//The arrays are stored as global constant C strings for easy access later
const char* Ranks[13] = { "ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING" };
const char* Suits[4] = { "SPADES", "HEARTS", "DIAMONDS", "CLUBS" };




/*
A card is defined by its rank and suit
*/
struct Card
{
	enum Rank rank;
	enum Suit suit;
};


struct CardPile deck;

int row;
int col;
int dealer;
int firstToPlay;
int players;


struct CardPile player1;
struct CardPile player2;
struct CardPile player3;
struct CardPile player4;


int turn;
struct CardPile discardPile;
int playerTurn = 1;
int playerAction;


struct CardPile* thisMeld;
struct CardPile* runCards;
struct Table table;

void initializeDeck();
void printDeck();
void shuffleDeck();
void dealCards();
void showPlayerHand();
void showDiscardPile();
void PlayGame();
void addFromDeckToDiscard();
void gainCard(int currentPlayer);
void drawCard(int currentPlayer);
void drawFromDiscard(int currentPlayer);
void discardCard(int currentPlayer);
void makeMove(int playerNumber);
void meld(int currentPlayer, int indices[], int countIndeces);
void run(int currentPlayer, int indices[], int countIndeces);
void layOff(int currentPlayer, int cardIndex, int meldIndex);
void showTable();
int checkForWinner();
char* printCard(struct Card* c);

int main()
{
	srand(time(NULL));


	initializeDeck();
	printDeck();
	shuffleDeck();
	printDeck();

	printf("Deck Suffled! Ready to play. \n");

	//keep prompting the player to input a valid number of players (between 2 and 4)
	do
	{
		printf("\n How Many Players are playing?\n");
		scanf("%d", &players);

		if (players < 2)
		{
			printf("You must have atleast two players \n");
		}
		else if (players > 4)
		{
			printf("Too many players, Max number of players is four \n");
		}
	} while (players < 2 || players > 4);

	//to give player 1 an option on his first turn
	addFromDeckToDiscard();
	showDiscardPile();
	dealCards();
	PlayGame();
}

//The removeCard method removes a card from the card pile and shifts the array accordingly
void removeCard(struct CardPile* cp, int index) {

	//Making sure that an invalid index is not provided
	if (index < 0 || index >= cp->currentSize) {
		return;
	}

	cp->currentSize--;
	//Shifting the array to left from the index of the card to be removed
	for (int i = index; i < cp->currentSize; i++) {
		cp->cards[i] = cp->cards[i + 1];
	}

}


//This method sorts an integer array using the selection sort algorithm
void sortArr(int indices[], int countIndices) {
	int i = 0;


	for (i = 0; i < countIndices - 1; i++)
	{

		int minIndex = i;
		int j = 0;

		//Going through the inner sub array and checking if a new minimum can be found
		for (j = i + 1; j < countIndices; j++) {
			if (indices[j] < indices[minIndex]) {
				minIndex = j;
			}
		}

		//The minimum element is now present at the minIndex variable, so swapping with it
		int temp = indices[minIndex];
		indices[minIndex] = indices[i];
		indices[i] = temp;
	}
}

//The removeCardPile method removes a card pile and shifts the array accordingly
void removeCardPile(struct Table* table, int index) {

	//Making sure that an invalid index is not provided
	if (index < 0 || index >= table->currentSize) {
		return;
	}

	table->currentSize--;
	//Shifting the array to left from the index of the card to be removed
	for (int i = index; i < table->currentSize; i++) {
		table->table[i] = table->table[i + 1];
	}

}







//This method  adds a card to the array
void addCard(struct CardPile* cp, struct Card* c) {

	//Cannot add card if the max size is reached
	if (cp->currentSize == 52) {
		return;
	}



	cp->cards[cp->currentSize] = c;

	cp->currentSize++;

}


//This method  adds a card pile to the array
void addCardPile(struct Table* table, struct CardPile* c) {

	//Cannot add card if the max size is reached
	if (table->currentSize == 10) {
		return;
	}

	table->table[table->currentSize] = c;

	table->currentSize++;

}


//This method shuffles the deck randomly
void shuffleDeck()
{

	int length = 52;

	//fisher-yates algorithm to shuffle deck
	for (int i = length - 1; i > 0; i--)
	{
		int j = (rand() % (i + 1));
		//swap deck[i] and deck[j]
		struct Card temp;
		temp.suit = deck.cards[i]->suit;
		temp.rank = deck.cards[i]->rank; // int temp = A

		//A = B
		deck.cards[i]->rank = deck.cards[j]->rank;
		deck.cards[i]->suit = deck.cards[j]->suit;

		//B = temp
		deck.cards[j]->rank = temp.rank;
		deck.cards[j]->suit = temp.suit;
	}
}

//create a deck of 13 ranks and 4 suits (52 cards in total)
void initializeDeck()
{
	//Initially the deck is empty
	deck.currentSize = 0;

	for (int suit = 0; suit < 4; suit++)
	{
		for (int rank = 0; rank < 13; rank++)
		{
			//Allocating space for card so that there is no need to copy it again and again
			struct Card* card = malloc(sizeof(struct Card));


			card->suit = suit;
			card->rank = rank;
			deck.cards[deck.currentSize++] = card;
		}
	}
}


//The printDeck method prints the entire deck on console
void printDeck()

{

	int i, j;
	for (i = 0; i < deck.currentSize; i++)
	{
		printf("%s\n", printCard(deck.cards[i]));
	}
}

void addFromDeckToDiscard()
{
	//move the first card from the deck and push it into the discard pile
	//then remove that card from the deck.

	//getting the card
	struct Card* card = deck.cards[deck.currentSize - 1];

	//adding the card to discard pile
	discardPile.cards[discardPile.currentSize++] = card;

	//removing card from the deck
	deck.cards[deck.currentSize - 1] = NULL;
	deck.currentSize--;
}

//only display the last element of the vector discardPile (to immitate the actual Rummy game)
void showDiscardPile()
{

	printf("==================\n");
	printf("   Discard Pile   \n");
	if (discardPile.currentSize == 0)
	{
		printf("   EMPTY   /n");
	}
	else
	{
		printf("%s\n", printCard(discardPile.cards[discardPile.currentSize - 1]));

	}
	printf("==================\n");
}

void showPlayerHand(int playerNum)
{
	//depending on who the current plaeer is,
	//iterate over the player's card pile and print out all the cards
	printf("==================\n");
	printf(" Player %d's cards= \n", (playerNum + 1));
	printf("==================\n");
	int i = 1;
	switch (playerNum)
	{
	case 0:
		//first player's hand
		for (i = 0; i < player1.currentSize; i++)
		{
			printf("%d. %s\n", i + 1, printCard(player1.cards[i]));
			
		}
		break;
	case 1:
		//seconds player's hand
		for (i = 0; i < player2.currentSize; i++)
		{
			printf("%d. %s\n", i + 1, printCard(player2.cards[i]));
			
		}
		break;
	case 2:
		//third player's hand
		for (i = 0; i < player3.currentSize; i++)
		{
			printf("%d. %s\n", i + 1, printCard(player3.cards[i]));
			
		}
		break;
	case 3:
		//fourth player's hand
		for (i = 0; i < player4.currentSize; i++)
		{
			printf("%d. %s\n", i + 1, printCard(player4.cards[i]));
			
		}
		break;
	}
	printf("==================\n");
}

char* printCard(struct Card* c)
{
	char* str = malloc(sizeof(char) * 100);
	char buffer[20];
	//Copying the rank to a new string
	strcpy(str, Ranks[c->rank]);

	//Concatinating the word " of "
	strcat(str, " of ");




	//Converting suit from integer to string using itoa method
	//The converted string is concatenated with the string that needs to be returned
	strcat(str, Suits[c->suit]);

	return str;
}


//The dealCards method is used at the start of the game to deal cards to the players
void dealCards()
{
	int cards = 0;
	if (players == 2)
	{
		//pop an element from the vector (deck vector)
		//push the card into the hand of the player
		//push 10 cards to player 1 and push 10 cards to player 2
		for (cards = 0; cards < 10; cards++)
		{
			addCard(&player1, deck.cards[deck.currentSize - 1]);
			removeCard(&deck, deck.currentSize - 1);
			addCard(&player2, deck.cards[deck.currentSize - 1]);
			removeCard(&deck, deck.currentSize - 1);

		}
	}
	else if (players == 3)
	{
		//deal 7 cards only when 3 players
		for (cards = 0; cards < 7; cards++)
		{
			addCard(&player1, deck.cards[deck.currentSize - 1]);
			removeCard(&deck, deck.currentSize - 1);

			addCard(&player2, deck.cards[deck.currentSize - 1]);
			removeCard(&deck, deck.currentSize - 1);

			addCard(&player3, deck.cards[deck.currentSize - 1]);
			removeCard(&deck, deck.currentSize - 1);


		}
	}
	else if (players == 4)
	{
		//for 4 players, each of them gets 7 cards as well
		for (cards = 0; cards < 7; cards++)
		{
			addCard(&player1, deck.cards[deck.currentSize - 1]);
			removeCard(&deck, deck.currentSize - 1);

			addCard(&player2, deck.cards[deck.currentSize - 1]);
			removeCard(&deck, deck.currentSize - 1);

			addCard(&player3, deck.cards[deck.currentSize - 1]);
			removeCard(&deck, deck.currentSize - 1);

			addCard(&player4, deck.cards[deck.currentSize - 1]);
			removeCard(&deck, deck.currentSize - 1);


		}
	}

	int i;
	//show all the players' hands
	for (i = 0; i < players; i++)
	{
		showPlayerHand(i);
		printf("\n");
	}
}

void PlayGame()
{
	//keeps track of whose turn it is ( 0 for player 1,  and 1 for player 2, etc..)
	int currentPlayer = 0; //0 , 1, 2, 3, 0 ,1 ...
	int gameOver = 0; // win/draw condition not met yet

	while (gameOver == 0)
	{
		//announce new player's turn
		printf("\n Player %d's turn \n", (currentPlayer + 1));

		//display the cards for the player so he/she can make a move
		showPlayerHand(currentPlayer);

		//enable the player to draw a card from the deck or discard pile
		gainCard(currentPlayer);

		showPlayerHand(currentPlayer);

		makeMove(currentPlayer);

		showTable();

		showPlayerHand(currentPlayer);

		discardCard(currentPlayer);

		//increment the current player and bring it back to 0 if it reaches the number of players
		currentPlayer = (currentPlayer + 1) % players;
	}
} //}

void gainCard(int currentPlayer)
{
	showDiscardPile();
	printf("\n Press 1 to draw from deck or 2 to draw from discard pile\n");
	scanf("%d", &playerAction);

	if (playerAction == 1)
	{
		drawCard(currentPlayer);
	}
	else if (playerAction == 2)
	{
		drawFromDiscard(currentPlayer);
	}
}







//This method removes card from deck and adds it to the discard pile
void discardCard(int currentPlayer)
{
	int n;
	printf("\n Press the number corresponding to the card you want to discard \n");
	scanf("%d", &n);

	switch (currentPlayer)
	{
	case 0:
		addCard(&discardPile, player1.cards[n - 1]);
		removeCard(&player1, n - 1);
		break;
	case 1:
		addCard(&discardPile, player2.cards[n - 1]);
		removeCard(&player2, n - 1);
		break;
	case 2:
		addCard(&discardPile, player3.cards[n - 1]);
		removeCard(&player3, n - 1);
		break;
	case 3:
		addCard(&discardPile, player4.cards[n - 1]);
		removeCard(&player4, n - 1);
		break;
	default:
		return;
	}
	//playerAction = 1  --> card 0
	;
}


//The drawFromDiscard method removes a card from the discard pile and adds it to the player's hand
void drawFromDiscard(int currentPlayer)
{
	switch (currentPlayer)
	{
	case 0:
		addCard(&player1, discardPile.cards[discardPile.currentSize - 1]);
		break;
	case 1:
		addCard(&player2, discardPile.cards[discardPile.currentSize - 1]);
		break;
	case 2:
		addCard(&player3, discardPile.cards[discardPile.currentSize - 1]);
		break;
	case 3:
		addCard(&player4, discardPile.cards[discardPile.currentSize - 1]);
		break;
	default:
		return;
	}

	//Removing the card that is drawn from the discard pile
	removeCard(&discardPile, discardPile.currentSize);
	showDiscardPile();
}


//The drawCard method removes a card from the deck and adds it to the players hand
void drawCard(int currentPlayer)
{
	switch (currentPlayer)
	{
	case 0:
		addCard(&player1, deck.cards[deck.currentSize - 1]);
		break;
	case 1:
		addCard(&player2, deck.cards[deck.currentSize - 1]);
		break;
	case 2:
		addCard(&player3, deck.cards[deck.currentSize - 1]);
		break;
	case 3:
		addCard(&player4, deck.cards[deck.currentSize - 1]);
		break;
	default:
		return;
	}

	//Removing the last card, the card that is drawn, from the deck
	removeCard(&deck, deck.currentSize - 1);
}

void makeMove(int currentPlayer)
{
	int n;
	int x;
	int pickedMeldCards;
	int cardIndex;
	int meldIndex;

	//allocating space for a maximum of 52 possible indices
	int* indices = malloc(sizeof(int) * 52);

	//The countIndices variable stores the current amount of indices present in the array
	int countIndices = 0;
	int gameOver;

	//a do while loop allows the player to play an action at least once,
	//and he can meld/layoff many times in one turn
	do
	{
		showTable();
		printf("\n Click 1 to Meld/Run \n");
		printf(" or 2 to LayOff or 3 to end your turn\n");
		scanf("%d", &n);
		if (n == 1)
		{
			do
			{
				printf("how many cards do you want to meld/run?\n");
				scanf("%d", &x);
				if (x < 3)
				{
					printf("you can only meld 3 or more cards\n");
				}
			} while (x < 3);
			int i;
			for (i = 0; i < x; i++)
			{
				printf("what is the index of the cards you want to meld/run\n");
				scanf("%d", &pickedMeldCards);
				//add this index to the vector of indices
				indices[countIndices++] = (pickedMeldCards);

			}

			//pass the indices of the cards and check if they can be melded by player "currentPlayer"
			meld(currentPlayer, indices, countIndices);

			//to avoid ruining the next iteration if the player melded invalid cards and then later melded valid ones
			countIndices = 0;
		}
		else if (n == 2)
		{
			printf("What is the index of the card you want to layoff?\n");
			scanf("%d", &cardIndex);

			printf("On which meld would you like to lay it off?\n");
			scanf("%d", &meldIndex);
			layOff(currentPlayer, cardIndex, meldIndex);
		}
		gameOver = checkForWinner();
		if (gameOver)
		{
			break;
		}

		//showPlayerHand(currentPlayer);
		//when the player inputs a 3 or more, his turn ends
	} while (n < 3);
}

void meld(int currentPlayer, int indices[], int countIndeces)
{
	struct CardPile* player;

	//determine which player is melding
	switch (currentPlayer)
	{
	case 0:
		player = &player1;

		break;
	case 1:
		player = &player2;
		break;
	case 2:
		player = &player3;
		break;
	case 3:
		player = &player4;
		break;
	default:
		return;
	}
	int i;


	//Initializing space for meld
	thisMeld = (struct CardPile*)malloc(sizeof(struct CardPile));
	thisMeld->currentSize = 0;


	//fetch the cards from the player's hand
	for (i = 0; i < countIndeces; i++)
	{
		addCard(thisMeld, player->cards[indices[i] - 1]);
	}

	//sort the indices so that deleting them from the vector is possile. We would start witht the last element and work backwards
	sortArr(indices, countIndeces);



	int tempCardRank;
	int tempCardSuit;
	int j;
	//sort the meld in order to check if it is in increasing order
	//using bubble sort    (because could not call sort on a vector of cards)
	for (i = 0; i < thisMeld->currentSize; i++)
	{
		// Last i elements are already in place
		for (j = 0; j < thisMeld->currentSize - i - 1; j++)
		{
			if (thisMeld->cards[j]->rank > thisMeld->cards[j + 1]->rank)
			{
				//swap the elements
				tempCardRank = thisMeld->cards[j]->rank;
				tempCardSuit = thisMeld->cards[j]->suit;

				thisMeld->cards[j]->rank = thisMeld->cards[j + 1]->rank;
				thisMeld->cards[j]->suit = thisMeld->cards[j + 1]->suit;
				thisMeld->cards[j + 1]->rank = (enum Rank)tempCardRank;
				thisMeld->cards[j + 1]->suit = (enum Suit)tempCardSuit;
			}
		}
	}

	//needed to determine if the player is dropping a meld or a run
	int isMeld;
	isMeld = 1;
	int isRun;
	isRun = 1;

	struct Card* c = thisMeld->cards[0];

	//loop over the meld vector and compare each two adjacent cards
	for (int i = 1; i < thisMeld->currentSize - 1; i++)
	{
		//checking for meld
		//if the adjacent cards have different ranks, then this is not a meld
		if (c->rank != thisMeld->cards[i]->rank)
		{
			isMeld = 0;
		}
		//checking for run
		//if the adjacent cards are not in increasing order with a defference of 1 in their ranks, then it is not a run
		//they also must have the same suit
		if (c->rank != (thisMeld->cards[i]->rank - 1) || c->suit != thisMeld->cards[i]->suit)
		{
			isRun = 0;
			printf("invalid Run!\n");

		}

		c = thisMeld->cards[i];
	}

	//valid move
	if (isMeld || isRun)
	{
		//take out the cards from the player's hand and place
		//them into the middle
		addCardPile(&table, thisMeld);

		//delete the elements from the appropriate player's hand starting from the last index
		//and going back to the first index
		//this avoids the problem when the vector replaces a deleted element with the next one.
		for (int i = countIndeces - 1; i >= 0; i--)
		{
			switch (currentPlayer)
			{
			case 0:
				removeCard(&player1, indices[i] - 1);
				break;
			case 1:
				removeCard(&player2, indices[i] - 1);
				break;
			case 2:
				removeCard(&player3, indices[i] - 1);
				break;
			case 3:
				removeCard(&player4, indices[i] - 1);
				break;
			default:
				return;
			}
		}
		printf("valid Run!\n");
		showPlayerHand(currentPlayer);
	}

	//empty the vectors so they can hold the next set of cards
	countIndeces = 0;
	thisMeld = NULL;
}

//takes the index of the card and the index of the meld on the table
void layOff(int currentPlayer, int cardIndex, int meldIndex)
{
	struct Card* c = NULL;
	printf("called layoff\n");
	switch (currentPlayer)
	{
	case 0:
		c = player1.cards[cardIndex - 1];
		break;
	case 1:
		c = player2.cards[cardIndex - 1];
		break;
	case 2:
		c = player3.cards[cardIndex - 1];
		break;
	case 3:
		c = player4.cards[cardIndex - 1];
		break;
	default:
		break;
	}
	int isMeld = 1;
	int isRun = 1;

	//table is a vector of vectors of cards
	//to access its elements we write table[0][0]
	//as it is a 2D array

	int validMove;

	//firstly, we need to check if that row in the table is a meld or a run
	if (meldIndex < table.currentSize) {
		struct Card* c2 = table.table[meldIndex - 1]->cards[0];
		for (int k = 1; k < table.table[meldIndex - 1]->currentSize; k++)
		{
			if (c2->rank != table.table[meldIndex - 1]->cards[k]->rank)
			{
				isMeld = 0;
			}

			if (c2->rank != ((table.table[meldIndex - 1]->cards[k]->rank) - 1) || c2->suit != (table.table[meldIndex - 1]->cards[k])->suit)
			{
				isRun = 0;
			}

			c2 = table.table[meldIndex - 1]->cards[k];
		}

		validMove = 1;
		// Card
		//2 cases: adding to a meld or adding to a run
		if (isMeld)
		{
			if (c != NULL && c->rank == table.table[meldIndex - 1]->cards[0]->rank)
			{
				//valid move. Insert to the rank
				switch (currentPlayer)
				{
				case 0:
					printf("%s\n", printCard(player1.cards[cardIndex - 1]));
					addCard(table.table[meldIndex - 1], player1.cards[cardIndex - 1]);

					removeCard(&player1, cardIndex - 1);
					break;
				case 1:
					printf("%s\n", printCard(player2.cards[cardIndex - 1]));
					addCard(table.table[meldIndex - 1], player2.cards[cardIndex - 1]);

					removeCard(&player2, cardIndex - 1);
					break;
				case 2:
					printf("%s\n", printCard(player3.cards[cardIndex - 1]));
					addCard(table.table[meldIndex - 1], player3.cards[cardIndex - 1]);

					removeCard(&player3, cardIndex - 1);
					break;
				case 3:
					printf("%s\n", printCard(player4.cards[cardIndex - 1]));
					addCard(table.table[meldIndex - 1], player4.cards[cardIndex - 1]);

					removeCard(&player4, cardIndex - 1);
				default:
					return;
				}
				showPlayerHand(currentPlayer);
			}
			else
			{
				validMove = 0;
			}
		}
		else if (isRun)
		{
			//for a valid move, a card could be smaller than the first element or larger
			//than the second element by 1
			//if smaller than the first element, we push the card to the end and then sort the meld
			int vectorSize = table.table[meldIndex - 1]->currentSize;

			//check if the card fits before the first element or after the last element of the meld
			if ((c->rank == (enum Rank)((table.table[meldIndex - 1]->cards[0])->rank - 1) &&
				c->suit == (table.table[meldIndex - 1]->cards[0])->suit) ||
				(c->rank == (enum Rank)((table.table[meldIndex - 1]->cards[vectorSize - 1])->rank + 1) &&
					c->suit == table.table[meldIndex - 1]->cards[vectorSize - 1]->suit))
			{

				//place the card in the meld vector and sort the vector
				switch (currentPlayer)
				{
				case 0:
					addCard(table.table[meldIndex - 1], player1.cards[cardIndex - 1]);
					removeCard(&player1, cardIndex - 1);
					break;
				case 1:
					addCard(table.table[meldIndex - 1], player2.cards[cardIndex - 1]);
					removeCard(&player2, cardIndex - 1);
					break;
				case 2:
					addCard(table.table[meldIndex - 1], player3.cards[cardIndex - 1]);
					removeCard(&player3, cardIndex - 1);
					break;
				case 3:
					addCard(table.table[meldIndex - 1], player4.cards[cardIndex - 1]);
					removeCard(&player4, cardIndex - 1);
					break;
				default:
					return;
				}

				enum Rank tempCardRank;
				enum Suit tempCardSuit;
				int i, j;
				//bubble sort the run on the table
				for (i = 0; i < table.table[meldIndex - 1]->currentSize; i++)
				{
					// Last i elements are already in place
					for (j = 0; j < table.table[meldIndex - 1]->currentSize - i - 1; j++)
					{
						//if the two adjacent cards are out of ourder
						if (table.table[meldIndex - 1]->cards[j]->rank > table.table[meldIndex - 1]->cards[j + 1]->rank)
						{
							//swap the elements
							tempCardRank = table.table[meldIndex - 1]->cards[j]->rank;
							tempCardSuit = table.table[meldIndex - 1]->cards[j]->suit;

							table.table[meldIndex - 1]->cards[j]->rank = table.table[meldIndex - 1]->cards[j + 1]->rank;
							table.table[meldIndex - 1]->cards[j]->suit = table.table[meldIndex - 1]->cards[j + 1]->suit;
							table.table[meldIndex - 1]->cards[j + 1]->rank = (enum Rank)tempCardRank;
							table.table[meldIndex - 1]->cards[j + 1]->suit = (enum Suit)tempCardSuit;
						}
					}
				}

				showPlayerHand(currentPlayer);
			}
			else
			{
				validMove = 0;
			}
		}
	}
	else {
		validMove = 0;
	}

	if (!validMove)
	{
		printf("\ninvalid layout move\n");
	}
}

void showTable()
{
	printf(" The table now contais: \n");
	//if the table is empty, print that out and exit the function
	if (table.currentSize == 0)
	{
		printf("NOTHING\n");
		return;
	}

	int i, j;

	//nested for loop
	//first for loop goes over the vectors inside the table
	for (i = 0; i < table.currentSize; i++)
	{
		struct CardPile* currPile = table.table[i];

		printf("%d. ", (i + 1));
		//second for loop iterates over the cards inside that vector
		for (j = 0; j < currPile->currentSize; j++)
		{
			printf("%s , ", printCard(currPile->cards[j]));
		}
		printf("\n-------------------------------------------------------\n");
	}
}

int checkForWinner()
{
	if (player1.currentSize == 0)
	{
		printf("=================\n");
		printf(" PLAYER 1 WINS!!\n");
		printf("=================\n");

		return 1;
	}

	if (player2.currentSize == 0)
	{
		printf("=================\n");
		printf(" PLAYER 2 WINS!!\n");
		printf("=================\n");

		return 1;
	}

	if (players >= 3)
	{

		if (player3.currentSize == 0)
		{
			printf("=================\n");
			printf(" PLAYER 3 WINS!!\n");
			printf("=================\n");
			return 1;
		}
	}

	if (players == 4)
	{
		if (player4.currentSize == 0)
		{
			printf("=================\n");
			printf(" PLAYER 4 WINS!!\n");
			printf("=================\n");
			return 1;
		}
	}
	return 0;
}
