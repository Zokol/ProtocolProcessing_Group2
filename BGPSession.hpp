/*! \file  BGPSession.hpp
 *  \brief     Header file of BGPSession module
 *  \details   
 *  \author    Antti Siirilä, 501449
 *  \version   1.0
 *  \date      12.2.2013
 */

/*!
 * \class BGPSession
 * \brief BGPSession module handles the HoldDown and Keepalive timers
 * of the session and sends keepalive messages to the session peer
 *  \details BGP session is a sub module of Control Plane. Control
 * Plane has full control on BGP session. First the session is
 * elaborated by Control Plane. Then the session is dedicated for some
 * peer by assigning the peer's BGP identifier to the session. After
 * that the session is started by calling the sessionStart-function.
 * After that the session automatically send the keepalive messages to
 * the peer whenever the keepalive timer expires. Control Plane needs to
 * reset the HoldDown timer whenever it receives a
 * message from the peer. Similarly, whenever Control Plane send a
 * message to the peer, it shall reset the Keepalive timer of the
 * session. The resets is done by calling the functions
 * resetHoldDown and resetKeepalive. Control Plane can track message
 * and the session by comparing the peer's identifier to the one in
 * the session using the isThisSession-function. When ever the
 * HoldDown timer expires the session is stopped automatically.
 * Control plane may check whether the session is still vaid or not
 * using the isSessionValid-function. The checking shall be done
 * before the timers are reset. Whenever Control Plane notices that
 * the session is not valid it shall update the Routing table
 * accordingly and generate required notification messages.
 */


#include "systemc"
#include "BGPSession_If.hpp"
#include "BGPMessage.hpp"
#include "Configuration.hpp"
#include "Output_If.hpp"
#include "StringTools.hpp"
#include "Interface_If.hpp"

using namespace std;
using namespace sc_core;
using namespace sc_dt;

#ifndef _BGPSESSION_H_
#define _BGPSESSION_H_

#define TCP_RT_DELAY 20
#define OPENSEND_RT_DELAY 20

/*!\enum enum BGP_States{IDLE, CONNECT, ACTIVE, OPEN_SENT, OPEN_CONFIRM, ESTABLISHED};
 * \brief Defines the bgp states
 */
enum BGP_States{IDLE, CONNECT, ACTIVE, OPEN_SENT, OPEN_CONFIRM, ESTABLISHED};

/*!\enum enum enum TCP_States{SYN, SYN_ACK, ACK};
 * \brief Defines the tcp states
 */
enum TCP_States{SYN, ACK, OPEN_SEND};




class BGPSession: public sc_module, public BGPSession_If
{

public:

    /*! \brief System clock signal
     * \details The router's internal clock
     * \public
     */
    sc_in_clk port_Clk;

    /*! \brief Output port to Data Plane module
     * \details The BGP session writes all the BGP messages to be send
     * to its neighbors into
     * this port. The port shall be bind to the Data Plane's.
     * receiving FIFO
     * \public
     */
    sc_port<Output_If<BGPMessage> > port_ToDataPlane;

    /*! \property sc_port<Output_If> port_ToRoutingTable
     * \brief Output port to Routing Table module
     * \public
     */
    sc_port<Output_If<BGPMessage> > port_ToRoutingTable;

    /*! \brief Control port to the session NIC
     * \public
     */
    sc_port<Interface_If> port_InterfaceControl;

    sc_fifo<BGPMessage> m_FsmInputBuffer;


    void before_end_of_elaboration()
    {

        m_Client = m_Config->isClient(m_PeeringInterface);

    }

    /*! \brief Elaborates the BGPSession module
     * \details 
     * @param[in] sc_module_name p_ModuleName Defines a unique name
     * for this module
     * @param[in] int p_PeeringInterface The outbound interface to
     * which the peer connects
     * @param[in] BGPSessionParameters p_SessionParameters Holds the
     * keepalive fraction, holddown time, etc. values for this session
     * \public
     */
    BGPSession(sc_module_name p_ModuleName, int p_PeeringInterface, BGPSessionParameters * const p_SessionParam);

    /*! \brief Elaborates the BGPSession module
     * \details 
     * @param[in] sc_module_name p_ModuleName Defines a unique name
     * for this module
     * @param[in] BGPSessionParameters p_SessionParameters Holds the
     * keepalive fraction, holddown time, etc. values for this session
     * \public
     */
    BGPSession(sc_module_name p_ModuleName, BGPSessionParameters * const p_SessionParam);

