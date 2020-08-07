/*
 * ©2015 Symbol Technologies LLC. All rights reserved.
 */

#ifndef SAMPLEEVENTLISTENER_H_
#define SAMPLEEVENTLISTENER_H_



#include "CsIEventListenerXml.h"
#include "CsUserDefs.h"
#include "CsBarcodeTypes.h"
#include "Cslibcorescanner_xml.h"

/* standard C includes */
#include <stdio.h>
#include <stdlib.h>

/* standard template library includes */
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class SampleEventListener : public IEventListenerXml
{
public:
	explicit SampleEventListener();
	virtual ~SampleEventListener();

    virtual void OnImageEvent( short eventType, int size, short imageFormat, char* sfimageData, int dataLength, std::string& pScannerData );
    virtual void OnVideoEvent( short eventType, int size, char* sfvideoData, int dataLength, std::string& pScannerData );
    virtual void OnBarcodeEvent( short eventType, std::string& pscanData );
    virtual void OnPNPEvent( short eventType, std::string ppnpData );
    virtual void OnCommandResponseEvent( short status, std::string& prspData );
    virtual void OnScannerNotification( short notificationType, std::string& pScannerData );
    virtual void OnIOEvent( short type, unsigned char data );
    virtual void OnScanRMDEvent( short eventType, std::string& prmdData );
    virtual void OnDisconnect();
    virtual void OnBinaryDataEvent( short eventType, int size, short dataFormat, unsigned char* sfBinaryData, std::string&  pScannerData);


    void Open();
    void GetScanners();
    void GetAttribute();
    void GetAttributeNext();
    void DiscoverTunnelingDevice();
    void GetAllAttributes();
    void SetAttribute();
    void SetAttributeStore();
    void SetZeroWeight();
    void Close();
    
    void executeError();
    void RebootScanner();
    void ExecuteActionCommand(CmdOpcode opCode);
    void GetDeviceTopology();
    void FirmwareUpdate();
    void FirmwareUpdateFromPlugin();
    void StartNewFirmware();
    void AbortFirmwareUpdate();
    void GetVersion();
};


#endif /* SAMPLEEVENTLISTENER_H_ */
