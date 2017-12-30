
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



https://philhawthorne.com/j-a-r-v-i-s-inspired-announcementgreeting-for-home-assistant/


UpdateMe:
  action:
    service: tts.amazon_polly_say
    entity_id: media_player.mpd
    data_template:
      message: >
          <speak>
            {% if now().strftime("%H")|int < 12 %}
            Good morning.
            {% elif now().strftime("%H")|int < 18 %}
            Good afternoon.
            {% else %}
            Good evening.
            {% endif %}
            My name is Alice. I'm currently running version {{states('sensor.current_version')}}. The time now is {{states('sensor.time')}} and I see that your status is currently set to {{ states('device_tracker.iphone') }}. The current temperature in Hong Kong is {{ states('sensor.dark_sky_temperature') }} °C. And is expected to be {{ states('sensor.dark_sky_summary') }}. SSL certificate expiring in {{states('sensor.ssl_certificate_expiry')}} days. Your bitcoin is currently valued at {{ (states('sensor.exchange_rate_1_btc') | float * 0.113) | round(0) }}.
          </speak>
