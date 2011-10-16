#include <stdio.h>
#include <stdlib.h>

typedef struct FlashCard{
    char* name;
    char* answer;

    struct FlashCard* next;
} FlashCard;

void read(FlashCard** cards, FILE* input) {
    FlashCard* card = malloc(sizeof(FlashCard));
    *cards = card;
    while (fscanf(input, "%m[^:]:%m[^\n]\n",
                &card->name,
                &card->answer) == 2) {
        if(feof(input)) break;
        card->next = malloc(sizeof(FlashCard));
        card = card->next;
    }
    card->next = NULL;
}

void go(FlashCard* cards) {
    FlashCard* card = cards;
    while(card != NULL) {
        printf("%s: \n", card->name);
        getchar();
        printf("%s\n", card->answer);
        card = card->next;
    }
}

int main(int argc, char** argv) {
    if (argc != 2) { 
        fprintf(stderr, "USAGE: %s FILE\n", 
                argv[0]);
        exit(0);
    }
    FlashCard* cards; FILE* file = fopen(argv[1], "r");
    
    if (file == NULL) {
        fprintf(stderr, "Could not open file '%s'\n", 
                argv[1]);
        exit(0);
    }
    
    read(&cards, file);
    go(cards);
    return 0;
}
