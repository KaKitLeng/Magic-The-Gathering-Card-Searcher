// Author: Ka Kit Leng
// Project "Magic:The Gathering" card searcher
// Description: Search for specific cards

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include "card.h"

int comparator(const void*, const void*);

int main(int argc, char *argv[]) {
    errno = 0;
    char user_input[50] = "";
    while(strcmp(user_input,"q") != 0 ){
        struct card **cards = malloc(10000 * sizeof(struct card*));
        FILE *index_infile = fopen("index.bin", "rb");
        if (errno == ENOENT){
        	  fprintf(stderr, "%s\n", "./search: cannot open(\"index.bin\"): No such file or directory");	
        	  return 1;
        }
        
        int buff = 0;
        int i = 0;
    
        while (fread(&buff, sizeof(int), 1, index_infile)>0){
            cards[i] = malloc(sizeof(char)* 1000);

            cards[i]->name = calloc(1000, sizeof(char));
            fread(cards[i]->name, sizeof(char), buff, index_infile);
            //store the offset
            fread(&cards[i]->offset, sizeof(off_t), 1, index_infile);
            i++;
            //set buff to 0 before contining the loop
            buff = 0;

        }
        
        //print >>
        printf(">> ");
        //get user input and display user input
        fgets(user_input, 100, stdin);
        if (isatty(0) == 0){
            printf("%s", user_input);
        }
        
        //set the end of userinput as null pointer so that it will be a valid input
        user_input[strlen(user_input) - 1] = '\0';
        //if user input is "q", then call free and break
        if (strcmp(user_input,"q") == 0 ){
            for(int y = 0; y < i; y++){
                free(cards[y]->name);
                free(cards[y]);
            }
            free(cards);
            fclose(index_infile);
            break;
        }
        
        //use bsearch() to binary search the name (if found return true)
        bool card_name_found = false;
        int name_index = 0;
        for (int x=0; x<i; x++){
            if(strcmp(user_input, cards[x]->name) == 0){
                //card name found
                name_index = x;
                card_name_found = true;
            }
        }
        //card name not found
        if (!card_name_found){
            fprintf(stdout, "./search: '%s' not found!\n", user_input);
        }
        //card name found
        else {
            //cards.bin does not exist
            FILE *cards_infile = fopen("cards.bin", "rb");
            if (errno == ENOENT){
                fprintf(stderr, "%s\n", "./search: cannot open(\"cards.bin\"): No such file or directory");	
		    return 1;
            }
        
        //cards.bin file exist
        fseek(cards_infile, cards[name_index]->offset, SEEK_SET);
        
        //read id
        fread(&cards[name_index]->id, sizeof(int), 1, cards_infile);
        
        //read cost
        int costLength = 0;
        fread(&costLength, sizeof(int), 1, cards_infile);
        cards[name_index]->cost = calloc(50, sizeof(char));
        fread(cards[name_index]->cost, sizeof(char), costLength, cards_infile);
        
        //read converted_cost
        fread(&cards[name_index]->converted_cost, sizeof(int), 1, cards_infile);

        //read type
        int typeLength = 0;
        fread(&typeLength, sizeof(int), 1, cards_infile);
        cards[name_index]->type = calloc(100, sizeof(char));
        fread(cards[name_index]->type, sizeof(char), typeLength, cards_infile);
        
        //read text
        int textLength = 0;
        fread(&textLength, sizeof(int), 1, cards_infile);
        cards[name_index]->text = calloc(10000, sizeof(char));
        fread(cards[name_index]->text, sizeof(char), textLength, cards_infile);
        
        //read stats
        int statsLength = 0;
        fread(&statsLength, sizeof(int), 1, cards_infile);
        cards[name_index]->stats = calloc(100, sizeof(char));
        fread(cards[name_index]->stats, sizeof(char), statsLength, cards_infile);
        
        //read rarity
        fread(&cards[name_index]->rarity, sizeof(int), 1, cards_infile);
        
        //pretty print
        int padding_length = 52;
    	char *padding = "-------------------------------------------------------------------------------------------------------------------------------------------";
		int rowOne = 51 - strlen(cards[name_index]->name);
		int rowTwo = 51 - strlen(cards[name_index]->type);
		printf("%s %*s\n",cards[name_index]->name, rowOne, cards[name_index]->cost);
		int rarity = cards[name_index]->rarity;
		if (rarity == 0) {
			printf("%s %*s\n",cards[name_index]->type, rowTwo, "common");
		}
        else if (rarity == 1){
			printf("%s %*s\n",cards[name_index]->type, rowTwo, "uncommon");
        }
		else if (rarity == 2){
			printf("%s %*s\n",cards[name_index]->type, rowTwo, "rare");
		}
        else if (rarity == 3){
			printf("%s %*s\n",cards[name_index]->type, rowTwo, "mythic");
		}
		printf("%*.*s\n", padding_length, padding_length, padding);
		char *temptext = cards[name_index]->text;
		for (int z = 0; z < strlen(cards[name_index]->text); z++){
			if (temptext[z] == '\\'){
				if (temptext[z+1] == 'n'){
					printf("\n");
					z++;
				}
			} 
			else{
				printf("%c", temptext[z]);
			}
		}
		printf("\n%*.*s\n", padding_length, padding_length, padding);
		printf("%52s\n\n", cards[name_index]->stats);
		
		fclose(cards_infile);
		
		free(cards[name_index]->stats);
        free(cards[name_index]->text);
        free(cards[name_index]->type);
        free(cards[name_index]->cost);
        }
        
        fclose(index_infile);
        for (int y = i-1; y >= 0; y--){
            free(cards[y]->name);
            free(cards[y]);
        }
        free(cards);
    }
}

int comparator(const void *a, const void *b){
    struct card *a_st = *((struct card **) a);
    struct card *b_st = *((struct card **) b);
    return strcmp(a_st->name, b_st->name);
}