/*
 * ©2015 Symbol Technologies LLC. All rights reserved.
 */

#include "ConsoleSampleEventListener.h"
#include "ISO15434formatEnvelope.h"

/* standard C includes */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* standard template library includes */
#include <string>
#include <iostream>
#include <sstream>
#include <errno.h>
#include <cstring>
#include <time.h>
SampleEventListener::SampleEventListener()
{

}

SampleEventListener::~SampleEventListener()
{
	Close();
}

void SampleEventListener::Open()
{
    StatusID status;
    ::Open(this, SCANNER_TYPE_ALL, &status);

    std::string inXml = "<inArgs><cmdArgs><arg-int>6</arg-int><arg-int>1,2,4,8,16,32</arg-int></cmdArgs></inArgs>";
    std::string outXml;
    ::ExecCommand(CMD_REGISTER_FOR_EVENTS, inXml, outXml, &status);
}

void SampleEventListener::GetScanners()
{
    unsigned short count;
    vector<unsigned int> list;
    string outXml;
    StatusID eStatus;

    ::GetScanners(&count, &list, outXml, &eStatus);

    if ( eStatus != STATUS_OK )
    {
        std::cout << "GetScanners failed. Can't connect to the corescanner." << std::endl;
        return ;
    }

    cout << "GetScanners ******* Scanner Count: " << count << endl;
    cout << outXml << endl;
}

void SampleEventListener::GetDeviceTopology()
{
    string inXml;
    string outXml;
    StatusID sId;
    
    cout << "GetDeviceTopology" << endl;
    ::ExecCommand(CMD_GET_DEVICE_TOPOLOGY, inXml, outXml, &sId);
    
    cout << "GetDeviceTopology" << endl;
    cout << outXml << endl;
}

void SampleEventListener::GetAttribute()
{
    //system("clear");
    cout << "================================" << endl;
    cout << "GetAttribute " << endl;
    std::string scannerID = "";
    cout << "Enter Scanner ID" << endl;
    cin >> scannerID;
    cout << "Enter attribute number or comma separated attribute numbers : " ;
    std::string attribute_number = "";
    cin >> attribute_number;

    std::string inXml = "<inArgs><scannerID>" + scannerID + 
                        "</scannerID><cmdArgs><arg-xml><attrib_list>" + 
                        attribute_number + "</attrib_list></arg-xml></cmdArgs></inArgs>";
    
    cout << "In XML : " << inXml << endl << endl;
    StatusID sId;
    std::string outXml;
    ::ExecCommand(CMD_RSM_ATTR_GET, inXml, outXml, &sId);

    cout << "Out XML : " << outXml << endl;
    cout << "================================" << endl;
}

void SampleEventListener::GetAttributeNext()
{
    //system("clear");
    cout << "================================" << endl;
    cout << "GetAttribute " << endl;
    std::string scannerID = "";
    cout << "Enter Scanner ID" << endl;
    cin >> scannerID;
    cout << "Enter current attribute number : " ;
    std::string attribute_number = "";
    cin >> attribute_number;

    std::string inXml = "<inArgs><scannerID>" + scannerID + 
                        "</scannerID><cmdArgs><arg-xml><attrib_list>" + 
                        attribute_number + "</attrib_list></arg-xml></cmdArgs></inArgs>";
    
    cout << "In XML : " << inXml << endl << endl;
    StatusID sId;
    std::string outXml;
    ::ExecCommand(CMD_RSM_ATTR_GETNEXT, inXml, outXml, &sId);

    cout << "Out XML : " << outXml << endl;
    cout << "================================" << endl;
}

void SampleEventListener::GetAllAttributes()
{    
    std::string scannerID = "";
    std::string subscannerID = "";
    //system("clear");
    cout << "================================" << endl;
    cout << "Enter scanner ID: ";
    cin >> scannerID;
    
    std::string inXml = "<inArgs><scannerID>"+scannerID+"</scannerID></inArgs>";
    
    cout << "InXML : " << inXml << endl;
    StatusID sId;
    std::string outXml;

    ::ExecCommand(CMD_RSM_ATTR_GETALL, inXml, outXml, &sId);

    cout << "Out XML" << outXml << endl;
    cout << "================================" << endl;
}

