//
// Created by paul on 4/4/21.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "edl2mkvchapters.h"

const char * xmlHeader =
        "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"
        "<!DOCTYPE Chapters SYSTEM \"matroskachapters.dtd\">\n"
        "<Chapters>\n"
        "  <EditionEntry>\n";

const char * chapterEntry =
        "    <ChapterAtom>\n"
        "      <ChapterTimeStart>%02d:%02d:%2.3f</ChapterTimeStart>\n"
        "      <ChapterTimeEnd>%02d:%02d:%2.3f</ChapterTimeEnd>\n"
        "      <ChapterDisplay>\n"
        "        <ChapterString>%s</ChapterString>\n"
        "      </ChapterDisplay>\n"
        "    </ChapterAtom>\n";

const char * xmlTrailer =
        "  </EditionEntry>\n"
        "</Chapters>\n";

typedef struct {
    unsigned short  hours;
    unsigned short  minutes;
    float           seconds;
} tTimestamp;

void usage(void)
{
    fprintf( stderr, "usage: \n");
}

void setTimestamp(tTimestamp * timestamp, float seconds )
{
    timestamp->hours   = seconds / (60 * 60);
    seconds -= timestamp->hours * (60 * 60);

    timestamp->minutes = seconds / 60;
    seconds -= timestamp->minutes * 60;

    timestamp->seconds = seconds;
}

void processFile( const char * inputFilename )
{
    FILE * input  = NULL;
    FILE * output = NULL;

    input = fopen( inputFilename, "r" );
    if ( input == NULL )
    {
        fprintf( stderr, "Error: unable to open \"%s\" to read.\n", inputFilename );
        return;
    }
    char * outputFilename = strdup( inputFilename );
    char * p = strrchr( outputFilename, '.');
    if ( p == NULL || strcasecmp( p, ".edl" ) != 0 )
    {
        /* Didn't find '.edl' extension at the end. Make
           a little extra room to append an extension  */
        int len = strlen( outputFilename );
        outputFilename = realloc( outputFilename, len + 5 );
        p = outputFilename + len;
    }
    strncpy( p, ".xml", 5 );
    output = fopen( outputFilename, "w" );

    if ( input != NULL && output != NULL )
    {
        float           startSec, endSec, prevSec;
        tTimestamp      start, end;
        int             type;
        const char *    typeAsStr;

        fprintf( output, "%s", xmlHeader );

        prevSec = 0;

        while (!feof(input))
        {
            fscanf( input, "%f %f %d\n", &startSec, &endSec, &type );

            if ( startSec > prevSec)
            {
                setTimestamp( &start, prevSec );
                setTimestamp( &end, startSec );
                typeAsStr = "Show";

                fprintf( output, chapterEntry,
                         start.hours, start.minutes, start.seconds,
                         end.hours, end.minutes, end.seconds,
                         typeAsStr
                );
            }

            setTimestamp( &start, startSec );
            setTimestamp( &end, endSec );

            switch ( type )
            {
            case 0:
                typeAsStr = "Cut";
                break;

            case 1:
                typeAsStr = "Mute";
                break;

            case 2:
                typeAsStr = "Scene";
                break;

            case 3:
                typeAsStr = "Commercials";
                break;

            default:
                typeAsStr = "(unknown)";
                break;
            }
            fprintf( output, chapterEntry,
                     start.hours, start.minutes, start.seconds,
                     end.hours,   end.minutes,   end.seconds,
                     typeAsStr
                     );

            prevSec = endSec;
        }

        fprintf( output, "%s", xmlTrailer );

        fclose( input );
        fclose( output );
    }
}

int main( int argc, const char * argv[] )
{
    if ( argc < 2 )
    {
        fprintf( stderr, "Error: needs at least one file to process.\n" );
        usage();
    }
    else
    {
        for ( int i = 1; i < argc; i++ )
        {
            if ( argv[1][0] != '-' )
            {
                processFile( argv[i] );
            }
        }
    }

    return 0;
}