//
//  NHOTemplateBroadcaster.hpp
//  Network
//
//  Created by Frédéric Rubio on 13/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef ESP32_ADAFRUIT_FEATHER
    #include <arpa/inet.h>
#endif
/**
* Initialize network stuff.
**/
template <class T>
bool NHOTemplateBroadcaster<T>::initiate(){
#ifdef ESP32_ADAFRUIT_FEATHER
    if (this->udp.connect(IPAddress(255,255,255,255), this->port)) {
        NHOFILE_LOG(logINFO) << "NHOTemplateBroadcaster::initiate - ASync UDP connected." << std::endl;
    }
    else {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::initiate: Unable to create emission udp resources" << std::endl;
        return(false);
    }
    if (this->udp.listen(this->port)) {
        NHOFILE_LOG(logINFO) << "NHOTemplateBroadcaster::initiate: listen" << std::endl;
        udp.onPacket([this](AsyncUDPPacket packet) {
            NHOFILE_LOG(logINFO) << "NHOTemplateBroadcaster::initiate: onPacket" << std::endl;
            NHOTemplateBroadcaster<T>* local = this;
            IPAddress localIP = packet.localIP();
            IPAddress remoteIP = packet.remoteIP();
            // NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::initiate: local @ " << localIP.toString().c_str() << std::endl;
            // NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::initiate: remote @ " << remoteIP.toString().c_str() << std::endl;
            if ( localIP != remoteIP) {
                this->receive(packet);
            }
        });
    }
#else
    struct sockaddr_in addr;
    int broadcastEnable = 1;
    
    // Création du socket UDP
    this->emrecSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->emrecSocket < 0) {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::initiate failed to create socket." << std::endl;
        return false;
    }
    
    // Autoriser le broadcast
    if (setsockopt(this->emrecSocket, SOL_SOCKET, SO_BROADCAST,
                   &broadcastEnable, sizeof(broadcastEnable)) < 0) {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::initiate failed to set SO_BROADCAST." << std::endl;
        close(this->emrecSocket);
        return false;
    }
    
    // Configuration de l'adresse locale
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(this->port);
    addr.sin_addr.s_addr = INADDR_ANY;
    this->addressSize = sizeof(addr);
    
    // Bind pour pouvoir recevoir
    if (bind(this->emrecSocket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::initiate failed to bind." << std::endl;
        close(this->emrecSocket);
        return false;
    }
    this->thread = new std::thread(&NHOTemplateBroadcaster::receive, std::ref(*this));
#endif
    return true;
}

/**
 * Terminte reception loop and close socket.
 **/
template <class T>
bool NHOTemplateBroadcaster<T>::terminate() {
    
    this->keepGoing = false;
#ifdef ESP32_ADAFRUIT_FEATHER
    this->udp.close();
    return(true);
#else    
    return (close(this->emrecSocket) ==0);
#endif    
}
    
/**
 * Receive
 **/
