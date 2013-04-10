/*! \file  Configuration.hpp
 *  \brief    Defines the BGPSessionParameters,ControlPlaneConfig, Connection, RouterConfig, and
 *  SimulationConfig classes
 *  \details   
 *  \author    Antti Siirilä, 501449
 *  \version   1.0
 *  \date      Wed Mar 20 15:30:11 2013
 */

#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "systemc"

using namespace sc_dt;
using namespace std;
/*!
 * \class BGPSessionParameters
 * \brief Holds the parameters for BGP session
 *  \details Members are Keepalive time and HoldDown time factor
 */
class BGPSessionParameters
{

public:
    
    /*!
      \Brief Default constructor of BGPSessionParameters-class
      \details Sets the keepalive time to 60 and the holddown time
      factor to 3
      * \public
      */  
    BGPSessionParameters();

    /*!
      \Brief Constructor of BGPSessionParameters-class with parameters
      \details Allows to set the member values while initiating the object
      @param[in] int p_KeepaliveTime The keepalive time value
      @param[in] int p_HoldDownTimeFactor The multiplier that defines
      the HoldDown time in relation to Keepalive time.
      \public
    */
    BGPSessionParameters(int p_KeepaliveTime, int p_HoldDownTimeFactor);

    /*!
      \Brief Default destructor
      \details The class does not contain dynamic memory allocations
      * \public
      */  
    virtual ~BGPSessionParameters();

    /*! \fn void setKeepaliveTime(int p_KeepaliveTime);     
     *  \brief Sets the Keepalive time
     *  @param[in] int p_KeepaliveTime The keepalive time value
     * \public
     */
    void setKeepaliveTime(int p_KeepaliveTime);    

    /*! \fn void setHoldDownTimeFactor(int p_HoldDownTimeFactor);
     *  \brief Sets the HoldDown time factor
     *  @param[in] int p_HoldDownTimeFactor The multiplier that defines
     the HoldDown time in relation to Keepalive time.
     * \public
     */
    void setHoldDownTimeFactor(int p_HoldDownTimeFactor);    

    /*! \fn int getKeepaliveTime(void); 
     *  \brief Returns the keepalive time
     *  \return integer value
     * \public
     */
    int getKeepaliveTime(void);

    /*! \fn int getHoldDownTime(void); 
     *  \brief Returns the hold-down time
     *  \return integer value
     * \public
     */
    int getHoldDownTime(void);

    /*! \fn int getHolDownTimeFactor(void);
     *  \brief Returns the hold-down time factor
     *  \return integer value
     * \public
     */
    int getHolDownTimeFactor(void);

    /*! \fn BGPSessionParameters& operator = (const BGPSessionParameters& p_Original);
     *  \brief clones the passed BGPSessionParameters object to this object
     *  \return reference BGPSessionParameters& 
     * \public
     */
    BGPSessionParameters& operator = (const BGPSessionParameters& p_Original);

protected:

    /*! \brief HoldDown time for this session
     * \details Defines the keepalive time for this session. The default
     * value needs to be set in the elaboration phase. After that the
     * BGP session may negotiated a new value between the session peers
     * \private
     */
    int m_KeepaliveTime;
    
    /*! \brief HoldDown time for this session
     * \details Defines the holdDown time for this session. The default
     * value needs to be set in the elaboration phase. After that the
     * BGP session may negotiated a new value between the session peers
     * \private
     */
    int m_HoldDownTime;

    /*! \brief HoldDown time factor
     * \details Defines the multiplier that determines the holdDown
     * time by m_KeepaliveTime X m_HoldDownTimeFactor
     * \private
     */
    int m_HoldDownTimeFactor;

private:

    void setHoldDownTime(void);    
};


/*!
 * \class ControlPlaneConfig
 * \brief Holds the BGP parameters
 *  \details   
 */
class ControlPlaneConfig: public BGPSessionParameters
{

public:
    
    inline ControlPlaneConfig(){};
    inline virtual ~ControlPlaneConfig(){};

    /*! \fn void setNumberOfInterfaces(int p_NumberOfInterfaces); 
     *  \brief Sets the number of interfaces allocated in the router
     *  @param[in] int p_NumberOfInterfaces The interface count
     * \public
     */
    void setNumberOfInterfaces(int p_NumberOfInterfaces);

    /*! \fn void setPrefix(sc_int<32> p_Prefix);
     *  \brief Sets the prefix IP of the AS
     *  @param[in] sc_int<32> p_Prefix The IP prefix
     * \public
     */
    void setPrefix(sc_int<32> p_Prefix);

    /*! \fn void setPrefixMask(sc_int<32> p_PrefixMask);
     *  \brief Sets the mask for the prefix IP
     *  @param[in] sc_int<32> p_PrefixMask The prefix mask
     * \public
     */
    void setPrefixMask(sc_int<32> p_PrefixMask);

    /*! \fn void setPrefixMask(sc_int<32> p_PrefixMask);
     *  \brief Sets the AS Identifier to which this router connects
     *  @param[in] int p_ASNumber The AS identifier of the local AS 
     * \public
     */
    void setASNumber(int p_ASNumber);

    /*! \fn void setMED(int p_MED);
     *  \brief Sets the Multi-exit discriminator(MED) attribute of BGP
     *  @param[in] int m_MED The Multi-exit discriminator value
     * \public
     */
    void setMED(int p_MED);

