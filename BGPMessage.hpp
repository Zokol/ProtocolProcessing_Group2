/*! \file BGPMessage.hpp
 *  \brief     Header file of BGP message class
 *  \details   defines the BGPMessage class.
 *  \author    Antti Siiril�, 501449
 *  \version   1.0
 *  \date      11.2.2013
 */

/*! \class BGPMessage
 *  \brief
 *  \details
 */


#include <systemc>


using std::cout;
using std::endl;
using std::ostream;
using std::string;

using sc_core::sc_trace_file;
using sc_core::sc_trace;
using sc_dt::sc_int;

#ifndef BGPMESSAGE_H
#define BGPMESSAGE_H


/*! \brief BGP message type definitions
  \details 1 - OPEN, 2 - UPDATE, 3 - NOTIFICATION, 4 - KEEPALIVE
*/


/*! \def OPEN
 *  \brief Defines the BGP open message type value
 */
#define OPEN 1

/*! \def UPDATE
 *  \brief Defines the BGP update message type value
 */
#define UPDATE 2

/*! \def NOTIFICATION
 *  \brief Defines the BGP notification message type value
 */
#define NOTIFICATION 3

/*! \def KEEPALIVE
 *  \brief Defines the BGP keepalive message type value
 */
#define KEEPALIVE 4

/*! \def TCP_SYN
 *  \brief Defines the TCP syn message type
 */
#define TCP_SYN 5

/*! \def TCP_SYNACK
 *  \brief Defines the TCP synack message type
 */
#define TCP_SYNACK 6

/*! \def TCP_ACK
 *  \brief Defines the TCP ack message type
 */
#define TCP_ACK 7

class BGPMessage
{
public:


    /*! \brief Holds the BGP message type value
     * \details
     * \public
     */
    int m_Type;

    /*! \brief The originator's BGP identifier
     * \details
     * \public
     */
    string m_BGPIdentifier;
    //    routeAsString.append("-");
    //    routeAsString.append(m_RTConfig->getASNumberAsString());

    /*! \brief The originator's BGP identifier
     * \details
     * \public
     */
    int m_OutboundInterface;

    /*! \property int m_AS 
     * \brief AS identifier
     * \details 
     * \public
     */
    int m_AS;
    
    int m_HoldDownTime;

    unsigned long m_MsgId;

    /*! \property string m_Message 
     * \brief BGP message fields
     * \details 
     * \public
     */
    string m_Message;

    BGPMessage():m_Type(-1), m_OutboundInterface(-1){};

    ~BGPMessage(){};

    BGPMessage(BGPMessage& p_Msg);








    /*! \relates sc_signal
     * \brief Overload stream operator
     * \details Write data members into given output stream.
     * @param[in] os Reference to outputstream in which the member field values is to be written.
     * @param[in] p_Msg Reference to BGPMessage-object, which member fields is to be written into outputstream.
     * \return \b <ostream> Reference to the outputstream containing the written values
     */

    inline friend ostream& operator << (ostream& os,  BGPMessage const & p_Msg )
    {

        os << endl  << "BGP type: " << p_Msg.m_Type  << " BGP identifier: " << p_Msg.m_BGPIdentifier  << " BGP session interface: " << p_Msg.m_OutboundInterface  << " AS number: " << p_Msg.m_AS  << endl <<"MsgId: " << p_Msg.m_MsgId << " Hold-down: " << p_Msg.m_HoldDownTime << " Message: " << p_Msg.m_Message;
        return os;
    }


    /*! \relates sc_trace_file
     * \brief Set trace file for this packet
     * \details All the member fields shall be traced. Allow systemC library to access the private members of this class by declaring the function as friend
     * @param[out] sc_trace_file p_TraceFilePointer Pointer to sc_trace_file-object
     * @param[in] BGPMessage p_Msg Reference to BGPMessage-object to be traced
     * @param[in] string p_TraceObjectName Name of the Packet-object
     *
     */
    inline friend void sc_trace(sc_trace_file *p_TraceFilePointer, const BGPMessage& p_Msg, const string & p_TraceObjectName )
    {
        sc_trace(p_TraceFilePointer, p_Msg.m_Type, p_TraceObjectName + ".type");
    }



    /*!
     * \brief Overload of assign operator
     * \details Copy the data fields of given BGPMessage-object to the this. Return a reference to this.
     * @param[in] Packet p_BGPMessage Reference to a Packet-object, which member values is to be assigned to the member fields of this object accordingly
     * \return \b <Packet> Reference to this object
     * \public
     */
    BGPMessage& operator = (const BGPMessage& p_Msg);



    /*!
     * \brief Overload of compare operator
     * \details Compare the data fields of this pkt-object to the onces in the given pkt-object.
     * @param[in] BGPMessage p_Msg Reference to a BGPMessage-object to be compared with this object
     * \return \b <bool> true if the member fields of both objects match otherwise false.
     * \public
     */
    bool operator == (const BGPMessage& p_Msg) const;

    void clearMessage(void);
private:



};


#endif
