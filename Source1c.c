#define _CRT_SECURE_NO_WARNINGS

/////// Developed by Alex Higuera Pierre Noel and Cameron Arthorn Brown ///////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

typedef struct card_s {
	char color[7];
	int value;
	char action[15];
	struct card_s* pt;
} card;

// Function definitions

void fillDeck(card* deck, FILE* inp);
void addCard(card** h, card** t, card* playerCard);
void discardCard(card** h, card** t, int i);
void dealing(card* h1, card* t1, card* h2, card* t2, card* deck);
void shuffling(card* deck, card* shuffled);
void printCard(card* h, int i);
void printSpecial(card* h);
void printDeck(card* h, int i, int* counter);
int specialCards(card* h, card* discard);
bool validOrNot(card** h, card** t, card* discard, int i, int* special);
bool illegalORnot(card* h, card* discard);
void reshufflingWhenEmpty(card* shuffled, card* deck, int i);

// Main code


int main(void) {
	card* deck;
	card* shuffled;
	card* headp1, * headp2, * tailp1, * tailp2;
	headp1 = NULL;
	headp2 = NULL;
	tailp1 = NULL;
	tailp2 = NULL;
	deck = (card*)malloc(108 * sizeof(card));
	shuffled = (card*)malloc(108 * sizeof(card));
	FILE* inp = fopen("myUnoDeck.txt", "r");
	char yORn = 'y', yORn2, fileName[30], newColor[10];
	int oneOrTwo, win = 0, cardSel, i = 0, special = 0;
	int counter1 = 0, counter2 = 0;

	while (yORn == 'y') {
		printf("Let's Play a Game of UNO\n");
		printf("Press 1 to shuffle the UNO deck or 2 to load a deck from a file: ");
		scanf("%d", &oneOrTwo);
		if (oneOrTwo == 1) {
			fillDeck(deck, inp);
			shuffling(deck, shuffled);
			deck[0] = shuffled[14];
			for (int j = 0; j < 14; j++) {  // This for loop performs the initial dealing
				if ((j % 2) == 0) {
					addCard(&headp1, &tailp1, &shuffled[j]);
				}
				else {
					addCard(&headp2, &tailp2, &shuffled[j]);
				}
			}

			printf("The deck is shuffled and the players get their cards. The game starts\n");
			if (strcmp(deck[0].action, "wild") == 0) {
				// This checks whether the game starts wiht special cards in this case it starts with a wild card.
				printf("Player one chooses the new color: ");
				scanf("%s", newColor);
				newColor[strlen(newColor)] = '\0';
				strcpy(deck[0].color, newColor);
				printf("%s is the starting color!\n\n", newColor);
			}
			else if (strcmp(deck[0].action, "drawFour") == 0) {
				//This is if the game starts +4. 
				printf("Game cannot start with Wild Draw Four card, deck will be reshuffled\n\n");
				deck[0] = shuffled[15];
			}
			else if (strcmp(deck[0].action, "drawTwo") == 0) {
				//This is what happens if the game starts with a +2.
				printf("Player one has to draw two cards!\n\n");
				for (int j = 0; j < 2; j++) {
					addCard(&headp1, &tailp1, &shuffled[14 + i]);
					++i;
				}
				if (strcmp(deck[0].action, "none") != 0) {
					if (strcmp(deck[0].action, "wild") == 0 || strcmp(deck[0].action, "drawFour") == 0) {
						//prints out the wild and drawFour into the discard Pile
						printf("Discard pile: %s\n\n", deck[0].action);
					}
					else {
						printf("Discard pile: %s %s\n\n", deck[0].color, deck[0].action);
					}
				}
				else {
					printf("Discard pile: %d %s\n\n", deck[0].value, deck[0].color);
				}
				printDeck(headp2, 2, &counter2);
				//prints that player two has uno when one card remains.
				if (counter2 == 1) {
					printf("Player two has UNO\n\n");
					printf("Press 1 to play the card from your hand, or press zero to draw a card from the deck: ");
					scanf("%d", &cardSel);
				}
				else {
					printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter2);
					scanf("%d", &cardSel);
					printf("\n");
				}
				if (cardSel == 0) {
					//This is when the player 2 asks for another card and they have to play 0 to ask for it.
					addCard(&headp2, &tailp2, &shuffled[14 + i]);
					++i;
				}
				if ((validOrNot(&headp2, &tailp2, deck, cardSel, &special) == false) && cardSel != 0) {
					//This uses the function valid or not to see whether a card can actually be placed on top of the other and if it does not, it will prompt the user.
					printCard(headp2, cardSel);
					printf("cannot be placed on top of %d %s\n\n", deck[0].value, deck[0].color);
				}
				else {
					//This is for special cards. If the value from our int Special cards corresponds to the value that we stated above, we give that number that its special action which is shown below.
					if (special == 1) {
						//This is for the reverse and skip cards. in this case, we have two players, so they do the same thing.
						printf("Player one has been skipped/reversed!\n\n");
						printDeck(headp2, 2, &counter2);
						if (counter2 == 1) {
							printf("Player two has UNO\n\n");
							printf("Press 1 to play the card from your hand, or press zero to draw a card from the deck: ");
							scanf("%d", &cardSel);
							printf("\n");
						}
						else {
							printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter2);
							scanf("%d", &cardSel);
						}
						if (cardSel == 0) {
							addCard(&headp2, &tailp2, &shuffled[14 + i]);
							++i;
						}
						if ((validOrNot(&headp2, &tailp2, deck, cardSel, &special) == false) && cardSel != 0) {
							printCard(headp2, cardSel);
							printf("cannot be placed on top of %d %s\n", deck[0].value, deck[0].color);
						}
					}
					else if (special == 2) {
						//This is for the draw two card. 
						printf("Player one has to draw two cards!\n\n");
						for (int j = 0; j < 2; j++) {
							addCard(&headp1, &tailp1, &shuffled[14 + i]);
							++i;
						}
						if (special == 1) {
							printf("Player one has to draw two cards!\n");
							printDeck(headp2, 2, &counter2);
							if (counter2 == 1) {
								printf("Player two has UNO\n\n");
								printf("Press 1 to play the card from your hand, or press zero to draw a card from the deck: ");
								scanf("%d", &cardSel);
								printf("\n");
							}
							else {
								printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter2);
								scanf("%d", &cardSel);
							}
							if (cardSel == 0) {
								addCard(&headp2, &tailp2, &shuffled[14 + i]);
								++i;
							}
							if ((validOrNot(&headp2, &tailp2, deck, cardSel, &special) == false) && cardSel != 0) {
								printCard(headp2, cardSel);
								printf("cannot be placed on top of %d %s\n", deck[0].value, deck[0].color);
							}
						}
						else if (special == 3) {
							//This is for wild cards.
							printf("Choose the new color: ");
							scanf("%s", newColor);
							newColor[strlen(newColor)] = '\0';
							strcpy(deck[0].color, newColor);
							printf("%s is the new color!\n\n", newColor);
						}
						else if (special == 4) {
							//This introduces the +4 card and the challenge rule between the players. 
							printf("Does player one want to challenge player two (y/n)?");
							scanf(" %c", &yORn2);
							printf("\n");
							if (yORn2 == 'y') {
								if (illegalORnot(headp2, deck) == true) {
									printf("Illegal Wild Draw Four card, player two draws four cards!\n\n");
									for (int j = 0; j < 4; j++) {
										addCard(&headp2, &tailp2, &shuffled[14 + i]);
										++i;
									}
								}
								else {
									printf("False accusation! Player one draws six cards!\n\n");
									for (int j = 0; j < 6; j++) {
										addCard(&headp1, &tailp1, &shuffled[14 + i]);
										++i;
									}
									printf("Choose the new color: ");
									scanf("%s", newColor);
									printf("\n");
									newColor[strlen(newColor)] = '\0';
									strcpy(deck[0].color, newColor);
									strcpy(deck[0].action, "drawFour");
									printf("%s is the new color!\n\n", newColor);
								}
							}
							else {
								printf("Player one has to draw four cards!\n\n");
								for (int j = 0; j < 4; j++) {
									addCard(&headp1, &tailp1, &shuffled[14 + i]);
									++i;
								}
								printf("Choose the new color: ");
								scanf("%s", newColor);
								printf("\n");
								newColor[strlen(newColor)] = '\0';
								strcpy(deck[0].color, newColor);
								strcpy(deck[0].action, "drawFour");
								printf("%s is the new color!\n\n", newColor);
							}
						}
					}
				}
			}
			else if (strcmp(deck[0].action, "skip") == 0 || strcmp(deck[0].action, "reverse") == 0) {
				printf("Initial skip/reverse card, player one skipped/reversed!\n\n");
				if (strcmp(deck[0].action, "none") != 0) {
					if (strcmp(deck[0].action, "wild") == 0 || strcmp(deck[0].action, "drawFour") == 0) {
						printf("Discard pile: %s\n\n", deck[0].action);
					}
					else {
						printf("Discard pile: %s %s\n\n", deck[0].color, deck[0].action);
					}
				}
				else {
					printf("Discard pile: %d %s\n\n", deck[0].value, deck[0].color);
				}
				printDeck(headp2, 2, &counter2);
				if (counter2 == 1) {
					printf("Player two has UNO\n\n");
					printf("Press 1 to play the card from your hand, or press zero to draw a card from the deck: ");
					scanf("%d", &cardSel);
					printf("\n");
				}
				else {
					printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter2);
					scanf("%d", &cardSel);
					printf("\n");
				}
				if (cardSel == 0) {
					addCard(&headp2, &tailp2, &shuffled[14 + i]);
					++i;
				}
				if ((validOrNot(&headp2, &tailp2, deck, cardSel, &special) == false) && cardSel != 0) {
					printCard(headp2, cardSel);
					printf("cannot be placed on top of %d %s\n\n", deck[0].value, deck[0].color);
				}
				else {
					if (special == 1) {
						printf("Player one has been skipped/reversed!\n\n");
						printDeck(headp2, 2, &counter2);
						if (counter2 == 1) {
							printf("Player two has UNO\n\n");
							printf("Press 1 to play the card from your hand, or press zero to draw a card from the deck: ");
							scanf("%d", &cardSel);
							printf("\n");
						}
						else {
							printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter2);
							scanf("%d", &cardSel);
							printf("\n");
						}
						if (cardSel == 0) {
							addCard(&headp2, &tailp2, &shuffled[14 + i]);
							++i;
						}
						if ((validOrNot(&headp2, &tailp2, deck, cardSel, &special) == false) && cardSel != 0) {
							printCard(headp2, cardSel);
							printf("cannot be placed on top of %d %s\n\n", deck[0].value, deck[0].color);
						}
					}
					else if (special == 2) {
						printf("Player one has to draw two cards!\n\n");
						for (int j = 0; j < 2; j++) {
							addCard(&headp1, &tailp1, &shuffled[14 + i]);
							++i;
						}
						printDeck(headp2, 2, &counter2);
						if (counter2 == 1) {
							printf("Player two has UNO\n\n");
							printf("Press 1 to play the card from your hand, or press zero to draw a card from the deck: ");
							scanf("%d", &cardSel);
							printf("\n");
						}
						else {
							printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter2);
							scanf("%d", &cardSel);
							printf("\n");
						}
						if (cardSel == 0) {
							addCard(&headp2, &tailp2, &shuffled[14 + i]);
							++i;
						}
						if ((validOrNot(&headp2, &tailp2, deck, cardSel, &special) == false) && cardSel != 0) {
							printCard(headp2, cardSel);
							printf("cannot be placed on top of %d %s\n\n", deck[0].value, deck[0].color);
						}
					}
					else if (special == 3) {
						printf("Choose the new color: ");
						scanf("%s", newColor);
						printf("\n");
						newColor[strlen(newColor)] = '\0';
						strcpy(deck[0].color, newColor);
						printf("%s is the new color!\n\n", newColor);
					}
					else if (special == 4) {
						printf("Does player one want to challenge player two (y/n)?");
						scanf(" %c", &yORn2);
						printf("\n");
						if (yORn2 == 'y') {
							if (illegalORnot(headp2, deck) == true) {
								printf("Illegal Wild Draw Four card, player two draws four cards!\n\n");
								for (int j = 0; j < 4; j++) {
									addCard(&headp2, &tailp2, &shuffled[14 + i]);
									++i;
								}
							}
							else {
								printf("False accusation! Player one draws six cards!\n\n");
								for (int j = 0; j < 6; j++) {
									addCard(&headp1, &tailp1, &shuffled[14 + i]);
									++i;
								}
								printf("Choose the new color: ");
								scanf("%s", newColor);
								printf("\n");
								newColor[strlen(newColor)] = '\0';
								strcpy(deck[0].color, newColor);
								strcpy(deck[0].action, "drawFour");
								printf("%s is the new color!\n\n", newColor);
							}
						}
						else {
							printf("Player one has to draw four cards!\n\n");
							for (int j = 0; j < 4; j++) {
								addCard(&headp1, &tailp1, &shuffled[14 + i]);
								++i;
							}
							printf("Choose the new color: ");
							scanf("%s", newColor);
							printf("\n");
							newColor[strlen(newColor)] = '\0';
							strcpy(deck[0].color, newColor);
							strcpy(deck[0].action, "drawFour");
							printf("%s is the new color!\n\n", newColor);
						}
					}
				}
			}
			while (win != 1) { //This is all the same for player 1. We just copied what we had for player 2 to player 1. The code is identical to player 2 and player 1, so everything written above is the how it should work for player 1.
				printf("i : %d\n\n", i);
				special = 0;
				printf("\n");
				if (strcmp(deck[0].action, "none") != 0) {
					if (strcmp(deck[0].action, "wild") == 0 || strcmp(deck[0].action, "drawFour") == 0) {
						printf("Discard pile: %s\n\n", deck[0].action);
					}
					else {
						printf("Discard pile: %s %s\n\n", deck[0].color, deck[0].action);
					}
				}
				else {
					printf("Discard pile: %d %s\n\n", deck[0].value, deck[0].color);
				}
				printDeck(headp1, 1, &counter1);
				if (counter1 == 1) {
					printf("Player one has UNO\n\n");
					printf("Press 1 to play any card from your hand, or press zero to draw a card from the deck: ");
					scanf("%d", &cardSel);
					printf("\n");
				}
				else {
					printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter1);
					scanf("%d", &cardSel);
					printf("\n");
				}
				if (cardSel == 0) {
					addCard(&headp1, &tailp1, &shuffled[14 + i]);
					++i;
					reshufflingWhenEmpty(shuffled, deck, i);
					if (strcmp(deck[0].action, "none") != 0) {
						if (strcmp(deck[0].action, "wild") == 0 || strcmp(deck[0].action, "drawFour") == 0) {
							printf("Discard pile: %s\n\n", deck[0].action);
						}
						else {
							printf("Discard pile: %s %s\n\n", deck[0].color, deck[0].action);
						}
					}
					else {
						printf("Discard pile: %d %s\n\n", deck[0].value, deck[0].color);
					}
					printDeck(headp1, 1, &counter1);
					printf("Press 1-%d to play any card from your hand, or press zero AGAIN to keep your card: ", counter1);
					scanf("%d", &cardSel);
					printf("\n");
				}
				if ((validOrNot(&headp1, &tailp1, deck, cardSel, &special) == false) && cardSel != 0) {
					printCard(headp1, cardSel);
					printf("cannot be placed on top of %d %s\n\n", deck[0].value, deck[0].color);
				}
				else {
					if (special == 1) {
						printf("Player two has been skipped/reversed!\n\n");
						printDeck(headp1, 1, &counter1);
						if (counter1 == 1) {
							printf("Player one has UNO\n\n");
							printf("Press 1 to play the card from your hand, or press zero to draw a card from the deck: ");
							scanf("%d", &cardSel);
							printf("\n");
						}
						else {
							printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter1);
							scanf("%d", &cardSel);
							printf("\n");
						}
						if (cardSel == 0) {
							addCard(&headp1, &tailp1, &shuffled[14 + i]);
							++i;
							reshufflingWhenEmpty(shuffled, deck, i);
							if (strcmp(deck[0].action, "none") != 0) {
								if (strcmp(deck[0].action, "wild") == 0 || strcmp(deck[0].action, "drawFour") == 0) {
									printf("Discard pile: %s\n\n", deck[0].action);
								}
								else {
									printf("Discard pile: %s %s\n\n", deck[0].color, deck[0].action);
								}
							}
							else {
								printf("Discard pile: %d %s\n\n", deck[0].value, deck[0].color);
							}
							printDeck(headp1, 1, &counter1);
							printf("Press 1-%d to play any card from your hand, or press zero AGAIN to keep your card: ", counter1);
							scanf("%d", &cardSel);
							printf("\n");
						}
						if ((validOrNot(&headp1, &tailp1, deck, cardSel, &special) == false) && cardSel != 0) {
							printCard(headp1, cardSel);
							printf("cannot be placed on top of %d %s\n\n", deck[0].value, deck[0].color);
						}
					}
					else if (special == 2) {
						printf("Player two has to draw two cards!\n\n");
						for (int j = 0; j < 2; j++) {
							addCard(&headp2, &tailp2, &shuffled[14 + i]);
							++i;
							reshufflingWhenEmpty(shuffled, deck, i);
						}
					}
					else if (special == 3) {
						printf("Choose the new color: ");
						scanf("%s", newColor);
						printf("\n");
						newColor[strlen(newColor)] = '\0';
						strcpy(deck[0].color, newColor);
						printf("%s is the new color!\n\n", newColor);
					}
					else if (special == 4) {
						printf("Does player two want to challenge player one (y/n)?");
						scanf(" %c", &yORn2);
						printf("\n");
						if (yORn2 == 'y') {
							if (illegalORnot(headp1, deck) == true) {
								printf("Illegal Wild Draw Four card, player one draws four cards!\n\n");
								for (int j = 0; j < 4; j++) {
									addCard(&headp1, &tailp1, &shuffled[14 + i]);
									++i;
									reshufflingWhenEmpty(shuffled, deck, i);
								}
							}
							else {
								printf("False accusation! Player two draws six cards!\n\n");
								for (int j = 0; j < 6; j++) {
									addCard(&headp2, &tailp2, &shuffled[14 + i]);
									++i;
									reshufflingWhenEmpty(shuffled, deck, i);
								}
								printf("Choose the new color: ");
								scanf("%s", newColor);
								printf("\n");
								newColor[strlen(newColor)] = '\0';
								strcpy(deck[0].color, newColor);
								strcpy(deck[0].action, "drawFour");
								printf("%s is the new color!\n\n", newColor);
							}
						}
						else {
							printf("Player two has to draw four cards!\n\n");
							for (int j = 0; j < 4; j++) {
								addCard(&headp2, &tailp2, &shuffled[14 + i]);
								++i;
								reshufflingWhenEmpty(shuffled, deck, i);
							}
							printf("Choose the new color: ");
							scanf("%s", newColor);
							printf("\n");
							newColor[strlen(newColor)] = '\0';
							strcpy(deck[0].color, newColor);
							strcpy(deck[0].action, "drawFour");
							printf("%s is the new color!\n\n", newColor);
						}
					}
				}
				if (headp1 == NULL) {
					//This happens when player one has no cards, so they win. 
					printf("Player one wins!\n");
					win = 1;
				}
				special = 0;
				if (strcmp(deck[0].action, "none") != 0) {
					if (strcmp(deck[0].action, "wild") == 0 || strcmp(deck[0].action, "drawFour") == 0) {
						printf("Discard pile: %s\n\n", deck[0].action);
					}
					else {
						printf("Discard pile: %s %s\n\n", deck[0].color, deck[0].action);
					}
				}
				else {
					printf("Discard pile: %d %s\n\n", deck[0].value, deck[0].color);
				}
				printDeck(headp2, 2, &counter2);
				if (counter2 == 1) {
					printf("Player two has UNO\n\n");
					printf("Press 1 to play the card from your hand, or press zero to draw a card from the deck: ");
					scanf("%d", &cardSel);
					printf("\n");
				}
				else {
					printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter2);
					scanf("%d", &cardSel);
					printf("\n");
				}
				if (cardSel == 0) {
					addCard(&headp2, &tailp2, &shuffled[14 + i]);
					++i;
					reshufflingWhenEmpty(shuffled, deck, i);
					if (strcmp(deck[0].action, "none") != 0) {
						if (strcmp(deck[0].action, "wild") == 0 || strcmp(deck[0].action, "drawFour") == 0) {
							printf("Discard pile: %s\n\n", deck[0].action);
						}
						else {
							printf("Discard pile: %s %s\n\n", deck[0].color, deck[0].action);
						}
					}
					else {
						printf("Discard pile: %d %s\n\n", deck[0].value, deck[0].color);
					}
					printDeck(headp2, 2, &counter2);
					printf("Press 1-%d to play any card from your hand, or press zero again to keep your card: ", counter2);
					scanf("%d", &cardSel);
					printf("\n");
				}
				if ((validOrNot(&headp2, &tailp2, deck, cardSel, &special) == false) && cardSel != 0) {
					printCard(headp2, cardSel);
					printf("cannot be placed on top of %d %s\n\n", deck[0].value, deck[0].color);
				}
				else {
					if (special == 1) {
						printf("Player one has been skipped/reversed!\n\n");
						printDeck(headp2, 2, &counter2);
						if (counter2 == 1) {
							printf("Player two has UNO\n\n");
							printf("Press 1 to play the card from your hand, or press zero to draw a card from the deck: ");
							scanf("%d", &cardSel);
							printf("\n");
						}
						else {
							printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter2);
							scanf("%d", &cardSel);
							printf("\n");
						}
						if (cardSel == 0) {
							addCard(&headp2, &tailp2, &shuffled[14 + i]);
							++i;
							reshufflingWhenEmpty(shuffled, deck, i);
							if (strcmp(deck[0].action, "none") != 0) {
								if (strcmp(deck[0].action, "wild") == 0 || strcmp(deck[0].action, "drawFour") == 0) {
									printf("Discard pile: %s\n\n", deck[0].action);
								}
								else {
									printf("Discard pile: %s %s\n\n", deck[0].color, deck[0].action);
								}
							}
							else {
								printf("Discard pile: %d %s\n\n", deck[0].value, deck[0].color);
							}
							printDeck(headp2, 2, &counter2);
							printf("Press 1-%d to play any card from your hand, or press zero again to keep your card: ", counter2);
							scanf("%d", &cardSel);
							printf("\n");
						}
						if ((validOrNot(&headp2, &tailp2, deck, cardSel, &special) == false) && cardSel != 0) {
							printCard(headp2, cardSel);
							printf("cannot be placed on top of %d %s\n\n", deck[0].value, deck[0].color);
						}
					}
					else if (special == 2) {
						printf("Player one has to draw two cards!\n\n");
						for (int j = 0; j < 2; j++) {
							addCard(&headp1, &tailp1, &shuffled[14 + i]);
							++i;
							reshufflingWhenEmpty(shuffled, deck, i);
						}
					}
					else if (special == 3) {
						printf("Choose the new color: ");
						scanf("%s", newColor);
						printf("\n");
						newColor[strlen(newColor)] = '\0';
						strcpy(deck[0].color, newColor);
						printf("%s is the new color!\n\n", newColor);
					}
					else if (special == 4) {
						printf("Does player one want to challenge player two (y/n)?");
						scanf(" %c", &yORn2);
						printf("\n");
						if (yORn2 == 'y') {
							if (illegalORnot(headp2, deck) == true) {
								printf("Illegal Wild Draw Four card, player two draws four cards!\n\n");
								for (int j = 0; j < 4; j++) {
									addCard(&headp2, &tailp2, &shuffled[14 + i]);
									++i;
									reshufflingWhenEmpty(shuffled, deck, i);
								}
							}
							else {
								printf("False accusation! Player one draws six cards!\n\n");
								for (int j = 0; j < 6; j++) {
									addCard(&headp1, &tailp1, &shuffled[14 + i]);
									++i;
									reshufflingWhenEmpty(shuffled, deck, i);
								}
								printf("Choose the new color: ");
								scanf("%s", newColor);
								printf("\n");
								newColor[strlen(newColor)] = '\0';
								strcpy(deck[0].color, newColor);
								strcpy(deck[0].action, "drawFour");
								printf("%s is the new color!\n\n", newColor);
							}
						}
						else {
							printf("Player one has to draw four cards!\n\n");
							for (int j = 0; j < 4; j++) {
								addCard(&headp1, &tailp1, &shuffled[14 + i]);
								++i;
								reshufflingWhenEmpty(shuffled, deck, i);
							}
							printf("Choose the new color: ");
							scanf("%s", newColor);
							printf("\n");
							newColor[strlen(newColor)] = '\0';
							strcpy(deck[0].color, newColor);
							strcpy(deck[0].action, "drawFour");
							printf("%s is the new color!\n\n", newColor);
						}
					}
				}
				if (headp2 == NULL) {
					//This is when the first card of player 2 is NULL that is when player 2 has no cards meaning that they have won.
					printf("Player two wins!\n");
					win = 1;
				}
			}

		}
		else {
			printf("Type the name of your deck file: ");
			scanf("%*c");
			fgets(fileName, 30, stdin);
			fileName[strlen(fileName) - 1] = '\0';
			inp = fopen(fileName, "r");
			if (inp == NULL) {
				printf("File not found. Try again.\n");
			}
			else {
				fillDeck(deck, inp);
				shuffling(deck, shuffled);
				deck[0] = shuffled[14];
				for (int j = 0; j < 14; j++) {
					if ((j % 2) == 0) {
						addCard(&headp1, &tailp1, &shuffled[j]);
					}
					else {
						addCard(&headp2, &tailp2, &shuffled[j]);
					}
				}

				printf("The deck is shuffled and the players get their cards. The game starts\n");
				if (strcmp(deck[0].action, "wild") == 0) {
					// This checks whether the game starts wiht special cards in this case it starts with a wild card.
					printf("Player one chooses the new color: ");
					scanf("%s", newColor);
					newColor[strlen(newColor)] = '\0';
					strcpy(deck[0].color, newColor);
					printf("%s is the starting color!\n\n", newColor);
				}
				else if (strcmp(deck[0].action, "drawFour") == 0) {
					//This is if the game starts +4. 
					printf("Game cannot start with Wild Draw Four card, deck will be reshuffled\n\n");
					deck[0] = shuffled[15];
				}
				else if (strcmp(deck[0].action, "drawTwo") == 0) {
					//This is what happens if the game starts with a +2.
					printf("Player one has to draw two cards!\n\n");
					for (int j = 0; j < 2; j++) {
						addCard(&headp1, &tailp1, &shuffled[14 + i]);
						++i;
					}
					if (strcmp(deck[0].action, "none") != 0) {
						if (strcmp(deck[0].action, "wild") == 0 || strcmp(deck[0].action, "drawFour") == 0) {
							//prints out the wild and drawFour into the discard Pile
							printf("Discard pile: %s\n\n", deck[0].action);
						}
						else {
							printf("Discard pile: %s %s\n\n", deck[0].color, deck[0].action);
						}
					}
					else {
						printf("Discard pile: %d %s\n\n", deck[0].value, deck[0].color);
					}
					printDeck(headp2, 2, &counter2);
					//prints that player two has uno when one card remains.
					if (counter2 == 1) {
						printf("Player two has UNO\n\n");
						printf("Press 1 to play the card from your hand, or press zero to draw a card from the deck: ");
						scanf("%d", &cardSel);
					}
					else {
						printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter2);
						scanf("%d", &cardSel);
						printf("\n");
					}
					if (cardSel == 0) {
						//This is when the player 2 asks for another card and they have to play 0 to ask for it.
						addCard(&headp2, &tailp2, &shuffled[14 + i]);
						++i;
					}
					if ((validOrNot(&headp2, &tailp2, deck, cardSel, &special) == false) && cardSel != 0) {
						//This uses the function valid or not to see whether a card can actually be placed on top of the other and if it does not, it will prompt the user.
						printCard(headp2, cardSel);
						printf("cannot be placed on top of %d %s\n\n", deck[0].value, deck[0].color);
					}
					else {
						//This is for special cards. If the value from our int Special cards corresponds to the value that we stated above, we give that number that its special action which is shown below.
						if (special == 1) {
							//This is for the reverse and skip cards. in this case, we have two players, so they do the same thing.
							printf("Player one has been skipped/reversed!\n\n");
							printDeck(headp2, 2, &counter2);
							if (counter2 == 1) {
								printf("Player two has UNO\n\n");
								printf("Press 1 to play the card from your hand, or press zero to draw a card from the deck: ");
								scanf("%d", &cardSel);
								printf("\n");
							}
							else {
								printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter2);
								scanf("%d", &cardSel);
							}
							if (cardSel == 0) {
								addCard(&headp2, &tailp2, &shuffled[14 + i]);
								++i;
							}
							if ((validOrNot(&headp2, &tailp2, deck, cardSel, &special) == false) && cardSel != 0) {
								printCard(headp2, cardSel);
								printf("cannot be placed on top of %d %s\n", deck[0].value, deck[0].color);
							}
						}
						else if (special == 2) {
							//This is for the draw two card. 
							printf("Player one has to draw two cards!\n\n");
							for (int j = 0; j < 2; j++) {
								addCard(&headp1, &tailp1, &shuffled[14 + i]);
								++i;
							}
							if (special == 1) {
								printf("Player one has to draw two cards!\n");
								printDeck(headp2, 2, &counter2);
								if (counter2 == 1) {
									printf("Player two has UNO\n\n");
									printf("Press 1 to play the card from your hand, or press zero to draw a card from the deck: ");
									scanf("%d", &cardSel);
									printf("\n");
								}
								else {
									printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter2);
									scanf("%d", &cardSel);
								}
								if (cardSel == 0) {
									addCard(&headp2, &tailp2, &shuffled[14 + i]);
									++i;
								}
								if ((validOrNot(&headp2, &tailp2, deck, cardSel, &special) == false) && cardSel != 0) {
									printCard(headp2, cardSel);
									printf("cannot be placed on top of %d %s\n", deck[0].value, deck[0].color);
								}
							}
							else if (special == 3) {
								//This is for wild cards.
								printf("Choose the new color: ");
								scanf("%s", newColor);
								newColor[strlen(newColor)] = '\0';
								strcpy(deck[0].color, newColor);
								printf("%s is the new color!\n\n", newColor);
							}
							else if (special == 4) {
								//This introduces the +4 card and the challenge rule between the players. 
								printf("Does player one want to challenge player two (y/n)?");
								scanf(" %c", &yORn2);
								printf("\n");
								if (yORn2 == 'y') {
									if (illegalORnot(headp2, deck) == true) {
										printf("Illegal Wild Draw Four card, player two draws four cards!\n\n");
										for (int j = 0; j < 4; j++) {
											addCard(&headp2, &tailp2, &shuffled[14 + i]);
											++i;
										}
									}
									else {
										printf("False accusation! Player one draws six cards!\n\n");
										for (int j = 0; j < 6; j++) {
											addCard(&headp1, &tailp1, &shuffled[14 + i]);
											++i;
										}
										printf("Choose the new color: ");
										scanf("%s", newColor);
										printf("\n");
										newColor[strlen(newColor)] = '\0';
										strcpy(deck[0].color, newColor);
										strcpy(deck[0].action, "drawFour");
										printf("%s is the new color!\n\n", newColor);
									}
								}
								else {
									printf("Player one has to draw four cards!\n\n");
									for (int j = 0; j < 4; j++) {
										addCard(&headp1, &tailp1, &shuffled[14 + i]);
										++i;
									}
									printf("Choose the new color: ");
									scanf("%s", newColor);
									printf("\n");
									newColor[strlen(newColor)] = '\0';
									strcpy(deck[0].color, newColor);
									strcpy(deck[0].action, "drawFour");
									printf("%s is the new color!\n\n", newColor);
								}
							}
						}
					}
				}
				else if (strcmp(deck[0].action, "skip") == 0 || strcmp(deck[0].action, "reverse") == 0) {
					printf("Initial skip/reverse card, player one skipped/reversed!\n\n");
					if (strcmp(deck[0].action, "none") != 0) {
						if (strcmp(deck[0].action, "wild") == 0 || strcmp(deck[0].action, "drawFour") == 0) {
							printf("Discard pile: %s\n\n", deck[0].action);
						}
						else {
							printf("Discard pile: %s %s\n\n", deck[0].color, deck[0].action);
						}
					}
					else {
						printf("Discard pile: %d %s\n\n", deck[0].value, deck[0].color);
					}
					printDeck(headp2, 2, &counter2);
					if (counter2 == 1) {
						printf("Player two has UNO\n\n");
						printf("Press 1 to play the card from your hand, or press zero to draw a card from the deck: ");
						scanf("%d", &cardSel);
						printf("\n");
					}
					else {
						printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter2);
						scanf("%d", &cardSel);
						printf("\n");
					}
					if (cardSel == 0) {
						addCard(&headp2, &tailp2, &shuffled[14 + i]);
						++i;
					}
					if ((validOrNot(&headp2, &tailp2, deck, cardSel, &special) == false) && cardSel != 0) {
						printCard(headp2, cardSel);
						printf("cannot be placed on top of %d %s\n\n", deck[0].value, deck[0].color);
					}
					else {
						if (special == 1) {
							printf("Player one has been skipped/reversed!\n\n");
							printDeck(headp2, 2, &counter2);
							if (counter2 == 1) {
								printf("Player two has UNO\n\n");
								printf("Press 1 to play the card from your hand, or press zero to draw a card from the deck: ");
								scanf("%d", &cardSel);
								printf("\n");
							}
							else {
								printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter2);
								scanf("%d", &cardSel);
								printf("\n");
							}
							if (cardSel == 0) {
								addCard(&headp2, &tailp2, &shuffled[14 + i]);
								++i;
							}
							if ((validOrNot(&headp2, &tailp2, deck, cardSel, &special) == false) && cardSel != 0) {
								printCard(headp2, cardSel);
								printf("cannot be placed on top of %d %s\n\n", deck[0].value, deck[0].color);
							}
						}
						else if (special == 2) {
							printf("Player one has to draw two cards!\n\n");
							for (int j = 0; j < 2; j++) {
								addCard(&headp1, &tailp1, &shuffled[14 + i]);
								++i;
							}
							printDeck(headp2, 2, &counter2);
							if (counter2 == 1) {
								printf("Player two has UNO\n\n");
								printf("Press 1 to play the card from your hand, or press zero to draw a card from the deck: ");
								scanf("%d", &cardSel);
								printf("\n");
							}
							else {
								printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter2);
								scanf("%d", &cardSel);
								printf("\n");
							}
							if (cardSel == 0) {
								addCard(&headp2, &tailp2, &shuffled[14 + i]);
								++i;
							}
							if ((validOrNot(&headp2, &tailp2, deck, cardSel, &special) == false) && cardSel != 0) {
								printCard(headp2, cardSel);
								printf("cannot be placed on top of %d %s\n\n", deck[0].value, deck[0].color);
							}
						}
						else if (special == 3) {
							printf("Choose the new color: ");
							scanf("%s", newColor);
							printf("\n");
							newColor[strlen(newColor)] = '\0';
							strcpy(deck[0].color, newColor);
							printf("%s is the new color!\n\n", newColor);
						}
						else if (special == 4) {
							printf("Does player one want to challenge player two (y/n)?");
							scanf(" %c", &yORn2);
							printf("\n");
							if (yORn2 == 'y') {
								if (illegalORnot(headp2, deck) == true) {
									printf("Illegal Wild Draw Four card, player two draws four cards!\n\n");
									for (int j = 0; j < 4; j++) {
										addCard(&headp2, &tailp2, &shuffled[14 + i]);
										++i;
									}
								}
								else {
									printf("False accusation! Player one draws six cards!\n\n");
									for (int j = 0; j < 6; j++) {
										addCard(&headp1, &tailp1, &shuffled[14 + i]);
										++i;
									}
									printf("Choose the new color: ");
									scanf("%s", newColor);
									printf("\n");
									newColor[strlen(newColor)] = '\0';
									strcpy(deck[0].color, newColor);
									strcpy(deck[0].action, "drawFour");
									printf("%s is the new color!\n\n", newColor);
								}
							}
							else {
								printf("Player one has to draw four cards!\n\n");
								for (int j = 0; j < 4; j++) {
									addCard(&headp1, &tailp1, &shuffled[14 + i]);
									++i;
								}
								printf("Choose the new color: ");
								scanf("%s", newColor);
								printf("\n");
								newColor[strlen(newColor)] = '\0';
								strcpy(deck[0].color, newColor);
								strcpy(deck[0].action, "drawFour");
								printf("%s is the new color!\n\n", newColor);
							}
						}
					}
				}
				while (win != 1) { //This is all the same for player 1. We just copied what we had for player 2 to player 1. The code is identical to player 2 and player 1, so everything written above is the how it should work for player 1.
					special = 0;
					printf("\n");
					if (strcmp(deck[0].action, "none") != 0) {
						if (strcmp(deck[0].action, "wild") == 0 || strcmp(deck[0].action, "drawFour") == 0) {
							printf("Discard pile: %s\n\n", deck[0].action);
						}
						else {
							printf("Discard pile: %s %s\n\n", deck[0].color, deck[0].action);
						}
					}
					else {
						printf("Discard pile: %d %s\n\n", deck[0].value, deck[0].color);
					}
					printDeck(headp1, 1, &counter1);
					if (counter1 == 1) {
						printf("Player one has UNO\n\n");
						printf("Press 1 to play any card from your hand, or press zero to draw a card from the deck: ");
						scanf("%d", &cardSel);
						printf("\n");
					}
					else {
						printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter1);
						scanf("%d", &cardSel);
						printf("\n");
					}
					if (cardSel == 0) {
						addCard(&headp1, &tailp1, &shuffled[14 + i]);
						++i;
						reshufflingWhenEmpty(shuffled, deck, i);
						if (strcmp(deck[0].action, "none") != 0) {
							if (strcmp(deck[0].action, "wild") == 0 || strcmp(deck[0].action, "drawFour") == 0) {
								printf("Discard pile: %s\n\n", deck[0].action);
							}
							else {
								printf("Discard pile: %s %s\n\n", deck[0].color, deck[0].action);
							}
						}
						else {
							printf("Discard pile: %d %s\n\n", deck[0].value, deck[0].color);
						}
						printDeck(headp1, 1, &counter1);
						printf("Press 1-%d to play any card from your hand, or press zero AGAIN to keep your card: ", counter1);
						scanf("%d", &cardSel);
						printf("\n");
					}
					if ((validOrNot(&headp1, &tailp1, deck, cardSel, &special) == false) && cardSel != 0) {
						printCard(headp1, cardSel);
						printf("cannot be placed on top of %d %s\n\n", deck[0].value, deck[0].color);
					}
					else {
						if (special == 1) {
							printf("Player two has been skipped/reversed!\n\n");
							printDeck(headp1, 1, &counter1);
							if (counter1 == 1) {
								printf("Player one has UNO\n\n");
								printf("Press 1 to play the card from your hand, or press zero to draw a card from the deck: ");
								scanf("%d", &cardSel);
								printf("\n");
							}
							else {
								printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter1);
								scanf("%d", &cardSel);
								printf("\n");
							}
							if (cardSel == 0) {
								addCard(&headp1, &tailp1, &shuffled[14 + i]);
								++i;
								reshufflingWhenEmpty(shuffled, deck, i);
								if (strcmp(deck[0].action, "none") != 0) {
									if (strcmp(deck[0].action, "wild") == 0 || strcmp(deck[0].action, "drawFour") == 0) {
										printf("Discard pile: %s\n\n", deck[0].action);
									}
									else {
										printf("Discard pile: %s %s\n\n", deck[0].color, deck[0].action);
									}
								}
								else {
									printf("Discard pile: %d %s\n\n", deck[0].value, deck[0].color);
								}
								printDeck(headp1, 1, &counter1);
								printf("Press 1-%d to play any card from your hand, or press zero AGAIN to keep your card: ", counter1);
								scanf("%d", &cardSel);
								printf("\n");
							}
							if ((validOrNot(&headp1, &tailp1, deck, cardSel, &special) == false) && cardSel != 0) {
								printCard(headp1, cardSel);
								printf("cannot be placed on top of %d %s\n\n", deck[0].value, deck[0].color);
							}
						}
						else if (special == 2) {
							printf("Player two has to draw two cards!\n\n");
							for (int j = 0; j < 2; j++) {
								addCard(&headp2, &tailp2, &shuffled[14 + i]);
								++i;
								reshufflingWhenEmpty(shuffled, deck, i);
							}
						}
						else if (special == 3) {
							printf("Choose the new color: ");
							scanf("%s", newColor);
							printf("\n");
							newColor[strlen(newColor)] = '\0';
							strcpy(deck[0].color, newColor);
							printf("%s is the new color!\n\n", newColor);
						}
						else if (special == 4) {
							printf("Does player two want to challenge player one (y/n)?");
							scanf(" %c", &yORn2);
							printf("\n");
							if (yORn2 == 'y') {
								if (illegalORnot(headp1, deck) == true) {
									printf("Illegal Wild Draw Four card, player one draws four cards!\n\n");
									for (int j = 0; j < 4; j++) {
										addCard(&headp1, &tailp1, &shuffled[14 + i]);
										++i;
										reshufflingWhenEmpty(shuffled, deck, i);
									}
								}
								else {
									printf("False accusation! Player two draws six cards!\n\n");
									for (int j = 0; j < 6; j++) {
										addCard(&headp2, &tailp2, &shuffled[14 + i]);
										++i;
										reshufflingWhenEmpty(shuffled, deck, i);
									}
									printf("Choose the new color: ");
									scanf("%s", newColor);
									printf("\n");
									newColor[strlen(newColor)] = '\0';
									strcpy(deck[0].color, newColor);
									strcpy(deck[0].action, "drawFour");
									printf("%s is the new color!\n\n", newColor);
								}
							}
							else {
								printf("Player two has to draw four cards!\n\n");
								for (int j = 0; j < 4; j++) {
									addCard(&headp2, &tailp2, &shuffled[14 + i]);
									++i;
									reshufflingWhenEmpty(shuffled, deck, i);
								}
								printf("Choose the new color: ");
								scanf("%s", newColor);
								printf("\n");
								newColor[strlen(newColor)] = '\0';
								strcpy(deck[0].color, newColor);
								strcpy(deck[0].action, "drawFour");
								printf("%s is the new color!\n\n", newColor);
							}
						}
					}
					if (headp1 == NULL) {
						//This happens when player one has no cards, so they win. 
						printf("Player one wins!\n");
						win = 1;
					}
					special = 0;
					if (strcmp(deck[0].action, "none") != 0) {
						if (strcmp(deck[0].action, "wild") == 0 || strcmp(deck[0].action, "drawFour") == 0) {
							printf("Discard pile: %s\n\n", deck[0].action);
						}
						else {
							printf("Discard pile: %s %s\n\n", deck[0].color, deck[0].action);
						}
					}
					else {
						printf("Discard pile: %d %s\n\n", deck[0].value, deck[0].color);
					}
					printDeck(headp2, 2, &counter2);
					if (counter2 == 1) {
						printf("Player two has UNO\n\n");
						printf("Press 1 to play the card from your hand, or press zero to draw a card from the deck: ");
						scanf("%d", &cardSel);
						printf("\n");
					}
					else {
						printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter2);
						scanf("%d", &cardSel);
						printf("\n");
					}
					if (cardSel == 0) {
						addCard(&headp2, &tailp2, &shuffled[14 + i]);
						++i;
						reshufflingWhenEmpty(shuffled, deck, i);
						if (strcmp(deck[0].action, "none") != 0) {
							if (strcmp(deck[0].action, "wild") == 0 || strcmp(deck[0].action, "drawFour") == 0) {
								printf("Discard pile: %s\n\n", deck[0].action);
							}
							else {
								printf("Discard pile: %s %s\n\n", deck[0].color, deck[0].action);
							}
						}
						else {
							printf("Discard pile: %d %s\n\n", deck[0].value, deck[0].color);
						}
						printDeck(headp2, 2, &counter2);
						printf("Press 1-%d to play any card from your hand, or press zero again to keep your card: ", counter2);
						scanf("%d", &cardSel);
						printf("\n");
					}
					if ((validOrNot(&headp2, &tailp2, deck, cardSel, &special) == false) && cardSel != 0) {
						printCard(headp2, cardSel);
						printf("cannot be placed on top of %d %s\n\n", deck[0].value, deck[0].color);
					}
					else {
						if (special == 1) {
							printf("Player one has been skipped/reversed!\n\n");
							printDeck(headp2, 2, &counter2);
							if (counter2 == 1) {
								printf("Player two has UNO\n\n");
								printf("Press 1 to play the card from your hand, or press zero to draw a card from the deck: ");
								scanf("%d", &cardSel);
								printf("\n");
							}
							else {
								printf("Press 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", counter2);
								scanf("%d", &cardSel);
								printf("\n");
							}
							if (cardSel == 0) {
								addCard(&headp2, &tailp2, &shuffled[14 + i]);
								++i;
								reshufflingWhenEmpty(shuffled, deck, i);
								if (strcmp(deck[0].action, "none") != 0) {
									if (strcmp(deck[0].action, "wild") == 0 || strcmp(deck[0].action, "drawFour") == 0) {
										printf("Discard pile: %s\n\n", deck[0].action);
									}
									else {
										printf("Discard pile: %s %s\n\n", deck[0].color, deck[0].action);
									}
								}
								else {
									printf("Discard pile: %d %s\n\n", deck[0].value, deck[0].color);
								}
								printDeck(headp2, 2, &counter2);
								printf("Press 1-%d to play any card from your hand, or press zero again to keep your card: ", counter2);
								scanf("%d", &cardSel);
								printf("\n");
							}
							if ((validOrNot(&headp2, &tailp2, deck, cardSel, &special) == false) && cardSel != 0) {
								printCard(headp2, cardSel);
								printf("cannot be placed on top of %d %s\n\n", deck[0].value, deck[0].color);
							}
						}
						else if (special == 2) {
							printf("Player one has to draw two cards!\n\n");
							for (int j = 0; j < 2; j++) {
								addCard(&headp1, &tailp1, &shuffled[14 + i]);
								++i;
								reshufflingWhenEmpty(shuffled, deck, i);
							}
						}
						else if (special == 3) {
							printf("Choose the new color: ");
							scanf("%s", newColor);
							printf("\n");
							newColor[strlen(newColor)] = '\0';
							strcpy(deck[0].color, newColor);
							printf("%s is the new color!\n\n", newColor);
						}
						else if (special == 4) {
							printf("Does player one want to challenge player two (y/n)?");
							scanf(" %c", &yORn2);
							printf("\n");
							if (yORn2 == 'y') {
								if (illegalORnot(headp2, deck) == true) {
									printf("Illegal Wild Draw Four card, player two draws four cards!\n\n");
									for (int j = 0; j < 4; j++) {
										addCard(&headp2, &tailp2, &shuffled[14 + i]);
										++i;
										reshufflingWhenEmpty(shuffled, deck, i);
									}
								}
								else {
									printf("False accusation! Player one draws six cards!\n\n");
									for (int j = 0; j < 6; j++) {
										addCard(&headp1, &tailp1, &shuffled[14 + i]);
										++i;
										reshufflingWhenEmpty(shuffled, deck, i);
									}
									printf("Choose the new color: ");
									scanf("%s", newColor);
									printf("\n");
									newColor[strlen(newColor)] = '\0';
									strcpy(deck[0].color, newColor);
									strcpy(deck[0].action, "drawFour");
									printf("%s is the new color!\n\n", newColor);
								}
							}
							else {
								printf("Player one has to draw four cards!\n\n");
								for (int j = 0; j < 4; j++) {
									addCard(&headp1, &tailp1, &shuffled[14 + i]);
									++i;
									reshufflingWhenEmpty(shuffled, deck, i);
								}
								printf("Choose the new color: ");
								scanf("%s", newColor);
								printf("\n");
								newColor[strlen(newColor)] = '\0';
								strcpy(deck[0].color, newColor);
								strcpy(deck[0].action, "drawFour");
								printf("%s is the new color!\n\n", newColor);
							}
						}
					}
					if (headp2 == NULL) {
						//This is when the first card of player 2 is NULL that is when player 2 has no cards meaning that they have won.
						printf("Player two wins!\n");
						win = 1;
					}
				}
			}
		}

		printf("Would you like to play again (y/n)? ");
		scanf(" %c", &yORn);
		printf("\n");
	}

	fclose(inp);
	free(deck);
	free(shuffled);
	printf("Bye bye\n");

	return 0;
}

