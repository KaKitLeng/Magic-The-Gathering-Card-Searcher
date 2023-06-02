**Author**: Ka Kit Leng\
**Email**: jasonlengkakit@gmail.com\
**Assignment**: *Magic:The Gathering*ne Card Searcher\
**Language**: C

# Introduction
The purpose of this project is to practice my C knowledge by creating a card parser and a searcher.

# Run Instructions
Prerequisite: C
1. Run the make file

```bash
~/Magic-The-Gathering-Card-Searcher$ make
```

2. Run the parser to parse the cards.csv (contains card information for *Magic:The Gathering*)

```bash
~/Magic-The-Gathering-Card-Searcher$ ./parse cards.csv
```

3. Run search to search the specific card

```bash
~/Magic-The-Gathering-Card-Searcher$ ./search
```
Arrow prompt will show up, then type in the card you wanted to search

```bash
>> [card name]
```
When you're done searching, type *q or quit* to quit

# Example Output

```bash
~/Magic-The-Gathering-Card-Searcher$ ./search
>> Worthy Knight
Worthy Knight                                 {1}{W}
Creature - Human Knight                         rare
----------------------------------------------------
Whenever you cast a Knight spell, create a 1/1 white Human creature token.
----------------------------------------------------
                                                 2/2

>> testing card   
./search: 'testing card' not found!
>> Eternal Isolation
Eternal Isolation                             {1}{W}
Sorcery                                     uncommon
----------------------------------------------------
Put target creature with power 4 or greater on the bottom of its owner's library.
----------------------------------------------------
                                                   
>> q
```


