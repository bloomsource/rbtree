#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <set>
#include "rbtree.h"
/*      a performance compare example with std::set<int> */

using namespace std;

#define CNT  1000000

typedef struct{
    rbnode _;
    int val;
}int_node;


int int_node_cmp( rbnode* node1, rbnode* node2 );

int int_value_cmp( void* value, rbnode* node );

void int_node_swap( rbnode* node1, rbnode* node2 );

int main( int argc, char* argv[] )
{
    rbtree int_tree;
    int val;
    int* values;
    int i;
    int cnt=0;
    int time_spend;
    int_node* node;
    struct timeval t1, t2;
    set<int> intset;
    set<int>::iterator pos;
        
    srandom( time( NULL ) );
    rbtree_init( &int_tree, int_node_cmp, int_value_cmp, int_node_swap );
    
    values = (int*)malloc( sizeof(int) * CNT );
    
    if( !values )
    {
        printf( "malloc memory failed!\n" );
        return 1;
    }
    
    
    for( i = 0; i < CNT; i++ )
    {
        val = random();
        if( !rbtree_find( &int_tree, &val ) )
        {
            node = (int_node*)malloc( sizeof(int_node) );
            if( !node )
                continue;
            
            node->val = val;
            
            rbtree_insert( &int_tree, (rbnode*)node );
            intset.insert( val );
            
            values[cnt] = val;
            cnt++;
        }
    }
    
    gettimeofday( &t1, NULL );
    for( i = 0; i < cnt; i++ )
    {
        val = values[i];
        if( !rbtree_find( &int_tree, &val ) )
        {
            printf( "error ! value %d not found!", val );
            return 1;
        }
    }
    
    gettimeofday( &t2, NULL );
    time_spend = ( t2.tv_sec - t1.tv_sec ) * 1000000 + (t2.tv_usec - t1.tv_usec);
    printf( "find in rbtree: total values %d, time spend: %d microseconds!\n", cnt, time_spend );


    gettimeofday( &t1, NULL );
    for( i = 0; i < cnt; i++ )
    {
        val = values[i];
        pos = intset.find( val );
        
        if( pos == intset.end() )
        {
            printf( "error ! value %d not found!", val );
            return 1;
            
        }       
    }
    
    gettimeofday( &t2, NULL );
    time_spend = ( t2.tv_sec - t1.tv_sec ) * 1000000 + (t2.tv_usec - t1.tv_usec);
    printf( "find in std::set values %d, time spend: %d microseconds!\n", cnt, time_spend );
    
    return 0;
    
}

int int_node_cmp( rbnode* node1, rbnode* node2 )
{
    int_node *n1, *n2;
    
    n1 = (int_node*)node1;
    n2 = (int_node*)node2;
    
    return n1->val > n2->val ? 1 : ( n1->val == n2->val ? 0 : -1 );
    
}

int int_value_cmp( void* value, rbnode* node )
{
    int val;
    int_node* n;
    
    val = *(int*)value;
    n = (int_node*)node;
    
    return val > n->val ? 1 : ( val == n->val ? 0 : -1 );
    
}

void int_node_swap( rbnode* node1, rbnode* node2 )
{
    int val;
    int_node *n1, *n2;
    
    n1 = (int_node*)node1;
    n2 = (int_node*)node2;
    
    val = n1->val;
    n1->val = n2->val;
    n2->val = val;
    
}