// Functions

void fillDeck(card* deck, FILE* inp) {//this function scans the file for the deck.
	int i = 0;
	while (fscanf(inp, "%d%s%s", &deck[i].value, deck[i].color, deck[i].action) != EOF) {
		++i;
	}
}

void addCard(card** h, card** t, card* playerCard) { //This function adds cards to the players. 
	if (*h == NULL) {
		*h = playerCard;
		*t = playerCard;
	}
	else {
		(*t)->pt = playerCard;
		*t = playerCard;
	}
	playerCard->pt = NULL;
}

void discardCard(card** h, card** t, int i) { //This function deletes the cards of the players as they play the game.
	card* target = *h;
	card** temp = h;

	for (int j = 0; j < (i - 1); j++) {
		target = target->pt;
	}
	while ((*temp) != target) {
		temp = &(*temp)->pt;
	}
	if (target == *h)
		*h = target->pt;
	if (target == *t)
		*t = *temp;
	*temp = target->pt;
	//free(target); 
}

void dealing(card* h1, card* t1, card* h2, card* t2, card* deck) { // This functions does the initial dealing.
	for (int i = 0; i < 14; i++) {
		if ((i % 2) == 0) {
			addCard(&h1, &t1, &deck[i]);
		}
		else {
			addCard(&h2, &t2, &deck[i]);
		}
	}
}