void SampleEventListener::DiscoverTunnelingDevice()
{
    
    //::RefreshDevicelTopology();
    /**std::string inXml = "<inArgs><scannerID>1</scannerID></inArgs>";
    //std::string inXml = "";
    StatusID sId;
    std::string outXml;

    ::ExecCommand(CMD_RSM_TUNNEL_REPORT_NEXT_HOP, inXml, outXml, &sId);

    cout << outXml << endl;*/
}

void SampleEventListener::SetAttribute()
{
    //system("clear");
    cout << "================================" << endl;
    cout << "SetAttribute " << endl;
    std::string scannerID = "";
    cout << "Enter scanner ID : " << endl;
    cin >> scannerID;
    std::string attributeNumber = "";
    cout << "Enter attribute number : " << endl;
    cin >> attributeNumber;
    std::string dataType = "";
    cout << "Enter data type : " << endl;
    cin >> dataType;
    std::string attributeValue = "";
    cout << "Enter attribute value : " << endl;
    cin >> attributeValue;
    

    std::string inXml = "<inArgs><scannerID>"+ scannerID +
                        "</scannerID><cmdArgs><arg-xml><attrib_list><attribute><id>" + attributeNumber + 
                        "</id><datatype>" + dataType + 
                        "</datatype><value>" + attributeValue + 
                        "</value></attribute></attrib_list></arg-xml></cmdArgs></inArgs>";
    
    //std::string inXml = "";
    cout << "In XML : " << inXml << endl << endl;
    
    StatusID sId;
    
    std::string outXml;
    ::ExecCommand(CMD_RSM_ATTR_SET, inXml, outXml, &sId);

    cout << "Out XML : " << outXml << endl;
    
    cout << "================================" << endl;

}

void SampleEventListener::SetAttributeStore()
{
    //system("clear");
    cout << "================================" << endl;
    cout << "StoreAttribute " << endl;
    std::string scannerID = "";
    cout << "Enter scanner ID : " << endl;
    cin >> scannerID;
    std::string attributeNumber = "";
    cout << "Enter attribute number : " << endl;
    cin >> attributeNumber;
    std::string dataType = "";
    cout << "Enter data type : " << endl;
    cin >> dataType;
    std::string attributeValue = "";
    cout << "Enter attribute value : " << endl;
    cin >> attributeValue;
    

    std::string inXml = "<inArgs><scannerID>"+ scannerID +
                        "</scannerID><cmdArgs><arg-xml><attrib_list><attribute><id>" + attributeNumber + 
                        "</id><datatype>" + dataType + 
                        "</datatype><value>" + attributeValue + 
                        "</value></attribute></attrib_list></arg-xml></cmdArgs></inArgs>";
    
    //std::string inXml = "";
    cout << "In XML : " << inXml << endl << endl;
    
    StatusID sId;
    
    std::string outXml;
    ::ExecCommand(CMD_RSM_ATTR_STORE, inXml, outXml, &sId);

    cout << "Out XML : " << outXml << endl;
    
    cout << "================================" << endl;
}

void SampleEventListener::SetZeroWeight()
{
    std::string inXml = "<inArgs><scannerID>1</scannerID><cmdArgs><arg-xml><attrib_list><attribute><id>6019</id><datatype>X</datatype><value>0</value></attribute></attrib_list></arg-xml></cmdArgs></inArgs>";

    cout << "In XML : " << inXml << endl << endl;
    StatusID sId;
    std::string outXml;
    ::ExecCommand(CMD_RSM_ATTR_SET, inXml, outXml, &sId);

    cout << "Out XML : " << outXml << endl;

}

//Event EventListener for get Get Version number of Core scanner
void SampleEventListener::GetVersion() {
    
    std::string inXml = "<inArgs></inArgs>";
    cout << "In XML : " << inXml << endl << endl;
    StatusID sId;
    std::string outXml;
    ::ExecCommand(CMD_GET_VERSION, inXml, outXml, &sId);
    cout << "Out XML : " << outXml << endl;
    
}

