// Author: Ka Kit Leng
// Project: "Magic:The Gathering" card searcher
// Description: Header file for parser.c 

enum rarity
{
	common,
	uncommon,
	rare,
	mythic
};

struct card
{
	unsigned int id;
	char* name;
	char* cost;
	unsigned int converted_cost;
	char* type;
	char* text;
	char* stats;
	enum rarity rarity;
	off_t offset;
	
};