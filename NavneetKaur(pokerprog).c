
//COMP 3761, Project
// Card shuffling, dealing and probability of hand types


#include <conio.h>   // needed for _getch() call at end of main()
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h> // needed for _getch() call at end of main()

#define SUITS 4
#define FACES 13
#define CARDS 52
#define HAND_SIZE 5
#define HANDS_IN_DECK 10
#define NUMBER_OF_HANDS_PLAYED 10000000  //Currently set to 10 million
#define NUMB_HANDS_STR "10 million" //Currently set to 10 million
#define TRUE 1
#define FALSE 0

//required functions
void deal(const unsigned int wDeck[], const char* wFace[], //display all cards in deck
    const char* wSuit[]);
void dealNextHand(unsigned int wDeck[], unsigned int hand[]); //deal out next hand from the deck
int isFourOfAKind(const unsigned int hand[]);  // return true if hand contains four of a kind and false otherwise

// prototypes of functions to be implemented
void swap(unsigned int* const, unsigned int* const); //swap the two cards pointed to by the 2 pointers
void shuffle(unsigned int wDeck[]); //shuffle deck
int isPair(const unsigned int hand[]);  // return true if hand contains a pair and false otherwise
int isTwoPair(const unsigned int hand[]);  // return true if hand contains a two pair and false otherwise
int isThreeOfAKind(const unsigned int hand[]);  // return true if hand contains three of a kind and false otherwise
int isStraight(const unsigned int hand[]);  // return true if hand is a straight and false otherwise
int isFlush(const unsigned int hand[]);  // return true if hand is a flush and false otherwise
int isFullHouse(const unsigned int hand[]);  // return true if hand is a full house and false otherwise


int main(void)
{
    // define and initialize deck array
    unsigned int deck[CARDS];

    // initialize deck with values
    // value / 13 caluclates suit #     { "Hearts", "Diamonds", "Clubs", "Spades" };
    // value % 13 calculates face card  {Ace, 2, 3, ... 10, Jack, Queen, King}
    for (size_t card = 0; card < CARDS; ++card)
    {
        deck[card] = card;
    }

    srand((unsigned int)time(NULL)); // seed random-number generator to generate random numbers.

    // initialize suit array                       
    const char* suit[SUITS] =
    { "Hearts", "Diamonds", "Clubs", "Spades" };

    // initialize face array                   
    const char* face[FACES] =
    { "Ace", "Deuce", "Three", "Four",
     "Five", "Six", "Seven", "Eight",
     "Nine", "Ten", "Jack", "Queen", "King" };

    
    shuffle(deck);  
    
    deal(deck, face, suit); 

    unsigned int hand[HAND_SIZE]; // will contain the cards in the hand.

    //Define and initialize variables used to count each type of hand
    unsigned int pairCount = 0;
    unsigned int twoPairCount = 0;
    unsigned int threeOfAKindCount = 0;
    unsigned int straightCount = 0;
    unsigned int flushCount = 0;
    unsigned int fullHouseCount = 0;
    unsigned int fourOfAKindCount = 0;
    unsigned int straightFlushCount = 0; 

    // Shuffle the deck for the first time
    // After this, we shuffle deck every time we do not have enough undealth cards 
    // for a complete hand which will be every 10 deals assuming five card hands
    shuffle(deck);

    // Deal out NUMBER_OF_HANDS_PLAYED hands
    for (size_t hands = 1; hands < NUMBER_OF_HANDS_PLAYED; ++hands)
    {
        dealNextHand(deck, hand); // Deal out next 5 cards from the deck into the array hand

        // Does hand have a pair?
        if (isPair(hand))
        {
            ++pairCount;  // Yes, increment pair count
        }

        // Does hand have two pair?
        if (isTwoPair(hand))
        {
            ++twoPairCount;
        }

        // Does hand have three of a kind?
        if (isThreeOfAKind(hand))
        {
            ++threeOfAKindCount;
        }

        // Does hand have a straight?
        if (isStraight(hand))
        {
            // Check if also a flush
            if (isFlush(hand))
            {
                ++straightFlushCount; //both straight and flush
            }
            else
            {
                ++straightCount;
            }
        }

        // Does hand have a flush?
        else if (isFlush(hand))
        {
            ++flushCount;
        }

        // Does hand have a full house?
        if (isFullHouse(hand))
        {
            ++fullHouseCount;
        }

        // Does hand have four of a kind?
        if (isFourOfAKind(hand))
        {
            ++fourOfAKindCount;
        }
    }

    printf("\nA pair occurred %d times in "NUMB_HANDS_STR" hands, which means the probability of a pair is %.2f % c", pairCount, pairCount * 100.0 / NUMBER_OF_HANDS_PLAYED, '%');
    printf("\nTwo pair occurred %d times in "NUMB_HANDS_STR" hands, which means the probability of two pair is %.2f %c", twoPairCount, twoPairCount * 100.0 / NUMBER_OF_HANDS_PLAYED, '%');
    printf("\nThree of a kind occurred %d times in "NUMB_HANDS_STR" hands, which means the probability of three of a kind is %.2f %c", threeOfAKindCount, threeOfAKindCount * 100.0 / NUMBER_OF_HANDS_PLAYED, '%');
    printf("\nA straight occurred %d times in "NUMB_HANDS_STR" hands, which means the probability of a straight is %.3f %c", straightCount, straightCount * 100.0 / NUMBER_OF_HANDS_PLAYED, '%');
    printf("\nA flush occurred %d times in "NUMB_HANDS_STR" hands, which means the probability of a flush is %.3f %c", flushCount, flushCount * 100.0 / NUMBER_OF_HANDS_PLAYED, '%');
    printf("\nA full house occurred %d times in "NUMB_HANDS_STR" hands, which means the probability of a flush is %.4f %c", fullHouseCount, fullHouseCount * 100.0 / NUMBER_OF_HANDS_PLAYED, '%');
    printf("\nFour of a kind occurred %d times in "NUMB_HANDS_STR" hands, which means the probability of four of a kind is %.4f %c", fourOfAKindCount, fourOfAKindCount * 100.0 / NUMBER_OF_HANDS_PLAYED, '%');
    printf("\nStraight flush occurred %d times in "NUMB_HANDS_STR" hands, which means the probability of a straight flush is %.5f %c", straightFlushCount, straightFlushCount * 100.0 / NUMBER_OF_HANDS_PLAYED, '%');
    puts("");

    _getch();
}

