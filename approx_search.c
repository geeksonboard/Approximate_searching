/* Appriximate searching
 *
 * GeeoksOnBoard
 * 01.2014
 *
 * Program Call:
 * ./approx_search <file name> <pattern>
 */

#include<stdio.h>
#include<string.h>
#include<ctype.h>
#define MAXLINE 1000 /*maximal line lenght*/

int readword(char line[], char word[], int* i, int* wlen, int* space);

/* levenshtein
 *   PRE:
 *      Function compares 2 words according to levensteing algorythm (from wikipedia.org)
 *   POST:
 *      Returns "distance" between two words.
 */
int levenshtein(char *word1,char *word2);

/* min
 *   PRE:
 *      Compares 2 inteegers.
 *   POST:
 *      Returns smaller.
 */
int min(int x, int y);

/* highlight
 *   PRE:
 *      Prints to stdout one line of text of the 'len' lenght and highlight (in 
 *      the next line), chars with non-zero value in 'i' table.
 *   POST:
 *      Returns nothing.
 */
void highlight(char line[], int i[]);

/* main
 *   PRE:
 *      Loads the name of the file to search the pattern in.
 *   POST:
 *      Returns 0 if success and 1 if not.
 */
int main(int argc, char *argv[]) {

  /*variables*/
  char *nazwa;         /*name of loaded file        */
  FILE *plik;          /*pointer -//-               */
  char pattern[20];    /*pattern                    */
  int i=1;             /*incomming arguments counter*/

  char line[MAXLINE];  /*contains currently checked text line      */
  char word[20];       /*contains currently checked word           */
  int wlen;            /*word length                               */
  int nr=0;            /*number of checked line                    */
  int highlighty[MAXLINE] = {0};/*flags what to highlight in line  */
  _Bool czy_linia=0;   /*to print line or not to print             */
  int n;               /*moving in highlights table                */
  int space;           /*moving in new tale                        */

  /*open text file*/
  if(i<=argc) {
     nazwa=argv[i++];
  }
  else {
    printf("Error: No file specified!\n");
    return 1;
  }
  if((plik=fopen(nazwa, "r"))==NULL) {
    printf("Error: Failed to open file!\n");
    return 1;
  }

  /*loading pattern*/
  if(i<=argc) {
     strcpy(pattern, argv[i]);
  }
  else {
    printf("Error: no pattern specified!\n");
    return 1;
  }

  /*changing pattern to low capitals*/
  for(i=0; i<strlen(pattern); i++) {
    pattern[i]=tolower(pattern[i]);
  }

  /*main loop to go through the whole file*/

  /*line loading*/
  while(fgets(line, MAXLINE, plik)) {
    nr++;
    czy_linia=0;
    i=0;
    /*words loading*/
    while(readword(line, word, &i, &wlen, &space)) {
      /*checking similarity*/
      if(levenshtein(word, pattern)<2) {
        czy_linia=1;
        /*adding highlights to highlight tale*/
        for(n=i; i<space+n; i++) {
          highlighty[i]=0;
        }
        for(n=i; i<wlen+n; i++) {
          highlighty[i]=1;
        }
      }
      else {
        for(n=i; i<wlen+space+n; i++) {
          highlighty[i]=0;
        }
      }
    }
    /*printing and highlighting*/
    if(czy_linia==1) {
      printf("line no %d:\n", nr);
      highlight(line, highlighty);
    }
  }
  /*end of the main loop*/

  /*getting rid of trash*/
  fclose(plik);
  return 0;
}

/*readword*/
int readword(char line[], char word[], int* i, int* wlen, int* space ) {

   int a, b=0;
   char c=line[*i];
   (*space)=0;
   (*wlen)=0;
   /*resetting the word*/
   for(a=0; a<20; a++) {
     word[a]='\0';
   }
   a=0;
   
   /*condition to leave*/
   if(c=='\0') return 0;

   /*skipping no-letters*/
   while(!isalpha(c)) {
     c=line[++a+(*i)];
     (*space)++;
   }
   /*loading letters*/
   while(isalpha(c)) {
     word[b++]=c;
     (*wlen)++;
     c=line[++a+(*i)];
   }

   (*i)+=a;

   return 1;

}

/* levenshtein */
int levenshtein(char *word1, char *word2) {
  int i, j;
  int k = strlen(word1);
  int l = strlen(word2);
  int cost;
  int matrix[k + 1][l + 1];

  for(i = 0; i <= k; i++)
      matrix[i][0] = i;
  for(j = 1; j <= l; j++)
      matrix[0][j] = j;

  for(i = 1; i <= k; i++)
    for(j = 1; j <= l; j++) {
       if(word1[i-1] == word2[j-1])
          cost = 0;
       else
          cost = 1;
       matrix[i][j] = min(matrix[i-1][j] + 1, min(matrix[i][j-1] + 1, matrix[i-1][j-1] + cost));
    }
  return matrix[k][l];
}

/* min */
int min(int x, int y) {
  return x < y ? x : y;
}

/*highlight*/
void highlight(char line[], int i[]) {

  int a;
  int len=strlen(line);

  printf("%s", line);
  for(a=0; a<len; a++) {
    if(i[a]==1) printf("^");
    else printf(" ");
  }
  printf("\n");

}

/* COMMENTS:
   1. Program works correctly for line of lenght < 1000 characters and words under 20 chars.
   2. Program finds words which Levenstein distance is <=1 accoring to pattern.
   3. Program is not case sensitive.

*/