void SampleEventListener::Close()
{
    StatusID status;
    ::Close(0, &status);

}

string getStringFromRawData(unsigned char* rawData,int startIndex, int endIndex){
    int numElements = endIndex-startIndex;
    char buffer [numElements];
    int j=0;
    for(int i=startIndex;i<endIndex;i++){
        sprintf (&buffer[j++], "%c", rawData[i]);
    }
    string str(buffer);
    return str;
}
void SampleEventListener::OnImageEvent( short eventType, int size, short imageFormat, char* sfimageData, int dataLength, std::string& pScannerData )
{
	cout << "OnImageEvent" << endl;
}

void SampleEventListener::OnBinaryDataEvent( short eventType, int size, short dataFormat, unsigned char* sfBinaryData, std::string&  pScannerData)
{    
     // Constants for ISO15434 messages
        const unsigned char ISO_RS = 0x1E;  // ISO15454 Format Trailer Character
//        const unsigned char ISO_GS = 0x1D;  // ISO15454 Data Element Separator
        const unsigned char ISO_EOT = 0x04;  // ISO15454 Message Trailer Character
        const unsigned char MSG_EASYCAP = 0;     // ISO15451 Message DocCap message number
        FILE *imgFile; 
     	cout << "OnBinaryDataEvent" << endl;
		
        int packetLength = (sfBinaryData[0] << 24) |
                        (sfBinaryData[1] << 16) |
                        (sfBinaryData[2] << 8) |
                        sfBinaryData[3];
        
        if(packetLength+4!= size)
        {
            cout << "Incorrect packet size\n" << endl;
        }
        else
        {
            if(sfBinaryData[4]!= MSG_EASYCAP)
            {
                cout << "Incorrect Msg type\n" << endl;
            }
            else
            {
                // ISO15434 Envelope: is message header correct?
                if ((sfBinaryData[5] != '[') || (sfBinaryData[6] != ')') || (sfBinaryData[7] != '>') || (sfBinaryData[8] != ISO_RS))
                {
                    cout << "Incorrect message header\n" << endl;
                }
                else
                {
                     // ISO15434 Envelope: is message header correct?
                    if (sfBinaryData[size - 1] != ISO_EOT)
                    {
                        cout << "Incorrect message header ISO_EOT\n" << endl;
                    }
                    else
                    {
                        cout << "Correct packet received \n" << endl;
                        ISO15434formatEnvelope *anEnvelope = new ISO15434formatEnvelope(sfBinaryData,size, 9);
                        while (anEnvelope->getNext())
                        {
                            string fileType = anEnvelope->getFileType();

                            if (fileType == "BarCode")
                            {
                                string decodeData=getStringFromRawData(sfBinaryData,anEnvelope->getDataIndex(),anEnvelope->getDataIndex()+anEnvelope->getDataLength());
                                cout << "Barcode received " << endl;
                                cout << " Data type = "<<  (int)decodeData.at(0) << endl;
                                cout << " Data = "<< decodeData.substr(1,anEnvelope->getDataLength())<< endl;
                            }else{
                                char filename[200];
                                string fileFormat = anEnvelope->getFileType();
                                time_t t = time(0);   // get time now
                                tm* now = localtime(&t);
                                sprintf(filename,"ZIMG-%d%d%d%d%d%d%s%s", now->tm_year + 1900, now->tm_mon + 1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec,".",fileFormat.c_str());
                                imgFile = fopen(filename, "w");
                                if (!imgFile)
                                {
                                    cout << "Unable to open file " << filename<< endl;
                                    continue;
                                }
                                fwrite(&sfBinaryData[anEnvelope->getDataIndex()]+6,sizeof(unsigned char),anEnvelope->getDataLength()+6,imgFile);
                                fflush(imgFile);
                                fclose (imgFile);
                                cout << "ImageData saved in " <<filename<< endl;
                            }
                        }
                    }
                }
                
            }
        }
}

void SampleEventListener::OnVideoEvent( short eventType, int size, char* sfvideoData, int dataLength, std::string& pScannerData )
{
	cout << "OnVideoEvent" << endl;
}

