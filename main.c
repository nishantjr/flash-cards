#include <time.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct FlashCard {
    char* name;
    char* answer;

    struct FlashCard* next;
    struct FlashCard* prev;
} FlashCard;

typedef struct Options {
    short reverse; //Value then name.
    short random;
    FILE* input;
} Options;

void add_card(FlashCard** set, char* name, char* answer) {
    FlashCard* new_card = calloc(1, sizeof(FlashCard));
    if (*set == NULL) {
        new_card->next = new_card;
        new_card->prev = new_card;
    } else {
        new_card->next = (*set)->next;
        new_card->next->prev = new_card;

        (*set)->next = new_card;
        (*set)->next->prev = (*set);
    }
    *set = new_card;
    new_card->name = name;
    new_card->answer = answer;
}

FlashCard* remove_card(FlashCard** set, FlashCard* card) {
    if (card == NULL) return NULL;
    if (card->next == card) {
        *set = NULL;
    }
    card->prev->next = card->next;
    card->next->prev = card->prev;
    return card;
}

Options options;
void read(FlashCard** set) {
    FILE* input = options.input;
    *set = NULL;
    char *name, *answer;
    while (fscanf(input, " %m[^:] : %m[^\n]",
                &name,
                &answer) == 2) {
        if (feof(input)) break;
        add_card(set, name, answer);
    }
}

FlashCard* get_next(FlashCard* card) {
    if (card->next == card) return NULL;
    if (options.random == 1) {
        int forward = rand() % 1001;
        while (forward--)
            card = card->next;
    }
    return card->next;
}

void go(FlashCard** set) {
    FlashCard* card = *set;
    while((card = remove_card(set, get_next(card))) != NULL) {
        printf("%s: ", options.reverse ? card->name : card->answer);
        getchar();
        printf("\t%s\n",   options.reverse ? card->answer : card->name);
        free(card);
    }
}

#define CHECK_OPT(longopt, shortopt, num_params)  \
        ((strcmp(argv[i],     "-" shortopt) == 0  \
          || strcmp(argv[i], "--" longopt)  == 0) \
         && i < argc - num_params)

static void read_options(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "USAGE: %s [-r] FILE\n",
                argv[0]);
        exit(0);
    }

    for (int i = 1; i < argc; i++) {
        if        (CHECK_OPT("reverse", "r", 0)) {
            options.reverse = 1;
        } else if (CHECK_OPT("random",  "x", 0)) {
            srand(time(NULL));
            options.random = 1;
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
    go(&cards);
    return 0;
}
