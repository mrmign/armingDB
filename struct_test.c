#include <stdio.h>
 
struct foo {
    int bar;
};
 
void function( struct foo *instance )
{
    instance->bar = 5;
}
 
int main( void )
{
    struct foo instance;
    instance.bar = 1;
    printf("instance.bar = %d\n", instance.bar); /* 1 */
    function( &instance );
    printf("instance.bar = %d\n", instance.bar ); /* still 1 */
    return 0;
}