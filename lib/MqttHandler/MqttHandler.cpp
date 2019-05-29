#include <App.hpp>
#include <Arduino.h>
#include <PubSubClient.h>
#include <RelayHandler.hpp>
#include <WiFiClient.h>
#include <WifiHandler.hpp>
#include <Hlw8012Handler.hpp>
#include "MqttHandler.hpp"

MqttHandler mqttHandler;

static WiFiClient wifiClient;
static PubSubClient client(wifiClient);
static long lastReconnectAttempt = 0;

static void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }

  Serial.println();

  if (length == 1)
  {
    if (payload[0] == '1')
    {
      relayHandler.delayedOn();
    }

    if (payload[0] == '0')
    {
      relayHandler.delayedOff();
    }
  }
  else if (length == 2)
  {
    if ((payload[0] == 'o' || payload[0] == 'O') &&
        (payload[1] == 'n' || payload[1] == 'N'))
    {
      relayHandler.delayedOn();
    }
  }
  else if (length == 3)
  {
    if ((payload[0] == 'o' || payload[0] == 'O') &&
        (payload[1] == 'f' || payload[1] == 'F') &&
        (payload[2] == 'f' || payload[2] == 'F'))
    {
      relayHandler.delayedOff();
    }
  }
}

MqttHandler::MqttHandler() { initialized = false; }

bool MqttHandler::reconnect()
{
  if ((appcfg.mqtt_useauth &&
       client.connect(appcfg.mqtt_clientid, appcfg.mqtt_user,
                      appcfg.mqtt_password)) ||
      (!appcfg.mqtt_useauth && client.connect(appcfg.mqtt_clientid)))
  {
    LOG0("mqtt broker connected\n");
    client.subscribe(appcfg.mqtt_intopic);
  }

  return client.connected();
}

////////

void MqttHandler::setup()
{
  LOG0("MQTT Setup...\n");
  client.setServer(appcfg.mqtt_host, appcfg.mqtt_port);
  client.setCallback(callback);
  initialized = true;
}

void MqttHandler::handle(time_t now)
{
  if (appcfg.mqtt_enabled && wifiHandler.isReady())
  {
    if (initialized == false)
    {
      setup();
    }

    if (!client.connected())
    {
      long now = millis();

      if (now - lastReconnectAttempt > 5000)
      {
        lastReconnectAttempt = now;

        if (reconnect())
        {
          lastReconnectAttempt = 0;
        }
      }
    }
    else
    {
      client.loop();

#if defined(HAVE_ENERGY_SENSOR)
      if ( appcfg.mqtt_sending_interval > 0 &&
       ( now - lastPublishTimestamp ) > (appcfg.mqtt_sending_interval*1000))
      {
        char buffer[128];
        lastPublishTimestamp = now;
        
#if defined(HAVE_HLW8012)
        sendValue( appcfg.mqtt_topic_voltage, hlw8012Handler.getVoltage());
        delay(5);
        sendValue( appcfg.mqtt_topic_current, hlw8012Handler.getCurrent());
        delay(5);
        sendValue( appcfg.mqtt_topic_power, hlw8012Handler.getPower());
        delay(5);
        sprintf( buffer, "{\"voltage\":%0.1f,\"current\":%0.2f,\"power\":%0.1f}", 
        hlw8012Handler.getVoltage(), hlw8012Handler.getCurrent(), hlw8012Handler.getPower() );
        sendValue( appcfg.mqtt_topic_json, buffer );
#endif
      }
#endif
    }
  }
}

void MqttHandler::sendValue( const char* topic, const char *value )
{
  if( appcfg.mqtt_enabled && wifiHandler.isReady() && client.connected())
  {
    if ( topic != NULL && value != NULL && strlen(topic) > 0 && topic[0] != '-' )
    {
      client.publish( topic, value );
    }
  }
}

void MqttHandler::sendValue( const char* topic, const float value )
{
  char buffer[32];
  sprintf( buffer, "%0.2f", value );
  sendValue( topic, buffer );
}
