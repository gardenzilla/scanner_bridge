# scanner_bridge
Scanner bridge service, bridging between local barcode scanner and websocket

Currently works with only Zebra scanner.

This application is made for GNU/Linux platform,
and depends on the latest Zebra Scanner SDK for Linux.

What you need to use mainly:

```bash
make dev
```

```bash
make release
```

Release will generate scanner and scanner_bridge binaries into the bin folder. You need to copy both of them, and place somewhere - but within the same folder.

scanner_bridge is going to start scanner as a subprocess, and communicate with it via pipe. Scanner_bridge is going to close that subprocess on close.

Scanner_bridge is going to create a websocket on the localhost, if a zebra scanner is available, then it will send the scanned data to the websocket subscribers with data

```json
{
    "code": "demo-content"
}
```

and it can handle "error" as an input, that will signal an error event to the scanner: red light flash and error voice.