void SampleEventListener::OnPNPEvent( short eventType, std::string ppnpData )
{
    string str;
    if (eventType == SCANNER_ATTACHED) {
        cout << "Scanner attached" << endl;
        str = ppnpData;
    } else if (eventType == SCANNER_DETACHED) {
        cout << "Scanner detached" << endl;
        str =  ppnpData;
    } else {
        str = " UNKNOWN PNP Event ";
    }
    cout << str << endl;
}

void SampleEventListener::OnCommandResponseEvent( short status, std::string& prspData )
{
    cout << endl << "Scanner data: " << prspData << endl;
	cout << "OnCommandResponseEvent" << endl;
	cout << prspData << endl;
}

void SampleEventListener::OnScannerNotification( short notificationType, std::string& pScannerData )
{
    cout << endl << "Scanner event data: " << pScannerData << endl;
	cout << "OnScannerNotification" << endl;
}

void SampleEventListener::OnIOEvent( short type, unsigned char data )
{
	cout << "OnIOEvent" << endl;
}

void SampleEventListener::OnScanRMDEvent( short eventType, std::string& prmdData )
{
	cout << "OnScanRMDEvent" << endl;
        cout << "Out XML " << endl;
        cout << prmdData << endl;
}

void SampleEventListener::OnDisconnect()
{
	cout << "OnDisconnect" << endl;
}

void SampleEventListener::OnBarcodeEvent(short int eventType, std::string & pscanData)
{
	cout << pscanData << endl;
}

void SampleEventListener::RebootScanner()
{
    
}

void SampleEventListener::FirmwareUpdate()
{
    std::string inXml;
    std::string outXml;
    std::string datFilePath; 
    StatusID sId;
    std::string scannerID;
    std::string bulkOption;
    
    cout << "================================" << endl;
    std::cout << "FirmwareUpdate" << std::endl;
    std::cout << "Enter Scanner ID: " << std::endl;
    std::cin >> scannerID;
    std::cout << "Enter Firmware DAT file path: " << std::endl;
    std::cin >> datFilePath;
    std::cout << "Enter USB communication mode 1=hid, 2=bulk : ";
    std::cin >> bulkOption;
    
    inXml = "<inArgs><scannerID>" + scannerID + "</scannerID><cmdArgs><arg-string>" + datFilePath + "</arg-string><arg-int>" + bulkOption + "</arg-int></cmdArgs></inArgs>";
    
    cout << "InXML : " << inXml << endl;
    
    ::ExecCommand(CMD_DEVICE_UPDATE_FIRMWARE, inXml, outXml, &sId);
    
    cout << "================================" << endl;
    cout << outXml << endl;
}

void SampleEventListener::FirmwareUpdateFromPlugin()
{
     std::string inXml;
    std::string outXml;
    std::string pluginFilePath=""; 
    StatusID sId;
    std::string scannerID;
    std::string bulkOption;
    
    cout << "================================" << endl;
    std::cout << "FirmwareUpdate From Plug-in." << std::endl;   
    std::cout << "Enter Scanner ID: " << std::endl;
    std::cin >> scannerID;
    std::cout << "Enter Firmware Plug-in file path: " << std::endl;
    //std::cin >>  pluginFilePath;

    while ( pluginFilePath.size() < 4 ){
        std::getline(std::cin, pluginFilePath);

    }
    
    if ( !(pluginFilePath.substr(pluginFilePath.find_last_of(".")+ 1) == "SCNPLG") ){
        std::cout << "Please Enter a file with extension .SCNPLG." << std::endl;
        return;
    }
    
    std::cout << "Enter USB communication mode 1=hid, 2=bulk : ";
    std::cin >> bulkOption;
    
    inXml = "<inArgs><scannerID>" + scannerID + "</scannerID><cmdArgs><arg-string>" + pluginFilePath + "</arg-string><arg-int>" + bulkOption + "</arg-int></cmdArgs></inArgs>";
    
    cout << "InXML : " << inXml << endl;
    
    ::ExecCommand(CMD_DEVICE_UPDATE_FIRMWARE_FROM_PLUGIN, inXml, outXml, &sId);
    
    cout << "================================" << endl;
    cout << outXml << endl;   
}

