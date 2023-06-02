// Author: Ka Kit Leng
// Porject: "Magic:The Gathering" card searcher
// Description: Parse csv file containing cards information

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#include "card.h"

int comparator(const void*, const void*);

int main(int argc, char *argv[]) {
	errno = 0;
	char *file = argv[1];

    if (argc == 1){
        fprintf(stderr, "%s\n", "./parser: an input path is required!");
        return 1;
    }
    
	FILE *infile = fopen(file, "r");
	if (errno == ENOENT){
		fprintf(stderr, "%s\"%s\"%s\n", "./parser: cannot open(", file,"): No such file or directory");	
		return 1;
	}
    char str[30000];
    //char cards[10000];
	struct card **cards = malloc(sizeof(struct card*) * 15000);
	int arraylen = 1000;
	int i = 0;
    
    fgets(str, 3000, infile);
	while (fgets(str, 30000, infile)) {
	    while (strstr(str, "\\n") != NULL){
	        char *z = strstr(str, "\\n");
	        if (z != NULL){
	            memcpy(z, "\n", 1);
	            memmove(z+1, z+2, strlen(z+3));
	        }
	    }
		if (i == arraylen){
			arraylen += 300;
			cards = realloc(cards, sizeof(struct card*) * arraylen);
		}
		
		char *line = str;
		
		//get id
	    char *token = strsep(&line, ",");
		int tempid = atoi(token);
		
		//get name
		token = strsep(&line, ",");
		
		//if the last char is not '"' then strcat
		if (token[strlen(token)-1] != '"'){	
			char temp[10000];
			char character = ',';
			strcpy(temp, token);
			strncat(temp, &character, 1);
			token = strsep(&line, ",");
			token = strcat(temp,token);
		}
		token++;
		token[strlen(token)-1] = '\0';
		char *tempname = token;
		
		//compare name to see of it is the same, if same then see which id is the largest
		bool dupfound = false;
		//use bool to check larger Id
		bool largerId = false;
		int replacecardindex = 0;
		
		for (int p = 0; p < i; p++){
			if ((strcmp(tempname, cards[p]->name) == 0)){
				dupfound = true;
				if (tempid > cards[p]->id || tempid == cards[p]->id){
					replacecardindex = p;
					largerId = true;
				}else{
					largerId = false;
				}
			}
		}
			
		if (!dupfound || (dupfound && largerId)){
			cards[i] = malloc(30000 * sizeof(char));
			cards[i]->name = malloc(15000 * sizeof(char));
			cards[i]->cost = malloc(5000 * sizeof(char));
			cards[i]->type = malloc(5000 * sizeof(char));
			cards[i]->text = malloc(15000 * sizeof(char));
			cards[i]->stats = malloc(1000 * sizeof(char));
		}
		if (dupfound && largerId){
			for (int x = replacecardindex; x< i-1; x++){
			    cards[x] -> id = cards[x+1] -> id;
			    memmove(cards[x]->name,cards[x+1]->name, strlen(cards[x+1]->name)+1);
				memmove(cards[x]->cost,cards[x+1]->cost, strlen(cards[x+1]->cost)+1);
				cards[x]->converted_cost = cards[x+1]->converted_cost;
				memmove(cards[x]->type,cards[x+1]->type, strlen(cards[x+1]->type)+1);
				memmove(cards[x]->text,cards[x+1]->text, strlen(cards[x+1]->text)+1);
				memmove(cards[x]->stats,cards[x+1]->stats, strlen(cards[x+1]->stats)+1);
				cards[x]->rarity = cards[x+1]->rarity;
			}
		    free(cards[i]->name);
			free(cards[i]->cost);
			free(cards[i]->type);
			free(cards[i]->text);
			free(cards[i]->stats);
			free(cards[i]);
			i--;
		}
		//start parsing after eliminating dups
		if (!dupfound || (dupfound && largerId)){
		//get name and id
		    strcpy(cards[i]->name, tempname);
			cards[i]->id = tempid;
		    
		//get cost
		    token = strsep(&line, ",");
    		if (strlen(token) != 0){
    			token++;
    			token[strlen(token)-1] = '\0';
    		}
    		
    		strcpy(cards[i]->cost, token);
    	//get converted cost
    	    token = strsep(&line, ",");
		    cards[i]->converted_cost = atoi(token);
		//get type
		    token = strsep(&line, ",");
    		token++;
    		token[strlen(token)-1] = '\0';
    		strcpy(cards[i]->type, token);
    	//get text
    	    //set bool false for emptytextfield
    	    //check if the text string is complete
    	    bool emptytextfield = false;
    	    bool completeString = false; 
    		char temp2[20000];
    		strcpy(temp2,"");
    		char *temptoken = "";
    
            //if text field is empty set emptytextfield true and put empty string
    		token = strsep(&line, "\"");	
    		if (token[0] == ','){	
    			strcpy(cards[i]->text, "");
    			emptytextfield = true;
    		}
    		//if text field not empty and text not completed
    		else{
    		while (completeString == false){
    			emptytextfield = false;
    			char *templine = strdup(line);
    			char *templineCopy = templine;
    
    			temptoken = strsep(&templineCopy, "\"");
    			temptoken = strsep(&templineCopy, "\"");
    			
    			if (strlen(temptoken) == 0){ 
    				temptoken = strsep(&templineCopy, "\"");
                    //if triple qoute, end of text field and include the double quote
    				if (strlen(temptoken) == 0){ 
    					token = strsep(&line, "\"");
    					char temp3[20000];
    					char ch = '"';
    					strcpy(temp3, token);
    					strncat(temp3, &ch, 1);
    					strcat(temp2,temp3);
    					strcpy(cards[i]->text, temp2);
    					completeString = true;
    				}
    				//if double quote continue text field
    				else{ 
    					token = strsep(&line, "\"");
    					char temp4[20000];
    					char ch = '"';
    					strcpy(temp4, token);
    					strncat(temp4, &ch, 1);
    					strcat(temp2, temp4);
    
    					strsep(&line, "\"");
    				}
    			}
    			//if single quote, end of line also
    			else{ 
    				token = strsep(&line, "\"");
    				strcat(temp2, token);
    				strcpy(cards[i]->text, temp2);
    
    				completeString = true;
    			}
    			free(templine);
    		}
    		}
    	//get Stats
    	    if (emptytextfield){
    			token = strsep(&line, ",");
    			token[strlen(token)-1] = '\0';
    		}
    		else{
    			token = strsep(&line, ",");
    			token = strsep(&line, ",");
    			if (strlen(token) != 0){
    				token++;
    				token[strlen(token)-1] = '\0';
    			}
    		}
    
    		strcpy(cards[i]->stats, token);
    	//get rarity
    	    token = strsep(&line, "\"");
    		token = strsep(&line, "\"");
    
    		if (token[0] == 'c'){
    			cards[i]->rarity = common;
    		}else if (token[0] == 'u'){
    			cards[i]->rarity = uncommon;
    		}else if (token[0] == 'r'){
    			cards[i]->rarity = rare;
    		}else if (token[0] == 'm'){
    			cards[i]->rarity = mythic;
    		}
    
    		i++;
    	}
    	}
    
    //store every cards data in cards.bin WITHOUT SORTING!!
    FILE *outfile = fopen("cards.bin", "wb");
    if (outfile == NULL){
        return 1;
    }
    
    for (int y = 0; y<i; y++){
        //offset in which the record begins
        cards[y]->offset = ftell(outfile);
        
        //write id field
        fwrite(&cards[y]->id, sizeof(int), 1, outfile);
        
        //write cost field
    	int costLength = strlen(cards[y]->cost);
    	fwrite(&costLength, sizeof(int), 1, outfile);
    	fwrite(cards[y]->cost, sizeof(char), strlen(cards[y]->cost), outfile);
    
    	//write converted_cost field
    	fwrite(&cards[y]->converted_cost, sizeof(int), 1, outfile);
    
    	//write type filed
    	int typeLength = strlen(cards[y]->type);
    	fwrite(&typeLength, sizeof(int), 1, outfile);
    	fwrite(cards[y]->type, sizeof(char), strlen(cards[y]->type), outfile);
    
    	//write text feild
    	int textLength = strlen(cards[y]->text);
    	fwrite(&textLength, sizeof(int), 1, outfile);
    	fwrite(cards[y]->text, sizeof(char), strlen(cards[y]->text), outfile);
    
    	//write stats field
    	int statsLength = strlen(cards[y]->stats);
    	fwrite(&statsLength, sizeof(int), 1, outfile);
    	fwrite(cards[y]->stats, sizeof(char), strlen(cards[y]->stats), outfile);
    
    	//write rarity field
    	fwrite(&cards[y]->rarity, sizeof(int), 1, outfile);
        }
        
    //sort array first befor putting into index.bin
    qsort(cards, i, sizeof(struct card *), comparator);
    
    //store name of cards into index.bin for search later
    FILE *outfile2 = fopen("index.bin", "wb");
    if (outfile2 == NULL){
        return 1;
    }
    for (int a = 0; a<i; a++){
        //write name field
        int name_len = strlen(cards[a]->name);
        fwrite(&name_len, sizeof(int), 1, outfile2);
        fwrite(cards[a]->name, sizeof(char), strlen(cards[a]->name), outfile2);
        
        //write offset
        fwrite(&cards[a]->offset, sizeof(off_t), 1, outfile2);
    }
    
    fclose(outfile2);
    fclose(outfile);
    
	for (int z = i-1; z >= 0; z--){
		free(cards[z]->name);
		free(cards[z]->cost);
		free(cards[z]->type);
		free(cards[z]->text);
		free(cards[z]->stats);
		free(cards[z]);
	}
		
	free(cards);
	fclose(infile);
}

int comparator(const void *a, const void *b){
    struct card *a_st = *((struct card **) a);
    struct card *b_st = *((struct card **) b);
    return strcmp(a_st->name, b_st->name);
}