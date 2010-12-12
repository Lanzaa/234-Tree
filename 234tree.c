#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAXLEN 120
#define T 2

/* Our node structure */
typedef struct nodes {
   int keys[2 * T - 1];
   struct nodes *children[2 * T];
   int n;
   bool leaf;
} Node;

Node* insert(Node**, int);
Node* insertnonfull(Node*, int);
Node* newnode();
Node* search(Node*, int);
void freetree(Node*);
void printNode(Node*);
void splitChild(Node*, int);

int main(int argc, char *argv[]) {
   if (argc < 2) {
      printf("Usage: %s FILENAME\n", argv[0]);
      return 1;
   }
   FILE *fin = fopen(argv[1],"r");
   if (fin == NULL) {
      printf("Unable to open file.\n");
      return 2;
   }

   /* things seem to be ok, lets build the tree */
   char curline[MAXLEN];
   char command;
   int value;
   int count = 0;
   Node* r = newnode();
   Node** root = &r;

   while ( fgets(curline, MAXLEN, fin) ) {
      count++;
      if ( 2 != sscanf(curline, "%c %d", &command, &value)) {
         printf("Error parsing line %d\n",count);
         return 3;
      }
      //fprintf(stderr,"line: %d, %c, %d\n", count, command, value);
      if (command == 'I') {
         printNode(insert(root,value));
      } else if (command == 'S') {
         printNode(search(*root, value));
      } else {
         printf("Unrecognized command.\n");
      }
   }
   freetree(*root);
   fclose(fin);
   return 0;
}

void freetree(Node* x) {
   if (!x->leaf) {
      for (int i = 0; i < x->n; i++) {
         freetree(x->children[i]);
      }
   }
   free(x);
}

Node* newnode() {
   Node* x = malloc(sizeof(Node));
   x->n = 0;
   x->leaf = true;
   return x;
}

Node* insert(Node** root, int value) {
   Node* r = *root;
   if (r->n == 2*T-1) {
      // TODO implement
      Node* a = newnode();
      a->children[0] = r;
      a->leaf = false;
      *root = a;
      splitChild(a,0);
      return insertnonfull(a, value);
   } else {
      return insertnonfull(r,value);
   }
}

Node* insertnonfull(Node* x, int value) {
   //find where to place value
   int i = 0;
   while ( i < x->n && value > x->keys[i])
      i++;
   if (x->leaf) {
      int j;
      for (j = x->n; j > i; j--) {
         x->keys[j] = x->keys[j-1];
      }
      x->keys[j] = value;
      x->n++;
      return x;
   } else {
      if (x->children[i]->n == 2*T-1) {
         // node is full split it
         splitChild(x, i);
         if (value > x->keys[i]) {
            i++;
         }
      }
      return insertnonfull(x->children[i], value);
   }
}

Node* search(Node* x, int value) {
   if (x->leaf) {
      return x;
   }

   int i = 0;
   while ( i < x->n && value > x->keys[i]) {
      i++;
   }
   if (value == x->keys[i]) {
      return x;
   } else {
      return search(x->children[i], value);
   }
}

void splitChild(Node* x, int i) {
   //fprintf(stderr,"Entering splitChild.\n"); // DEBUG
   Node* a = x->children[i];
   Node* b = newnode();

   for (int j = x->n; j > i; j--) {
      x->children[j+1] = x->children[j];
      x->keys[j] = x->keys[j-1];
   }
   // bring up the new key to i
   x->keys[i] = a->keys[T-1];
   x->children[i+1] = b;
   x->n++;
   if (a->leaf) {
      b->leaf = true;
      //fprintf(stderr,"a->leaf\n");
      for (int j = 0; j < T - 1; j++) {
         b->keys[j] = a->keys[j + T];
         b->n++;
      }
      a->n = T-1;
   } else {
      b->leaf = false;
      //fprintf(stderr,"!a->leaf\n");
      for (int j = 0; j < T - 1; j++) {
         b->keys[j] = a->keys[j + T];
         b->children[j] = a->children[j + T];
         b->n++;
      }
      b->children[T-1] = a->children[2*T-1];
      a->n = T-1;
      //fprintf(stderr,"!a->leaf\n");
   }
   // TODO TODO TODO TODO
}

void printNode(Node* x) {
   if (x == NULL) {
      printf("Sorry no node there.\n");
      return;
   } else if (x->n < 1) {
      printf("There are no keys in this node.\n");
      return;
   }
   int i;
   for (i = 0; i < x->n - 1; i++) {
      printf("%d ",x->keys[i]);
   }
   printf("%d\n",x->keys[i]);
}
