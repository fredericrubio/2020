//
//  NHOTemplateBroadcastReceiver.hpp
//  Network
//
//  Created by Frédéric Rubio on 13/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

/**
* Initialize network stuff.
**/
template <class T>
bool NHOTemplateBroadcaster<T>::initiate(){
#ifdef ESP32_ADAFRUIT_FEATHER
    if (this->udp.connect(IPAddress(255,255,255,255), this->port)) {
        NHOFILE_LOG(logINFO) << "NHOBroadcastEmitter::initiate - ASync UDP connected." << std::endl;
    }
    else {
        NHOFILE_LOG(logERROR) << "NHOBroadcastEmitter::initiate: Unable to create emission udp resources" << std::endl;
        return(false);
    }
    if (this->udp.listen(this->port)) {
        NHOFILE_LOG(logINFO) << "NHOBroadcastEmitter::initiate: listen" << std::endl;
        udp.onPacket([this](AsyncUDPPacket packet) {
            NHOFILE_LOG(logINFO) << "NHOBroadcastEmitter::initiate: onPacket" << std::endl;
            NHOTemplateBroadcaster<T>* local = this;
            IPAddress localIP = packet.localIP();
            IPAddress remoteIP = packet.remoteIP();
            // NHOFILE_LOG(logDEBUG) << "NHOBroadcastEmitter::initiate: local @ " << localIP.toString().c_str() << std::endl;
            // NHOFILE_LOG(logDEBUG) << "NHOBroadcastEmitter::initiate: remote @ " << remoteIP.toString().c_str() << std::endl;
            if ( localIP != remoteIP) {
                this->receive(packet);
            }
        });
    }
#else
    // local variables
    struct addrinfo hints, *servinfo, *p;
    int rv;
    //    int broadcast = 1;
    //    socklen_t optlen = sizeof(broadcast);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;//AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, std::to_string(this->port).c_str(), &hints, &servinfo)) != 0) {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcastReceiver::initiate getaddrinfo:" << gai_strerror(rv) << std::endl;
        return false;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        this->emrecSocket = socket(p->ai_family,
        p->ai_socktype,
        p->ai_protocol);
        if (this->emrecSocket == -1) {
            NHOFILE_LOG(logERROR) << "NHOTemplateBroadcastReceiver::initiate socket (socket):" << strerror(errno) << std::endl;
            continue;
        }
        // to allow address reuse (in case of of two close execution.
        int option = 1;
        if (setsockopt(this->emrecSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1) {
            NHOFILE_LOG(logERROR) << "NHOTemplateBroadcastReceiver::initiate socket (SO_REUSEADDR):" << strerror(errno) << std::endl;
            continue;
        }
        option = 1;
        if (setsockopt(this->emrecSocket, SOL_SOCKET, SO_REUSEPORT, &option, sizeof(option)) == -1) {
            NHOFILE_LOG(logERROR) << "NHOTemplateBroadcastReceiver::initiate socket (SO_REUSEPORT):" << strerror(errno) << std::endl;
            continue;
        }
        // a security based on time
        struct timeval timeout;
        timeout.tv_sec = 100;
        timeout.tv_usec = 0;
        if (setsockopt (this->emrecSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
            NHOFILE_LOG(logERROR) << "NHOTemplateBroadcastReceiver::initiate socket (SO_RCVTIMEO):" << strerror(errno) << std::endl;
            continue;
        }

        option = 1;
        if (setsockopt(this->emrecSocket, SOL_SOCKET, SO_BROADCAST, &option, sizeof(option)) == -1) {
            NHOFILE_LOG(logERROR) << "NHOTemplateBroadcastReceiver::initiate socket (SO_BROADCAST):" << strerror(errno) << std::endl;
            continue;
        }

        if (bind(this->emrecSocket, p->ai_addr, p->ai_addrlen) == -1) {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcastReceiver::initiate bind:" << strerror(errno) << std::endl;
            close(this->emrecSocket);
            continue;
        }

        linger lin;
        lin.l_onoff = 1;
        lin.l_linger = 0;
        if (setsockopt( this->emrecSocket,
                        SOL_SOCKET,
                        SO_LINGER,
                        (const char *)&lin,
                        sizeof(lin)) == -1) {
            NHOFILE_LOG(logERROR) << "NHOTemplateBroadcastReceiver::initiate socket:" << strerror(errno) << std::endl;
            continue;
        }

        break;
    }

    if (p == NULL) {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcastReceiver::initiate failed to bind socket." << std::endl;
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
this->mutex.lock();
    this->message->setData((int) pPacket.length(), (char *) pPacket.data());
    this->message->setAddress(pPacket.remoteIP());
    this->message->unserialize();
    this->setVal(this->message);
    this->notify();
this->mutex.unlock();
#else
bool NHOTemplateBroadcaster<T>::receive(){
    long numbytes;
    struct sockaddr_storage their_addr;
this->mutex.lock();
    unsigned long size = this->message->getSize();
this->mutex.unlock();
    char* buf = new char[size];
    socklen_t addr_len;
    
    while (this->keepGoing) {
        
        NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcastReceiver::receive stalled on recvfrom\n";

        addr_len = sizeof their_addr;
        if ((numbytes = recvfrom(this->emrecSocket, buf, size , 0,
                                 (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            std::cout << "NHOTemplateBroadcastReceiver::receive recvfrom error " << strerror(errno) << "\n";
//                return(false);
        }
        else {
            // check we do not receive a broadcast we sent (same as EPS32)
this->mutex.lock();
            this->message->setData((int) numbytes, buf);
            this->message->setAddress((struct sockaddr *) &their_addr);
            this->message->unserialize();
            this->setVal(this->message);
            this->notify();
this->mutex.unlock();
            if (numbytes > 0) {
                NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcastReceiver::receive message type : " <<     NHOMessage::getType(this->message->getData());
                NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcastReceiver::receive data received\n";
            }
        }
    }
#endif
    // NHOFILE_LOG(logDEBUG) << "NHOTemplateBroadcastReceiver::receive End\n";
    return true;
}

/**
 * Emit one.
 **/
template <class T>
#ifdef ESP32_ADAFRUIT_FEATHER
bool NHOTemplateBroadcaster<T>::send(const IPAddress*  pAddress, const  NHOMessage  *pMsg) const {
    // if (! ((std::remove_const<const AsyncUDP>::type) this->udp).connected()) {
    if (!((AsyncUDP) this->udp).connected()) {
        NHOFILE_LOG(logERROR) << "NHOBroadcastEmitter::send: socket not initialized <" << ((std::remove_const<const AsyncUDP>::type) this->udp).lastErr() << ">." << std::endl;
        return false;
    }
    
    if ((*pMsg).getSize() > 65507) {
        NHOFILE_LOG(logERROR) << "NHOBroadcastEmitter::send: message size exceeds limit." << std::endl;
        return false;
    }
    
    // send message
    size_t lWrittenBytes = 0;
    AsyncUDPMessage message ; // to do: initialisation...
    lWrittenBytes = ((AsyncUDP )this->udp).sendTo(message, *pAddress, this->getEmissionPort());

    if (lWrittenBytes != pMsg->getSize()) {
        NHOFILE_LOG(logERROR) << "NHOEmitter::send: Parameters data <" << (uint8_t *) pMsg->getData() << "> port <" << this->getEmissionPort() << ">";
        NHOFILE_LOG(logERROR) << "NHOEmitter::send: Sendig message failed <" << (unsigned int) lWrittenBytes << ">"
        << " vs <" << pMsg->getSize() << ">" << std::endl;
        Serial.print("Written bytes : ");
        Serial.println(lWrittenBytes);
        return(false);
    }
#else
bool NHOTemplateBroadcaster<T>::send(const sockaddr*  pAddress, const  NHOMessage  *pMsg) const {
    if (!emrecSocket) {
        NHOFILE_LOG(logERROR) << "NHOSVBroadcast::send: socket not initialized." << std::endl;
        return false;
    }
    if ((*pMsg).getSize() > 65507) {
        NHOFILE_LOG(logERROR) << "NHOSVBroadcast::send: message size exceeds limit." << std::endl;
        return false;
    }
    
    socklen_t optlen = sizeof(*pAddress);
    
    // send message
    size_t lWrittenBytes = 0;
    lWrittenBytes = sendto(emrecSocket,
                           pMsg->getData(),
                           pMsg->getSize(),
                           0,
                           pAddress,
                           optlen);
    if (lWrittenBytes != pMsg->getSize()) {
        NHOFILE_LOG(logERROR) << "NHOSVBroadcast::send: Sendig message failed <" << lWrittenBytes << ">"
        << " vs <" << pMsg->getSize() << ">" << std::endl;
        NHOFILE_LOG(logERROR) << "NHOSVBroadcast::send: Sendig message failed <" << errno << ">" << std::endl;
        return(false);
    }
    
    NHOFILE_LOG(logINFO) << "NHOSVBroadcast::send: Sent.\n";
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
    // NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::send: waiting for lock." << std::endl;
this->mutex.lock();
    lWrittenBytes = this->udp.broadcastTo((uint8_t *) pMsg->getData(), (size_t) pMsg->getSize(), this->getEmissionPort());
this->mutex.unlock();
    // NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::send: lock released." << std::endl;
    if (lWrittenBytes != pMsg->getSize()) {
        NHOFILE_LOG(logERROR) << "NHOTemplateBroadcaster::send: Sendig message failed <" << (unsigned int) lWrittenBytes << ">"
        << " vs <" << pMsg->getSize() << ">" << std::endl;
        Serial.print("Written bytes : ");
        Serial.println(lWrittenBytes);
        return(false);
    }
#else
    if (!emrecSocket) {
        NHOFILE_LOG(logERROR) << "NHOSVBroadcast::send: socket not initialized." << std::endl;
        return false;
    }
    if ((*pMsg).getSize() > 65507) {
        NHOFILE_LOG(logERROR) << "NHOSVBroadcast::send: message size exceeds limit." << std::endl;
        return false;
    }
    
    // configure for emission
    struct sockaddr_in lInfoServAddr;
    struct hostent* he = gethostbyname("255.255.255.255");
    // broadcast => .255 in the following address
    //    struct hostent* he = gethostbyname("192.168.0.255");
    //    192.168.1.32
    bzero((char *) &lInfoServAddr, sizeof(lInfoServAddr));
    lInfoServAddr.sin_family = AF_INET;
    //    lInfoServAddr.sin_addr.s_addr = INADDR_ANY; // INADDR_BROADCAST //? sure about that ?
    lInfoServAddr.sin_addr = *((struct in_addr *)he->h_addr);
    lInfoServAddr.sin_port = htons(emrecSocket);
    socklen_t optlen = sizeof(lInfoServAddr);
    
    // send message
    size_t lWrittenBytes = 0;
    lWrittenBytes = sendto(emrecSocket,
                           pMsg->getData(),
                           pMsg->getSize(),
                           0,
                           (struct sockaddr *)&lInfoServAddr,
                           optlen);
    if (lWrittenBytes != pMsg->getSize()) {
        NHOFILE_LOG(logERROR) << "NHOSVBroadcast::send: Sendig message failed <" << lWrittenBytes << ">"
        << " vs <" << pMsg->getSize() << ">" << std::endl;
        NHOFILE_LOG(logERROR) << "NHOSVBroadcast::send: Sendig message failed <" << errno << ">" << std::endl;
        return(false);
    }

    NHOFILE_LOG(logINFO) << "NHOSVBroadcast::send: Sent.\n";
    //delete pMsg;
#endif
    return(true);
}

/**
 * Emit one.
 **/

#ifdef ESP32_ADAFRUIT_FEATHER
template <class T>
void NHOTemplateBroadcaster<T>::send(const String message) const {
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
        NHOFILE_LOG(logDEBUG) << "NHOSVPingPong::send : Message sent successfully.";
    }
    else{
        Serial.println("Error sending the message");
        Serial.print("HTTP response code: ");
        Serial.println(httpResponseCode);
    }

    // Free resources
    http.end();
}
#else
template <class T>
void NHOTemplateBroadcaster<T>::send(const std::string message) const {
    NHOFILE_LOG(logDEBUG) << "NHOSVPingPong::send sendig warning.";
}
#endif
