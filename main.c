#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct FlashCard {
    char* name;
    char* answer;

    struct FlashCard* next;
} FlashCard;

typedef struct Options {
    short reverse; //Value then name.
    FILE* input;
} Options;

Options options;
void read(FlashCard** cards) {
    FILE* input = options.input;
    FlashCard* card = malloc(sizeof(FlashCard));
    *cards = card;
    while (fscanf(input, "%m[^:]:%m[^\n]\n",
                &card->name,
                &card->answer) == 2) {
        if(feof(input)) break;
        card->next = malloc(sizeof(FlashCard));
        card = card->next;
    }
    card->next = *cards; //Circular list
}

void go(const FlashCard* cards) {
    const FlashCard* card = cards;
    do {
        printf("%s: ", options.reverse ? card->name : card->answer);
        getchar();
        printf("%s\n",   options.reverse ? card->answer : card->name);
        card = card->next;
    } while(card != cards);
}

void read_options(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "USAGE: %s [-r] FILE\n",
                argv[0]);
        exit(0);
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
           options.reverse = 1;
        } else {
            options.input = fopen(argv[i], "r");
            if (options.input == NULL) {
                fprintf(stderr, "Could not open file '%s'\n",
                        argv[i]);
                exit(0);
            }
        }
    }
}

int main(int argc, char** argv) {
    FlashCard* cards;
    read_options(argc, argv);
    read(&cards);
    go(cards);
    return 0;
}
