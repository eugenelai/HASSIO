
# HASS.io
This repository is used to store my latest HASS config files. For my personal use only but feel free to add!


Intent Script examples:
https://github.com/arsaboo/homeassistant-config/blob/master/intent_script.yaml


Code Snippets
I see that you're currently {{ states('device_tracker.eugeiphone') }}.
The current temperature is {{ states('sensor.dark_sky_temperature') }} °C

SSL certificate expiring in {{states("sensor.ssl_certificate_expiry")}} days
The current exchange rate of 1 Bitcoin is {{ states('sensor.exchange_rate_1_btc') }} Hong Kong Dollars


Sensor Template for Bitcoin
- platform: template
  sensors:
    exchange_rate_1_btc:
      friendly_name: bitcion value
      entity_id: sensor.exchange_rate_1_btc
      unit of measurement: 'HKD'
      value template: "{{ (states('sensor.exchange_rate_1_btc') | float * 0.113) | round(0) }}"
hello