void shuffling(card* deck, card* shuffled) { //This fucntion shuffles the deck.
	int index1, index2;
	srand(time(NULL));
	for (int i = 0; i < 10000; i++) {
		index1 = rand() % 108;
		index2 = rand() % 108;
		shuffled[index1] = deck[index2];
		shuffled[index2] = deck[index1];
	}
}

void printCard(card* h, int i) { //This function prints the specific card that cannot be played in case there is an attempt.
	card* target = h;
	for (int j = 1; j < i; j++) {
		target = target->pt;
	}
	printf("The %d %s ", target->value, target->color);
}

void printSpecial(card* h) { //This function prints reverses, skips, wilds, and plus4s.
	if ((strcmp(h->action, "wild") != 0) && (strcmp(h->action, "drawFour") != 0)) {
		printf("%s %s, ", h->color, h->action);
	}
	else {
		printf("%s, ", h->action);
	}
}

void printDeck(card* h, int i, int* counter) { //This function makes sure to print the deck of each player depending which cards are recieved.
	int c = 0;
	if (i == 1) {
		printf("Player one's hand: ");
		while (h != NULL) {
			if (strcmp(h->action, "none") != 0) {
				printSpecial(h);
				++c;
			}
			else {
				printf("%d %s, ", h->value, h->color);
				++c;
			}
			h = h->pt;
		}
		printf("\n");
	}
	else {
		printf("Player two's hand: ");
		while (h != NULL) {
			if (strcmp(h->action, "none") != 0) {
				printSpecial(h);
				++c;
			}
			else {
				printf("%d %s, ", h->value, h->color);
				++c;
			}
			h = h->pt;
		}
		printf("\n");
	}
	*counter = c;
}

