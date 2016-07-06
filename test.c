#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "rbtree.h"

#define RED    1
#define BLACK  2

#define CNT 10
#define TEST_CNT 1000
#define MAX_NAME 20

typedef struct{
    rbnode _;   // must be first member of you defined node
    int id;
    char name[MAX_NAME+1];
} userinfo;

void print_user( rbnode* n );
int  user_cmp( rbnode* n1, rbnode* n2 );
int  value_cmp( void* val, rbnode* n );
void user_swap( rbnode* n1, rbnode* n2 );
void free_node( rbnode* n );
char* node_value( rbnode* n );

typedef struct{
    char* word;
    int len;
    char tail;
}word_ident;

int split_words( char* src, word_ident words[], int size, int* count );
char* strtrim(char* str);

int main( int argc, char* argv[] )
{
    userinfo* node;
    word_ident words[10];
    
    char tmp[100];
    rbtree tree;
    
    rbtree_init( &tree, user_cmp, value_cmp, user_swap );
    
    int testcase = 0;
    int val;
    int rc;
    int wcnt;
    int i;
    
    if( argc > 1 )
        testcase = atoi( argv[1] );
        
if( testcase == 0 )
{
    while( 1 )
    {
        printf( "# " );
        gets( tmp );
        
        strtrim( tmp );
        if( strlen( tmp ) == 0 )
            continue;

        if( strcmp( tmp, "quit" ) == 0 || strcmp( tmp, "q" ) == 0 )
        {
            rbtree_free( &tree, free_node );
            break;
        }
        
        if( strcmp( tmp, "print" ) == 0 || strcmp( tmp, "p" ) == 0 )
        {
            rbtree_print( &tree, print_user );
            continue;
        }
        
        if( strcmp( tmp, "dot" ) == 0 )
        {
            rbtree_dot( &tree, "rbtree.dot", node_value );
            continue;
        }
        
        split_words( tmp, words, 10, &wcnt );
        if( wcnt != 2 )
            continue;
            
        words[0].word[words[0].len] = 0;
        words[1].word[words[1].len] = 0;
        
        val = atoi( words[1].word );

        switch( words[0].word[0] )
        {
            case 'a':
            case 'A':
                node = malloc( sizeof(userinfo) );
                if( !node )
                {
                    printf( "malloc failed!\n" );
                    break;
                }
                node->id = val;
                snprintf( node->name, sizeof(node->name), "user%d", val );
                
                rc = rbtree_insert( &tree, (rbnode*)node );
                if( rc )
                {
                    printf( "insert into tree failed!\n" );
                    free( node );
                    continue;
                }
                rbtree_print( &tree, print_user );
                if( rbtree_verify( &tree ) )
                    printf( "verity rbtree failed!\n" );

                break;
            
            case 'f':
            case 'F':
                node = (userinfo*)rbtree_find( &tree, &val );
                if( node )
                    print_user( (rbnode*)node );
                else
                    printf( "not found!\n" );
                
                break;
                
                
            case 'd':
            case 'D':
                printf( "delete value %d\n", val );
                node = (userinfo*)rbtree_delete( &tree, (void*)&val );
                if( !node )
                {
                    printf( "node not exist!\n" );
                    continue;
                }
                free( node );
                rbtree_print( &tree, print_user );
                if( rbtree_verify( &tree ) )
                    printf( "verity rbtree failed!\n" );                
                break;
        }
    }

}


if( testcase == 1 )
{

    int values[TEST_CNT];
    int valcnt=0;
    
    for( i = 0; i < TEST_CNT; i++ )
    {
        val = rand() % ( TEST_CNT*50 );
        node = malloc( sizeof(userinfo) );
        node->id = val;
        snprintf( node->name, sizeof(node->name), "user%d", val );
        rc = rbtree_insert( &tree, (rbnode*)node );
        if( rc )
            printf( "value %d already exist!\n", val );
        else
            values[valcnt++] = val;
        
        if( rbtree_verify( &tree ) )
        {
            printf( "rbtree verify failed!\n" );
            return 1;
        }
    }
    
    for( i = 0; i < valcnt; i++ )
    {
        if( !rbtree_find( &tree, values+i ) )
            printf( "error ! value %d not found!\n", values[i] );
    }
    
    rbtree_print( &tree, print_user );


    rbtree_walk( &tree, 0, print_user );
    
    for( i = 0; i < valcnt; i++ )
    {
        if( !rbtree_delete( &tree, values+i ) )
        {
            printf( "delete value failed!\n" );
            return 1;
        }
        
        if( rbtree_verify( &tree ) )
        {
            printf( "rbtree verify failed!\n" );
            return 1;
        }
    }
    

    if( tree.root != NULL && tree.count != 0 )
    {
        printf( "rbtree final status not ok!\n" );
    }
    









}

    return 0;
    
    
    
}