// Display all of the cards in the deck
void deal(const unsigned int wDeck[], const char* wFace[],
    const char* wSuit[])
{
    // deal each of the cards
    for (size_t card = 0; card < CARDS; ++card) {
        size_t suit = wDeck[card] / 13;
        size_t face = wDeck[card] % 13;
        printf("%5s of %-8s%c", wFace[face], wSuit[suit],
            card % 4 == 3 ? '\n' : '\t'); // 4-column format 
    }
}

// Deal out the next HAND_SIZE cards into the array hand[]
// If not enough undealt cards left in wDeck, shuffle wDeck and 
//    start dealing from beginning of the wDeck again.
void dealNextHand(unsigned int wDeck[], unsigned int hand[])
{
    static unsigned int currentCard = 0; //remembers which is the next card to be dealt.

    // Shuffle deck and start from beginning of deck if not enough cards left for whole hand
    if ((currentCard + HAND_SIZE) >= CARDS)
    {
        shuffle(wDeck);
        currentCard = 0;
    }

    // Deal out HAND_SIZE cards from wDeck into the hand array
    for (size_t card = 0; card < HAND_SIZE; ++card)
    {
        hand[card] = wDeck[currentCard++];
    }
}

// Returns true if hand contains four of kind and false otherwise
int isFourOfAKind(const unsigned int hand[])
{
    unsigned int faceCnt[FACES] = { 0 };

    // Calculate the frequency of each face card in the hand
    for (size_t card = 0; card < HAND_SIZE; ++card)
    {
        ++faceCnt[hand[card] % 13];
    }

    // Iterate through all of the frequency counts to find out if 
    //     any of them occurred 4 times in the hand
    for (size_t face = 0; face < FACES; ++face)
    {
        if (faceCnt[face] == 4)
        {
            return TRUE; // Yes!!! Return true.
        }
    }

    return FALSE; //Nope ;-(
}


// Swap the two unsigned ints pointed to by the pointers card1 and card2
void swap(unsigned int* const card1, unsigned int* const card2)
{    
    int temp;  //temp int for swapping cards
    temp = *card2;
    *card2 = *card1;
    *card1 = temp;
}

// Shuffle cards in deck
void shuffle(unsigned int wDeck[])
{
    size_t i;
    size_t j;
    int temp;

    for (i = 0; i < CARDS; ++i)    //is using swap function
    {
        j = rand() % CARDS;
        temp = wDeck[i];
        wDeck[i] = wDeck[j];
        wDeck[j] = temp;
    }
}


