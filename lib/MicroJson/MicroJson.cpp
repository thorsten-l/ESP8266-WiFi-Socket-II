#include <App.hpp>
#include <Arduino.h>
#include <FS.h>

#include "MicroJson.hpp"

uJson::uJson(File _file) {
  file = _file;
  firstEntry = true;
}

void uJson::writeAttributeName(String entryName) {
  if (firstEntry == false) {
    file.write(",\n");
  } else {
    firstEntry = false;
  }

  file.write("\"");
  file.write(entryName.c_str());
  file.write("\":");
}

void uJson::writeHeader() { file.write("{\n"); }

void uJson::writeEntry(String entryName, bool value) {
  writeAttributeName(entryName);
  file.write((value) ? "true" : "false");
}

void uJson::writeEntry(String entryName, int value) {
  writeAttributeName(entryName);
  sprintf(buffer, "%d", value);
  file.write((const char *)buffer);
}

void uJson::writeEntry(String entryName, time_t value) {
  writeAttributeName(entryName);
  sprintf(buffer, "%lu", value);
  file.write((const char *)buffer);
}

void uJson::writeEntry(String entryName, const char *value) {
  writeAttributeName(entryName);
  file.write("\"");
  file.write(value);
  file.write("\"");
}

void uJson::writeFooter() { file.write("\n}\n"); }

int uJson::readLine()
{
  int i = file.readBytesUntil( '\n', (uint8_t*)buffer, 127 );
  buffer[i] = 0;
  return i;
}

bool uJson::readHeader() 
{ 
  return readLine() > 0 && strcmp( "{", buffer ) == 0; 
}

bool uJson::checkAttribute( const char* n1, const char* n2, char **p )
{
  if ( strcmp( n1, n2 ) == 0 )
  {
    *p = buffer + strlen(n1) + 2;
    if ( **p == ':' )
    {
      *p = *p + 1;
      return true;
    }
  }
  
  return false;
}

bool uJson::findValueEnd( char *p1, char **p2 )
{
  int i=0;
  while ( i<128 && *p1 != '\r' && *p1 != '\n' && *p1 != '\"' && *p1 != ',' && *p1 != 0 )
  {
    p1++;
    i++;
  }
  *p2 = p1;
  return i<128;
}


bool uJson::readEntryBoolean( const char* n1, const char* n2, bool *value ) 
{ 
   char * p1 = NULL, *p2;

   if ( checkAttribute( n1, n2, &p1 ))
   {   
      if ( p1 != NULL && findValueEnd( p1, &p2 ))
      {
        *p2 = 0;
        if ( strcmp( "true", p1 ) == 0 )
        {
          *value = true;
          return false;
        }
        if ( strcmp( "false", p1 ) == 0 )
        {
          *value = false;
          return false;
        }
      }
   }
   else
   {
     return false;
   }
   
   return true; 
}


bool uJson::readEntryInteger( const char* n1, const char* n2, int *value ) 
{
   char * p1 = NULL, *p2;

   if ( checkAttribute( n1, n2, &p1 ))
   {   
      if ( p1 != NULL && findValueEnd( p1, &p2 ))
      {
        *p2 = 0;
        if ( sscanf( p1, "%d", value ) == 1 )
        {
          return false;
        }
      }
   }
   else
   {
     return false;
   }
   
   return true; 
}

bool uJson::readEntryLong( const char* n1, const char* n2, long int *value ) 
{ 
   char * p1 = NULL, *p2;

   if ( checkAttribute( n1, n2, &p1 ))
   {   
      if ( p1 != NULL && findValueEnd( p1, &p2 ))
      {
        *p2 = 0;
        if ( sscanf( p1, "%ld", value ) == 1 )
        {
          return false;
        }
      }
   }
   else
   {
     return false;
   }
   
   return true;
}

bool uJson::readEntryChars( const char* n1, const char* n2, char *value ) 
{ 
   char * p1 = NULL, *p2;

   if ( checkAttribute( n1, n2, &p1 ))
   {  
      if ( p1 == NULL || *p1 != '"' )
      {
        return true;
      } 

      p1++;

      if ( findValueEnd( p1, &p2 ))
      {
        *p2 = 0;
        strcpy( value, p1 );
        return false;
      }
   }
   else
   {
     return false;
   }
   
   return true;
}

bool uJson::readAttributeName( char *attributeName ) 
{ 
  bool attributeNameFound = false;

  readLine();
  
  if ( buffer[0] == '"' )
  {
    char *p = index( buffer+1, '"' );
    if ( p != NULL && p[1] == ':' )
    {
      int len = p-(buffer+1);
      strncpy( attributeName, buffer+1, len );
      attributeName[len] = 0;
      attributeNameFound = true;
    }
  }

  return attributeNameFound; 
}