    /*! \brief Destructor of the BGPSession module
     * \details Free's all the dynamically allocated memory 
     * \public
     */
    ~BGPSession();

    /*! \fn void keepaliveTimer(void)
     *  \brief Send a keepalive message to the peer
     * \details A SystemC method, which is sensitive to m_BGPKeepalive event
     * \public
     */
    void keepaliveTimer(void);

    /*! \fn void sessionInvalidation(void)
     *  \brief Invalidates this session
     * \details A SystemC method, which is sensitive to m_BGPHoldDown event
     * \public
     */
    void sessionInvalidation(void);

    /*! \fn void retransmissionTimer(void)
     *  \brief Defines the retransmission period
     * \details The period length is set with m_Retransmission event
     * \public
     */
    void retransmissionTimer(void);

    /*! \fn void fsmRoutine(void)
     *  \brief Updates the session state
     * \details
     * \public
     */
    void fsmRoutine(void);

    /*! \fn void resetHoldDown(void)
     *  \brief Resets the HoldDown timer
     * \details Allows the control plane to reset the HoldDown timer
     * whenever a message from the session peer is received
     * \public
     */
    void resetHoldDown(void);

    /*! \fn bool isSessionValid(void)
     *  \brief Checks whether this session is valid or not
     * \details Allows the control plane to check whether this session
     * is still valid or not. I.e. is the HoldDown timer expired.
     * \public
     */
    virtual bool isSessionValid(void);

    /*! \fn void sessionStop(void)
     *  \brief Stops this session
     * \details HoldDown and Keepalive timers are stopped and no
     * keepalive messages are sent after a call of this function
     * \public
     */
    virtual void sessionStop(void);

    /*! \fn void sessionStart(void)
     *  \brief Starts the session
     * \details HoldDown and Keepalive timers are reset and the
     * sending of keepalive messages starts
     * \public
     */
    void sessionStart(void);

    /*! \fn void setPeerIdentifier(string p_BGPIdentifier)
     *  \brief Sets the BGP Identifier of the session peer
     * \details
     * @param[in] string p_BGPIdentifier of the session peer
     * received message
     * \public
     */
    void setPeerIdentifier(string p_BGPIdentifier);

    /*! \fn void setPeeringInterface(int p_Interface); 
     * \brief Set the interface id, which connects to the peering session
     * \details 
     * @param [in] int p_Interface  
     * \public
     */
    void setPeeringInterface(int p_Interface);
    
    /*! \fn void setAS(int p_PeerAS); 
     * \brief The AS of the peer
     * \details 
     * @param [in] int p_PeerAS  
     * \public
     */
    void setPeerAS(int p_PeerAS);

    /*! \fn void setBGPCurrentState(BGP_States p_State)
     * \brief Current FSM state of the BGP session
     * \details
     * @param [in] BGP_State p_State
     * \public
     */
    void setBGPCurrentState(BGP_States p_State);

    /*! \fn void setConnectionCurrentState(TCP_States p_State)
     * \brief Current FSM state of the TCP connection
     * \details
     * @param [in] TCP_States p_State
     * \public
     */
    void setConnectionCurrentState(TCP_States p_State);

    /*! \fn void setTCPId(int p_Value)
     * \brief Holds the value of TCP id
     * \details
     * @param [in] int p_Value
     * \public
     */
    void setTCPId(int p_Value);

    /*! \fn bool isThisSession(string p_BGPIdentifier)
     *  \brief Checks whether this session is for the passed BGP Identifier
     * \details
     * @param[in] string p_BGPIdentifier The BGP Identifier of the
     * received message
     * \return <bool> True: if this session corresponds the received
     * message a identifier. False: in any other case
     * \public
     */
    bool isThisSession(string p_BGPIdentifier);


    /*! \fn void resetKeepalive(void)
     *  \brief Resets the Keepalive timer
     * \details 
     * \public
     */
    void resetKeepalive(void);

    /*!
     * \sa BGPSession_If
     */
    virtual int getPeeringInterface(void);

    /*!
     * \sa BGPSession_If
     */
    virtual string getPeerAS(void);
    /*!
     * \sa BGPSession_If
     */
    virtual string getPeerIdentifier(void);
    
    /*! /fn BGP_States getBGPCurrentState(void)
     *  \brief Returns the current session state
     */
    BGP_States getBGPCurrentState(void);

    /*! /fn TCP_States getConnectionCurrentState(void)
     *  \brief Returns the current connection state
     */
    TCP_States getConnectionCurrentState(void);

