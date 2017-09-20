#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>

using namespace std;

set< int >& move( set< int > X, char c, map< pair< int, char >, set< int > >& Automat )
{
    set< int >* local = new set< int >;
    
    for( set< int >::iterator i = X.begin( ); i != X.end( ); ++i )
        if( Automat[ make_pair( ( *i ), c ) ].size( ) > 0 )
            for( set< int >::iterator j = Automat[ make_pair( ( *i ), c ) ].begin( ); j != Automat[ make_pair( ( *i ), c ) ].end( ); ++j )
                local->insert( ( *j ) );
    
    return *local;
}

int esteFinala( set< int >& Multime, map< int, int >& FinaleNFA )
{
    for( set< int >::iterator it = Multime.begin( ); it != Multime.end( ); ++it )
        if( FinaleNFA[ ( *it ) ] == 1 )
            return 1;
    
    return 0;
}

int main( int argc, char* argv[ ] )
{
    int i, temp, stareCurenta, stareFinala,
    nrStariNFA, nrFinaleNFA, nrSimbNFA,
    nrStariDFA = 0, nrFinaleDFA = 0, nrSimbDFA = 0;
    
    char _character;
    
    FILE* f = fopen( argv[ 1 ], "r" );
    
    map< pair< int, char >, set< int > > NAuto;
    map< pair< int, char >, int > DAuto;
    vector< set< int > > Q;
    map< int, int > FinaleNFA;
    set< char > SimbFinaleNFA;
    set< int > FinaleDFA;
    set< char > SimboluriDFA;
    map< set< int >, int > Marcat, Exista, Transform;
    
    fscanf( f, "%d %d %d", &nrStariNFA, &nrFinaleNFA, &nrSimbNFA );
    
    for( i = 0; i < nrFinaleNFA; i++ )
    {
        fscanf( f, "%d", &temp );
        
        FinaleNFA[ temp ] = 1;
    }
    
    while( fscanf( f, "%d %c %d", &stareCurenta, &_character, &stareFinala ) != EOF )
    {
        SimbFinaleNFA.insert( _character );
        
        NAuto[ make_pair( stareCurenta, _character ) ].insert( stareFinala );
    }
    
    set< int > local;
    local.insert( 0 );
    
    Q.push_back( local );
    
    Exista[ local ] = 1;
    Transform[ local ] = nrStariDFA++;
    
    if( esteFinala( local, FinaleNFA ) )
    {
        nrFinaleDFA++;
        
        FinaleDFA.insert( Transform[ local ] );
    }
    
    for( i = 0; i < Q.size( ); i++ )
    {
        if( Marcat.count( Q[ i ] ) == 0 )
        {
            Marcat[ Q[ i ] ] = 1;
            
            for( set< char >::iterator iteratorCaracter = SimbFinaleNFA.begin( ); iteratorCaracter != SimbFinaleNFA.end( ); ++iteratorCaracter )
            {
                set< int >& _q = move( Q[ i ], *iteratorCaracter, NAuto );
                
                if( Exista.count( _q ) == 0 && _q.size( ) > 0 )
                {
                    Exista[ _q ] = 1;
                    
                    Q.push_back( _q );
                    
                    Transform[ _q ] = nrStariDFA++;
                    
                    if( esteFinala( _q, FinaleNFA ) )
                    {
                        nrFinaleDFA++;
                        
                        FinaleDFA.insert( Transform[ _q ] );
                    }
                }
                
                if( _q.size( ) > 0 )
                {
                    DAuto[ make_pair( Transform[ Q[ i ] ], *iteratorCaracter ) ] = Transform[ _q ];
                    
                    SimboluriDFA.insert( *iteratorCaracter );
                }
            }
        }
    }
    
    fclose( f );
    
    nrSimbDFA = ( int )SimboluriDFA.size( );
    
    printf( "%d %d %d\n", nrStariDFA, nrFinaleDFA, nrSimbDFA );
    
    for( set< int >::iterator it = FinaleDFA.begin( ); it != FinaleDFA.end( ); ++it )
        printf( "%d ", ( *it ) );
    
    printf( "\n" );
    
    for( map< pair< int, char >, int >::iterator i = DAuto.begin( ); i != DAuto.end( ); ++i )
        printf( "%d %c %d\n", ( *i ).first.first, ( *i ).first.second, ( *i ).second );
    
    return 0;
}