void SampleEventListener::StartNewFirmware()
{
    std::string inXml;
    std::string outXml;
    StatusID sId;
    std::string scannerID;
    
    cout << "================================" << endl;
    std::cout << "Starting new firmware" << std::endl;
    std::cout << "Enter Scanner ID: " << std::endl;
    std::cin >> scannerID;
    std::cout << "Enter Firmware DAT file path: " << std::endl;
    
    inXml = "<inArgs><scannerID>" + scannerID + "</scannerID></inArgs>";
    
    cout << "InXML : " << inXml << endl;
    
    ::ExecCommand(CMD_START_NEW_FIRMWARE, inXml, outXml, &sId);
    
    cout << "================================" << endl;
    cout << outXml << endl;
}

void SampleEventListener::AbortFirmwareUpdate()
{
    std::string inXml;
    std::string outXml;
    StatusID sId;
    std::string scannerID;
    
    cout << "================================" << endl;
    std::cout << "Abort Firmware Update" << std::endl;
    std::cout << "Enter Scanner ID: " << std::endl;
    std::cin >> scannerID;
    
    inXml = "<inArgs><scannerID>" + scannerID + "</scannerID></inArgs>";
    
    cout << "InXML : " << inXml << endl;
    
    ::ExecCommand(CMD_DEVICE_ABORT_UPDATE_FIRMWARE, inXml, outXml, &sId);
    
    cout << "================================" << endl;
    cout << outXml << endl;
}

void SampleEventListener::executeError()
{
    StatusID sId;
    std::string outXml;
    std::string cmd;

    cmd = "<inArgs><scannerID>1</scannerID><cmdArgs><arg-int>21</arg-int></cmdArgs></inArgs>";
    ::ExecCommand(CMD_DEVICE_BEEP_CONTROL, cmd, outXml, &sId);

    // Red len on
    cmd = "<inArgs><scannerID>1</scannerID><cmdArgs><arg-int>47</arg-int></cmdArgs></inArgs>";
    ::ExecCommand(CMD_DEVICE_BEEP_CONTROL, cmd, outXml, &sId);

    usleep(300000);

    cmd = "<inArgs><scannerID>1</scannerID><cmdArgs><arg-int>48</arg-int></cmdArgs></inArgs>";
    ::ExecCommand(CMD_DEVICE_BEEP_CONTROL, cmd, outXml, &sId);
}

/**
 * Method to execute action attribute related commands.
 * added to v1.3.0 release.
 * @param opCode
 */
void SampleEventListener::ExecuteActionCommand(CmdOpcode opCode)
{
    std::string scannerID = ""; // For now. it was empty
    std::string ledNumber = "";
    std::string beeperCode = "";
    std::string inXml;
    
    cout << "================================" << endl;
    
    switch (opCode)
    {
        case CMD_DEVICE_LED_ON:
        case CMD_DEVICE_LED_OFF:
        {
            cout << "Enter scanner ID: ";
            cin >> scannerID;
            cout << "Enter LED number: ";
            cin >> ledNumber;

            inXml = "<inArgs><scannerID>" + scannerID + 
                                "</scannerID><cmdArgs><arg-int>" + ledNumber +
                                "</arg-int></cmdArgs></inArgs>";
            break;
        }
        case CMD_DEVICE_BEEP_CONTROL:
        {
            cout << "Enter scanner ID: ";
            cin >> scannerID;
            cout << "Enter Beeper code: ";
            cin >> beeperCode;

            inXml = "<inArgs><scannerID>" + scannerID + 
                                "</scannerID><cmdArgs><arg-int>" + beeperCode +
                                "</arg-int></cmdArgs></inArgs>";
            break;
        }
        default:
        {
            cout << "Enter scanner ID: ";
            cin >> scannerID;

            inXml = "<inArgs><scannerID>" + scannerID + "</scannerID></inArgs>";
            break;
        }
    }
    
    cout << "InXML : " << inXml << endl;
    StatusID sId;
    std::string outXml;

    ::ExecCommand(opCode, inXml, outXml, &sId);

    cout << "Out XML" << outXml << endl;
    cout << "================================" << endl;
}