// Returns true if hand contains only a pair and false other wise
// Will return FALSE if the hand contains any of these hand types as well:
//  1) Hand has 2 pairs
//  2) Hand has 3 of a kind
//  2) Hand is a full house
//  3) Hand has 4 of a kind
int isPair(const unsigned int hand[])
{
    unsigned int faceCnt[FACES] = { 0 }; //Calculates the frequency of each face card in the hand

    for (size_t card = 0; card < HAND_SIZE; ++card) 
    {
        ++faceCnt[hand[card] % 13];
    }
    for (size_t face = 0; face < FACES; ++face) // iterate through all of the frequency count to count how many them occured in a pair
    {
        if (faceCnt[face] == 1)
        {
            return TRUE;  // yes.. prints out there is a pair
        }
    }
    return FALSE;

}

// Returns true if hand contains two pairs and false otherwise
// NOTE: Will return FALSE if the hand contains any of these hand types as well:
//  1) Hand has 3 of a kind
//  2) Hand is a full house
//  3) Hand has 4 of a kind
int isTwoPair(const unsigned int hand[])
{
    unsigned int faceCnt[FACES] = { 0 };

    for (size_t card = 0; card < HAND_SIZE; ++card) // Calculates the frequency of each face card
    {
        ++faceCnt[hand[card] % 13];
    }

    for (size_t face = 0; face < FACES; ++face) // finds out how many times pair of two occured
    {
        if (faceCnt[face] == 2)
        {
            return TRUE;   // returns occurences of pair of two's.
        }
    }
    return FALSE;
}

// Returns true if hand contains 3 of a kind and false otherwise
// NOTE: Will return FALSE if the hand contains any of these hand types as well:
//  1) Hand has a full house
//  2) Hand has 4 of a kind
int isThreeOfAKind(const unsigned int hand[])
{ 
    unsigned int faceCnt[FACES] = { 0 };
    for (size_t card = 0; card < HAND_SIZE; ++card)  // to count the frequency of each face card
    {
        ++faceCnt[hand[card] % 13];
    }
     
    for (size_t face = 0; face < FACES; ++face) //to count number of occurences of threeofkind cards
    {
        if (faceCnt[face] == 3)
        {
            return TRUE;
        }
    }

    return FALSE;
}

// Returns true if hand is a straight and false otherwise

// The Ace can be used to create a straight in 2 different ways:
//  Ace, 2, 3, 4, 5 and 
//  10, Jack, Queen, King, Ace
int isStraight(const unsigned int hand[])
{
    //As, Ace makes a sequence i.e 2,3,4,5, we need to use sorting method, i am using bubble sort to sort column location
    int st[5] = { 0 };  
    int temp;
    int r, pass, comp;

    for (r = 0; r < 5; ++r)
    {
        st[r] = hand[r];
    }
    for (pass = 1; pass < 5; ++pass) // applying bubble sort algorithm which includes use of temp
    {
        for (comp = 0; comp < 4; ++comp)
        {
            if (st[comp] > st[comp + 1])
            {
                temp = st[comp];
                st[comp] = st[comp + 1];
                st[comp + 1] = temp;
            }
        }
    }
    // Now, we need to check if the sorted columns are a straight
    if (st[4] - 1 == st[3] && st[3] - 1 == st[2] && st[2] - 1 == st[1] && st[1] == st[0]) //aplied AND condition to check straight
    {
        return TRUE;  // it is a straight
    }
    return FALSE;
}

// Returns true if hand is a flush and false otherwise
int isFlush(const unsigned int hand[])
{
    unsigned int faceCnt[FACES] = { 0 };
    for (size_t card = 0; card < HAND_SIZE; ++card)  //records frequecy of each face card
    {
        ++faceCnt[hand[card] % 4];
    }

    for (size_t face = 0; face < FACES; ++face)  
    {
        if (faceCnt[face] == 5)
        {
            return TRUE;
        }
    }

    return FALSE;
}

// return true if hand is a full house and false otherwise
int isFullHouse(const unsigned int hand[])
{
    unsigned int faceCnt[FACES] = { 0 };
    for (size_t card = 0; card < HAND_SIZE; ++card)
    {
        ++faceCnt[hand[card] % 13];
    }
    for (size_t face = 0; face < FACES; ++face)
    {
        if (faceCnt[face] == 3 && faceCnt[face] == 2) // there is a full house if there are ThreeofaKind and ONE pairoftwo
            return TRUE;
    }

    return FALSE;
}