    /*! /fn int getTCPId(void)
     *  \brief Returns the TCP ID
     */
    int getTCPId(void);


    /*! \brief Indicate the systemC producer that this module has a process.
     * \sa http://www.iro.umontreal.ca/~lablasso/docs/SystemC2.0.1/html/classproducer.html
     * \public
     */
    SC_HAS_PROCESS(BGPSession);

private:

    /*! \property int m_PeerAS 
     * \brief AS identifier of the peer
     * \details 
     * \private
     */
    int m_PeerAS;

    /*! \property sc_mutex m_KeepaliveMutex
     *  \brief Handles the arbitration for Keepalive reset
     * \details Keepalive can be reset either internally by the
     * session or externally by the Control Plane.
     * \private
     */
    sc_mutex m_KeepaliveMutex;

    /*! \property sc_event m_Retransmission
     *  \brief Retransmission event
     * \details Used in CONNECT and OPEN_SENT states as a retransmission timer
     * \private
     */
    sc_event m_Retransmission;

    /*! \property sc_event m_BGPKeepalive
     *  \brief BGP session keepalive timer
     * \details There is one instance for each session. The keepalive
     * timer defines when the next keepalice message is to be sent to
     * the corresponding session.
     * \private
     */
    sc_event m_BGPKeepalive;

    /*! \property sc_event m_BGPHoldDown
     *  \brief BGP session hold down timer
     * \details There is one instance of sc_event for each session. If
     * hold down timer expires the link of the corresponding session
     * shall be concidered to be down and the required action need to be taken.
     * \private
     */
    sc_event m_BGPHoldDown;

    /*! \property int m_PeeringInterface
     *  \brief Interface of the Session Peer
     * \details Index of the Interface of this router to which the
     * peer of this session connects
     * \private
     */
    int m_PeeringInterface;
    
    /*! \property bool m_SessionValidity
     *  \brief Indicates whether this session is valid or not
     * \details Is set to True when the session start. When ever the
     * HoldDown timer expires the value of m_SessionValidity shall be
     * set to Fasle
     * \private
     */
    bool m_SessionValidity;

    /*! \property BGPMessage m_KeepaliveMsg
     *  \brief BGP message object
     * \details This holds the keepalive messages
     * \private
     */
    BGPMessage m_KeepaliveMsg;

    /*! \property BGPMessage m_BGPIn
     *  \brief BGP message object
     * \details The input for fsmRoutine
     * \private
     */
    BGPMessage m_BGPIn;

    /*! \property BGPMessage m_BGPInPrevious
     *  \brief BGP message object
     * \details The input for fsmRoutine
     * \private
     */
    BGPMessage m_BGPInPrevious;

    /*! \property BGPMessage m_BGPOut
     *  \brief BGP message object
     * \details The output of fsmRoutine
     * \private
     */
    BGPMessage m_BGPOut;

    /*! \property string m_BGPIdentifierPeer
     *  \brief The BGP identifier of the session peer
     * \details 
     * \private
     */
    string m_BGPIdentifierPeer;

    /*! \property BGPSessionParameters *m_Config
     *  \brief Simulation parameters for the session
     * \details 
     * \private
     */
    BGPSessionParameters *m_Config;

    /*! \property StringTools m_RTool
     *  \brief Used in reporting debug messages
     * \details 
     * \private
     */
    StringTools m_RTool;
    BGP_States m_BGPCurrentState;
    BGP_States m_BGPPreviousState;

    TCP_States m_ConnectionCurrentState;
    TCP_States m_ConnectionPreviousState;

    int m_TCPId;
    bool m_ReSend;
    sc_mutex m_ReSendMutex;
    int m_RetransmissonCount;
    sc_mutex m_BGPCurrentStateMutex;
    bool m_KeepaliveFlag;
    sc_mutex m_KeepaliveFlagMutex;
   bool m_Client;
   bool m_BeeingHere;

   bool m_NewFsmInput;



    /***************************Private functions*****************/

    void setRetransmissionTimer(int p_Delay);
    void stopRetransmissionTimer(void);
    void setReSend(bool p_Value);
    void fsmReportRoutineBGP(string p_Report);
    void fsmReportRoutineConnection(string p_Report);
    void setKeepaliveFlag(bool p_Value);
    bool isKeepaliveTime(void);
    void sendKeepalive(void);
};




#endif /* _BGPSESSION_H_ */
