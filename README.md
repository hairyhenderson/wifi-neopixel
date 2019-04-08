# wifi-neopixel

## OTA Updating

Over-the-air updating is supported (via `ESP8266HTTPUpdateServer.h`), and a binary
can be uploaded by browsing to http://neopixel.local/update and uploading the binary.

1. `Verify`: (`⎇-⌘-r` in VSCode) - builds
2. Verify that the `.bin` file is updated in the output directory (`/tmp/wifi-neopixel` by default)
3. Go to http://neopixel.local/update and choose the file, and upload it
4. Rinse, repeat!

### Uploading with cURL

```console
$ curl -F update=@/tmp/wifi-neopixel/wifi-neopixel.ino.bin http://neopixel.local/update
...
```
