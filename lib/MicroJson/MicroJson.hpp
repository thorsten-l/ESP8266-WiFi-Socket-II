#ifndef __MICRO_JSON_HPP__
#define __MICRO_JSON_HPP__

#include <Arduino.h>
#include <FS.h>

class uJson
{
private:
  File file;
  bool firstEntry;
  char buffer[128];

  bool checkAttribute( const char* n1, const char* n2, char **p );
  bool findValueEnd( char *p1, char **p2 );
  void writeAttributeName( String entryName );
  int readLine();

public:
  uJson( File _file );

  void writeHeader();
  void writeEntry( String entryName, bool value );
  void writeEntry( String entryName, int value );
  void writeEntry( String entryName, time_t value );
  void writeEntry( String entryName, const char *value );
  void writeFooter();

  bool readHeader();
  bool readAttributeName( char *attributeName );
  bool readEntryBoolean( const char* n1, const char* n2, bool *value );
  bool readEntryInteger( const char* n1, const char* n2, int *value );
  bool readEntryLong( const char* n1, const char* n2, long int *value );
  bool readEntryChars( const char* n1, const char* n2, char *value );
  bool readFooter();
};

#endif