template <class T>
#ifdef ESP32_ADAFRUIT_FEATHER
bool NHOTemplateBroadcaster<T>::receive(AsyncUDPPacket pPacket){
//NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::send(receive(AsyncUDPPacket): waiting for lock." << std::endl;
this->mutex.lock();
    this->message->setData((int) pPacket.length(), (char *) pPacket.data());
    this->message->setAddress(pPacket.remoteIP());
    this->message->unserialize();
    this->setVal(this->message);
    this->mutex.unlock();
//NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::send(receive(AsyncUDPPacket): lock released." << std::endl;
this->notify();
#else
bool NHOTemplateBroadcaster<T>::receive(){
    long numbytes;
    struct sockaddr_storage their_addr;
//    NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::receive : waiting for lock." << std::endl;
this->mutex.lock();
    unsigned long size = this->message->getSize();
this->mutex.unlock();
//    NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::receive : lock released." << std::endl;
    char* buf = new char[size];
    socklen_t addr_len;
    
    while (this->keepGoing) {
        
        NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::receive stalled on recvfrom\n";

        addr_len = sizeof their_addr;
        if ((numbytes = recvfrom(this->emrecSocket, buf, size , 0,
                                 (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            std::cout << "NHOTemplateBroadcaster::receive recvfrom error " << strerror(errno) << "\n";
//                return(false);
        }
        else {
            // check we do not receive a broadcast we sent (same as EPS32)
//            NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::receive : receive waiting for lock." << std::endl;
this->mutex.lock();
            this->message->setData((int) numbytes, buf);
            this->message->setAddress((struct sockaddr *) &their_addr);
            this->message->unserialize();
            this->setVal(this->message);
this->mutex.unlock();
//            NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::receive : receive lock released." << std::endl;
            this->notify();
            if (numbytes > 0) {
                NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::receive message type : " <<     NHOMessage::getType(this->message->getData());
                NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::receive data received\n";
            }
        }
    }
#endif
    // NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::receive End\n";
    return true;
}

/**
 * Emit one.
 **/
template <class T>
#ifdef ESP32_ADAFRUIT_FEATHER
bool NHOTemplateBroadcaster<T>::send(const IPAddress*  pAddress, const  NHOMessage* const pMsg)  {
    // if (! ((std::remove_const<const AsyncUDP>::type) this->udp).connected()) {
    if (!((AsyncUDP) this->udp).connected()) {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::send: socket not initialized <" << ((std::remove_const<const AsyncUDP>::type) this->udp).lastErr() << ">." << std::endl;
        return false;
    }
    
    if ((*pMsg).getSize() > 65507) {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::send: message size exceeds limit." << std::endl;
        return false;
    }
    
    // send message
    size_t lWrittenBytes = 0;
    AsyncUDPMessage message ; 
    message.print(pMsg->getData());
//NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::send(const IPAddress*, const  NHOMessage): waiting for lock." << std::endl;
this->mutex.lock();
    lWrittenBytes = ((AsyncUDP )this->udp).sendTo(message, *pAddress, this->getEmissionPort());
this->mutex.unlock();
//NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::send(const IPAddress*, const  NHOMessage): lock released." << std::endl;
    NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::send(const IPAddress*  pAddress, const  NHOMessage* const pMsg) done\n";

    if (lWrittenBytes != pMsg->getSize()) {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::send: Parameters data <" << (uint8_t *) pMsg->getData() << "> port <" << this->getEmissionPort() << ">";
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::send: Sendig message failed <" << (unsigned int) lWrittenBytes << ">"
        << " vs <" << pMsg->getSize() << ">" << std::endl;
        Serial.print("Written bytes : ");
        Serial.println(lWrittenBytes);
        return(false);
    }
#else
bool NHOTemplateBroadcaster<T>::send(const sockaddr*  pAddress, const  NHOMessage  *pMsg)  {
    if (!emrecSocket) {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::send: socket not initialized." << std::endl;
        return false;
    }
    if ((*pMsg).getSize() > 65507) {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::send: message size exceeds limit." << std::endl;
        return false;
    }
    
    socklen_t optlen = sizeof(*pAddress);
    
    // send message
    size_t lWrittenBytes = 0;
//    NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::send(const IPAddress*, const  NHOMessage): waiting for lock." << std::endl;
this->mutex.lock();
    lWrittenBytes = sendto(emrecSocket,
                           pMsg->getData(),
                           pMsg->getSize(),
                           0,
                           pAddress,
                           optlen);
this->mutex.unlock();
//    NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::send(const IPAddress*, const  NHOMessage): lock released." << std::endl;
    if (lWrittenBytes != pMsg->getSize()) {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::send: Sendig message failed <" << lWrittenBytes << ">"
        << " vs <" << pMsg->getSize() << ">" << std::endl;
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::send: Sendig message failed <" << errno << ">" << std::endl;
        return(false);
    }
    
    NHOFILE_LOG(logINFO) << "NHOTemplateBroadcaster::send: Sent.\n";
    //delete pMsg;
#endif
    return(true);
}

/**
 * Emit one.
 **/
template <class T>
bool NHOTemplateBroadcaster<T>::send(const NHOMessage *  pMsg) {
    // NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::send: " << std::endl;
#ifdef ESP32_ADAFRUIT_FEATHER
    // if (! ((std::remove_const<const AsyncUDP>::type) this->udp).connected()) {
    if (! this->udp.connected()) {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::send: socket not initialized <" << ((std::remove_const<const AsyncUDP>::type) this->udp).lastErr() << ">." << std::endl;
        return false;
    }
    
    if ((*pMsg).getSize() > 65507) {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::send: message size exceeds limit." << std::endl;
        return false;
    }

    // send message
    size_t lWrittenBytes = 0; 
//    NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::send(const NHOMessage *): waiting for lock." << std::endl;
 this->mutex.lock();
    lWrittenBytes = this->udp.broadcastTo((uint8_t *) pMsg->getData(), (size_t) pMsg->getSize(), this->getEmissionPort());
 this->mutex.unlock();
//    NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::send(const NHOMessage *): lock released." << std::endl;
    if (lWrittenBytes != pMsg->getSize()) {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::send: Sendig message failed <" << (unsigned int) lWrittenBytes << ">"
        << " vs <" << pMsg->getSize() << ">" << std::endl;
        Serial.print("Written bytes : ");
        Serial.println(lWrittenBytes);
        return(false);
    }
#else
    if (!emrecSocket) {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::send(const NHOMessage *): socket not initialized." << std::endl;
        return false;
    }
    if ((*pMsg).getSize() > 65507) {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::send(const NHOMessage *): message size exceeds limit." << std::endl;
        return false;
    }
    
    // Adresse de broadcast
    struct sockaddr_in broadcastAddr;
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(this->port);
    broadcastAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
    
    // send message
    size_t lWrittenBytes = 0;
    //    NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::send(const NHOMessage *): waiting for lock." << std::endl;
this->mutex.lock();
    lWrittenBytes = sendto(emrecSocket,
                           pMsg->getData(),
                           pMsg->getSize(),
                           0,
                           (struct sockaddr *)&broadcastAddr,
                           this->addressSize);
  this->mutex.unlock();
    if (lWrittenBytes != pMsg->getSize()) {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::send: Sendig message failed <" << lWrittenBytes << ">"
        << " vs <" << pMsg->getSize() << ">" << std::endl;
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::send: Sendig message failed <" << errno << ">" << std::endl;
        return(false);
    }

    NHOFILE_LOG(logINFO) << "NHOTemplateBroadcaster::send: Sent.\n";
    //delete pMsg;
#endif
    return(true);
}

/**
 * Emit one.
 **/
#ifdef ESP32_ADAFRUIT_FEATHER
template <class T>
    void NHOTemplateBroadcaster<T>::send(const String message) {
    // enough elapsed time?
    long long currentTime = TS_NTP::clockMS();
    if (currentTime - this->lastWhatsappSendTime < 60 * 1000) {
        // no => return
        return;
    }

    // a message is about to be sent, update the last message emission
    this->lastWhatsappSendTime = currentTime;
        
    String phoneNumber = "+33626293184";
    String apiKey = "1344826";

    // Data to send with HTTP POST
    String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
    HTTPClient http;
    http.begin(url);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Send HTTP POST request
    int httpResponseCode = http.POST(url);
    if (httpResponseCode == 200){
        NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::send : Message sent successfully.";
    }
    else{
        Serial.println("Error sending the message");
        Serial.print("HTTP response code: ");
        Serial.println(httpResponseCode);
    }

    // Free resources
    http.end();
#else
template <class T>
void NHOTemplateBroadcaster<T>::send(const std::string message) {
    NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcaster::send sending whatsapp warning.";
#endif
}
