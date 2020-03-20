#include <Arduino.h>
#include <Esp.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <App.hpp>
#include <RelayHandler.hpp>
#include <WifiHandler.hpp>

#include "InternetConnectionCheckHandler.hpp"

InternetConnectionCheckHandler internetConnectionCheckHandler;

static AsyncClient *aClient = NULL;

static bool successFound;
static char *successTxt = (char *)"Success";
static char *matchPtr;
static time_t startTimestamp;
static bool initStateRead;
static bool lastState;

void runAction( bool connected )
{
  LOG1( "run acion = %s\n", successFound ? "true" : "false" );
  if ( initStateRead )
  {
    if ( lastState != connected )
    {
      LOG0( "Internet connection state changed\n" );

      switch( appcfg.inet_check_action )
      {
        case INET_CHECK_ACTION_ON_CONNECT_SWITCH_OFF:
          if ( connected )
          {
            relayHandler.delayedOff();
          }
          break;

        case INET_CHECK_ACTION_ON_CONNECT_SWITCH_ON:
          if ( connected )
          {
            relayHandler.delayedOn();
          }
          break;

        case INET_CHECK_ACTION_ON_DISCONNECT_SWITCH_OFF:
          if ( !connected )
          {
            relayHandler.delayedOff();
          }
          break;

        case INET_CHECK_ACTION_ON_DISCONNECT_SWITCH_ON:
          if ( !connected )
          {
            relayHandler.delayedOn();
          }
          break;

        case INET_CHECK_ACTION_SHOW_CONNECTION_STATE:
          if ( connected )
          {
            relayHandler.delayedOn();
          }
          else
          {
            relayHandler.delayedOff();
          }
          break;

        case INET_CHECK_ACTION_SHOW_CONNECTION_STATE_INV:
          if ( connected )
          {
            relayHandler.delayedOff();
          }
          else
          {
            relayHandler.delayedOn();
          }
          break;

        default:
          LOG0( "Unknown action\n" );
      }

      lastState = connected;
    }
  }
  else
  {
    LOG0( "Saving internet connection state\n" );
    lastState = connected;
    initStateRead = true;
  }
}

InternetConnectionCheckHandler::InternetConnectionCheckHandler() 
{
  initialized = false;
  initStateRead = false;
}

void InternetConnectionCheckHandler::setup()
{
  LOG0("Internet Connection Check Setup...\n");
  initialized = true;
  lastCheckedTimestamp = 0l;
  LOG0("done\n");
}

void InternetConnectionCheckHandler::handle(time_t now)
{
  if ( appcfg.inet_check_enabled == true && wifiHandler.isInStationMode())
  {
    if ( !initialized ) 
    { 
      setup(); 
    }

  ////////////////////////////////////////////////////////////////////////
  
    if (( now - lastCheckedTimestamp ) >= (((time_t)appcfg.inet_check_period) * 1000l ))
    {
      LOG0("Checking internet connection...\n");

      if ( !wifiHandler.isConnected() )
      {
        runAction( false );
        lastCheckedTimestamp = now;
        return;
      }

        startTimestamp = millis();
        aClient = new AsyncClient();

        successFound = false;
        matchPtr = successTxt;

        if (aClient)
        {
          aClient->onError([](void *arg, AsyncClient *client, int error) {
            Serial.println("\nConnect Error");
            aClient = NULL;
            delete client;
            runAction(false);
          }, NULL);

          aClient->onConnect([](void *arg, AsyncClient *client) {
            Serial.println("Connected");

            aClient->onError(NULL, NULL);

            client->onDisconnect([](void *arg, AsyncClient *client) {
              Serial.printf("Disconnected duration=%ld\n", millis() - startTimestamp );
              aClient = NULL;
              delete client;
              runAction( successFound );
            }, NULL);

            client->onData([](void *arg, AsyncClient *client, void *data, size_t len) {
              Serial.print("Data: ");
              Serial.println(len);
              char *d = (char *)data;
              for (size_t i = 0; i < len; i++)
              {
                // Serial.write(d[i]);
                if ( *matchPtr == 0 )
                {
                  successFound = true;
                }
                else
                {
                  if ( *matchPtr == d[i] )
                  {
                    matchPtr++;
                  }
                  else
                  {
                    matchPtr = successTxt;
                  }
                }
              }
            }, NULL);

            //send the request
            client->write("GET / HTTP/1.0\r\nHost: captive.apple.com\r\n\r\n");
          }, NULL);

          if (!aClient->connect("captive.apple.com", 80))
          {
            Serial.println("Connect Fail");
            AsyncClient *client = aClient;
            aClient = NULL;
            delete client;
            runAction( false );
          }
        }


      lastCheckedTimestamp = now;
    }

  ////////////////////////////////////////////////////////////////////////
  }
}