int split_words( char* src, word_ident words[], int size, int* count )
{
    int i;
    int inword = 0;
    int start;
    int cnt = 0;
    int len;
    if( !src )
    {
        *count = 0;
        return 0;
    }
    
    len = strlen( src );
    for( i = 0; i < len; i++ )
    {

        if( isspace( src[i] ) )  //
        {
            if( inword ) // come out of word
            {
                if( cnt + 1 > size )
                    return -1;
                words[cnt].word = src + start;
                words[cnt].len  = i - start;
                words[cnt].tail = src[i];
                cnt++;
                inword = 0;
            }
        }
        else
        {
            if( !inword ) //come in to a word
            {
                inword =1;
                start = i;
            }
        }
        
    }
    
    if( inword )
    {
        if( cnt + 1 > size )
            return -1;
        words[cnt].word = src + start;
        words[cnt].len  = i - start;
        words[cnt].tail = src[i];
        cnt++;
    }
    
    *count = cnt;
    return 0;
}


char* strtrim(char* str)
{
    int idx;
    idx = strlen(str)-1;
    
    while( idx >= 0 && ( str[idx]== ' ' || str[idx]== '\r' || str[idx]== '\n' || str[idx]== '\t' ) )
    {
        str[idx]=0;
        idx --;
    }
    
    return str;
    
}

void print_user( rbnode* n )
{
    userinfo* u;
    u = (userinfo*)n;
    
    printf( "node: %8p, user id:%5d, name:%10s, color:%5s, parent:%10p, left:%10p, right:%10p\n", n, u->id, u->name, n->color == RED ? "red" : "black",  n->parent, n->left, n->right );
    
    
    
    
}

int  user_cmp( rbnode* n1, rbnode* n2 )
{
    userinfo *u1, *u2;
    u1 = (userinfo*) n1;
    u2 = (userinfo*) n2;
    
    return u1->id > u2->id ? 1: ( u1->id == u2->id ? 0 : -1 );
}

int  value_cmp( void* val, rbnode* n )
{
    int* ip;
    int v;
    userinfo* u = (userinfo*)n;
    
    ip = val;
    v = *ip;
    
    return v > u->id ? 1 : ( v == u->id ? 0 : -1 );
    
}

void user_swap( rbnode* n1, rbnode* n2 )
{
    int id;
    char tmp[MAX_NAME+1];
    
    userinfo *u1, *u2;
    u1 = (userinfo*) n1;
    u2 = (userinfo*) n2;
    
    id = u1->id;
    u1->id = u2->id;
    u2->id = id;
    
    memcpy( tmp, u1->name, sizeof(tmp) );
    memcpy( u1->name, u2->name, sizeof(tmp) );
    memcpy( u2->name, tmp, sizeof(tmp) );
    
}


void free_node( rbnode* n )
{
    free( n );
}

char* node_value( rbnode* n )
{
    userinfo* u;
    static char tmp[20];
    
    u = (userinfo*)n;
    snprintf( tmp, sizeof(tmp), "%d", u->id );
    return tmp;
}