    /*! \fn void setLocalPref(int p_LocalPref);
     *  \brief Sets the Local Preference atribute of BGP
     *  @param[in] int m_LocalPref The Local preference value
     * \public
     */
    void setLocalPref(int p_LocalPref);

    /*! \fn int getNumberOfInterfaces(void); 
     *  \brief Returns the number of interface
     *  \return integer value
     * \public
     */
    int getNumberOfInterfaces(void);

    /*! \fn sc_int<32> getPrefix(void);
     *  \brief Returns the IP prefix
     *  \return sc_int<32> value
     * \public
     */
    sc_int<32> getPrefix(void);

    /*! \fn sc_int<32> getPrefixMask(void);
     *  \brief Returns the prefix mask
     *  \return sc_int<32> value
     * \public
     */
    sc_int<32> getPrefixMask(void);

    /*! \fn int getASNumber(void);
     *  \brief Returns the AS number
     *  \return integer value
     * \public
     */
    int getASNumber(void);

    /*! \fn int getMED(void);
     *  \brief Returns the Multi-exit discriminator value
     *  \return integer value
     * \public
     */
    int getMED(void);

    /*! \fn int getLocalPref(void);
     *  \brief Returns the Local preference value
     *  \return integer value
     * \public
     */
    int getLocalPref(void);

    /*! \fn ControlPlaneConfig& operator = (const ControlPlaneConfig& p_Original);
     *  \brief clones the passed ControlplaneConfig object to this object
     *  \return reference ControlPlaneConfig& 
     * \public
     */
    ControlPlaneConfig& operator = (const ControlPlaneConfig& p_Original);

protected:

    /*! \property int m_NumberOfInterfaces
     * \brief Number of network interfaces that this router should allocate
     * \details
     * \private
     */
    int m_NumberOfInterfaces;

    /*! \brief The prefix of the AS connecting this router
     * \details 
     * \public
     */
    sc_int<32> m_Prefix;

    /*! \brief The mask defined by prefix /-notation
     * \details 
     * \public
     */
    sc_int<32> m_PrefixMask;

    /*! \brief The AS number of this router
     * \details 
     * \public
     */
    int m_ASNumber;

    /*! \brief BGP MED variable
     * \details 
     * \public
     */
    int m_MED;

    /*! \brief BGP Local Preference variable
     * \details 
     * \public
     */
    int m_LocalPref;
};

/*!
 * \class Connection
 * \brief Holds the connection parameters for local interface
 *  \details   
 */
class Connection
{

public:

    /*! \brief The id of the neighbor interface to where this router connects
     * \public
     */
    int m_NeighborInterfaceId;

    /*! \brief The id of the neighbor router on which the neighbor
     * interface is located
     * \details
     * \public
     */
    int m_NeighborRouterId;    

};

/*!
 * \class RouterConfig
 * \brief Holds the parameters for a router including interface
 * connection parameters
 *  \details   
 */
class RouterConfig:public ControlPlaneConfig
{

public:

    inline RouterConfig(void){};
    RouterConfig(int p_NumberOfInterfaces);

    ~RouterConfig();

    void addConnectionConfig( int p_LocalInterfaceId, int p_NeighborRouterId, int p_NeighborInterfaceId);


    
private:    
    /*! \brief Linked list that holds the connection information for
     * each interface of the router
     * \details
     * 
     * \public
     */
    Connection **m_NeighborConnections;

    /*! \fn RouterConfig& operator = (const RouterConfig& p_Original);
     *  \brief clones the passed RouterConfig object to this object
     *  \return reference RouterConfig& 
     * \public
     */
    RouterConfig& operator = (const RouterConfig& p_Original);


};

/*! \Class SimulationConfig
 *  \brief Holds all the simulation parameters received from GUI
 *  \details Used to build up the simulatin environment
 *
 */
class SimulationConfig
{

public:

    inline SimulationConfig(){};    

    SimulationConfig(int p_NumberOfRouters);

    virtual ~SimulationConfig();

    void addRouterConfig(int p_RouterId, int p_NumberOfInterfaces);
    
    void addConnectionConfig(int p_LocalRouterId, int p_LocalInterfaceId, int p_NeighborInterfaceId, int p_NeighborRouterId);

    void addBGPSessionParameters(int p_LocalRouterId, int p_KeepaliveTime, int p_HoldDownTimeFactor);

    /*! \fn void setNumberOfRouters(int p_NumberOfRouters);
     *  \brief Sets the number of routers used in this simulation
     *  @param[in] int p_NumberOfRouters The router count
     * \public
     */
    void setNumberOfRouters(int p_NumberOfRouters);

    /*! \fn int getNumberOfRouters(void);
     *  \brief Returns the number of routers in the simulation
     *  \return integer value
     * \public
     */
    int getNumberOfRouters(void);

    /*! \fn RouterConfig& getRouterConfiguration(int p_RouterId);
     *  \brief Returns the reference to the RouterConfig-object
     *  defined by p_RouterId
     *  \return reference to RouterConfig object
     * \public
     */
    RouterConfig& getRouterConfiguration(int p_RouterId);

    SimulationConfig& operator = (const SimulationConfig& p_Original);


    /*! \brief Number of routers that this simulation should allocate
     * \details
     * \public
     */
    int m_NumberOfRouters;

    /*! \property RouterConfig** m_RouterConfiguration
     * \brief Holds the router configurations for each router in this simulation
     * \details
     * \public
     */
    RouterConfig **m_RouterConfiguration;


};



#endif /* _CONFIGURATION_H_ */