int specialCards(card* h, card* discard) { //This assigns values for each special card so that the main knows what special card is being played.
	if (strcmp(h->action, "skip") == 0 && (strcmp(h->color, discard[0].color) == 0)) {
		return 1;
	}
	else if (strcmp(h->action, "reverse") == 0 && (strcmp(h->color, discard[0].color) == 0)) {
		return 1;
	}
	else if (strcmp(h->action, "drawTwo") == 0 && (strcmp(h->color, discard[0].color) == 0)) {
		return 2;
	}
	else if (strcmp(h->action, "wild") == 0) {
		return 3;
	}
	else if (strcmp(h->action, "drawFour") == 0) {
		return 4;
	}
	else {
		return 0;
	}
}

bool validOrNot(card** h, card** t, card* discard, int i, int* special) { //This bool checks whether the card is playable or not. 
	card* target = *h;
	for (int j = 1; j < i; j++) {
		target = target->pt;
	}
	if (i == 0) {
		return false;
	}
	if (strcmp(target->action, "none") != 0) {
		*special = specialCards(target, discard);
		if (*special == 0) {
			return false;
		}
		else {
			discard[0] = *target;
			discardCard(h, t, i);
			return true;
		}
	}
	if ((strcmp(target->color, discard[0].color) == 0) || target->value == discard[0].value) {
		discard[0] = *target;
		discardCard(h, t, i);
		return true;
	}
	else {
		return false;
	}
}

bool illegalORnot(card* h, card* discard) { //This bool checks whether the plus+4 rule is applied.
	while (h != NULL) {
		if ((strcmp(h->color, discard[0].color) == 0) || h->value == discard[0].value) {
			return true;
		}
		h = h->pt;
	}
	return false;
}

void reshufflingWhenEmpty(card* shuffled, card* deck, int i) { //This reshuffles the cards when there are no more cards to draw from.
	if ((14 + i) >= 108) {
		printf("Drawing deck is empty, reshuffling...\n");
		shuffling(deck, shuffled);
		printf("Done!\n");
	}
}