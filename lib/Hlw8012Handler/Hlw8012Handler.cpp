#include <App.hpp>
#ifdef HAVE_HLW8012
#include "Hlw8012Handler.hpp"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <HLW8012.h>
#include <RelayHandler.hpp>

// These are the nominal values for the resistors in the circuit
#define CURRENT_RESISTOR 0.001
#define VOLTAGE_RESISTOR_UPSTREAM (5 * 470000) // Real: 2280k
#define VOLTAGE_RESISTOR_DOWNSTREAM (1000)     // Real 1.009k

HLW8012 hlw8012;
Hlw8012Handler hlw8012Handler;

void ICACHE_RAM_ATTR hlw8012_cf1_interrupt()
{
    hlw8012.cf1_interrupt();
}

void ICACHE_RAM_ATTR hlw8012_cf_interrupt()
{
    hlw8012.cf_interrupt();
}

void Hlw8012Handler::setup()
{
    hlw8012.begin(CF_PIN, CF1_PIN, SEL_PIN, CURRENT_MODE );
    hlw8012.setResistors(CURRENT_RESISTOR, VOLTAGE_RESISTOR_UPSTREAM, VOLTAGE_RESISTOR_DOWNSTREAM);

    attachInterrupt(CF1_PIN, hlw8012_cf1_interrupt, CHANGE);
    attachInterrupt(CF_PIN, hlw8012_cf_interrupt, CHANGE);

#ifdef HAVE_BL0937
    hlw8012.setVoltageMultiplier(252081);
    hlw8012.setCurrentMultiplier(18000);
#else
    hlw8012.setVoltageMultiplier(312349);
    hlw8012.setCurrentMultiplier(18000);
#endif
}

void Hlw8012Handler::handle(unsigned long timestamp)
{
    if (( timestamp - lastReadTimestamp ) >= UPDATE_TIME )
    {
       lastReadTimestamp = timestamp;
       activePower = hlw8012.getActivePower();
       double Vtmp = hlw8012.getVoltage();
       if( Vtmp < 400.0 && Vtmp > 0.0 ) voltage = Vtmp;
       current = hlw8012.getCurrent();

       if( relayHandler.isPowerOn() == false )
       {
         current = 0.0;
       }

       if( current == 0.0 ) activePower = 0.0;
    }
}

double Hlw8012Handler::getVoltage()
{
  return voltage;
}

double Hlw8012Handler::getCurrent()
{
  return current;
}

double Hlw8012Handler::getActivePower()
{
  return ((double)activePower) / 10000.0 ;
}

double Hlw8012Handler::getPower()
{
  return (double)voltage * current;
}

#endif